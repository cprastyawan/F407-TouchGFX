################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/generated/images/src/__designer/image_Blue_Gauges_original_gauge_background_style_01.cpp \
../TouchGFX/generated/images/src/__designer/image_Blue_Needles_original_gauge_needle_style_01.cpp 

OBJS += \
./TouchGFX/generated/images/src/__designer/image_Blue_Gauges_original_gauge_background_style_01.o \
./TouchGFX/generated/images/src/__designer/image_Blue_Needles_original_gauge_needle_style_01.o 

CPP_DEPS += \
./TouchGFX/generated/images/src/__designer/image_Blue_Gauges_original_gauge_background_style_01.d \
./TouchGFX/generated/images/src/__designer/image_Blue_Needles_original_gauge_needle_style_01.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/generated/images/src/__designer/%.o: ../TouchGFX/generated/images/src/__designer/%.cpp TouchGFX/generated/images/src/__designer/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX-2f-generated-2f-images-2f-src-2f-__designer

clean-TouchGFX-2f-generated-2f-images-2f-src-2f-__designer:
	-$(RM) ./TouchGFX/generated/images/src/__designer/image_Blue_Gauges_original_gauge_background_style_01.d ./TouchGFX/generated/images/src/__designer/image_Blue_Gauges_original_gauge_background_style_01.o ./TouchGFX/generated/images/src/__designer/image_Blue_Needles_original_gauge_needle_style_01.d ./TouchGFX/generated/images/src/__designer/image_Blue_Needles_original_gauge_needle_style_01.o

.PHONY: clean-TouchGFX-2f-generated-2f-images-2f-src-2f-__designer

