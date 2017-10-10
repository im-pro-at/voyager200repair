#include <plib.h>
#include "delay.h"

void delay_ms(int ms){
    WriteCoreTimer(0);
    while(ReadCoreTimer()<(GetSystemClock()/2000)*ms);
}

void delay_us(int us){
    WriteCoreTimer(0);
    while(ReadCoreTimer()<(GetSystemClock()/2000000)*us);
}