# Required NXP SDK Files for USB Audio

## Critical: You MUST download these from NXP SDK

### How to Get Them

1. **Go to**: https://mcuxpresso.nxp.com/en/welcome
2. **Select SDK Builder**
3. **Choose Board**: EVK-MIMXRT1176
4. **Select Middleware**:
   - ✅ USB Device Stack
   - ✅ USB Device Controller (LPCIP3511HS)
   - ✅ USB Audio Class
5. **Download SDK**
6. **Extract and copy files below**

---

## Required Files to Copy

### USB Device Core (Critical)
Copy from: `SDK_2.x/middleware/usb/device/`

```
middleware/usb/device/usb_device.c               ← Main USB device
middleware/usb/device/usb_device_dci.c           ← Device Controller Interface
middleware/usb/device/usb_device_ch9.c           ← Chapter 9 (Standard Requests)
middleware/usb/device/usb_device_class.c         ← Class management
```

### USB Controller Driver (RT1176 Specific)
Copy from: `SDK_2.x/middleware/usb/device/controller/`

```
middleware/usb/device/controller/usb_device_lpcip3511.c  ← RT1176 USB controller driver
middleware/usb/device/controller/usb_device_lpcip3511.h  ← Controller header
```

### USB Audio Class
Copy from: `SDK_2.x/middleware/usb/device/class/`

```
middleware/usb/device/class/audio/usb_device_audio.c     ← Audio Class implementation
middleware/usb/device/class/audio/usb_device_audio.h     ← Audio Class header
```

---

## Alternative: Use NXP Example Project

**Even Better Approach:**

1. In MCUXpresso IDE: **File → Import → MCUXpresso SDK Examples**
2. Search for: **"usbaudio"** or **"dev_audio_speaker"**
3. Import the USB Audio Speaker example for MIMXRT1176
4. Copy the entire `middleware/usb/device/` folder from that example to your project

This gives you ALL tested, working USB files.

---

## Why This is Necessary

The USB controller driver (`usb_device_lpcip3511.c`) contains:
- ~2000 lines of register-level code
- USB DMA configuration
- Endpoint management
- Interrupt handling
- Buffer descriptor management

**This cannot be recreated from scratch reliably.** NXP has tested and validated this code.

---

## After Downloading

Once you have the files:
1. Copy them to your project's `middleware/usb/` directories
2. I will help you integrate them
3. Fix any compilation issues
4. Get USB Audio working

**Let me know when you have the SDK files, and I'll complete the integration.**
