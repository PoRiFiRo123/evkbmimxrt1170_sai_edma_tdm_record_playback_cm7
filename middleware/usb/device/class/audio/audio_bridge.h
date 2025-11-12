#ifndef _AUDIO_BRIDGE_H_
#define _AUDIO_BRIDGE_H_

#include <stdint.h>
#include <stddef.h>

void audio_bridge_init(void);

/* Host -> Device (USB OUT). Push new packet from USB into bridge (for SAI TX). */
void audio_bridge_push_usb_out(const uint8_t *data, size_t len);

/* Device -> Host (USB IN). Pop up to max_len bytes into dest; return bytes copied. */
size_t audio_bridge_pop_for_usb(uint8_t *dest, size_t max_len);

/* SAI RX -> bridge (SAI delivers captured audio to be sent to host) */
void audio_bridge_push_sai_rx(const uint8_t *data, size_t len);

/* Bridge -> SAI TX (for playback). Pop up to max_len bytes into dest; return bytes copied. */
size_t audio_bridge_pop_for_sai_tx(uint8_t *dest, size_t max_len);

#endif /* _AUDIO_BRIDGE_H_ */
