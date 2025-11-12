/*
 * app_config.h
 *
 * Application-level constants: sample-rate, channels, packet sizing, buffer sizes.
 * Adjust these to match your SAI TDM configuration and desired USB descriptors.
 *
 * NOTE:
 * - Use BYTES_PER_SLOT = 4 to use 32-bit TDM slots (simplifies packing even for 24-bit audio).
 * - Packet sizes below are per 1ms USB frame (USB FS/HS concept). For HS (high speed)
 *   you still normally schedule in 1ms frames for audio. Adjust if using alternate rate.
 */

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

/* Audio format */
#define AUDIO_SAMPLE_RATE_HZ      (48000U)    /* 48 kHz base */
#define AUDIO_CHANNELS            (8U)        /* e.g., 8 channels TDM */
#define BYTES_PER_SAMPLE          (3U)        /* 3 bytes = 24-bit PCM (packed) */
#define BYTES_PER_SLOT            (4U)        /* using 32-bit slot simplifies DMA & USB packing */

/* Derived values */
#define SAMPLES_PER_MS            (AUDIO_SAMPLE_RATE_HZ / 1000U) /* e.g., 48 samples per ms */
#define USB_FRAMES_PER_PACKET     (1U) /* 1 ms per USB audio packet */

/* Packet byte counts (for 1 ms USB packet).
   Using BYTES_PER_SLOT keeps alignment simple. If you use packed 24-bit,
   adjust packing code in bridge. */
#define USB_IN_PACKET_SIZE    ((AUDIO_CHANNELS) * (BYTES_PER_SLOT) * (SAMPLES_PER_MS) * (USB_FRAMES_PER_PACKET))
#define USB_OUT_PACKET_SIZE   USB_IN_PACKET_SIZE

/* Ring buffer sizing:
   Keep a few ms worth of audio to absorb jitter. Set to e.g. 32 ms worth.
*/
#define AUDIO_RING_BUFFER_MS       (32U)
#define AUDIO_RING_BUFFER_BYTES    ((USB_IN_PACKET_SIZE) * (AUDIO_RING_BUFFER_MS))

/* EDMA / SAI configuration choices (tweak to match SDK example)
   - SAI_WORD_WIDTH: 32 bits per slot
*/
#define SAI_WORD_WIDTH_BITS        (32U)

/* Debug / build options */
#define APP_USE_FEEDBACK_ENDPOINT  1   /* 1 = use asynchronous USB feedback endpoint, 0 = synchronous */

#endif /* _APP_CONFIG_H_ */
