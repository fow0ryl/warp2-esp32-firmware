/* ***********************************************************
 * This file was automatically generated on 2021-11-18.      *
 *                                                           *
 * C/C++ for Microcontrollers Bindings Version 2.0.0         *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generators git repository on tinkerforge.com       *
 *************************************************************/


#include "bricklet_dc_v2.h"
#include "base58.h"
#include "endian_convert.h"
#include "errors.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#if TF_IMPLEMENT_CALLBACKS != 0
static bool tf_dc_v2_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    TF_DCV2 *dc_v2 = (TF_DCV2 *) dev;
    (void)payload;

    switch (fid) {
        case TF_DC_V2_CALLBACK_EMERGENCY_SHUTDOWN: {
            TF_DCV2_EmergencyShutdownHandler fn = dc_v2->emergency_shutdown_handler;
            void *user_data = dc_v2->emergency_shutdown_user_data;
            if (fn == NULL) {
                return false;
            }


            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal);
            hal_common->locked = true;
            fn(dc_v2, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_DC_V2_CALLBACK_VELOCITY_REACHED: {
            TF_DCV2_VelocityReachedHandler fn = dc_v2->velocity_reached_handler;
            void *user_data = dc_v2->velocity_reached_user_data;
            if (fn == NULL) {
                return false;
            }

            int16_t velocity = tf_packet_buffer_read_int16_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal);
            hal_common->locked = true;
            fn(dc_v2, velocity, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_DC_V2_CALLBACK_CURRENT_VELOCITY: {
            TF_DCV2_CurrentVelocityHandler fn = dc_v2->current_velocity_handler;
            void *user_data = dc_v2->current_velocity_user_data;
            if (fn == NULL) {
                return false;
            }

            int16_t velocity = tf_packet_buffer_read_int16_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal);
            hal_common->locked = true;
            fn(dc_v2, velocity, user_data);
            hal_common->locked = false;
            break;
        }

        default:
            return false;
    }

    return true;
}
#else
static bool tf_dc_v2_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    return false;
}
#endif
int tf_dc_v2_create(TF_DCV2 *dc_v2, const char *uid, TF_HAL *hal) {
    if (dc_v2 == NULL || uid == NULL || hal == NULL) {
        return TF_E_NULL;
    }

    memset(dc_v2, 0, sizeof(TF_DCV2));

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

    rc = tf_hal_get_tfp(hal, &dc_v2->tfp, TF_DC_V2_DEVICE_IDENTIFIER, inventory_index);

    if (rc != TF_E_OK) {
        return rc;
    }

    dc_v2->tfp->device = dc_v2;
    dc_v2->tfp->uid = numeric_uid;
    dc_v2->tfp->cb_handler = tf_dc_v2_callback_handler;
    dc_v2->response_expected[0] = 0x80;
    dc_v2->response_expected[1] = 0x03;

    return TF_E_OK;
}

int tf_dc_v2_destroy(TF_DCV2 *dc_v2) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    int result = tf_tfp_destroy(dc_v2->tfp);
    dc_v2->tfp = NULL;

    return result;
}

int tf_dc_v2_get_response_expected(TF_DCV2 *dc_v2, uint8_t function_id, bool *ret_response_expected) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_DC_V2_FUNCTION_SET_ENABLED:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 0)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_VELOCITY:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 1)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_MOTION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 2)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_FULL_BRAKE:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 3)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_DRIVE_MODE:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 4)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_PWM_FREQUENCY:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 5)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_ERROR_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 6)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_EMERGENCY_SHUTDOWN_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[0] & (1 << 7)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_VELOCITY_REACHED_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 0)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_CURRENT_VELOCITY_CALLBACK_CONFIGURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 1)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 2)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 3)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_RESET:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 4)) != 0;
            }
            break;
        case TF_DC_V2_FUNCTION_WRITE_UID:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dc_v2->response_expected[1] & (1 << 5)) != 0;
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_dc_v2_set_response_expected(TF_DCV2 *dc_v2, uint8_t function_id, bool response_expected) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_DC_V2_FUNCTION_SET_ENABLED:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 0);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 0);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_VELOCITY:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 1);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 1);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_MOTION:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 2);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 2);
            }
            break;
        case TF_DC_V2_FUNCTION_FULL_BRAKE:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 3);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 3);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_DRIVE_MODE:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 4);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 4);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_PWM_FREQUENCY:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 5);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 5);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_ERROR_LED_CONFIG:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 6);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 6);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_EMERGENCY_SHUTDOWN_CALLBACK_CONFIGURATION:
            if (response_expected) {
                dc_v2->response_expected[0] |= (1 << 7);
            } else {
                dc_v2->response_expected[0] &= ~(1 << 7);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_VELOCITY_REACHED_CALLBACK_CONFIGURATION:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 0);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 0);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_CURRENT_VELOCITY_CALLBACK_CONFIGURATION:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 1);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 1);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 2);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 2);
            }
            break;
        case TF_DC_V2_FUNCTION_SET_STATUS_LED_CONFIG:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 3);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 3);
            }
            break;
        case TF_DC_V2_FUNCTION_RESET:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 4);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 4);
            }
            break;
        case TF_DC_V2_FUNCTION_WRITE_UID:
            if (response_expected) {
                dc_v2->response_expected[1] |= (1 << 5);
            } else {
                dc_v2->response_expected[1] &= ~(1 << 5);
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_dc_v2_set_response_expected_all(TF_DCV2 *dc_v2, bool response_expected) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    memset(dc_v2->response_expected, response_expected ? 0xFF : 0, 2);

    return TF_E_OK;
}

int tf_dc_v2_set_enabled(TF_DCV2 *dc_v2, bool enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_ENABLED, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_ENABLED, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = enabled ? 1 : 0;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_enabled(TF_DCV2 *dc_v2, bool *ret_enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_ENABLED, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_enabled != NULL) { *ret_enabled = tf_packet_buffer_read_bool(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_velocity(TF_DCV2 *dc_v2, int16_t velocity) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_VELOCITY, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_VELOCITY, 2, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    velocity = tf_leconvert_int16_to(velocity); memcpy(buf + 0, &velocity, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_velocity(TF_DCV2 *dc_v2, int16_t *ret_velocity) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_VELOCITY, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_velocity != NULL) { *ret_velocity = tf_packet_buffer_read_int16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_current_velocity(TF_DCV2 *dc_v2, int16_t *ret_velocity) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_CURRENT_VELOCITY, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_velocity != NULL) { *ret_velocity = tf_packet_buffer_read_int16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_motion(TF_DCV2 *dc_v2, uint16_t acceleration, uint16_t deceleration) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_MOTION, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_MOTION, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    acceleration = tf_leconvert_uint16_to(acceleration); memcpy(buf + 0, &acceleration, 2);
    deceleration = tf_leconvert_uint16_to(deceleration); memcpy(buf + 2, &deceleration, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_motion(TF_DCV2 *dc_v2, uint16_t *ret_acceleration, uint16_t *ret_deceleration) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_MOTION, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_acceleration != NULL) { *ret_acceleration = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        if (ret_deceleration != NULL) { *ret_deceleration = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_full_brake(TF_DCV2 *dc_v2) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_FULL_BRAKE, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_FULL_BRAKE, 0, 0, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_drive_mode(TF_DCV2 *dc_v2, uint8_t mode) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_DRIVE_MODE, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_DRIVE_MODE, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = (uint8_t)mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_drive_mode(TF_DCV2 *dc_v2, uint8_t *ret_mode) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_DRIVE_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_pwm_frequency(TF_DCV2 *dc_v2, uint16_t frequency) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_PWM_FREQUENCY, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_PWM_FREQUENCY, 2, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    frequency = tf_leconvert_uint16_to(frequency); memcpy(buf + 0, &frequency, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_pwm_frequency(TF_DCV2 *dc_v2, uint16_t *ret_frequency) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_PWM_FREQUENCY, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_frequency != NULL) { *ret_frequency = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_power_statistics(TF_DCV2 *dc_v2, uint16_t *ret_voltage, uint16_t *ret_current) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_POWER_STATISTICS, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_voltage != NULL) { *ret_voltage = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        if (ret_current != NULL) { *ret_current = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_error_led_config(TF_DCV2 *dc_v2, uint8_t config) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_ERROR_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_ERROR_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_error_led_config(TF_DCV2 *dc_v2, uint8_t *ret_config) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_ERROR_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_emergency_shutdown_callback_configuration(TF_DCV2 *dc_v2, bool enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_EMERGENCY_SHUTDOWN_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_EMERGENCY_SHUTDOWN_CALLBACK_CONFIGURATION, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = enabled ? 1 : 0;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_emergency_shutdown_callback_configuration(TF_DCV2 *dc_v2, bool *ret_enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_EMERGENCY_SHUTDOWN_CALLBACK_CONFIGURATION, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_enabled != NULL) { *ret_enabled = tf_packet_buffer_read_bool(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_velocity_reached_callback_configuration(TF_DCV2 *dc_v2, bool enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_VELOCITY_REACHED_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_VELOCITY_REACHED_CALLBACK_CONFIGURATION, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = enabled ? 1 : 0;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_velocity_reached_callback_configuration(TF_DCV2 *dc_v2, bool *ret_enabled) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_VELOCITY_REACHED_CALLBACK_CONFIGURATION, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_enabled != NULL) { *ret_enabled = tf_packet_buffer_read_bool(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_current_velocity_callback_configuration(TF_DCV2 *dc_v2, uint32_t period, bool value_has_to_change) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_CURRENT_VELOCITY_CALLBACK_CONFIGURATION, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_CURRENT_VELOCITY_CALLBACK_CONFIGURATION, 5, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    period = tf_leconvert_uint32_to(period); memcpy(buf + 0, &period, 4);
    buf[4] = value_has_to_change ? 1 : 0;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_current_velocity_callback_configuration(TF_DCV2 *dc_v2, uint32_t *ret_period, bool *ret_value_has_to_change) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_CURRENT_VELOCITY_CALLBACK_CONFIGURATION, 0, 5, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_period != NULL) { *ret_period = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_value_has_to_change != NULL) { *ret_value_has_to_change = tf_packet_buffer_read_bool(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_spitfp_error_count(TF_DCV2 *dc_v2, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_SPITFP_ERROR_COUNT, 0, 16, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_error_count_ack_checksum != NULL) { *ret_error_count_ack_checksum = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_message_checksum != NULL) { *ret_error_count_message_checksum = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_frame != NULL) { *ret_error_count_frame = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_overflow != NULL) { *ret_error_count_overflow = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_bootloader_mode(TF_DCV2 *dc_v2, uint8_t mode, uint8_t *ret_status) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_BOOTLOADER_MODE, 1, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = (uint8_t)mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_bootloader_mode(TF_DCV2 *dc_v2, uint8_t *ret_mode) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_BOOTLOADER_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_write_firmware_pointer(TF_DCV2 *dc_v2, uint32_t pointer) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_WRITE_FIRMWARE_POINTER, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    pointer = tf_leconvert_uint32_to(pointer); memcpy(buf + 0, &pointer, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_write_firmware(TF_DCV2 *dc_v2, const uint8_t data[64], uint8_t *ret_status) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_WRITE_FIRMWARE, 64, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    memcpy(buf + 0, data, 64);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_set_status_led_config(TF_DCV2 *dc_v2, uint8_t config) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_SET_STATUS_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_SET_STATUS_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_status_led_config(TF_DCV2 *dc_v2, uint8_t *ret_config) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_STATUS_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_chip_temperature(TF_DCV2 *dc_v2, int16_t *ret_temperature) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_CHIP_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_reset(TF_DCV2 *dc_v2) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_RESET, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_RESET, 0, 0, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_write_uid(TF_DCV2 *dc_v2, uint32_t uid) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dc_v2_get_response_expected(dc_v2, TF_DC_V2_FUNCTION_WRITE_UID, &response_expected);
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_WRITE_UID, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dc_v2->tfp);

    uid = tf_leconvert_uint32_to(uid); memcpy(buf + 0, &uid, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_read_uid(TF_DCV2 *dc_v2, uint32_t *ret_uid) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_READ_UID, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_uid != NULL) { *ret_uid = tf_packet_buffer_read_uint32_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dc_v2_get_identity(TF_DCV2 *dc_v2, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dc_v2->tfp, TF_DC_V2_FUNCTION_GET_IDENTITY, 0, 25, response_expected);

    size_t i;
    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + tf_hal_get_common((TF_HAL *)dc_v2->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dc_v2->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        char tmp_connected_uid[8] = {0};
        if (ret_uid != NULL) { tf_packet_buffer_pop_n(&dc_v2->tfp->spitfp->recv_buf, (uint8_t*)ret_uid, 8);} else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 8); }
        tf_packet_buffer_pop_n(&dc_v2->tfp->spitfp->recv_buf, (uint8_t*)tmp_connected_uid, 8);
        if (ret_position != NULL) { *ret_position = tf_packet_buffer_read_char(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 1); }
        if (ret_hardware_version != NULL) { for (i = 0; i < 3; ++i) ret_hardware_version[i] = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 3); }
        if (ret_firmware_version != NULL) { for (i = 0; i < 3; ++i) ret_firmware_version[i] = tf_packet_buffer_read_uint8_t(&dc_v2->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 3); }
        if (ret_device_identifier != NULL) { *ret_device_identifier = tf_packet_buffer_read_uint16_t(&dc_v2->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dc_v2->tfp->spitfp->recv_buf, 2); }
        if (tmp_connected_uid[0] == 0 && ret_position != NULL) {
            *ret_position = tf_hal_get_port_name((TF_HAL *)dc_v2->tfp->hal, dc_v2->tfp->spitfp->port_id);
        }
        if (ret_connected_uid != NULL) {
            memcpy(ret_connected_uid, tmp_connected_uid, 8);
        }
        tf_tfp_packet_processed(dc_v2->tfp);
    }

    result = tf_tfp_finish_send(dc_v2->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}
#if TF_IMPLEMENT_CALLBACKS != 0
int tf_dc_v2_register_emergency_shutdown_callback(TF_DCV2 *dc_v2, TF_DCV2_EmergencyShutdownHandler handler, void *user_data) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dc_v2->tfp->needs_callback_tick = false;
        dc_v2->tfp->needs_callback_tick |= dc_v2->velocity_reached_handler != NULL;
        dc_v2->tfp->needs_callback_tick |= dc_v2->current_velocity_handler != NULL;
    } else {
        dc_v2->tfp->needs_callback_tick = true;
    }

    dc_v2->emergency_shutdown_handler = handler;
    dc_v2->emergency_shutdown_user_data = user_data;

    return TF_E_OK;
}


int tf_dc_v2_register_velocity_reached_callback(TF_DCV2 *dc_v2, TF_DCV2_VelocityReachedHandler handler, void *user_data) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dc_v2->tfp->needs_callback_tick = false;
        dc_v2->tfp->needs_callback_tick |= dc_v2->emergency_shutdown_handler != NULL;
        dc_v2->tfp->needs_callback_tick |= dc_v2->current_velocity_handler != NULL;
    } else {
        dc_v2->tfp->needs_callback_tick = true;
    }

    dc_v2->velocity_reached_handler = handler;
    dc_v2->velocity_reached_user_data = user_data;

    return TF_E_OK;
}


int tf_dc_v2_register_current_velocity_callback(TF_DCV2 *dc_v2, TF_DCV2_CurrentVelocityHandler handler, void *user_data) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dc_v2->tfp->needs_callback_tick = false;
        dc_v2->tfp->needs_callback_tick |= dc_v2->emergency_shutdown_handler != NULL;
        dc_v2->tfp->needs_callback_tick |= dc_v2->velocity_reached_handler != NULL;
    } else {
        dc_v2->tfp->needs_callback_tick = true;
    }

    dc_v2->current_velocity_handler = handler;
    dc_v2->current_velocity_user_data = user_data;

    return TF_E_OK;
}
#endif
int tf_dc_v2_callback_tick(TF_DCV2 *dc_v2, uint32_t timeout_us) {
    if (dc_v2 == NULL) {
        return TF_E_NULL;
    }

    return tf_tfp_callback_tick(dc_v2->tfp, tf_hal_current_time_us((TF_HAL *)dc_v2->tfp->hal) + timeout_us);
}

#ifdef __cplusplus
}
#endif