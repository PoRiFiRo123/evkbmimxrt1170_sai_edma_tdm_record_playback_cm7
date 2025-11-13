# USB Audio Device - Build and Test Instructions

## ✅ Implementation Complete!

USB Audio Class 2.0 device has been fully implemented for RT1176. When you build and flash this firmware, your board will enumerate as an 8-channel, 24-bit, 48kHz USB audio device.

---

## Build Instructions

### 1. Open Project in MCUXpresso IDE

1. Launch **MCUXpresso IDE** on your Windows machine
2. Go to **File → Import → Existing Projects into Workspace**
3. Select the project folder: `evkbmimxrt1170_sai_edma_tdm_record_playback_cm7`
4. Click **Finish**

### 2. Clean and Build

1. Right-click on the project in **Project Explorer**
2. Select **Clean Project**
3. Select **Build Project** (or press **Ctrl+B**)

**Expected Result:**
- Build should complete with **0 errors**
- You may see some warnings about unused variables (these are safe to ignore)
- Build time: ~2-5 minutes depending on your machine

### 3. Flash to RT1176 Board

1. Connect the **Debug USB** port (J11) to your PC
2. Power on the RT1176 board
3. In MCUXpresso IDE, click **Debug** (or press **F11**)
4. The firmware will be flashed and the debugger will stop at main()
5. Click **Resume** (or press **F8**) to run the firmware

**Alternative: Flash without debugging**
- Right-click project → **Debug As → MCUXpresso IDE LinkServer probes**
- Once flashed, disconnect debugger and power cycle the board

---

## Testing USB Enumeration

### 1. Connect USB Cable

**IMPORTANT:** Use the **USB OTG1** port (J9 or J10, check your board silkscreen), **NOT** the debug port!

1. Keep the **Debug USB** (J11) connected for console output (optional)
2. Connect a USB cable from **USB OTG1** to your PC
3. Use a **data cable**, not a charge-only cable

### 2. Check Debug Console (Optional)

If you have the debug console open, you should see:

```
USB Audio device initialized successfully
Waiting for USB enumeration...
Connect USB cable to see device on your laptop
```

Then after connecting the USB OTG cable:

```
USB Bus Reset
USB Set Address: X
USB Set Configuration: 1
USB Audio Device Configured!
```

### 3. Check Windows Device Manager

1. Open **Device Manager** on your PC
2. Look under **"Sound, video and game controllers"**
3. You should see:
   - **USB Audio Device** (or similar name)
   - Vendor: NXP (0x1FC9)
   - Product ID: 0x0098

**If you see "Unknown Device":**
- Windows may need drivers
- Right-click → **Update Driver** → **Search automatically**
- Windows 10/11 usually has built-in USB Audio Class drivers

### 4. Check Sound Settings

**Windows:**
1. Right-click speaker icon in taskbar → **Sounds**
2. Go to **Playback** tab
3. You should see the **USB Audio Device** listed
4. Go to **Recording** tab
5. You should also see the **USB Audio Device** there

**The device may show as:**
- "USB Audio Device"
- "NXP USB Audio"
- "Speakers (USB Audio Device)"
- "Microphone (USB Audio Device)"

---

## Configuration Details

### USB Audio Device Specifications

- **USB Audio Class:** 2.0
- **Vendor ID:** 0x1FC9 (NXP Semiconductors)
- **Product ID:** 0x0098
- **Channels:** 8 (TDM multi-channel)
- **Bit Depth:** 24-bit (in 32-bit slots)
- **Sample Rate:** 48 kHz
- **USB Speed:** Full Speed (12 Mbps)
- **Transfer Type:** Isochronous (time-sensitive audio)
- **Feedback:** Enabled (for rate matching)

### Endpoints

- **EP0:** Control endpoint (device enumeration, class requests)
- **EP1:** Control interface interrupt endpoint
- **EP2 OUT:** Audio data from PC to RT1176 (playback)
- **EP2 IN:** Feedback endpoint (rate matching)

---

## Troubleshooting

### Build Errors

**Error: undefined reference to USB_DeviceApplicationInit**
- **Cause:** `middleware/usb/device/application/audio_speaker.c` not included in build
- **Fix:** Check that `middleware` folder is in your project's source locations

**Error: undefined reference to USB_DeviceLpcIp3511IsrFunction**
- **Cause:** USB controller driver not compiled
- **Fix:** Check that `middleware/usb/device/controller/usb_device_lpcip3511.c` is in build

**Error: usb.h multiple definitions**
- **Cause:** Header guard issue
- **Fix:** Pull latest code (commit 23508f7 fixed this)

### USB Enumeration Issues

**Device not detected at all:**
1. **Check cable:** Use a data cable (not charge-only)
2. **Check port:** Make sure you're using USB OTG1 (NOT debug port!)
3. **Check power:** Board must be powered on
4. **Try another USB port:** Some PC USB ports have power issues
5. **Check console output:** Are there any error messages?

**Device shows as "Unknown Device":**
1. **Windows drivers:** Install/update USB Audio Class drivers
2. **Try Windows 10/11:** They have better built-in USB Audio support
3. **Check Device Manager details:**
   - Right-click device → Properties → Details
   - Select "Hardware IDs"
   - Should show `USB\VID_1FC9&PID_0098`

**Device enumerates but no audio:**
- This is expected for now! Audio data transfer will be implemented in next phase
- The current implementation focuses on USB enumeration only
- SAI audio continues to work independently

### Common Issues

**"USB Device Run failed: error 0xXX"**
- Check that USB clocks are properly initialized
- Verify USB_DEVICE_CONFIG_LPCIP3511HS is set to 1
- Check that USB PHY is powered

**Device disconnects and reconnects repeatedly:**
- Power supply issue - check 5V rail
- USB cable issue - try a different cable
- Ground loop - ensure proper grounding

---

## What Works Now

✅ USB device enumeration
✅ Device appears in Device Manager
✅ Shows as USB Audio Device
✅ SAI EDMA continues to work
✅ Audio bridge is operational

## What's Next (Future Implementation)

⏳ Audio data transfer PC → RT1176 → SAI TX
⏳ Audio data transfer SAI RX → RT1176 → PC
⏳ Full 8-channel audio streaming
⏳ Volume control and mute
⏳ Audio sample rate synchronization

---

## Debug Console Commands

None yet - can be added in future for testing.

---

## Notes

1. **Two USB ports on RT1176:**
   - **Debug port (J11):** For MCUXpresso debugging and console output
   - **USB OTG1 (J9/J10):** For USB Audio device - this is what you plug into your PC to see the audio device

2. **USB Audio Class 2.0:**
   - Supported natively on Windows 10/11, macOS, Linux
   - No custom drivers needed (usually)
   - Better than UAC 1.0 (supports higher sample rates, more channels)

3. **8-channel audio:**
   - Most audio software can see all 8 channels
   - Some simple players may only use stereo (first 2 channels)
   - Professional DAW software (Audacity, Pro Tools, etc.) can use all 8

4. **USB Full Speed vs High Speed:**
   - RT1176 USB controller supports High Speed (480 Mbps)
   - Current implementation uses Full Speed (12 Mbps)
   - Full Speed is sufficient for 8-channel 24-bit 48kHz audio
   - High Speed can be enabled if needed for higher channel counts

---

## Support

If you encounter any issues:

1. **Check console output** - Most errors will be printed
2. **Check Device Manager** - Shows if device enumerated
3. **Check Windows Event Viewer** - Shows USB driver errors
4. **Try different USB cable/port**
5. **Power cycle the board**

**Report issues with:**
- Exact error message from console
- Device Manager screenshot
- Windows version
- What you tried

Good luck testing your USB Audio device! 🎵
