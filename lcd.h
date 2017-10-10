#ifndef _MYLCD_H    /* Guard against multiple inclusion */
#define _MYLCD_H

#ifdef __cplusplus
extern "C" {
#endif

extern void lcd_Write_Data(unsigned char VH);
extern void lcd_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
extern void lcd_init();
extern int lcd_rgb(unsigned int r,unsigned int g,unsigned int b);
extern void lcd_clear();

#ifdef __cplusplus
}
#endif

#endif 

