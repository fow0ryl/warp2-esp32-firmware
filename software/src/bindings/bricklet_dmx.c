/* ***********************************************************
 * This file was automatically generated on 2021-11-18.      *
 *                                                           *
 * C/C++ for Microcontrollers Bindings Version 2.0.0         *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generators git repository on tinkerforge.com       *
 *************************************************************/


#include "bricklet_dmx.h"
#include "base58.h"
#include "endian_convert.h"
#include "errors.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#if TF_IMPLEMENT_CALLBACKS != 0
static bool tf_dmx_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    TF_DMX *dmx = (TF_DMX *) dev;
    (void)payload;

    switch (fid) {
        case TF_DMX_CALLBACK_FRAME_STARTED: {
            TF_DMX_FrameStartedHandler fn = dmx->frame_started_handler;
            void *user_data = dmx->frame_started_user_data;
            if (fn == NULL) {
                return false;
            }


            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dmx->tfp->hal);
            hal_common->locked = true;
            fn(dmx, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_DMX_CALLBACK_FRAME_AVAILABLE: {
            TF_DMX_FrameAvailableHandler fn = dmx->frame_available_handler;
            void *user_data = dmx->frame_available_user_data;
            if (fn == NULL) {
                return false;
            }

            uint32_t frame_number = tf_packet_buffer_read_uint32_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dmx->tfp->hal);
            hal_common->locked = true;
            fn(dmx, frame_number, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_DMX_CALLBACK_FRAME_LOW_LEVEL: {
            TF_DMX_FrameLowLevelHandler fn = dmx->frame_low_level_handler;
            void *user_data = dmx->frame_low_level_user_data;
            if (fn == NULL) {
                return false;
            }
            size_t i;
            uint16_t frame_length = tf_packet_buffer_read_uint16_t(payload);
            uint16_t frame_chunk_offset = tf_packet_buffer_read_uint16_t(payload);
            uint8_t frame_chunk_data[56]; for (i = 0; i < 56; ++i) frame_chunk_data[i] = tf_packet_buffer_read_uint8_t(payload);
            uint32_t frame_number = tf_packet_buffer_read_uint32_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dmx->tfp->hal);
            hal_common->locked = true;
            fn(dmx, frame_length, frame_chunk_offset, frame_chunk_data, frame_number, user_data);
            hal_common->locked = false;
            break;
        }

        case TF_DMX_CALLBACK_FRAME_ERROR_COUNT: {
            TF_DMX_FrameErrorCountHandler fn = dmx->frame_error_count_handler;
            void *user_data = dmx->frame_error_count_user_data;
            if (fn == NULL) {
                return false;
            }

            uint32_t overrun_error_count = tf_packet_buffer_read_uint32_t(payload);
            uint32_t framing_error_count = tf_packet_buffer_read_uint32_t(payload);
            TF_HALCommon *hal_common = tf_hal_get_common((TF_HAL *)dmx->tfp->hal);
            hal_common->locked = true;
            fn(dmx, overrun_error_count, framing_error_count, user_data);
            hal_common->locked = false;
            break;
        }

        default:
            return false;
    }

    return true;
}
#else
static bool tf_dmx_callback_handler(void *dev, uint8_t fid, TF_PacketBuffer *payload) {
    return false;
}
#endif
int tf_dmx_create(TF_DMX *dmx, const char *uid, TF_HAL *hal) {
    if (dmx == NULL || uid == NULL || hal == NULL) {
        return TF_E_NULL;
    }

    memset(dmx, 0, sizeof(TF_DMX));

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

    rc = tf_hal_get_tfp(hal, &dmx->tfp, TF_DMX_DEVICE_IDENTIFIER, inventory_index);

    if (rc != TF_E_OK) {
        return rc;
    }

    dmx->tfp->device = dmx;
    dmx->tfp->uid = numeric_uid;
    dmx->tfp->cb_handler = tf_dmx_callback_handler;
    dmx->response_expected[0] = 0x22;
    dmx->response_expected[1] = 0x00;

    return TF_E_OK;
}

int tf_dmx_destroy(TF_DMX *dmx) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    int result = tf_tfp_destroy(dmx->tfp);
    dmx->tfp = NULL;

    return result;
}

int tf_dmx_get_response_expected(TF_DMX *dmx, uint8_t function_id, bool *ret_response_expected) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_DMX_FUNCTION_SET_DMX_MODE:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 0)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_WRITE_FRAME_LOW_LEVEL:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 1)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_FRAME_DURATION:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 2)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_COMMUNICATION_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 3)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_ERROR_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 4)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_FRAME_CALLBACK_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 5)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 6)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_SET_STATUS_LED_CONFIG:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[0] & (1 << 7)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_RESET:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[1] & (1 << 0)) != 0;
            }
            break;
        case TF_DMX_FUNCTION_WRITE_UID:
            if (ret_response_expected != NULL) {
                *ret_response_expected = (dmx->response_expected[1] & (1 << 1)) != 0;
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_dmx_set_response_expected(TF_DMX *dmx, uint8_t function_id, bool response_expected) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    switch (function_id) {
        case TF_DMX_FUNCTION_SET_DMX_MODE:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 0);
            } else {
                dmx->response_expected[0] &= ~(1 << 0);
            }
            break;
        case TF_DMX_FUNCTION_WRITE_FRAME_LOW_LEVEL:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 1);
            } else {
                dmx->response_expected[0] &= ~(1 << 1);
            }
            break;
        case TF_DMX_FUNCTION_SET_FRAME_DURATION:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 2);
            } else {
                dmx->response_expected[0] &= ~(1 << 2);
            }
            break;
        case TF_DMX_FUNCTION_SET_COMMUNICATION_LED_CONFIG:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 3);
            } else {
                dmx->response_expected[0] &= ~(1 << 3);
            }
            break;
        case TF_DMX_FUNCTION_SET_ERROR_LED_CONFIG:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 4);
            } else {
                dmx->response_expected[0] &= ~(1 << 4);
            }
            break;
        case TF_DMX_FUNCTION_SET_FRAME_CALLBACK_CONFIG:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 5);
            } else {
                dmx->response_expected[0] &= ~(1 << 5);
            }
            break;
        case TF_DMX_FUNCTION_SET_WRITE_FIRMWARE_POINTER:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 6);
            } else {
                dmx->response_expected[0] &= ~(1 << 6);
            }
            break;
        case TF_DMX_FUNCTION_SET_STATUS_LED_CONFIG:
            if (response_expected) {
                dmx->response_expected[0] |= (1 << 7);
            } else {
                dmx->response_expected[0] &= ~(1 << 7);
            }
            break;
        case TF_DMX_FUNCTION_RESET:
            if (response_expected) {
                dmx->response_expected[1] |= (1 << 0);
            } else {
                dmx->response_expected[1] &= ~(1 << 0);
            }
            break;
        case TF_DMX_FUNCTION_WRITE_UID:
            if (response_expected) {
                dmx->response_expected[1] |= (1 << 1);
            } else {
                dmx->response_expected[1] &= ~(1 << 1);
            }
            break;
        default:
            return TF_E_INVALID_PARAMETER;
    }

    return TF_E_OK;
}

int tf_dmx_set_response_expected_all(TF_DMX *dmx, bool response_expected) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    memset(dmx->response_expected, response_expected ? 0xFF : 0, 2);

    return TF_E_OK;
}

int tf_dmx_set_dmx_mode(TF_DMX *dmx, uint8_t dmx_mode) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_DMX_MODE, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_DMX_MODE, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = (uint8_t)dmx_mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_dmx_mode(TF_DMX *dmx, uint8_t *ret_dmx_mode) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_DMX_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_dmx_mode != NULL) { *ret_dmx_mode = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_write_frame_low_level(TF_DMX *dmx, uint16_t frame_length, uint16_t frame_chunk_offset, const uint8_t frame_chunk_data[60]) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_WRITE_FRAME_LOW_LEVEL, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_WRITE_FRAME_LOW_LEVEL, 64, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    frame_length = tf_leconvert_uint16_to(frame_length); memcpy(buf + 0, &frame_length, 2);
    frame_chunk_offset = tf_leconvert_uint16_to(frame_chunk_offset); memcpy(buf + 2, &frame_chunk_offset, 2);
    memcpy(buf + 4, frame_chunk_data, 60);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_read_frame_low_level(TF_DMX *dmx, uint16_t *ret_frame_length, uint16_t *ret_frame_chunk_offset, uint8_t ret_frame_chunk_data[56], uint32_t *ret_frame_number) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_READ_FRAME_LOW_LEVEL, 0, 64, response_expected);

    size_t i;
    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_frame_length != NULL) { *ret_frame_length = tf_packet_buffer_read_uint16_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 2); }
        if (ret_frame_chunk_offset != NULL) { *ret_frame_chunk_offset = tf_packet_buffer_read_uint16_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 2); }
        if (ret_frame_chunk_data != NULL) { for (i = 0; i < 56; ++i) ret_frame_chunk_data[i] = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 56); }
        if (ret_frame_number != NULL) { *ret_frame_number = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_frame_duration(TF_DMX *dmx, uint16_t frame_duration) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_FRAME_DURATION, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_FRAME_DURATION, 2, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    frame_duration = tf_leconvert_uint16_to(frame_duration); memcpy(buf + 0, &frame_duration, 2);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_frame_duration(TF_DMX *dmx, uint16_t *ret_frame_duration) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_FRAME_DURATION, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_frame_duration != NULL) { *ret_frame_duration = tf_packet_buffer_read_uint16_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_frame_error_count(TF_DMX *dmx, uint32_t *ret_overrun_error_count, uint32_t *ret_framing_error_count) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_FRAME_ERROR_COUNT, 0, 8, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_overrun_error_count != NULL) { *ret_overrun_error_count = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        if (ret_framing_error_count != NULL) { *ret_framing_error_count = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_communication_led_config(TF_DMX *dmx, uint8_t config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_COMMUNICATION_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_COMMUNICATION_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_communication_led_config(TF_DMX *dmx, uint8_t *ret_config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_COMMUNICATION_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_error_led_config(TF_DMX *dmx, uint8_t config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_ERROR_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_ERROR_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_error_led_config(TF_DMX *dmx, uint8_t *ret_config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_ERROR_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_frame_callback_config(TF_DMX *dmx, bool frame_started_callback_enabled, bool frame_available_callback_enabled, bool frame_callback_enabled, bool frame_error_count_callback_enabled) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_FRAME_CALLBACK_CONFIG, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_FRAME_CALLBACK_CONFIG, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = frame_started_callback_enabled ? 1 : 0;
    buf[1] = frame_available_callback_enabled ? 1 : 0;
    buf[2] = frame_callback_enabled ? 1 : 0;
    buf[3] = frame_error_count_callback_enabled ? 1 : 0;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_frame_callback_config(TF_DMX *dmx, bool *ret_frame_started_callback_enabled, bool *ret_frame_available_callback_enabled, bool *ret_frame_callback_enabled, bool *ret_frame_error_count_callback_enabled) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_FRAME_CALLBACK_CONFIG, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_frame_started_callback_enabled != NULL) { *ret_frame_started_callback_enabled = tf_packet_buffer_read_bool(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        if (ret_frame_available_callback_enabled != NULL) { *ret_frame_available_callback_enabled = tf_packet_buffer_read_bool(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        if (ret_frame_callback_enabled != NULL) { *ret_frame_callback_enabled = tf_packet_buffer_read_bool(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        if (ret_frame_error_count_callback_enabled != NULL) { *ret_frame_error_count_callback_enabled = tf_packet_buffer_read_bool(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_spitfp_error_count(TF_DMX *dmx, uint32_t *ret_error_count_ack_checksum, uint32_t *ret_error_count_message_checksum, uint32_t *ret_error_count_frame, uint32_t *ret_error_count_overflow) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_SPITFP_ERROR_COUNT, 0, 16, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_error_count_ack_checksum != NULL) { *ret_error_count_ack_checksum = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_message_checksum != NULL) { *ret_error_count_message_checksum = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_frame != NULL) { *ret_error_count_frame = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        if (ret_error_count_overflow != NULL) { *ret_error_count_overflow = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_bootloader_mode(TF_DMX *dmx, uint8_t mode, uint8_t *ret_status) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_BOOTLOADER_MODE, 1, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = (uint8_t)mode;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_bootloader_mode(TF_DMX *dmx, uint8_t *ret_mode) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_BOOTLOADER_MODE, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_mode != NULL) { *ret_mode = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_write_firmware_pointer(TF_DMX *dmx, uint32_t pointer) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_WRITE_FIRMWARE_POINTER, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_WRITE_FIRMWARE_POINTER, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    pointer = tf_leconvert_uint32_to(pointer); memcpy(buf + 0, &pointer, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_write_firmware(TF_DMX *dmx, const uint8_t data[64], uint8_t *ret_status) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_WRITE_FIRMWARE, 64, 1, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    memcpy(buf + 0, data, 64);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_status != NULL) { *ret_status = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_set_status_led_config(TF_DMX *dmx, uint8_t config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_SET_STATUS_LED_CONFIG, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_SET_STATUS_LED_CONFIG, 1, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    buf[0] = (uint8_t)config;

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_status_led_config(TF_DMX *dmx, uint8_t *ret_config) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_STATUS_LED_CONFIG, 0, 1, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_config != NULL) { *ret_config = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_chip_temperature(TF_DMX *dmx, int16_t *ret_temperature) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_CHIP_TEMPERATURE, 0, 2, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_temperature != NULL) { *ret_temperature = tf_packet_buffer_read_int16_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 2); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_reset(TF_DMX *dmx) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_RESET, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_RESET, 0, 0, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_write_uid(TF_DMX *dmx, uint32_t uid) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_dmx_get_response_expected(dmx, TF_DMX_FUNCTION_WRITE_UID, &response_expected);
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_WRITE_UID, 4, 0, response_expected);

    uint8_t *buf = tf_tfp_get_payload_buffer(dmx->tfp);

    uid = tf_leconvert_uint32_to(uid); memcpy(buf + 0, &uid, 4);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_read_uid(TF_DMX *dmx, uint32_t *ret_uid) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_READ_UID, 0, 4, response_expected);

    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        if (ret_uid != NULL) { *ret_uid = tf_packet_buffer_read_uint32_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 4); }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_get_identity(TF_DMX *dmx, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->locked) {
        return TF_E_LOCKED;
    }

    bool response_expected = true;
    tf_tfp_prepare_send(dmx->tfp, TF_DMX_FUNCTION_GET_IDENTITY, 0, 25, response_expected);

    size_t i;
    uint32_t deadline = tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + tf_hal_get_common((TF_HAL *)dmx->tfp->hal)->timeout;

    uint8_t error_code = 0;
    int result = tf_tfp_transmit_packet(dmx->tfp, response_expected, deadline, &error_code);

    if (result < 0) {
        return result;
    }

    if (result & TF_TICK_TIMEOUT) {
        return TF_E_TIMEOUT;
    }

    if (result & TF_TICK_PACKET_RECEIVED && error_code == 0) {
        char tmp_connected_uid[8] = {0};
        if (ret_uid != NULL) { tf_packet_buffer_pop_n(&dmx->tfp->spitfp->recv_buf, (uint8_t*)ret_uid, 8);} else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 8); }
        tf_packet_buffer_pop_n(&dmx->tfp->spitfp->recv_buf, (uint8_t*)tmp_connected_uid, 8);
        if (ret_position != NULL) { *ret_position = tf_packet_buffer_read_char(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 1); }
        if (ret_hardware_version != NULL) { for (i = 0; i < 3; ++i) ret_hardware_version[i] = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 3); }
        if (ret_firmware_version != NULL) { for (i = 0; i < 3; ++i) ret_firmware_version[i] = tf_packet_buffer_read_uint8_t(&dmx->tfp->spitfp->recv_buf);} else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 3); }
        if (ret_device_identifier != NULL) { *ret_device_identifier = tf_packet_buffer_read_uint16_t(&dmx->tfp->spitfp->recv_buf); } else { tf_packet_buffer_remove(&dmx->tfp->spitfp->recv_buf, 2); }
        if (tmp_connected_uid[0] == 0 && ret_position != NULL) {
            *ret_position = tf_hal_get_port_name((TF_HAL *)dmx->tfp->hal, dmx->tfp->spitfp->port_id);
        }
        if (ret_connected_uid != NULL) {
            memcpy(ret_connected_uid, tmp_connected_uid, 8);
        }
        tf_tfp_packet_processed(dmx->tfp);
    }

    result = tf_tfp_finish_send(dmx->tfp, result, deadline);

    if (result < 0) {
        return result;
    }

    return tf_tfp_get_error(error_code);
}

int tf_dmx_write_frame(TF_DMX *dmx, const uint8_t *frame, uint16_t frame_length) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    int ret = TF_E_OK;
    uint16_t frame_chunk_offset = 0;
    uint8_t frame_chunk_data[60];
    uint16_t frame_chunk_length = 0;

    if (frame_length == 0) {
        memset(&frame_chunk_data, 0, sizeof(uint8_t) * 60);

        ret = tf_dmx_write_frame_low_level(dmx, frame_length, frame_chunk_offset, frame_chunk_data);
    } else {
        while (frame_chunk_offset < frame_length) {
            frame_chunk_length = frame_length - frame_chunk_offset;

            if (frame_chunk_length > 60) {
                frame_chunk_length = 60;
            }

            memcpy(frame_chunk_data, &frame[frame_chunk_offset], sizeof(uint8_t) * frame_chunk_length);
            memset(&frame_chunk_data[frame_chunk_length], 0, sizeof(uint8_t) * (60 - frame_chunk_length));

            ret = tf_dmx_write_frame_low_level(dmx, frame_length, frame_chunk_offset, frame_chunk_data);

            if (ret != TF_E_OK) {
                break;
            }

            frame_chunk_offset += 60;
        }

    }

    return ret;
}

int tf_dmx_read_frame(TF_DMX *dmx, uint8_t *ret_frame, uint16_t *ret_frame_length, uint32_t *ret_frame_number) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    int ret = TF_E_OK;
    uint16_t max_frame_length = 0;
    uint16_t frame_length = 0;
    uint16_t frame_chunk_offset = 0;
    uint8_t frame_chunk_data[56];
    bool frame_out_of_sync;
    uint16_t frame_chunk_length = 0;

    ret = tf_dmx_read_frame_low_level(dmx, &frame_length, &frame_chunk_offset, frame_chunk_data, ret_frame_number);

    if (ret != TF_E_OK) {
        if (ret_frame_length != NULL) {
            *ret_frame_length = frame_length;
        }

        return ret;
    }

    frame_out_of_sync = frame_chunk_offset != 0;

    if (!frame_out_of_sync) {
        frame_chunk_length = max_frame_length - frame_chunk_offset;

        if (frame_chunk_length > 56) {
            frame_chunk_length = 56;
        }

        if (ret_frame != NULL) {
            memcpy(ret_frame, frame_chunk_data, sizeof(uint8_t) * frame_chunk_length);
        }

        frame_length = frame_chunk_length;

        while (frame_length < max_frame_length) {
            ret = tf_dmx_read_frame_low_level(dmx, &frame_length, &frame_chunk_offset, frame_chunk_data, ret_frame_number);

            if (ret != TF_E_OK) {
                if (ret_frame_length != NULL) {
                    *ret_frame_length = frame_length;
                }

                return ret;
            }

            frame_out_of_sync = frame_chunk_offset != frame_length;

            if (frame_out_of_sync) {
                break;
            }

            frame_chunk_length = max_frame_length - frame_chunk_offset;

            if (frame_chunk_length > 56) {
                frame_chunk_length = 56;
            }

            if (ret_frame != NULL) {
                memcpy(&ret_frame[frame_length], frame_chunk_data, sizeof(uint8_t) * frame_chunk_length);
            }

            frame_length += frame_chunk_length;
        }
    }

    if (frame_out_of_sync) {
        if (ret_frame_length != NULL) {
            *ret_frame_length = 0; // return empty array
        }

        // discard remaining stream to bring it back in-sync
        while (frame_chunk_offset + 56 < max_frame_length) {
            ret = tf_dmx_read_frame_low_level(dmx, &frame_length, &frame_chunk_offset, frame_chunk_data, ret_frame_number);

            if (ret != TF_E_OK) {
                return ret;
            }
        }

        ret = TF_E_STREAM_OUT_OF_SYNC;
    }

    return ret;
}
#if TF_IMPLEMENT_CALLBACKS != 0
int tf_dmx_register_frame_started_callback(TF_DMX *dmx, TF_DMX_FrameStartedHandler handler, void *user_data) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dmx->tfp->needs_callback_tick = false;
        dmx->tfp->needs_callback_tick |= dmx->frame_available_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_low_level_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_error_count_handler != NULL;
    } else {
        dmx->tfp->needs_callback_tick = true;
    }

    dmx->frame_started_handler = handler;
    dmx->frame_started_user_data = user_data;

    return TF_E_OK;
}


int tf_dmx_register_frame_available_callback(TF_DMX *dmx, TF_DMX_FrameAvailableHandler handler, void *user_data) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dmx->tfp->needs_callback_tick = false;
        dmx->tfp->needs_callback_tick |= dmx->frame_started_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_low_level_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_error_count_handler != NULL;
    } else {
        dmx->tfp->needs_callback_tick = true;
    }

    dmx->frame_available_handler = handler;
    dmx->frame_available_user_data = user_data;

    return TF_E_OK;
}


int tf_dmx_register_frame_low_level_callback(TF_DMX *dmx, TF_DMX_FrameLowLevelHandler handler, void *user_data) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dmx->tfp->needs_callback_tick = false;
        dmx->tfp->needs_callback_tick |= dmx->frame_started_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_available_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_error_count_handler != NULL;
    } else {
        dmx->tfp->needs_callback_tick = true;
    }

    dmx->frame_low_level_handler = handler;
    dmx->frame_low_level_user_data = user_data;

    return TF_E_OK;
}


int tf_dmx_register_frame_error_count_callback(TF_DMX *dmx, TF_DMX_FrameErrorCountHandler handler, void *user_data) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    if (handler == NULL) {
        dmx->tfp->needs_callback_tick = false;
        dmx->tfp->needs_callback_tick |= dmx->frame_started_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_available_handler != NULL;
        dmx->tfp->needs_callback_tick |= dmx->frame_low_level_handler != NULL;
    } else {
        dmx->tfp->needs_callback_tick = true;
    }

    dmx->frame_error_count_handler = handler;
    dmx->frame_error_count_user_data = user_data;

    return TF_E_OK;
}
#endif
int tf_dmx_callback_tick(TF_DMX *dmx, uint32_t timeout_us) {
    if (dmx == NULL) {
        return TF_E_NULL;
    }

    return tf_tfp_callback_tick(dmx->tfp, tf_hal_current_time_us((TF_HAL *)dmx->tfp->hal) + timeout_us);
}

#ifdef __cplusplus
}
#endif