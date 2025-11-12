################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_anatop_ai.c \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_dcdc.c \
../drivers/fsl_dmamux.c \
../drivers/fsl_edma.c \
../drivers/fsl_gpc.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpi2c.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_mu.c \
../drivers/fsl_pmu.c \
../drivers/fsl_sai.c \
../drivers/fsl_sai_edma.c \
../drivers/fsl_soc_src.c 

C_DEPS += \
./drivers/fsl_anatop_ai.d \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_dcdc.d \
./drivers/fsl_dmamux.d \
./drivers/fsl_edma.d \
./drivers/fsl_gpc.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpi2c.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_mu.d \
./drivers/fsl_pmu.d \
./drivers/fsl_sai.d \
./drivers/fsl_sai_edma.d \
./drivers/fsl_soc_src.d 

OBJS += \
./drivers/fsl_anatop_ai.o \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_dcdc.o \
./drivers/fsl_dmamux.o \
./drivers/fsl_edma.o \
./drivers/fsl_gpc.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpi2c.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_mu.o \
./drivers/fsl_pmu.o \
./drivers/fsl_sai.o \
./drivers/fsl_sai_edma.o \
./drivers/fsl_soc_src.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1176DVMAA -DCPU_MIMXRT1176DVMAA_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DMCUXPRESSO_SDK -DCODEC_CS42448_ENABLE -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\source" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\drivers" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\i2c" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec\port\cs42448" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\codec\port" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\utilities" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\device" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\uart" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\component\lists" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\startup" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\xip" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\CMSIS" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\board" -I"D:\Onedrive\OneDrive - Tessolve Semiconductor Private Limited\Documents\MCUXpressoIDE_11.9.0_2144\workspace\evkbmimxrt1170_sai_edma_tdm_record_playback_cm7\evkbmimxrt1170\driver_examples\sai\edma_tdm_record_playback\cm7" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_anatop_ai.d ./drivers/fsl_anatop_ai.o ./drivers/fsl_cache.d ./drivers/fsl_cache.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_dcdc.d ./drivers/fsl_dcdc.o ./drivers/fsl_dmamux.d ./drivers/fsl_dmamux.o ./drivers/fsl_edma.d ./drivers/fsl_edma.o ./drivers/fsl_gpc.d ./drivers/fsl_gpc.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpi2c.d ./drivers/fsl_lpi2c.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_mu.d ./drivers/fsl_mu.o ./drivers/fsl_pmu.d ./drivers/fsl_pmu.o ./drivers/fsl_sai.d ./drivers/fsl_sai.o ./drivers/fsl_sai_edma.d ./drivers/fsl_sai_edma.o ./drivers/fsl_soc_src.d ./drivers/fsl_soc_src.o

.PHONY: clean-drivers

