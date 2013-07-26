#ifndef _GPIO_H_
#define _GPIO_H_


void SetGpioFunction	(unsigned int pinNum, unsigned int funcNum);
void SetGpio			(unsigned int pinNum, unsigned int pinVal);



enum DETECT_TYPE {
	DETECT_NONE,
	DETECT_RISING,
	DETECT_FALLING
};

void EnableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type);
void DisableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type);

void ClearGpioInterrupt(unsigned int pinNum);



#endif
