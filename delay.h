#ifndef _MYDELAY_H    /* Guard against multiple inclusion */
#define _MYDELAY_H

#define	GetSystemClock()      (40000000ul)
#define	GetPeripheralClock()  (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock() (GetSystemClock())

#ifdef __cplusplus
extern "C" {
#endif

extern void delay_ms(int ms);
extern void delay_us(int us);

#ifdef __cplusplus
}
#endif

#endif 

