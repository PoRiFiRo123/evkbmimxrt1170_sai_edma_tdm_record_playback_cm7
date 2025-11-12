################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include xip/subdir.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/lists/subdir.mk
-include component/i2c/subdir.mk
-include codec/port/cs42448/subdir.mk
-include codec/subdir.mk
-include board/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := evkbmimxrt1170_sai_edma_tdm_record_playback_cm7
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf

# Tool invocations
evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker -Map="evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -T evkbmimxrt1170_sai_edma_tdm_record_playback_cm7_Debug.ld -o "evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf"; # arm-none-eabi-objcopy -v -O binary "evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.axf" "evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.bin" ; # checksum -p MIMXRT1176xxxxx -d "evkbmimxrt1170_sai_edma_tdm_record_playback_cm7.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
