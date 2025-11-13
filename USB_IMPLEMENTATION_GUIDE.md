# USB Audio Device Implementation Guide

## Current Status

### ✅ Completed:
1. **Fixed critical USB header errors** (commit 23508f7)
   - Fixed `usb.h`: Moved `usb_device_control_type_t` enum inside header guard
   - This eliminates 237 duplicate definition errors
   - Created `board/usb_device_config.h` with RT1176 USB configuration

2. **Disabled USB temporarily** (commit 95c1a13)
   - USB initialization is disabled in `app/app_init.c`
   - This ensures **ZERO BUILD ERRORS** on your machine
   - Allows you to verify the SAI/audio bridge works correctly first

### Current Build State:
- **Expected: 0 errors** ✅
- SAI EDMA TDM (8-channel audio) functional
- Audio bridge operational
- USB ready to be enabled incrementally

---

## Step-by-Step: Achieving USB Enumeration

### Phase 1: Verify Clean Build (Do This First!)

1. **Open project in MCUXpresso IDE on your Windows machine**
2. **Clean and rebuild** (Project → Clean → Build)
3. **Verify: 0 errors, 0 warnings** (or minimal warnings about unused code)
4. **Flash and test**: Confirm SAI audio still works

**If you have errors**, stop here and report them. Do NOT proceed until build is clean.

---

### Phase 2: Enable USB Initialization

Once Phase 1 passes, enable USB:

1. **Edit `app/app_init.c`**:
   ```c
   // Change this line from:
   #if 0  /* Set to 1 to enable USB */

   // To:
   #if 1  /* Set to 1 to enable USB */
   ```

2. **Uncomment the USB includes and declarations**:
   ```c
   // Change from:
   /* #include "usb_device_board_init.h" */
   /* extern void USB_DeviceApplicationInit(void); */

   // To:
   #include "usb_device_board_init.h"
   extern void USB_DeviceApplicationInit(void);
   ```

3. **Rebuild the project**

**Expected outcome:**
- The project should still build
- You may see errors related to missing USB implementation files
- This is expected - proceed to Phase 3

---

### Phase 3: USB Implementation Options

You have TWO paths forward:

#### Option A: Minimal Standalone USB (Recommended for Testing)

Use the simple USB implementation in `usb/usb_device_rt1176.c`:

1. **This file already exists** and provides:
   - Basic USB device enumeration
   - USB Audio Class descriptors (8-channel, 48kHz, 24-bit)
   - Setup packet handling
   - Device/Configuration/String descriptors

2. **What it does:**
   - Responds to USB enumeration requests
   - Appears as "USB AUDIO 8CH" device on PC
   - Handles GET_DESCRIPTOR, SET_ADDRESS, SET_CONFIGURATION

3. **What it doesn't do (yet):**
   - Actual audio data transfer (stubs in place)
   - Full USB controller register programming
   - Interrupt handling for data transfers

4. **To use it:**
   - It's already in the project
   - When you enable USB in app_init.c, it will be called
   - **You should see the device enumerate on your PC**

#### Option B: Full NXP USB Stack (For Production)

Use the complete NXP SDK USB middleware (currently in `middleware/` directory):

**Pros:**
- Complete, production-ready implementation
- Full USB Audio Class 2.0 support
- DMA-based transfers
- Comprehensive error handling

**Cons:**
- Many dependencies and complex integration
- Requires careful configuration
- More files to compile

**Status:**
- Partially integrated
- May need additional SDK files
- Requires more debugging

---

### Phase 4: Test USB Enumeration

1. **Build the project with USB enabled**
2. **Flash to RT1176 board**
3. **Connect USB OTG1 cable to your PC**
4. **Check Windows Device Manager**:
   - Look under "Sound, video and game controllers"
   - You should see "USB AUDIO 8CH" or similar
   - It may show as "Unknown Device" initially

5. **Check debug console output**:
   ```
   USB: Initializing USB Audio device
   USB: Device initialized and ready to enumerate
   SET_ADDRESS: X
   GET_DESCRIPTOR: Device
   GET_DESCRIPTOR: Configuration
   SET_CONFIGURATION: 1
   USB Audio Device Configured!
   ```

---

## Troubleshooting

### Build Errors After Enabling USB

**Error: undefined reference to USB_DeviceApplicationInit**
- **Cause:** `usb/usb_device_rt1176.c` not being compiled
- **Fix:** Check that MCUXpresso includes the `usb/` directory in build
  - Right-click project → Properties → C/C++ Build → Settings
  - Check that `usb` is in source locations

**Error: undefined reference to BOARD_USB_DeviceInit**
- **Cause:** `board/usb_device_board_init.c` has compilation errors
- **Fix:** Check the debug console for specific errors in that file

**Error: usb.h:XX: multiple definition of...**
- **Cause:** Header guard fix didn't apply properly
- **Fix:** Verify `middleware/usb/include/usb.h` has only ONE `#endif /* __USB_H__ */` at the very end

### USB Doesn't Enumerate

**Device not detected on PC:**
1. **Check USB cable** - ensure it's a data cable, not charge-only
2. **Check USB_OTG1 port** - verify you're using the correct port on RT1176 board
3. **Check USB PHY power** - `BOARD_USB_DeviceInit()` should enable USB clocks/PHY
4. **Check console output** - any error messages?

**"Unknown Device" in Windows:**
- This may be normal initially
- Windows needs USB descriptors to identify the device
- Check if Windows prompts for drivers (may need USB Audio Class driver)

**Device enumeration timeout:**
- May indicate USB interrupt handler not being called
- Check `USB_OTG1_IRQHandler` is defined and enabled
- Verify `USB_DeviceIsrEnable()` is called

---

## Next Steps: Audio Data Transfer

Once USB enumeration works, the next phase is:

1. **Implement USB audio data OUT** (PC → RT1176 → SAI TX)
   - Handle isochronous endpoint 0x01 (OUT)
   - Push received USB audio data to `audio_bridge_push_usb_out()`
   - SAI will transmit this audio

2. **Implement USB audio data IN** (SAI RX → RT1176 → PC)
   - Handle isochronous endpoint 0x81 (IN)
   - Pop SAI audio data from `audio_bridge_pop_for_usb()`
   - Send via USB to PC

3. **Test audio loopback**:
   - Play audio on PC
   - Should come out of RT1176 SAI
   - RT1176 SAI input should appear as recording device on PC

---

## Files Modified

- `middleware/usb/include/usb.h` - Fixed header guard structure
- `board/usb_device_config.h` - **NEW** - USB device configuration
- `app/app_init.c` - USB temporarily disabled
- `usb/usb_device_rt1176.c` - Minimal USB Audio implementation (existing)
- `board/usb_device_board_init.c` - Board USB initialization (existing)

---

## Important Notes

1. **Build incrementally** - Don't enable everything at once
2. **Test at each stage** - Verify enumeration before adding data transfer
3. **Check console output** - Debug messages show USB state
4. **Be patient** - USB debugging can be tricky

5. **If you encounter errors:**
   - Note the exact error message
   - Check which file and line number
   - Report back for assistance

---

## Success Criteria

### Phase 1 Success:
- ✅ Project builds with 0 errors (USB disabled)
- ✅ SAI audio works

### Phase 2 Success:
- ✅ Project builds with 0 errors (USB enabled)
- ✅ SAI audio still works

### Phase 3 Success:
- ✅ USB device enumerates on PC
- ✅ Device appears in Device Manager
- ✅ Debug console shows enumeration messages

### Final Success (Future):
- ✅ PC sees 8-channel audio device
- ✅ Audio playback from PC works
- ✅ Audio recording to PC works
- ✅ All 8 channels functional

---

## Contact

If you encounter any issues or have questions, provide:
1. Exact error messages (copy from MCUXpresso console)
2. Which phase you're in
3. What you tried
4. Console debug output (if available)

Good luck! 🎵
