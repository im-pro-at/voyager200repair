#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <xc.h>
#include <plib.h>
#include <ctype.h>

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>

#include "Configuration.h"

#include "delay.h"
#include "uart.h"
#include "lcd.h"


void _general_exception_handler(void)
{
    unsigned int _excep_addr;
    unsigned int _excep_code;
    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));
    _excep_code = (_excep_code & 0x0000007C) >> 2;    
    while(1)
    {
        //printf("EXSEPTION! %d\n\r %d\n\r",_excep_code, _excep_addr);
        delay_ms(1000);
    }
}



#define BufferSize 240/4*128*3

uint8_t dbuffer[BufferSize];


int main(){

    //Optimise System performance
    SYSTEMConfigPerformance(GetSystemClock());


    delay_ms(100);

    lcd_init();

    lcd_clear();
    
    
    //uart2Init(0);
    
    //DI1=RA0
    ANSELAbits.ANSA0=0;
    TRISAbits.TRISA0=1;
    CNPUAbits.CNPUA0=1;    
    //DI2=RA1
    ANSELAbits.ANSA1=0;
    TRISAbits.TRISA1=1;
    CNPUAbits.CNPUA1=1;    
    //DI3=RA2
    TRISAbits.TRISA2=1;
    CNPUAbits.CNPUA2=1;    
    //DI4=RA3
    TRISAbits.TRISA3=1;
    CNPUAbits.CNPUA3=1;    
    //FR =RA4
    TRISAbits.TRISA4=1;
    CNPUAbits.CNPUA4=1;    
    //SCP=RB2
    ANSELBbits.ANSB2=0;
    TRISBbits.TRISB2=1;
    CNPUBbits.CNPUB2=1;    
    PPSInput(3,INT2, RPB2);
    ConfigINT2(EXT_INT_DISABLE | FALLING_EDGE_INT | EXT_INT_PRI_4);
    //LP =RB3
    ANSELBbits.ANSB3=0;
    TRISBbits.TRISB2=1;
    CNPUBbits.CNPUB2=1;    
    PPSInput(1,INT4, RPB3);
    ConfigINT4(EXT_INT_DISABLE | FALLING_EDGE_INT | EXT_INT_PRI_2);

   
         
    //************DMA*************    
    // Open the desired DMA channel, in AUTO ENABLE mode. Priority 0.    
    DmaChnOpen(DMA_CHANNEL0, DMA_CHN_PRI3,DMA_OPEN_DEFAULT);   
 
    // set the transfer event control: what event is to start the DMA transfer (External Interrupt 1)    
    DmaChnSetEventControl(DMA_CHANNEL0, (DmaEvCtrlFlags)(DMA_EV_START_IRQ_EN|DMA_EV_START_IRQ(_EXTERNAL_2_IRQ)));    

    // set the transfer parameters: source & destination address, source & destination size, number of bytes per event    
    DmaChnSetTxfer(DMA_CHANNEL0, (const void*)&PORTA, dbuffer, 1, 240/4*128*3, 1);    
  
    //****************************    
 
    // Enable multi-vectored interrupts    
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();

    
    int w=240/4+1;
    while(true)
    {
        DmaChnEnable(DMA_CHANNEL0);  //read frame   

        delay_ms(50);

        //search for frame start:
        int offset=0;
        for(int i=0;i<(128*240/4*2);i++){
            if((dbuffer[i]&(1<<4))!=(dbuffer[i+1]&(1<<4))){
                offset=i+1;
                break;
            }
        }
        //Reorientate
        offset=offset+1-w;

        lcd_Address_set(0,0,(128+16)*2-1,(w-1)*4*2);


        //Print frame:
        for(int j=0;j<w-1;j++)
        for(int b=0;b<4;b++)
        for(int y=0;y<2;y++)
        for(int i=0;i<(128+16);i++) //160 => 16-128
        for(int x=0;x<2;x++)
                {
                    int d=0;
                    if(i>=16 && dbuffer[(offset+(i-16)*w+j)]&(1<<b)){
                        d=0xFF;
                    }
                    lcd_Write_Data(d);
                    lcd_Write_Data(d);                    
                }
            
        

    }
    
    return 1;
}
