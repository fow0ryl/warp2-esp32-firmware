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

#include "em_sdcard.h"

#include "api.h"
#include "modules.h"
#include "task_scheduler.h"

void EMSDcard::pre_setup()
{
    sdcard_state = Config::Object({
        {"sd_status",       Config::Uint32(0)},
        {"lfs_status",      Config::Uint32(0)},
        {"card_type",       Config::Uint32(0)},
        {"sector_count",    Config::Uint32(0)},
        {"sector_size",     Config::Uint16(0)},
        {"manufacturer_id", Config::Uint8(0)},
        {"product_rev",     Config::Uint8(0)},
        {"product_name",    Config::Str(".....", 5, 5)},
    });
}

void EMSDcard::setup()
{
    update_sdcard_info();

    task_scheduler.scheduleWithFixedDelay([this](){
        update_sdcard_info();
    }, 10 * 1000, 10 * 1000); // 10s
}

void EMSDcard::register_urls()
{
    api.addState("em_sdcard/state", &sdcard_state, {}, 1000);

    api.addRawCommand("em_sdcard_format", [this](char *c, size_t s) -> String {
        StaticJsonDocument<16> doc;

        DeserializationError error = deserializeJson(doc, c, s);

        if (error) {
            return String("Failed to deserialize string: ") + String(error.c_str());
        }

        if (!doc["do_i_know_what_i_am_doing"].is<bool>()) {
            return "you don't seem to know what you are doing";
        }

        if (!doc["do_i_know_what_i_am_doing"].as<bool>()) {
            return "SD card format NOT initiated";
        }

        task_scheduler.scheduleOnce([this](){
            logger.printfln("SD card format requested");
            format_sdcard();
        }, 0);

        return "";
    }, true);
}
void EMSDcard::loop()
{
}

void EMSDcard::format_sdcard()
{

}

void EMSDcard::update_sdcard_info()
{
    struct sdcard_info data;
    bzero(&data, sizeof(data));

    energy_manager.get_sdcard_info(&data);

    sdcard_state.get("sd_status")->updateUint(data.sd_status);
    sdcard_state.get("lfs_status")->updateUint(data.lfs_status);
    sdcard_state.get("card_type")->updateUint(data.card_type);
    sdcard_state.get("sector_count")->updateUint(data.sector_count);
    sdcard_state.get("sector_size")->updateUint(data.sector_size);
    sdcard_state.get("manufacturer_id")->updateUint(data.manufacturer_id);
    sdcard_state.get("product_rev")->updateUint(data.product_rev);
    sdcard_state.get("product_name")->updateString(data.product_name);
}