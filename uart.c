#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <plib.h>
#include "delay.h"
#include "uart.h"

static getCharFunaction funcuart2;

void uart2Init(getCharFunaction fuart2){
    funcuart2=fuart2;

    //UART2 DEBUG
    //Map TX to RB10
    TRISBbits.TRISB10=0; //OUTPUT
    PPSOutput(4, RPB10, U2TX);
    
    //Map RX to RB5
    TRISBbits.TRISB5=1; //input
    CNPUBbits.CNPUB5=1; //Pull-up
    PPSInput(2,U2RX, RPB5);
    
    U2MODEbits.SIDL=0; // Continue operation in Idle mode
    U2MODEbits.IREN=0; //IrDA is disabled
    U2MODEbits.UEN=0; //00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by corresponding bits in the PORTx register
    U2MODEbits.WAKE=0; //Wake-up disabled
    U2MODEbits.LPBACK=0; //Loopback mode is disabled
    U2MODEbits.ABAUD=0; // Baud rate measurement disabled or completed
    U2MODEbits.RXINV=0; //UxRX Idle state is ?1?
    U2MODEbits.BRGH=1; //Standard Speed mode ? 16x baud clock enabled
    U2MODEbits.PDSEL=0; //00 = 8-bit data, no parity
    U2MODEbits.STSEL=0; //0 = 1 Stop bit
    
    U2STAbits.ADM_EN=0; //Automatic Address Detect mode is enabled
    U2STAbits.UTXINV=0; //0 = UxTX Idle state is ?1?
    U2STAbits.URXEN=1; //  UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
    U2STAbits.UTXBRK=0; //Break transmission is disabled or completed
    U2STAbits.UTXEN=1; //= UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
    U2STAbits.ADDEN=0; //Address Detect mode is disabled

    U2BRG= ((GetPeripheralClock()/115200)/4) - 1;              //11500 bei 1MHz (61107G seite 13)

    U2MODEbits.ON=1; //UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits

    INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_1);
       
}

// UART 2 interrupt handler
void __ISR(_UART2_VECTOR, IPL2SOFT) IntUart2Handler(void)
{
    // Is this an RX interrupt?
    if (INTGetFlag(INT_SOURCE_UART_RX(UART2)))
    {
        //Call de callbackfunction
        if(U2STAbits.OERR==1){
            U2STAbits.OERR=0;
            if(funcuart2!=NULL)
                funcuart2(-1);                 
        }
        else
        {   
            int temp=U2RXREG;
            U2TXREG=temp; //ECHO
            if(funcuart2!=NULL)
                funcuart2(temp);        
        }
        // Clear the RX interrupt Flag
        INTClearFlag(INT_SOURCE_UART_RX(UART2));

    }

    // We don't care about TX interrupt
    if ( INTGetFlag(INT_SOURCE_UART_TX(UART2)) )
    {
        INTClearFlag(INT_SOURCE_UART_TX(UART2));
    }
}
