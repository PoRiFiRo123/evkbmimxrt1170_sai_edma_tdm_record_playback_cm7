################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../codec/fsl_codec_common.c \
../codec/fsl_codec_i2c.c \
../codec/fsl_cs42448.c 

C_DEPS += \
./codec/fsl_codec_common.d \
./codec/fsl_codec_i2c.d \
./codec/fsl_cs42448.d 

OBJS += \
./codec/fsl_codec_common.o \
./codec/fsl_codec_i2c.o \
./codec/fsl_cs42448.o 


# Each subdirectory must supply rules for building sources it contributes
codec/%.o: ../codec/%.c codec/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DMCUXPRESSO_SDK -DCODEC_CS42448_ENABLE -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\source" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\drivers" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\i2c" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec\port\cs42448" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec\port" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\utilities" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\device" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\uart" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\lists" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\startup" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\xip" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\CMSIS" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\board" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\evkbmimxrt1170\driver_examples\sai\edma_tdm_record_playback\cm7" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-codec

clean-codec:
	-$(RM) ./codec/fsl_codec_common.d ./codec/fsl_codec_common.o ./codec/fsl_codec_i2c.d ./codec/fsl_codec_i2c.o ./codec/fsl_cs42448.d ./codec/fsl_cs42448.o

.PHONY: clean-codec

