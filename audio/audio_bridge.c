#include "audio_bridge.h"
#include <string.h>
#include <stdint.h>
#include "fsl_common.h" /* for SDK critical sections (if available) */

/* Simple critical section macros for audio bridge */
#ifndef OSA_SR_ALLOC
#define OSA_SR_ALLOC() uint32_t primask
#define OSA_ENTER_CRITICAL() primask = __get_PRIMASK(); __disable_irq()
#define OSA_EXIT_CRITICAL() __set_PRIMASK(primask)
#endif

/* Tune these for your device (2-ch, 48k example uses 384 bytes per ms) */
#ifndef AUDIO_BRIDGE_BUFFER_BYTES
#define AUDIO_BRIDGE_BUFFER_BYTES (64 * 1024) /* 64 KB circular buffer (adjust as needed) */
#endif

static uint8_t s_bridge_rx_buf[AUDIO_BRIDGE_BUFFER_BYTES]; /* data from SAI RX -> USB IN */
static volatile uint32_t s_bridge_rx_head;
static volatile uint32_t s_bridge_rx_tail;

static uint8_t s_bridge_tx_buf[AUDIO_BRIDGE_BUFFER_BYTES]; /* data from USB OUT -> SAI TX */
static volatile uint32_t s_bridge_tx_head;
static volatile uint32_t s_bridge_tx_tail;

void audio_bridge_init(void)
{
    s_bridge_rx_head = s_bridge_rx_tail = 0;
    s_bridge_tx_head = s_bridge_tx_tail = 0;
}

/* helper: available */
static inline uint32_t buf_used(uint32_t head, uint32_t tail)
{
    if (head >= tail)
        return head - tail;
    return (AUDIO_BRIDGE_BUFFER_BYTES - tail) + head;
}
static inline uint32_t buf_space(uint32_t head, uint32_t tail)
{
    return AUDIO_BRIDGE_BUFFER_BYTES - buf_used(head, tail) - 1;
}

/* Push USB OUT data into TX buffer (host->device -> to be consumed by SAI TX) */
void audio_bridge_push_usb_out(const uint8_t *data, size_t len)
{
    uint32_t i;
    /* simple: drop if not enough space (prevent blocking in IRQ). */
    uint32_t space;
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    space = buf_space(s_bridge_tx_head, s_bridge_tx_tail);
    if (len > space)
    {
        /* Drop oldest to make room (or simply drop incoming); here we'll drop newest bytes */
        len = space;
    }
    for (i = 0; i < len; i++)
    {
        s_bridge_tx_buf[s_bridge_tx_head++] = data[i];
        if (s_bridge_tx_head >= AUDIO_BRIDGE_BUFFER_BYTES) s_bridge_tx_head = 0;
    }
    OSA_EXIT_CRITICAL();
}

/* Pop data for SAI TX */
size_t audio_bridge_pop_for_sai_tx(uint8_t *dest, size_t max_len)
{
    size_t i = 0;
    uint32_t used;
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    used = buf_used(s_bridge_tx_head, s_bridge_tx_tail);
    if (used == 0)
    {
        OSA_EXIT_CRITICAL();
        return 0;
    }
    if (max_len > used) max_len = used;
    for (i = 0; i < max_len; i++)
    {
        dest[i] = s_bridge_tx_buf[s_bridge_tx_tail++];
        if (s_bridge_tx_tail >= AUDIO_BRIDGE_BUFFER_BYTES) s_bridge_tx_tail = 0;
    }
    OSA_EXIT_CRITICAL();
    return i;
}

/* Push SAI RX captured audio into RX buffer (to be delivered to USB IN) */
void audio_bridge_push_sai_rx(const uint8_t *data, size_t len)
{
    uint32_t i;
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    {
        uint32_t space = buf_space(s_bridge_rx_head, s_bridge_rx_tail);
        if (len > space) len = space; /* drop if no space */
        for (i = 0; i < len; i++)
        {
            s_bridge_rx_buf[s_bridge_rx_head++] = data[i];
            if (s_bridge_rx_head >= AUDIO_BRIDGE_BUFFER_BYTES) s_bridge_rx_head = 0;
        }
    }
    OSA_EXIT_CRITICAL();
}

/* Pop bytes for USB IN packets */
size_t audio_bridge_pop_for_usb(uint8_t *dest, size_t max_len)
{
    size_t i = 0;
    uint32_t used;
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    used = buf_used(s_bridge_rx_head, s_bridge_rx_tail);
    if (used == 0)
    {
        OSA_EXIT_CRITICAL();
        return 0;
    }
    if (max_len > used) max_len = used;
    for (i = 0; i < max_len; i++)
    {
        dest[i] = s_bridge_rx_buf[s_bridge_rx_tail++];
        if (s_bridge_rx_tail >= AUDIO_BRIDGE_BUFFER_BYTES) s_bridge_rx_tail = 0;
    }
    OSA_EXIT_CRITICAL();
    return i;
}
