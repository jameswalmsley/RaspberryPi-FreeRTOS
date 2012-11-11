
CFLAGS += -march=armv6z -g
CFLAGS += -I $(BASE)FreeRTOS/Source/portable/GCC/RaspberryPi/
CFLAGS += -I $(BASE)FreeRTOS/Source/include/
CFLAGS += -I $(BASE)Demo/Drivers/

TOOLCHAIN=arm-none-eabi-
