################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/i2c_layer.c \
../src/marte_pistorms.c \
../src/marte_pistorms_brick.c \
../src/marte_pistorms_camera.c \
../src/marte_pistorms_motors.c \
../src/marte_pistorms_sensor_color.c \
../src/marte_pistorms_sensor_gyro.c \
../src/marte_pistorms_sensor_touch.c \
../src/marte_pistorms_sensor_ultrasonic.c \
../src/marte_pistorms_sensors.c \
../src/objects_prueba.c 

OBJS += \
./src/i2c_layer.o \
./src/marte_pistorms.o \
./src/marte_pistorms_brick.o \
./src/marte_pistorms_camera.o \
./src/marte_pistorms_motors.o \
./src/marte_pistorms_sensor_color.o \
./src/marte_pistorms_sensor_gyro.o \
./src/marte_pistorms_sensor_touch.o \
./src/marte_pistorms_sensor_ultrasonic.o \
./src/marte_pistorms_sensors.o \
./src/objects_prueba.o 

C_DEPS += \
./src/i2c_layer.d \
./src/marte_pistorms.d \
./src/marte_pistorms_brick.d \
./src/marte_pistorms_camera.d \
./src/marte_pistorms_motors.d \
./src/marte_pistorms_sensor_color.d \
./src/marte_pistorms_sensor_gyro.d \
./src/marte_pistorms_sensor_touch.d \
./src/marte_pistorms_sensor_ultrasonic.d \
./src/marte_pistorms_sensors.d \
./src/objects_prueba.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


