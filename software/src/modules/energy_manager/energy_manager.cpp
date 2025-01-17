/* esp32-firmware
 * Copyright (C) 2022 Olaf Lüke <olaf@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "energy_manager.h"

#include "bindings/errors.h"

#include "api.h"
#include "event_log.h"
#include "modules.h"
#include "task_scheduler.h"
#include "tools.h"
#include "web_server.h"

void EnergyManager::pre_setup()
{
    // States
    state = Config::Object({
        {"phases_switched", Config::Uint8(0)},
        {"input3_state", Config::Bool(false)},
        {"input4_state", Config::Bool(false)},
        {"relay_state", Config::Bool(false)},
        {"error_flags", Config::Uint32(0)},
    });

    low_level_state = Config::Object({
        {"contactor", Config::Bool(false)},
        {"contactor_check_state", Config::Uint8(0)},
        {"input_voltage", Config::Uint16(0)},
        {"led_rgb", Config::Array({Config::Uint8(0), Config::Uint8(0), Config::Uint8(0)},
            new Config{Config::Uint8(0)}, 3, 3, Config::type_id<Config::ConfUint>())
        },
    });
    meter_state = Config::Object({
        {"energy_meter_type", Config::Uint8(0)},
        {"energy_meter_power", Config::Float(0)}, // watt
        {"energy_meter_energy_import", Config::Float(0)}, // kWh
        {"energy_meter_energy_export", Config::Float(0)}, // kWh
    });

    // Config
    config = ConfigRoot(Config::Object({
        {"default_mode", Config::Uint(0, 0, 3)},
        {"auto_reset_mode", Config::Bool(false)},
        {"auto_reset_time", Config::Uint(0, 0, 1439)},
        {"excess_charging_enable", Config::Bool(false)},
        {"contactor_installed", Config::Bool(false)},
        {"phase_switching_mode", Config::Uint8(PHASE_SWITCHING_AUTOMATIC)},
        {"guaranteed_power", Config::Uint(0, 0, 22000)}, // in watt
        {"relay_config", Config::Uint8(0)},
        {"relay_rule_when", Config::Uint8(0)},
        {"relay_rule_is", Config::Uint8(0)},
        {"input3_rule_then", Config::Uint8(0)},
        {"input3_rule_then_limit", Config::Uint32(0)}, // in A
        {"input3_rule_is", Config::Uint8(0)},
        {"input3_rule_then_on_high", Config::Uint(MODE_DO_NOTHING, 0, 255)},
        {"input3_rule_then_on_low", Config::Uint(MODE_DO_NOTHING, 0, 255)},
        {"input4_rule_then", Config::Uint8(0)},
        {"input4_rule_then_limit", Config::Uint32(0)}, // in A
        {"input4_rule_is", Config::Uint8(0)},
        {"input4_rule_then_on_high", Config::Uint(MODE_DO_NOTHING, 0, 255)},
        {"input4_rule_then_on_low", Config::Uint(MODE_DO_NOTHING, 0, 255)},
    }), [](const Config &cfg) -> String {
        uint32_t max_current_ma = charge_manager.charge_manager_config_in_use.get("maximum_available_current")->asUint();
        uint32_t input3_rule_then_limit_ma = cfg.get("input3_rule_then_limit")->asUint();
        uint32_t input4_rule_then_limit_ma = cfg.get("input4_rule_then_limit")->asUint();

        if (input3_rule_then_limit_ma > max_current_ma) {
            return "Input 3 current limit exceeds maximum total current of all chargers.";
        }
        if (input4_rule_then_limit_ma > max_current_ma) {
            return "Input 4 current limit exceeds maximum total current of all chargers.";
        }

        return "";
    });

    debug_config = Config::Object({
        {"hysteresis_time", Config::Uint(HYSTERESIS_MIN_TIME_MINUTES, 0, 60)}, // in minutes
        {"target_power_from_grid", Config::Int32(0)}, // in watt
    });

    // Runtime config
    charge_mode = Config::Object({
        {"mode", Config::Uint(0, 0, 3)},
    });
    charge_mode_update = charge_mode;
}

void EnergyManager::apply_defaults()
{
    // TODO: Configure Energy Manager
}

void EnergyManager::setup_energy_manager()
{
    if (!this->DeviceModule::setup_device()) {
        logger.printfln("energy_manager: setup_device error. Reboot in 5 Minutes.");

        task_scheduler.scheduleOnce([](){
            trigger_reboot("Energy Manager");
        }, 5 * 60 * 1000);
        return;
    }

    this->apply_defaults();
    initialized = true;
}

void EnergyManager::check_debug()
{
    task_scheduler.scheduleOnce([this](){
        if (deadline_elapsed(last_debug_check + 60000)  && debug == true)
        {
            logger.printfln("Debug log creation canceled because no continue call was received for more than 60 seconds.");
            debug = false;
        }
        else if (debug == true)
            check_debug();
    }, 70000);
}

void EnergyManager::setup()
{
    setup_energy_manager();
    if (!device_found)
        return;

    // Forgets all settings when new setting is introduced: "Failed to restore persistent config config: JSON object is missing key 'input3_rule_then_limit'\nJSON object is missing key 'input4_rule_then_limit'"
    api.restorePersistentConfig("energy_manager/config", &config);
    config_in_use = config;

    if ((config_in_use.get("phase_switching_mode")->asUint() == PHASE_SWITCHING_AUTOMATIC) && !config_in_use.get("contactor_installed")->asBool()) {
        logger.printfln("energy_manager: Invalid configuration: Automatic phase switching selected but no contactor installed.");
        return;
    }

#if MODULE_DEBUG_AVAILABLE()
    api.restorePersistentConfig("energy_manager/debug_config", &debug_config);
#endif
    debug_config_in_use = debug_config;

#if MODULE_CHARGE_MANAGER_AVAILABLE()
    charge_manager.set_allocated_current_callback([this](uint32_t current_ma){
        //logger.printfln("energy_manager: allocated current callback: %u", current_ma);
        charge_manager_allocated_current_ma = current_ma;
    });
#endif

    update_all_data();

    rgb_led.setup();

    // Set up output relay and input pins
    output = new OutputRelay(config_in_use);
    input3 = new InputPin(3, 0, config_in_use, all_data.input[0]);
    input4 = new InputPin(4, 1, config_in_use, all_data.input[1]);

    // Cache config for energy update
    default_mode                = config_in_use.get("default_mode")->asUint();
    excess_charging_enable      = config_in_use.get("excess_charging_enable")->asBool();
    target_power_from_grid_w    = debug_config_in_use.get("target_power_from_grid")->asInt();          // watt
    guaranteed_power_w          = config_in_use.get("guaranteed_power")->asUint();               // watt
    contactor_installed         = config_in_use.get("contactor_installed")->asBool();
    phase_switching_mode        = config_in_use.get("phase_switching_mode")->asUint();
    switching_hysteresis_ms     = debug_config_in_use.get("hysteresis_time")->asUint() * 60 * 1000;    // milliseconds (from minutes)
    max_current_unlimited_ma    = charge_manager.charge_manager_config_in_use.get("maximum_available_current")->asUint();      // milliampere
    min_current_ma              = charge_manager.charge_manager_config_in_use.get("minimum_current")->asUint();                // milliampere

    uint32_t auto_reset_time    = config_in_use.get("auto_reset_time")->asUint();
    auto_reset_hour   = auto_reset_time / 60;
    auto_reset_minute = auto_reset_time % 60;

    mode = default_mode;
    charge_mode.get("mode")->updateUint(mode);

    // If the user accepts the additional wear, the minimum hysteresis time is 10s. Less than that will cause the control algorithm to oscillate.
    uint32_t hysteresis_min_ms = 10 * 1000;  // milliseconds
    if (switching_hysteresis_ms < hysteresis_min_ms)
        switching_hysteresis_ms = hysteresis_min_ms;

    // Pre-calculate various limits
    int32_t min_phases;
    if (phase_switching_mode == PHASE_SWITCHING_ALWAYS_1PHASE) {
        min_phases = 1;
    } else if (phase_switching_mode == PHASE_SWITCHING_ALWAYS_3PHASE) {
        min_phases = 3;
    } else { // automatic
        min_phases = 1;
    }
    overall_min_power_w = 230 * min_phases * min_current_ma / 1000;

    const int32_t max_1phase_w = 230 * 1 * max_current_unlimited_ma / 1000;
    const int32_t min_3phase_w = 230 * 3 * min_current_ma / 1000;

    if (min_3phase_w > max_1phase_w) { // have dead current range
        int32_t range_width = min_3phase_w - max_1phase_w;
        threshold_3to1_w = max_1phase_w + static_cast<int32_t>(0.25 * range_width);
        threshold_1to3_w = max_1phase_w + static_cast<int32_t>(0.75 * range_width);
    } else { // no dead current range, use simple limits
        threshold_3to1_w = min_3phase_w;
        threshold_1to3_w = max_1phase_w;
    }

    api.addFeature("energy_manager");

    // Initialize contactor check state so that the check doesn't trip immediately if the first response from the bricklet is invalid.
    all_data.contactor_check_state = 1;

    task_scheduler.scheduleWithFixedDelay([this](){
        this->update_all_data();
    }, 0, 250);

    task_scheduler.scheduleWithFixedDelay([this](){
        this->update_io();
    }, 250, 250);

    start_network_check_task();

    if (max_current_unlimited_ma == 0) {
        logger.printfln("energy_manager: No maximum current configured for chargers. Disabling energy distribution.");
        return;
    }

#if MODULE_CHARGE_MANAGER_AVAILABLE()
    // Can't check for chargers in setup() because CM's setup() hasn't run yet to load the charger configuration.
    task_scheduler.scheduleOnce([](){
        if (!charge_manager.have_chargers())
            logger.printfln("energy_manager: No chargers configured. Won't try to distribute energy.");
    }, 0);
#else
    logger.printfln("energy_manager: Module 'Charge Manager' not available. Disabling energy distribution.");
    return;
#endif

    task_scheduler.scheduleWithFixedDelay([this](){
        this->update_energy();
    }, 250, 250);

    task_scheduler.scheduleOnce([this](){
        uptime_past_hysteresis = true;
    }, switching_hysteresis_ms);

    if (config_in_use.get("auto_reset_mode")->asBool())
        start_auto_reset_task();
}

void EnergyManager::register_urls()
{
    if (!device_found)
        return;

#if MODULE_WS_AVAILABLE()
    server.on("/energy_manager/start_debug", HTTP_GET, [this](WebServerRequest request) {
        task_scheduler.scheduleOnce([this](){
            last_debug_check = millis();
            check_debug();
            ws.pushRawStateUpdate(this->get_energy_manager_debug_header(), "energy_manager/debug_header");
            debug = true;
        }, 0);
        return request.send(200);
    });

    server.on("/evse/continue_debug", HTTP_GET, [this](WebServerRequest request) {
        last_debug_check = millis();
        return request.send(200);
    });

    server.on("/energy_manager/stop_debug", HTTP_GET, [this](WebServerRequest request){
        task_scheduler.scheduleOnce([this](){
            debug = false;
        }, 0);
        return request.send(200);
    });
#endif

    api.addPersistentConfig("energy_manager/config", &config, {}, 1000);
#if MODULE_DEBUG_AVAILABLE()
    api.addPersistentConfig("energy_manager/debug_config", &debug_config, {}, 1000);
#endif
    api.addState("energy_manager/state", &state, {}, 1000);
    api.addState("energy_manager/low_level_state", &low_level_state, {}, 1000);
    api.addState("energy_manager/meter_state", &meter_state, {}, 1000);

    api.addState("energy_manager/charge_mode", &charge_mode, {}, 1000);
    api.addCommand("energy_manager/charge_mode_update", &charge_mode_update, {}, [this](){
        uint32_t new_mode = charge_mode_update.get("mode")->asUint();

        if (new_mode == MODE_DO_NOTHING)
            return;

        auto runtime_mode = charge_mode.get("mode");
        uint32_t old_mode = runtime_mode->asUint();
        runtime_mode->updateUint(new_mode);
        mode = new_mode;

        if (new_mode != old_mode)
            just_switched_mode = true;

        logger.printfln("energy_manager: Switched mode %i->%i", old_mode, mode);
    }, false);

    this->DeviceModule::register_urls();
}

void EnergyManager::loop()
{
    this->DeviceModule::loop();

#if MODULE_WS_AVAILABLE()
    static uint32_t last_debug = 0;
    if (debug && deadline_elapsed(last_debug + 50)) {
        last_debug = millis();
        ws.pushRawStateUpdate(this->get_energy_manager_debug_line(), "energy_manager/debug");
    }
#endif
}

void EnergyManager::update_all_data()
{
    update_all_data_struct();

    low_level_state.get("contactor")->updateBool(all_data.contactor_value);
    low_level_state.get("led_rgb")->get(0)->updateUint(all_data.rgb_value_r);
    low_level_state.get("led_rgb")->get(1)->updateUint(all_data.rgb_value_g);
    low_level_state.get("led_rgb")->get(2)->updateUint(all_data.rgb_value_b);
    state.get("input3_state")->updateBool(all_data.input[0]);
    state.get("input4_state")->updateBool(all_data.input[1]);
    state.get("relay_state")->updateBool(all_data.output);
    low_level_state.get("input_voltage")->updateUint(all_data.voltage);
    low_level_state.get("contactor_check_state")->updateUint(all_data.contactor_check_state);

    if (all_data.energy_meter_type != METER_TYPE_NONE) {
        meter_state.get("energy_meter_type")->updateUint(all_data.energy_meter_type);
        meter_state.get("energy_meter_power")->updateFloat(all_data.power);
        meter_state.get("energy_meter_energy_import")->updateFloat(all_data.energy_import);
        meter_state.get("energy_meter_energy_export")->updateFloat(all_data.energy_export);
    }

    // Update states derived from all_data
    is_3phase   = contactor_installed ? all_data.contactor_value : phase_switching_mode == PHASE_SWITCHING_ALWAYS_3PHASE;
    have_phases = 1 + is_3phase * 2;
    state.get("phases_switched")->updateUint(have_phases);

    power_at_meter_w = all_data.energy_meter_type ? all_data.power : meter.values.get("power")->asFloat(); // watt

    if (contactor_installed) {
        if ((all_data.contactor_check_state & 1) == 0) {
            logger.printfln("Contactor check tripped. Check contactor.");
            contactor_check_tripped = true;
            set_error(ERROR_FLAGS_CONTACTOR_MASK);
        }
    }
}

void EnergyManager::update_all_data_struct()
{
    int rc = tf_warp_energy_manager_get_all_data_1(
        &device,
        &all_data.contactor_value,
        &all_data.rgb_value_r,
        &all_data.rgb_value_g,
        &all_data.rgb_value_b,
        &all_data.power,
        &all_data.energy_import,
        &all_data.energy_export,
        &all_data.energy_meter_type,
        all_data.error_count,
        all_data.input,
        &all_data.output,
        &all_data.voltage,
        &all_data.contactor_check_state
    );

    check_bricklet_reachable(rc);
}

void EnergyManager::update_status_led()
{
    if (error_flags & ERROR_FLAGS_ALL_ERRORS_MASK)
        rgb_led.set_status(EmRgbLed::Status::Error);
    else if (error_flags & ERROR_FLAGS_ALL_WARNINGS_MASK)
        rgb_led.set_status(EmRgbLed::Status::Warning);
    else
        rgb_led.set_status(EmRgbLed::Status::OK);
}

void EnergyManager::clr_error(uint32_t error_mask)
{
    error_flags &= ~error_mask;
    state.get("error_flags")->updateUint(error_flags);
    update_status_led();
}

bool EnergyManager::is_error(uint32_t error_bit_pos)
{
    return (error_flags >> error_bit_pos) & 1;
}

void EnergyManager::set_error(uint32_t error_mask)
{
    error_flags |= error_mask;
    state.get("error_flags")->updateUint(error_flags);
    update_status_led();
}

void EnergyManager::check_bricklet_reachable(int rc) {
    if (rc == TF_E_OK) {
        consecutive_bricklet_errors = 0;
        if (!bricklet_reachable) {
            bricklet_reachable = true;
            clr_error(ERROR_FLAGS_BRICKLET_MASK);
            logger.printfln("energy_manager: Bricklet is reachable again.");
        }
    } else {
        if (rc == TF_E_TIMEOUT) {
            logger.printfln("energy_manager: get_all_data_1() timed out.");
        } else {
            logger.printfln("energy_manager: get_all_data_1() returned error %d.", rc);
        }
        if (bricklet_reachable && ++consecutive_bricklet_errors >= 8) {
            bricklet_reachable = false;
            set_error(ERROR_FLAGS_BRICKLET_MASK);
            logger.printfln("energy_manager: Bricklet is unreachable.");
        }
    }
}

void EnergyManager::update_io()
{
    output->update();

    // Oversampling inputs is currently not used because all of the implemented input pin functions require update_energy() to run anyway.
    //// We "over-sample" the two inputs compared to the other data in the all_data struct
    //// to make sure that we can always react in a timely manner to input changes
    //int rc = tf_warp_energy_manager_get_input(&device, all_data.input);
    //if (rc != TF_E_OK) {
    //    logger.printfln("get_input error %d", rc);
    //}

    // Restore values that can be changed by input pins.
    max_current_limited_ma      = max_current_unlimited_ma;

    input3->update(all_data.input[0]);
    input4->update(all_data.input[1]);
}

void EnergyManager::start_network_check_task()
{
    task_scheduler.scheduleWithFixedDelay([this](){
        bool connected = false;
#if MODULE_ETHERNET_AVAILABLE()
        connected = ethernet.get_connection_state() == EthernetState::CONNECTED;
#endif
#if MODULE_WIFI_AVAILABLE()
        if (!connected)
            connected = wifi.get_connection_state() == WifiState::CONNECTED;
#endif
        if (connected) {
            if (is_error(ERROR_FLAGS_NETWORK_BIT_POS))
                clr_error(ERROR_FLAGS_NETWORK_MASK);
        } else {
            set_error(ERROR_FLAGS_NETWORK_MASK);
        }
    }, 0, 5000);
}

void EnergyManager::start_auto_reset_task()
{
#if MODULE_NTP_AVAILABLE()
    task_scheduler.scheduleOnce([this](){
        if (ntp.state.get("synced")->asBool())
            schedule_auto_reset_task();
        else
            start_auto_reset_task();
    }, 30 * 1000);
#endif
}

void EnergyManager::schedule_auto_reset_task()
{
    uint32_t delay_ms = ms_until_time(auto_reset_hour, auto_reset_minute);
    task_scheduler.scheduleOnce([this](){
        switch_mode(default_mode);
        schedule_auto_reset_task();
    }, delay_ms);
}

void EnergyManager::limit_max_current(uint32_t limit_ma)
{
    if (max_current_limited_ma > limit_ma)
        max_current_limited_ma = limit_ma;
}

void EnergyManager::switch_mode(uint32_t new_mode)
{
    api.callCommand("energy_manager/charge_mode_update", Config::ConfUpdateObject{{
        {"mode", new_mode}
    }});
}

void EnergyManager::set_available_current(uint32_t current)
{
    is_on_last = current > 0;
#if MODULE_CHARGE_MANAGER_AVAILABLE()
    charge_manager.set_available_current(current);
#endif
}

void EnergyManager::update_energy()
{
#if !MODULE_CHARGE_MANAGER_AVAILABLE()
    logger.printfln("energy_manager: Module 'Charge Manager' not available. update_energy() does nothing.");
#else
    static SwitchingState prev_state = switching_state;
    if (switching_state != prev_state) {
        logger.printfln("energy_manager: now in state %d", (int)switching_state);
        prev_state = switching_state;
    }

    if (!bricklet_reachable) {
        set_available_current(0);
        return;
    }

    if (switching_state == SwitchingState::Monitoring) {
        if (contactor_check_tripped) {
            set_available_current(0);

            // The contactor check only detects a contactor defect when the contactor should be on.
            // Switch contactor off when a defect is detected, to make sure that it's not energized.
            if (all_data.contactor_value) {
                logger.printfln("energy_manager: Switching off possibly defective contactor.");
                wants_3phase = false;
                switching_state = SwitchingState::Stopping;
                switching_start = millis();
            }
            return;
        }

        const bool     is_on = is_on_last;
        const uint32_t charge_manager_allocated_power_w = 230 * have_phases * charge_manager_allocated_current_ma / 1000; // watt

        if (charging_blocked.combined) {
            if (is_on) {
                phase_state_change_blocked_until = on_state_change_blocked_until = millis() + switching_hysteresis_ms;
            }
            set_available_current(0);
            just_switched_phases = false;
            return;
        }

        // TODO Evil: Allow runtime changes, overrides input pins!
        target_power_from_grid_w    = debug_config.get("target_power_from_grid")->asInt(); // watt

        int32_t p_error_w;
        if (!excess_charging_enable) {
            p_error_w = 0;
        } else {
            p_error_w = target_power_from_grid_w - power_at_meter_w;

            if (p_error_w > 200)
                rgb_led.update_grid_balance(EmRgbLed::GridBalance::Export);
            else if (p_error_w < -200)
                rgb_led.update_grid_balance(EmRgbLed::GridBalance::Import);
            else
                rgb_led.update_grid_balance(EmRgbLed::GridBalance::Balanced);
        }

        switch (mode) {
            case MODE_FAST:
                power_available_w = 230 * 3 * max_current_limited_ma / 1000;
                break;
            case MODE_OFF:
            default:
                power_available_w = 0;
                break;
            case MODE_PV:
            case MODE_MIN_PV:
                // Excess charging enabled; use a simple P controller to adjust available power.
                int32_t p_adjust_w;
                if (!is_on) {
                    // When the power is not on, use p=1 so that the switch-on threshold can be reached properly.
                    p_adjust_w = p_error_w;
                } else {
                    // Some EVs may only be able to adjust their charge power in steps of 1500W,
                    // so smaller factors are required for smaller errors.
                    int32_t p_error_abs_w = abs(p_error_w);
                    if (p_error_abs_w < 1000) {
                        // Use p=0.5 for small differences so that the controller can converge without oscillating too much.
                        p_adjust_w = p_error_w / 2;
                    } else if (p_error_abs_w < 1500) {
                        // Use p=0.75 for medium differences so that the controller can converge reasonably fast while still avoiding too many oscillations.
                        p_adjust_w = p_error_w * 3 / 4;
                    } else {
                        // Use p=0.875 for large differences so that the controller can converge faster.
                        p_adjust_w = p_error_w * 7 / 8;
                    }
                }

                power_available_w = static_cast<int32_t>(charge_manager_allocated_power_w) + p_adjust_w;

                if (mode != MODE_MIN_PV)
                    break;

                // Check against guaranteed power only in MIN_PV mode.
                if (power_available_w < static_cast<int32_t>(guaranteed_power_w))
                    power_available_w = static_cast<int32_t>(guaranteed_power_w);

                break;
        }

        // CP disconnect support unknown if some chargers haven't replied yet.
        if (!charge_manager.seen_all_chargers()) {
            // Don't constantly complain if we don't have any chargers configured.
            if (charge_manager.have_chargers())
                logger.printfln("energy_manager: Not seen all chargers yet.");
            return;
        }

        // Check how many phases are wanted.
        if (phase_switching_mode == PHASE_SWITCHING_ALWAYS_1PHASE) {
            wants_3phase = false;
        } else if (phase_switching_mode == PHASE_SWITCHING_ALWAYS_3PHASE) {
            wants_3phase = true;
        } else { // automatic
            if (is_3phase) {
                wants_3phase = power_available_w >= threshold_3to1_w;
            } else { // is 1phase
                wants_3phase = power_available_w > threshold_1to3_w;
            }
        }

        // Need to get the time here instead of using deadline_elapsed(), to avoid stopping the charge when the phase switch deadline check fails but the start/stop deadline check succeeds.
        uint32_t time_now = millis();

        // Remember last decision change to start hysteresis time.
        if (wants_3phase != wants_3phase_last) {
            logger.printfln("energy_manager: wants_3phase decision changed to %i", wants_3phase);
            phase_state_change_blocked_until = time_now + switching_hysteresis_ms;
            wants_3phase_last = wants_3phase;
        }

        // Check if phase switching is allowed right now.
        bool switch_phases = false;
        if (wants_3phase != is_3phase) {
            if (!contactor_installed) {
                logger.printfln("energy_manager: Phase switch wanted but no contactor installed. Check configuration.");
            } else if (!charge_manager.is_control_pilot_disconnect_supported(time_now - 5000)) {
                logger.printfln("energy_manager: Phase switch wanted but not supported by all chargers.");
            } else if (!uptime_past_hysteresis) {
                // (Re)booted recently. Allow immediate switching.
                logger.printfln("energy_manager: Free phase switch to %s during start-up period. available=%i", wants_3phase ? "3 phases" : "1 phase", power_available_w);
                switch_phases = true;
            } else if (just_switched_mode) {
                // Just switched modes. Allow immediate switching.
                logger.printfln("energy_manager: Free phase switch to %s after changing modes. available=%i", wants_3phase ? "3 phases" : "1 phase", power_available_w);
                switch_phases = true;
            } else if (!is_on && a_after_b(time_now, on_state_change_blocked_until) && a_after_b(time_now, phase_state_change_blocked_until - switching_hysteresis_ms/2)) {
                // On/off deadline passed and at least half of the phase switching deadline passed.
                logger.printfln("energy_manager: Free phase switch to %s while power is off. available=%i", wants_3phase ? "3 phases" : "1 phase", power_available_w);
                switch_phases = true;
            } else if (!a_after_b(time_now, phase_state_change_blocked_until)) {
                //logger.printfln("energy_manager: Phase switch wanted but decision changed too recently. Have to wait another %ums.", phase_state_change_blocked_until - time_now);
            } else {
                logger.printfln("energy_manager wants phase change to %s: available=%i", wants_3phase ? "3 phases" : "1 phase", power_available_w);
                switch_phases = true;
            }
        }

        // Switch phases or deal with what's available.
        if (switch_phases) {
            set_available_current(0);
            switching_state = SwitchingState::Stopping;
            switching_start = time_now;
        } else {
            // Check against overall minimum power, to avoid wanting to switch off when available power is below 3-phase minimum but switch to 1-phase is possible.
            bool wants_on = power_available_w >= overall_min_power_w;

            // Remember last decision change to start hysteresis time.
            if (wants_on != wants_on_last) {
                logger.printfln("energy_manager: wants_on decision changed to %i", wants_on);
                on_state_change_blocked_until = time_now + switching_hysteresis_ms;
                wants_on_last = wants_on;
            }

            uint32_t current_available_ma;
            if (power_available_w <= 0)
                current_available_ma = 0;
            else
                current_available_ma = (power_available_w * 1000) / (230 * have_phases) * wants_on;

            // Check if switching on/off is allowed right now.
            if (wants_on != is_on) {
                if (a_after_b(time_now, on_state_change_blocked_until)) {
                    // Start/stop allowed
                    logger.printfln("energy_manager: Switch %s", wants_on ? "on" : "off");
                } else if (!uptime_past_hysteresis) {
                    // (Re)booted recently. Allow immediate switching.
                    logger.printfln("energy_manager: Free switch-%s during start-up period.", wants_on ? "on" : "off");
                    // Only one immediate switch on/off allowed; mark as used.
                    uptime_past_hysteresis = true;
                } else if (just_switched_mode) {
                    // Just switched modes. Allow immediate switching.
                    logger.printfln("energy_manager: Free switch-%s after changing modes.", wants_on ? "on" : "off");
                } else if (just_switched_phases && a_after_b(time_now, on_state_change_blocked_until - switching_hysteresis_ms/2)) {
                    logger.printfln("energy_manager: Opportunistic switch-%s", wants_on ? "on" : "off");
                } else { // Switched too recently
                    //logger.printfln("energy_manager: Start/stop wanted but decision changed too recently. Have to wait another %ums.", off_state_change_blocked_until - time_now);
                    if (is_on) { // Is on, needs to stay on at minimum current.
                        current_available_ma = min_current_ma;
                    } else { // Is off, needs to stay off.
                        current_available_ma = 0;
                    }
                }
            }

            // Apply minimum/maximum current limits.
            if (current_available_ma < min_current_ma) {
                if (current_available_ma != 0)
                    current_available_ma = min_current_ma;
            } else if (current_available_ma > max_current_limited_ma) {
                current_available_ma = max_current_limited_ma;
            }

            set_available_current(current_available_ma);
            just_switched_phases = false;
            just_switched_mode = false;
        }
    } else if (switching_state == SwitchingState::Stopping) {
        set_available_current(0);

        if (charge_manager.is_charging_stopped(switching_start)) {
            switching_state = SwitchingState::DisconnectingCP;
            switching_start = millis();
        }
    } else if (switching_state == SwitchingState::DisconnectingCP) {
        charge_manager.set_all_control_pilot_disconnect(true);

        if (charge_manager.are_all_control_pilot_disconnected(switching_start)) {
            switching_state = SwitchingState::TogglingContactor;
            switching_start = millis();
        }
    } else if (switching_state == SwitchingState::TogglingContactor) {
        tf_warp_energy_manager_set_contactor(&device, wants_3phase);

        if (all_data.contactor_value == wants_3phase) {
            switching_state = SwitchingState::ConnectingCP;
            switching_start = millis();
        }
    } else if (switching_state == SwitchingState::ConnectingCP) {
        charge_manager.set_all_control_pilot_disconnect(false);

        switching_state = SwitchingState::Monitoring;
        switching_start = 0;

        just_switched_phases = true;
    }
#endif
}

bool EnergyManager::get_sdcard_info(struct sdcard_info *data)
{
    int rc = tf_warp_energy_manager_get_sd_information(
        &device,
        &data->sd_status,
        &data->lfs_status,
        &data->sector_size,
        &data->sector_count,
        &data->card_type,
        &data->product_rev,
        data->product_name,
        &data->manufacturer_id
    );

    // Product name retrieved from the SD card is an unterminated 5-character string, so we have to terminate it here.
    data->product_name[sizeof(data->product_name) - 1] = 0;

    check_bricklet_reachable(rc);

    if (rc != TF_E_OK) {
        set_error(ERROR_FLAGS_SDCARD_MASK);
        logger.printfln("energy_manager: Failed to get SD card information. Error %i", rc);
        return false;
    }

    if (is_error(ERROR_FLAGS_SDCARD_BIT_POS))
        clr_error(ERROR_FLAGS_SDCARD_MASK);

    return true;
}

bool EnergyManager::format_sdcard()
{
    uint8_t ret_format_status;
    int rc = tf_warp_energy_manager_format_sd(&device, 0x4223ABCD, &ret_format_status);

    check_bricklet_reachable(rc);

    return rc == TF_E_OK && ret_format_status == TF_WARP_ENERGY_MANAGER_FORMAT_STATUS_OK;
}

uint16_t EnergyManager::get_energy_meter_detailed_values(float *ret_values)
{
    uint16_t len = 0;
    int rc = tf_warp_energy_manager_get_energy_meter_detailed_values(&device, ret_values, &len);

    check_bricklet_reachable(rc);

    return rc == TF_E_OK ? len : 0;
}

void EnergyManager::set_output(bool output)
{
    int result = tf_warp_energy_manager_set_output(&device, output);

    // Don't check if bricklet is reachable because the setter call won't tell us.

    if (result != TF_E_OK)
        logger.printfln("energy_manager: Failed to set output relay: error %i", result);
}

void EnergyManager::set_rgb_led(uint8_t r, uint8_t g, uint8_t b)
{
    int rc = tf_warp_energy_manager_set_rgb_value(&device, r, g, b);

    // Don't check if bricklet is reachable because the setter call won't tell us.

    if (rc != TF_E_OK)
        logger.printfln("energy_manager: Failed to set RGB LED values: error %i. Continuing anyway.", rc);
}
