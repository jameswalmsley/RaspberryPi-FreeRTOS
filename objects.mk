#
#	FreeRTOS portable layer for RaspberryPi
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/port.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/portisr.o

#
#	FreeRTOS Core
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/croutine.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/list.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/queue.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/tasks.o

#
#	Interrupt Manager & GPIO Drivers
#
OBJECTS += $(BUILD_DIR)Demo/Drivers/interrupts.o
OBJECTS += $(BUILD_DIR)Demo/Drivers/gpio.o

$(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/port.o: CFLAGS += -I $(BASE)Demo/

#
#	Selected HEAP implementation for FreeRTOS.
#
OBJECTS += $(BUILD_DIR)/FreeRTOS/Source/portable/MemMang/heap_4.o

#
#	Startup and platform initialisation code.
#
OBJECTS += $(BUILD_DIR)Demo/startup.o


#
#	Main Test Program
#
OBJECTS += $(BUILD_DIR)Demo/main.o
