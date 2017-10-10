#ifndef _MYUART_H    /* Guard against multiple inclusion */
#define _MYUART_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*getCharFunaction)(int);
    
    extern void uart2Init(getCharFunaction fuart2);
    
#ifdef __cplusplus
}
#endif

#endif 

