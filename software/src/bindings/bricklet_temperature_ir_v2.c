/* ***********************************************************
 * This file was automatically generated on 2021-11-18.      *
 *                                                           *
 * C/C++ for Microcontrollers Bindings Version 2.0.0         *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generators git repository on tinkerforge.com       *
 *************************************************************/


#include "bricklet_temperature_ir_v2.h"
#include "base58.h"
#include "endian_convert.h"
#include "errors.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#if TF_IMPLEMENT_CALLBACKS != 0
static bool tf_temperature_ir_v2_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    TF_TemperatureIRV2 *temperature_ir_v2 = (TF_TemperatureIRV2 *) dev;
    (void)payload;

    switch (fid) {
        case TF_TEMPERATURE_IR_V2_CALLBACK_AMBIENT_TEMPERATURE: {
            TF_TemperatureIRV2_AmbientTemperatureHandler fn = temperature_ir_v2->ambient_temperature_handler;
            void *user_data = temperature_ir_v2->ambient_temperature_user_data;
            if (fn == NULL) {
                return false;
            }

            int16_t temperature = tf_packet_buffer_read_int16_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal);
            hal_common->locked = true;
            fn(temperature_ir_v2, temperature, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_TEMPERATURE_IR_V2_CALLBACK_OBJECT_TEMPERATURE: {
            TF_TemperatureIRV2_ObjectTemperatureHandler fn = temperature_ir_v2->object_temperature_handler;
            void *user_data = temperature_ir_v2->object_temperature_user_data;
            if (fn == NULL) {
                return false;
            }

            int16_t temperature = tf_packet_buffer_read_int16_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal);
            hal_common->locked = true;
            fn(temperature_ir_v2, temperature, user_data);
            hal_common->locked = false;
            break;
        }

        default:
            return false;
    }

    return true;
}
#else
static bool tf_temperature_ir_v2_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    return false;
}
#endif
int tf_temperature_ir_v2_create(TF_TemperatureIRV2 *temperature_ir_v2, const char *uid, TF_HAL *hal) {
    if (temperature_ir_v2 == NULL || uid == NULL || hal == NULL) {
        return TF_E_NULL;
    }

    memset(temperature_ir_v2, 0, sizeof(TF_TemperatureIRV2));

    uint32_t numeric_uid;
    int rc = tf_base58_decode(uid, &numeric_uid);

    if (rc != TF_E_OK) {
        return rc;
    }

    uint8_t port_id;
    uint8_t inventory_index;
    rc = tf_hal_get_port_id(hal, numeric_uid, &port_id, &inventory_index);

    if (rc < 0) {
        return rc;
    }

    rc = tf_hal_get_tfp(hal, &temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_DEVICE_IDENTIFIER, inventory_index);

    if (rc != TF_E_OK) {
        return rc;
    }

    temperature_ir_v2->tfp->device = temperature_ir_v2;
    temperature_ir_v2->tfp->uid = numeric_uid;
    temperature_ir_v2->tfp->cb_handler = tf_temperature_ir_v2_callback_handler;
    temperature_ir_v2->response_expected[0] = 0x03;

    return TF_E_OK;
}

int tf_temperature_ir_v2_destroy(TF_TemperatureIRV2 *temperature_ir_v2) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    int result = tf_tfp_destroy(temperature_ir_v2->tfp);
    temperature_ir_v2->tfp = NULL;

    return result;
}

int tf_temperature_ir_v2_get_response_expected(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t function_id, bool *ret_response_expected) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 0)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 1)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_EMISSIVITY:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 2)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 3)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 4)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_RESET:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 5)) != 0;
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_WRITE_UID:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (temperature_ir_v2->response_expected[0] & (1 << 6)) != 0;
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_temperature_ir_v2_set_response_expected(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t function_id, bool response_expected) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 0);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 0);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 1);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 1);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_EMISSIVITY:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 2);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 2);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 3);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 3);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 4);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 4);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_RESET:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 5);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 5);
            }
            break;
        case TF_TEMPERATURE_IR_V2_FUNCTION_WRITE_UID:
            if (response_expected) {
                temperature_ir_v2->response_expected[0] |= (1 << 6);
            } else {
                temperature_ir_v2->response_expected[0] &= ~(1 << 6);
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_temperature_ir_v2_set_response_expected_all(TF_TemperatureIRV2 *temperature_ir_v2, bool response_expected) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    memset(temperature_ir_v2->response_expected, response_expected ? 0xFF : 0, 1);

    return TF_E_OK;
}

int tf_temperature_ir_v2_get_ambient_temperature(TF_TemperatureIRV2 *temperature_ir_v2, int16_t *ret_temperature) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_AMBIENT_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_ambient_temperature_callback_configuration(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t period, bool value_has_to_change, char option, int16_t min, int16_t max) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION, 10, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    period = tf_leconvert_uint32_to(period); memcpy(buf + 0, &period, 4);
    buf[4] = value_has_to_change ? 1 : 0;
    buf[5] = (uint8_t)option;
    min = tf_leconvert_int16_to(min); memcpy(buf + 6, &min, 2);
    max = tf_leconvert_int16_to(max); memcpy(buf + 8, &max, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_ambient_temperature_callback_configuration(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t *ret_period, bool *ret_value_has_to_change, char *ret_option, int16_t *ret_min, int16_t *ret_max) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION, 0, 10, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_period != NULL) { *ret_period = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_value_has_to_change != NULL) { *ret_value_has_to_change = tf_packet_buffer_read_bool(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_option != NULL) { *ret_option = tf_packet_buffer_read_char(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_min != NULL) { *ret_min = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        if (ret_max != NULL) { *ret_max = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_object_temperature(TF_TemperatureIRV2 *temperature_ir_v2, int16_t *ret_temperature) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_OBJECT_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_object_temperature_callback_configuration(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t period, bool value_has_to_change, char option, int16_t min, int16_t max) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION, 10, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    period = tf_leconvert_uint32_to(period); memcpy(buf + 0, &period, 4);
    buf[4] = value_has_to_change ? 1 : 0;
    buf[5] = (uint8_t)option;
    min = tf_leconvert_int16_to(min); memcpy(buf + 6, &min, 2);
    max = tf_leconvert_int16_to(max); memcpy(buf + 8, &max, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_object_temperature_callback_configuration(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t *ret_period, bool *ret_value_has_to_change, char *ret_option, int16_t *ret_min, int16_t *ret_max) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION, 0, 10, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_period != NULL) { *ret_period = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_value_has_to_change != NULL) { *ret_value_has_to_change = tf_packet_buffer_read_bool(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_option != NULL) { *ret_option = tf_packet_buffer_read_char(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_min != NULL) { *ret_min = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        if (ret_max != NULL) { *ret_max = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_emissivity(TF_TemperatureIRV2 *temperature_ir_v2, uint16_t emissivity) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_SET_EMISSIVITY, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_EMISSIVITY, 2, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    emissivity = tf_leconvert_uint16_to(emissivity); memcpy(buf + 0, &emissivity, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_emissivity(TF_TemperatureIRV2 *temperature_ir_v2, uint16_t *ret_emissivity) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_EMISSIVITY, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_emissivity != NULL) { *ret_emissivity = tf_packet_buffer_read_uint16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_spitfp_error_count(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_SPITFP_ERROR_COUNT, 0, 16, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_error_count_ack_checksum != NULL) { *ret_error_count_ack_checksum = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_message_checksum != NULL) { *ret_error_count_message_checksum = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_frame != NULL) { *ret_error_count_frame = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_overflow != NULL) { *ret_error_count_overflow = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_bootloader_mode(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t mode, uint8_t *ret_status) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_BOOTLOADER_MODE, 1, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    buf[0] = (uint8_t)mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_bootloader_mode(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t *ret_mode) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_BOOTLOADER_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_write_firmware_pointer(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t pointer) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    pointer = tf_leconvert_uint32_to(pointer); memcpy(buf + 0, &pointer, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_write_firmware(TF_TemperatureIRV2 *temperature_ir_v2, const uint8_t data[64], uint8_t *ret_status) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_WRITE_FIRMWARE, 64, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    memcpy(buf + 0, data, 64);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_set_status_led_config(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t config) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_SET_STATUS_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_SET_STATUS_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_status_led_config(TF_TemperatureIRV2 *temperature_ir_v2, uint8_t *ret_config) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_STATUS_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_chip_temperature(TF_TemperatureIRV2 *temperature_ir_v2, int16_t *ret_temperature) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_CHIP_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_reset(TF_TemperatureIRV2 *temperature_ir_v2) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_RESET, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_RESET, 0, 0, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_write_uid(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t uid) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_temperature_ir_v2_get_response_expected(temperature_ir_v2, TF_TEMPERATURE_IR_V2_FUNCTION_WRITE_UID, &response_expected);
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_WRITE_UID, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(temperature_ir_v2->tfp);

    uid = tf_leconvert_uint32_to(uid); memcpy(buf + 0, &uid, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_read_uid(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t *ret_uid) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_READ_UID, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_uid != NULL) { *ret_uid = tf_packet_buffer_read_uint32_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_temperature_ir_v2_get_identity(TF_TemperatureIRV2 *temperature_ir_v2, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(temperature_ir_v2->tfp, TF_TEMPERATURE_IR_V2_FUNCTION_GET_IDENTITY, 0, 25, response_expected);

    size_t i;
    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)temperature_ir_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(temperature_ir_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        char tmp_connected_uid[8] = {0};
        if (ret_uid != NULL) { tf_packet_buffer_pop_n(&temperature_ir_v2->tfp->spitfp->recv_buf, (uint8_t*)ret_uid, 8);} else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 8); }
        tf_packet_buffer_pop_n(&temperature_ir_v2->tfp->spitfp->recv_buf, (uint8_t*)tmp_connected_uid, 8);
        if (ret_position != NULL) { *ret_position = tf_packet_buffer_read_char(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_hardware_version != NULL) { for (i = 0; i < 3; ++i) ret_hardware_version[i] = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 3); }
        if (ret_firmware_version != NULL) { for (i = 0; i < 3; ++i) ret_firmware_version[i] = tf_packet_buffer_read_uint8_t(&temperature_ir_v2->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 3); }
        if (ret_device_identifier != NULL) { *ret_device_identifier = tf_packet_buffer_read_uint16_t(&temperature_ir_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&temperature_ir_v2->tfp->spitfp->recv_buf, 2); }
        if (tmp_connected_uid[0] == 0 && ret_position != NULL) {
            *ret_position = tf_hal_get_port_name((TF_HAL *)temperature_ir_v2->tfp->hal, temperature_ir_v2->tfp->spitfp->port_id);
        }
        if (ret_connected_uid != NULL) {
            memcpy(ret_connected_uid, tmp_connected_uid, 8);
        }
        tf_tfp_packet_processed(temperature_ir_v2->tfp);
    }

    result = tf_tfp_finish_send(temperature_ir_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}
#if TF_IMPLEMENT_CALLBACKS != 0
int tf_temperature_ir_v2_register_ambient_temperature_callback(TF_TemperatureIRV2 *temperature_ir_v2, TF_TemperatureIRV2_AmbientTemperatureHandler handler, void *user_data) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        temperature_ir_v2->tfp->needs_callback_tick = false;
        temperature_ir_v2->tfp->needs_callback_tick |= temperature_ir_v2->object_temperature_handler != NULL;
    } else {
        temperature_ir_v2->tfp->needs_callback_tick = true;
    }

    temperature_ir_v2->ambient_temperature_handler = handler;
    temperature_ir_v2->ambient_temperature_user_data = user_data;

    return TF_E_OK;
}


int tf_temperature_ir_v2_register_object_temperature_callback(TF_TemperatureIRV2 *temperature_ir_v2, TF_TemperatureIRV2_ObjectTemperatureHandler handler, void *user_data) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        temperature_ir_v2->tfp->needs_callback_tick = false;
        temperature_ir_v2->tfp->needs_callback_tick |= temperature_ir_v2->ambient_temperature_handler != NULL;
    } else {
        temperature_ir_v2->tfp->needs_callback_tick = true;
    }

    temperature_ir_v2->object_temperature_handler = handler;
    temperature_ir_v2->object_temperature_user_data = user_data;

    return TF_E_OK;
}
#endif
int tf_temperature_ir_v2_callback_tick(TF_TemperatureIRV2 *temperature_ir_v2, uint32_t timeout_us) {
    if (temperature_ir_v2 == NULL) {
        return TF_E_NULL;
    }

    return tf_tfp_callback_tick(temperature_ir_v2->tfp, tf_hal_current_time_us((TF_HAL *)temperature_ir_v2->tfp->hal) + timeout_us);
}

#ifdef __cplusplus
}
#endif