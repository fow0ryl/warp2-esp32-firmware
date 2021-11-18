/* ***********************************************************
 * This file was automatically generated on 2021-11-18.      *
 *                                                           *
 * C/C++ for Microcontrollers Bindings Version 2.0.0         *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generators git repository on tinkerforge.com       *
 *************************************************************/


#include "bricklet_ambient_light_v3.h"
#include "base58.h"
#include "endian_convert.h"
#include "errors.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#if TF_IMPLEMENT_CALLBACKS != 0
static bool tf_ambient_light_v3_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    TF_AmbientLightV3 *ambient_light_v3 = (TF_AmbientLightV3 *) dev;
    (void)payload;

    switch (fid) {
        case TF_AMBIENT_LIGHT_V3_CALLBACK_ILLUMINANCE: {
            TF_AmbientLightV3_IlluminanceHandler fn = ambient_light_v3->illuminance_handler;
            void *user_data = ambient_light_v3->illuminance_user_data;
            if (fn == NULL) {
                return false;
            }

            uint32_t illuminance = tf_packet_buffer_read_uint32_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal);
            hal_common->locked = true;
            fn(ambient_light_v3, illuminance, user_data);
            hal_common->locked = false;
            break;
        }

        default:
            return false;
    }

    return true;
}
#else
static bool tf_ambient_light_v3_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    return false;
}
#endif
int tf_ambient_light_v3_create(TF_AmbientLightV3 *ambient_light_v3, const char *uid, TF_HAL *hal) {
    if (ambient_light_v3 == NULL || uid == NULL || hal == NULL) {
        return TF_E_NULL;
    }

    memset(ambient_light_v3, 0, sizeof(TF_AmbientLightV3));

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

    rc = tf_hal_get_tfp(hal, &ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_DEVICE_IDENTIFIER, inventory_index);

    if (rc != TF_E_OK) {
        return rc;
    }

    ambient_light_v3->tfp->device = ambient_light_v3;
    ambient_light_v3->tfp->uid = numeric_uid;
    ambient_light_v3->tfp->cb_handler = tf_ambient_light_v3_callback_handler;
    ambient_light_v3->response_expected[0] = 0x01;

    return TF_E_OK;
}

int tf_ambient_light_v3_destroy(TF_AmbientLightV3 *ambient_light_v3) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    int result = tf_tfp_destroy(ambient_light_v3->tfp);
    ambient_light_v3->tfp = NULL;

    return result;
}

int tf_ambient_light_v3_get_response_expected(TF_AmbientLightV3 *ambient_light_v3, uint8_t function_id, bool *ret_response_expected) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_ILLUMINANCE_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 0)) != 0;
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 1)) != 0;
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 2)) != 0;
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_STATUS_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 3)) != 0;
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_RESET:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 4)) != 0;
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_WRITE_UID:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (ambient_light_v3->response_expected[0] & (1 << 5)) != 0;
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_ambient_light_v3_set_response_expected(TF_AmbientLightV3 *ambient_light_v3, uint8_t function_id, bool response_expected) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_ILLUMINANCE_CALLBACK_CONFIGURATION:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 0);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 0);
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_CONFIGURATION:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 1);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 1);
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 2);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 2);
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_SET_STATUS_LED_CONFIG:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 3);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 3);
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_RESET:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 4);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 4);
            }
            break;
        case TF_AMBIENT_LIGHT_V3_FUNCTION_WRITE_UID:
            if (response_expected) {
                ambient_light_v3->response_expected[0] |= (1 << 5);
            } else {
                ambient_light_v3->response_expected[0] &= ~(1 << 5);
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_ambient_light_v3_set_response_expected_all(TF_AmbientLightV3 *ambient_light_v3, bool response_expected) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    memset(ambient_light_v3->response_expected, response_expected ? 0xFF : 0, 1);

    return TF_E_OK;
}

int tf_ambient_light_v3_get_illuminance(TF_AmbientLightV3 *ambient_light_v3, uint32_t *ret_illuminance) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_ILLUMINANCE, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_illuminance != NULL) { *ret_illuminance = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_set_illuminance_callback_configuration(TF_AmbientLightV3 *ambient_light_v3, uint32_t period, bool value_has_to_change, char option, uint32_t min, uint32_t max) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_ILLUMINANCE_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_ILLUMINANCE_CALLBACK_CONFIGURATION, 14, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    period = tf_leconvert_uint32_to(period); memcpy(buf + 0, &period, 4);
    buf[4] = value_has_to_change ? 1 : 0;
    buf[5] = (uint8_t)option;
    min = tf_leconvert_uint32_to(min); memcpy(buf + 6, &min, 4);
    max = tf_leconvert_uint32_to(max); memcpy(buf + 10, &max, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_illuminance_callback_configuration(TF_AmbientLightV3 *ambient_light_v3, uint32_t *ret_period, bool *ret_value_has_to_change, char *ret_option, uint32_t *ret_min, uint32_t *ret_max) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_ILLUMINANCE_CALLBACK_CONFIGURATION, 0, 14, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_period != NULL) { *ret_period = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        if (ret_value_has_to_change != NULL) { *ret_value_has_to_change = tf_packet_buffer_read_bool(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        if (ret_option != NULL) { *ret_option = tf_packet_buffer_read_char(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        if (ret_min != NULL) { *ret_min = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        if (ret_max != NULL) { *ret_max = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_set_configuration(TF_AmbientLightV3 *ambient_light_v3, uint8_t illuminance_range, uint8_t integration_time) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_CONFIGURATION, 2, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    buf[0] = (uint8_t)illuminance_range;
    buf[1] = (uint8_t)integration_time;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_configuration(TF_AmbientLightV3 *ambient_light_v3, uint8_t *ret_illuminance_range, uint8_t *ret_integration_time) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_CONFIGURATION, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_illuminance_range != NULL) { *ret_illuminance_range = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        if (ret_integration_time != NULL) { *ret_integration_time = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_spitfp_error_count(TF_AmbientLightV3 *ambient_light_v3, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_SPITFP_ERROR_COUNT, 0, 16, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_error_count_ack_checksum != NULL) { *ret_error_count_ack_checksum = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_message_checksum != NULL) { *ret_error_count_message_checksum = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_frame != NULL) { *ret_error_count_frame = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_overflow != NULL) { *ret_error_count_overflow = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_set_bootloader_mode(TF_AmbientLightV3 *ambient_light_v3, uint8_t mode, uint8_t *ret_status) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_BOOTLOADER_MODE, 1, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    buf[0] = (uint8_t)mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_bootloader_mode(TF_AmbientLightV3 *ambient_light_v3, uint8_t *ret_mode) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_BOOTLOADER_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_set_write_firmware_pointer(TF_AmbientLightV3 *ambient_light_v3, uint32_t pointer) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_WRITE_FIRMWARE_POINTER, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_WRITE_FIRMWARE_POINTER, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    pointer = tf_leconvert_uint32_to(pointer); memcpy(buf + 0, &pointer, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_write_firmware(TF_AmbientLightV3 *ambient_light_v3, const uint8_t data[64], uint8_t *ret_status) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_WRITE_FIRMWARE, 64, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    memcpy(buf + 0, data, 64);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_set_status_led_config(TF_AmbientLightV3 *ambient_light_v3, uint8_t config) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_STATUS_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_SET_STATUS_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_status_led_config(TF_AmbientLightV3 *ambient_light_v3, uint8_t *ret_config) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_STATUS_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_chip_temperature(TF_AmbientLightV3 *ambient_light_v3, int16_t *ret_temperature) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_CHIP_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_reset(TF_AmbientLightV3 *ambient_light_v3) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_RESET, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_RESET, 0, 0, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_write_uid(TF_AmbientLightV3 *ambient_light_v3, uint32_t uid) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_ambient_light_v3_get_response_expected(ambient_light_v3, TF_AMBIENT_LIGHT_V3_FUNCTION_WRITE_UID, &response_expected);
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_WRITE_UID, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(ambient_light_v3->tfp);

    uid = tf_leconvert_uint32_to(uid); memcpy(buf + 0, &uid, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_read_uid(TF_AmbientLightV3 *ambient_light_v3, uint32_t *ret_uid) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_READ_UID, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_uid != NULL) { *ret_uid = tf_packet_buffer_read_uint32_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_ambient_light_v3_get_identity(TF_AmbientLightV3 *ambient_light_v3, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(ambient_light_v3->tfp, TF_AMBIENT_LIGHT_V3_FUNCTION_GET_IDENTITY, 0, 25, response_expected);

    size_t i;
    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + tf_hal_get_common((TF_HAL *)ambient_light_v3->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(ambient_light_v3->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        char tmp_connected_uid[8] = {0};
        if (ret_uid != NULL) { tf_packet_buffer_pop_n(&ambient_light_v3->tfp->spitfp->recv_buf, (uint8_t*)ret_uid, 8);} else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 8); }
        tf_packet_buffer_pop_n(&ambient_light_v3->tfp->spitfp->recv_buf, (uint8_t*)tmp_connected_uid, 8);
        if (ret_position != NULL) { *ret_position = tf_packet_buffer_read_char(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 1); }
        if (ret_hardware_version != NULL) { for (i = 0; i < 3; ++i) ret_hardware_version[i] = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 3); }
        if (ret_firmware_version != NULL) { for (i = 0; i < 3; ++i) ret_firmware_version[i] = tf_packet_buffer_read_uint8_t(&ambient_light_v3->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 3); }
        if (ret_device_identifier != NULL) { *ret_device_identifier = tf_packet_buffer_read_uint16_t(&ambient_light_v3->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&ambient_light_v3->tfp->spitfp->recv_buf, 2); }
        if (tmp_connected_uid[0] == 0 && ret_position != NULL) {
            *ret_position = tf_hal_get_port_name((TF_HAL *)ambient_light_v3->tfp->hal, ambient_light_v3->tfp->spitfp->port_id);
        }
        if (ret_connected_uid != NULL) {
            memcpy(ret_connected_uid, tmp_connected_uid, 8);
        }
        tf_tfp_packet_processed(ambient_light_v3->tfp);
    }

    result = tf_tfp_finish_send(ambient_light_v3->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}
#if TF_IMPLEMENT_CALLBACKS != 0
int tf_ambient_light_v3_register_illuminance_callback(TF_AmbientLightV3 *ambient_light_v3, TF_AmbientLightV3_IlluminanceHandler handler, void *user_data) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        ambient_light_v3->tfp->needs_callback_tick = false;
    } else {
        ambient_light_v3->tfp->needs_callback_tick = true;
    }

    ambient_light_v3->illuminance_handler = handler;
    ambient_light_v3->illuminance_user_data = user_data;

    return TF_E_OK;
}
#endif
int tf_ambient_light_v3_callback_tick(TF_AmbientLightV3 *ambient_light_v3, uint32_t timeout_us) {
    if (ambient_light_v3 == NULL) {
        return TF_E_NULL;
    }

    return tf_tfp_callback_tick(ambient_light_v3->tfp, tf_hal_current_time_us((TF_HAL *)ambient_light_v3->tfp->hal) + timeout_us);
}

#ifdef __cplusplus
}
#endif