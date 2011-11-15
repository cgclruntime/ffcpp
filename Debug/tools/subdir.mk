################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/main.cpp 

OBJS += \
./tools/main.o 

CPP_DEPS += \
./tools/main.d 


# Each subdirectory must supply rules for building sources it contributes
tools/%.o: ../tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/athrunarthur/boost_1_46_1/ -I/home/athrunarthur/log4cplus-1.0.4-rc7/include -I../include/ -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


