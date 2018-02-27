################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/i2c_layer.c \
../src/linux_pistorms.c \
../src/linux_pistorms_brick.c \
../src/linux_pistorms_camera.c \
../src/linux_pistorms_motors.c \
../src/linux_pistorms_sensor_color.c \
../src/linux_pistorms_sensor_gyro.c \
../src/linux_pistorms_sensor_touch.c \
../src/linux_pistorms_sensor_ultrasonic.c \
../src/linux_pistorms_sensors.c \
../src/main.c 

O_SRCS += \
../src/main.o 

OBJS += \
./src/i2c_layer.o \
./src/linux_pistorms.o \
./src/linux_pistorms_brick.o \
./src/linux_pistorms_camera.o \
./src/linux_pistorms_motors.o \
./src/linux_pistorms_sensor_color.o \
./src/linux_pistorms_sensor_gyro.o \
./src/linux_pistorms_sensor_touch.o \
./src/linux_pistorms_sensor_ultrasonic.o \
./src/linux_pistorms_sensors.o \
./src/main.o 

C_DEPS += \
./src/i2c_layer.d \
./src/linux_pistorms.d \
./src/linux_pistorms_brick.d \
./src/linux_pistorms_camera.d \
./src/linux_pistorms_motors.d \
./src/linux_pistorms_sensor_color.d \
./src/linux_pistorms_sensor_gyro.d \
./src/linux_pistorms_sensor_touch.d \
./src/linux_pistorms_sensor_ultrasonic.d \
./src/linux_pistorms_sensors.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


