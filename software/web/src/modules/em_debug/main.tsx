/* esp32-firmware
 * Copyright (C) 2023 Mattias Schäffersmann <mattias@tinkerforge.com>
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

import $ from "../../ts/jq";

import * as API  from "../../ts/api";
import * as util from "../../ts/util";
import { __ }    from "../../ts/translation";

import { h, render, Fragment, Component } from "preact";
import { Button } from "react-bootstrap";

import { FormRow        } from "../../ts/components/form_row";
import { FormSeparator  } from "../../ts/components/form_separator";
import { IndicatorGroup } from "../../ts/components/indicator_group";
import { InputFloat     } from "../../ts/components/input_float";
import { InputNumber    } from "../../ts/components/input_number";
import { InputText      } from "../../ts/components/input_text";
import { PageHeader     } from "../../ts/components/page_header";

interface EMDebugState {
    state: API.getType['energy_manager/state'];
    debug_running: boolean;
    debug_status: string;
}

export class EMDebug extends Component<{}, EMDebugState> {
    debug_log = "";

    constructor() {
        super();

        util.eventTarget.addEventListener('energy_manager/state', () => {
            this.setState({state: API.get('energy_manager/state')});
        });

        util.eventTarget.addEventListener("energy_manager/debug_header", (e) => {
            this.debug_log += e.data + "\n";
        }, false);

        util.eventTarget.addEventListener("energy_manager/debug", (e) => {
            this.debug_log += e.data + "\n";
        }, false);
    }

    async get_debug_report_and_event_log() {
        try {
            this.setState({debug_status: __("em_debug.script.loading_debug_report")});
            this.debug_log += await util.download("/debug_report").then(blob => blob.text());
            this.debug_log += "\n\n";
        } catch (error) {
            this.setState({debug_running: false, debug_status: __("em_debug.script.loading_debug_report_failed")});
            throw __("em_debug.script.loading_debug_report_failed") + ": " + error;
        }

        try {
            this.setState({debug_status: __("em_debug.script.loading_event_log")});
            this.debug_log += await util.download("/event_log").then(blob => blob.text());
            this.debug_log += "\n";
        } catch (error) {
            this.setState({debug_running: false, debug_status: __("em_debug.script.loading_event_log_failed")});
            throw __("em_debug.script.loading_event_log_failed") + ": " + error;
        }
    }

    debugTimeout: number;

    async resetDebugWd() {
        try {
            await util.download("/energy_manager/continue_debug");
        }
        catch{
            this.setState({debug_running: false, debug_status: __("em_debug.script.starting_debug_failed")});
        }
    }

    async debug_start() {
        this.debug_log = "";
        this.setState({debug_running: true});

        try {
            await this.get_debug_report_and_event_log();

            this.setState({debug_status: __("em_debug.script.starting_debug")});
        } catch(error) {
            this.setState({debug_running: false, debug_status: error});
            return;
        }

        try{
            await util.download("/energy_manager/start_debug");
        } catch {
            this.setState({debug_running: false, debug_status: __("em_debug.script.starting_debug_failed")});
            return;
        }

        this.debugTimeout = setInterval(this.resetDebugWd, 15000);

        this.setState({debug_status: __("em_debug.script.debug_running")});
    }

    async debug_stop() {
        this.setState({debug_running: false});
        clearInterval(this.debugTimeout);

        try {
            await util.download("/energy_manager/stop_debug");
        } catch {
            this.setState({debug_running: true, debug_status: __("em_debug.script.debug_stop_failed")});
        }

        try {
            this.debug_log += "\n\n";
            this.setState({debug_status: __("em_debug.script.debug_stopped")});

            await this.get_debug_report_and_event_log();
            this.setState({debug_status: __("em_debug.script.debug_done")});
        } catch (error) {
            this.debug_log += "\n\nError while stopping charge protocol: ";
            this.debug_log += error;

            this.setState({debug_status: error});
        }

        //Download log in any case: Even an incomplete log can be useful for debugging.
        util.downloadToFile(this.debug_log, "energy_manager-debug-log", "txt", "text/plain");
    }

    render(props: {}, s: Readonly<EMDebugState>) {
        if (!s || !s.state) {
            return (<></>);
        }

        if (s.debug_running) {
            window.onbeforeunload = (e: Event) => {
                e.preventDefault();
                // returnValue is not a boolean, but the string to be shown
                // in the "are you sure you want to close this tab" message
                // box. However this string is only shown in some browsers.
                e.returnValue = __("em_debug.script.tab_close_warning") as any;
            }
        } else {
            window.onbeforeunload = null;
        }

        return (
            <>
                <PageHeader title={__("em_debug.content.em_debug")} colClasses="col-xl-10"/>

                <FormSeparator heading={__("em_debug.content.protocol")} />
                <FormRow label={__("em_debug.content.protocol_description")} label_muted={__("em_debug.content.protocol_description_muted")}>
                    <div class="input-group pb-2">
                        <Button variant="primary" className="form-control rounded-right mr-2" onClick={() => { this.debug_start() }} disabled={s.debug_running}>{__("em_debug.content.protocol_start")}</Button>
                        <Button variant="primary" className="form-control rounded-left" onClick={() => { this.debug_stop() }} disabled={!s.debug_running}>{__("em_debug.content.protocol_stop")}</Button>
                    </div>
                    <InputText value={s.debug_status} />
                </FormRow>

                <FormSeparator heading={__("em_debug.content.low_level_state")} />
                <FormRow label={__("em_debug.content.contactor_control")}>
                    <IndicatorGroup
                        value={s.state.contactor ? 1 : 0}
                        items={[
                            ["secondary", __("em_debug.content.contactor_off")],
                            ["primary", __("em_debug.content.contactor_on")],
                        ]} />
                </FormRow>

                <FormRow label={__("em_debug.content.contactor_check")}>
                    <IndicatorGroup
                        value={s.state.contactor_check_state ? 0 : 1} // intentionally inverted, OK is first
                        items={[
                            ["success", __("em_debug.content.contactor_check_ok")],
                            ["danger", __("em_debug.content.contactor_check_fail")],
                        ]} />
                </FormRow>

                <FormRow label={__("em_debug.content.state_led")} label_muted={__("em_debug.content.state_led_names")}>
                    <div class="row mx-n1">
                        {s.state.led_rgb.map((x, i) => (
                            <div key={i} class="mb-1 col-4 px-1">
                                <InputText value={x} />
                            </div>
                        ))}
                    </div>
                </FormRow>

                <FormRow label={__("em_debug.content.gpios")} label_muted={__("em_debug.content.gpio_names_0")}>
                    <div class="row mx-n1">
                        {[...s.state.gpio_input_state, s.state.gpio_output_state].map((x, j) => (
                            <IndicatorGroup vertical key={j} class="mb-1 col px-1"
                                value={x ? 0 : 1} //intentionally inverted: the high button is the first
                                items={[
                                    ["primary", __("em_debug.content.high")],
                                    ["secondary", __("em_debug.content.low")]
                                ]} />
                        ))}
                    </div>
                </FormRow>

                <FormRow label={__("em_debug.content.state_input_voltage")}>
                    <InputFloat value={s.state.input_voltage} digits={3} unit={'V'} />
                </FormRow>
            </>
        )
    }
}

render(<EMDebug />, $('#em_debug')[0]);

export function init() {
}

export function add_event_listeners(source: API.APIEventTarget) {
}

export function update_sidebar_state(module_init: any) {
    $('#sidebar-em_debug').prop('hidden', !module_init.energy_manager);
}