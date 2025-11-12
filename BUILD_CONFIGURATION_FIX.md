# Build Configuration Fix for MCUXpresso IDE

## Error Analysis

The build is failing with:
```
fatal error: audio_bridge.h: No such file or directory
```

This occurs because the new directories we added are not in the project's include path.

---

## Fix Instructions for MCUXpresso IDE

### Step 1: Add Include Paths

1. **Right-click** on your project in Project Explorer
2. Select **Properties**
3. Navigate to: **C/C++ Build → Settings**
4. In the right panel, click on **MCU C Compiler → Includes**
5. In the "Include paths (-I)" section, click the **Add (green +)** button
6. Add these paths **one by one** (use **Workspace...** button and browse to each folder):

```
"${workspace_loc:/${ProjName}/app}"
"${workspace_loc:/${ProjName}/middleware/usb/device/application}"
"${workspace_loc:/${ProjName}/middleware/usb/device/class/audio}"
"${workspace_loc:/${ProjName}/middleware/usb/device/include}"
"${workspace_loc:/${ProjName}/middleware/usb/device/controller}"
"${workspace_loc:/${ProjName}/middleware/usb/device/core}"
"${workspace_loc:/${ProjName}/middleware/usb/device/descriptors}"
"${workspace_loc:/${ProjName}/middleware/usb/include}"
"${workspace_loc:/${ProjName}/middleware/usb/phy}"
```

7. Click **Apply**

### Step 2: Add New Source Files to Build

You need to add the new source files we created. There are two ways:

#### Method A: Using MCUXpresso GUI (Recommended)

1. In **Project Explorer**, expand your project
2. Right-click on each of these folders and select **Refresh**:
   - `app/`
   - `board/`
   - `middleware/usb/device/`

3. The following new files should appear and be automatically included:
   - `app/main.c` (modified - already in build)
   - `app/app_init.c` (modified - already in build)
   - `board/usb_device_board_init.c` (NEW)
   - `middleware/usb/device/application/audio_speaker.c` (NEW)

4. If any files don't appear, manually add them:
   - Right-click on the appropriate folder
   - Select **New → File** → Browse to select the file

#### Method B: Add to Source Locations

1. **Right-click** on project → **Properties**
2. Go to: **C/C++ General → Paths and Symbols → Source Location**
3. Click **Add Folder**
4. Add these folders if not present:
   - `app`
   - `board`
   - `middleware/usb/device/application`
   - `middleware/usb/device/class/audio`
   - `middleware/usb/device/controller`
   - `middleware/usb/device/core`
   - `middleware/usb/device/descriptors`

### Step 3: Exclude Unnecessary Files (Optional but Recommended)

Some USB middleware files might have platform-specific code you don't need:

1. In Project Explorer, navigate to `middleware/usb/device/controller/`
2. Find `usb_device_ehci.c`
3. **Right-click** → **Resource Configurations → Exclude from Build**
4. Check **Debug** and **Release**
5. Click **OK**

### Step 4: Clean and Rebuild

1. **Project** → **Clean...**
2. Select your project
3. Click **Clean**
4. After cleaning, **Project** → **Build All**

---

## Verification

After rebuilding, you should see:

### ✅ Successful Build Output:
```
Building file: ../app/main.c
Building file: ../app/app_init.c
Building file: ../board/usb_device_board_init.c
Building file: ../middleware/usb/device/application/audio_speaker.c
Building file: ../middleware/usb/device/class/audio/audio_bridge.c
...
Finished building target: evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf
```

### ✅ Output Files Created:
```
Debug/evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf
Debug/evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.bin
```

---

## Troubleshooting Common Issues

### Issue 1: "usb.h: No such file or directory"

**Solution:** Add `middleware/usb/include/` to include paths (Step 1 above)

### Issue 2: "usb_phy.h: No such file or directory"

**Solution:** Add `middleware/usb/phy/` to include paths:
```
"${workspace_loc:/${ProjName}/middleware/usb/phy}"
```

### Issue 3: Linker errors about USB functions

**Solution:** Make sure all USB middleware .c files are included in build:
- `middleware/usb/device/core/*.c`
- `middleware/usb/device/class/audio/*.c`
- `middleware/usb/device/controller/usb_device_ehci.c` (or IP3511HS controller)
- `middleware/usb/device/descriptors/usb_device_descriptor.c`

### Issue 4: Multiple definition of 'main'

**Cause:** Both `app/main.c` and `source/sai_edma_tdm_record_playback.c` had main() functions

**Solution:** Already fixed in our code changes - we renamed main() to sai_edma_tdm_record_playback_init()

### Issue 5: "undefined reference to `USB_DeviceClassGetSpeed`"

**Solution:** Include USB device core files:
- Ensure `middleware/usb/device/core/*.c` files are in the build
- Add to source locations if missing

### Issue 6: USBHSD_IRQS not defined

**Solution:** This is board-specific. The USB interrupt handler might need adjustment.

**For RT1176 with IP3511HS:**
Check `device/MIMXRT1176_cm7.h` has the interrupt definitions. If build fails on USB interrupt, you may need to adjust `board/usb_device_board_init.c`.

---

## Quick Command-Line Check (Alternative)

If you prefer command-line builds, you can check if files are found:

```bash
cd /path/to/project
find . -name "audio_bridge.h"
find . -name "audio_speaker.c"
find . -name "usb_device_board_init.c"
```

All three should be found in the respective directories.

---

## After Successful Build

Once the build succeeds:

1. **Flash the firmware** to your RT1176 board
2. **Connect USB cable** from board USB1 (J9) to your laptop
3. **Open serial console** (115200 baud)
4. **Check Device Manager** (Windows) or `lsusb` (Linux) for "USB AUDIO DEMO"

---

## Still Having Issues?

If you continue to have build errors after following these steps:

1. Take a screenshot of the **full error message**
2. Check **Project → Properties → C/C++ Build → Settings → MCU C Compiler → Includes**
   - Verify all 9 include paths are present
3. Check **Project Explorer**
   - Verify all new .c files are visible and not excluded
4. Try **Project → Clean** and rebuild from scratch

---

## Summary of Required Include Paths

For reference, here's the complete list of include paths your project needs:

**Original paths** (already in your project):
- source
- drivers
- component/i2c
- codec
- codec/port/cs42448
- codec/port
- utilities
- device
- component/uart
- component/lists
- startup
- xip
- CMSIS
- board

**New paths** (need to be added):
- **app**
- **middleware/usb/device/application**
- **middleware/usb/device/class/audio**
- **middleware/usb/device/include**
- **middleware/usb/device/controller**
- **middleware/usb/device/core**
- **middleware/usb/device/descriptors**
- **middleware/usb/include**
- **middleware/usb/phy**

---

## Next Steps After Build Success

Once your build completes successfully, refer to `IMPLEMENTATION_SUMMARY.md` for:
- Hardware setup instructions
- Testing procedures
- Expected serial console output
- How to verify USB enumeration on your laptop
