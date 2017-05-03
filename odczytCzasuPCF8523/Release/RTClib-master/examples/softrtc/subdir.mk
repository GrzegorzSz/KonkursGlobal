################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
/home/yolf/Pobrane/libraries/RTClib-master/examples/softrtc/softrtc.ino 

INO_DEPS += \
./RTClib-master/examples/softrtc/softrtc.ino.d 


# Each subdirectory must supply rules for building sources it contributes
RTClib-master/examples/softrtc/softrtc.o: /home/yolf/Pobrane/libraries/RTClib-master/examples/softrtc/softrtc.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/yolf/eclipse/arduinoPlugin/tools/arduino/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/cores/arduino" -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/variants/mega" -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/libraries/Wire" -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/libraries/Wire/src" -I"/home/yolf/Pobrane/libraries/RTClib-master" -I"/home/yolf/eclipse/arduinoPlugin/libraries/SD/1.0.6" -I"/home/yolf/eclipse/arduinoPlugin/libraries/SD/1.0.6/src" -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/libraries/SPI" -I"/home/yolf/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.13/libraries/SPI/src" -I"/home/yolf/Pobrane/arduino-1.6.11/libraries/UTFT" -I"/home/yolf/Pobrane/libraries/RFID" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '


