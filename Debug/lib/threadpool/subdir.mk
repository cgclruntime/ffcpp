################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/threadpool/eager_scheduler.cpp \
../lib/threadpool/finished_pfids.cpp \
../lib/threadpool/function_scheduler.cpp \
../lib/threadpool/function_thread_pool.cpp \
../lib/threadpool/function_worker.cpp \
../lib/threadpool/waits_defender.cpp 

OBJS += \
./lib/threadpool/eager_scheduler.o \
./lib/threadpool/finished_pfids.o \
./lib/threadpool/function_scheduler.o \
./lib/threadpool/function_thread_pool.o \
./lib/threadpool/function_worker.o \
./lib/threadpool/waits_defender.o 

CPP_DEPS += \
./lib/threadpool/eager_scheduler.d \
./lib/threadpool/finished_pfids.d \
./lib/threadpool/function_scheduler.d \
./lib/threadpool/function_thread_pool.d \
./lib/threadpool/function_worker.d \
./lib/threadpool/waits_defender.d 


# Each subdirectory must supply rules for building sources it contributes
lib/threadpool/%.o: ../lib/threadpool/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/athrunarthur/boost_1_46_1/ -I/home/athrunarthur/log4cplus-1.0.4-rc7/include -I../include/ -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


