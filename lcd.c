
#include <plib.h>
#include "delay.h"
#include "lcd.h"

#define LCD_WR   LATBbits.LATB7     
#define LCD_RS   LATBbits.LATB6        
#define LCD_REST LATBbits.LATB4


void Lcd_Writ_Bus(unsigned char d)
{
    LCD_WR=0;
    LATBCLR=0xFF00;
    LATBSET=0x100*d;
    LCD_WR=1;
}


void lcd_Write_Com(unsigned char VH)  
{   
  LCD_RS=0;
  Lcd_Writ_Bus(VH);
}

void lcd_Write_Data(unsigned char VH)
{
  LCD_RS=1;
  Lcd_Writ_Bus(VH);
}

void lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
  lcd_Write_Com(com);
  lcd_Write_Data(dat);
}

void lcd_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    lcd_Write_Com(0x36);
    lcd_Write_Data(0b10001001);

    lcd_Write_Com(0x2a);
    lcd_Write_Data(x1>>8);
    lcd_Write_Data(x1);
    lcd_Write_Data(x2>>8);
    lcd_Write_Data(x2);
    lcd_Write_Com(0x2b);
    lcd_Write_Data(y1>>8);
    lcd_Write_Data(y1);
    lcd_Write_Data(y2>>8);
    lcd_Write_Data(y2);
    lcd_Write_Com(0x2c); 							 
}


void lcd_init(){
    TRISBbits.TRISB4=0; //OUTPUT
    TRISBbits.TRISB6=0; //OUTPUT
    TRISBbits.TRISB7=0; //OUTPUT
    TRISBbits.TRISB8=0; //OUTPUT
    TRISBbits.TRISB9=0; //OUTPUT
    TRISBbits.TRISB10=0; //OUTPUT
    TRISBbits.TRISB11=0; //OUTPUT
    TRISBbits.TRISB12=0; //OUTPUT
    TRISBbits.TRISB13=0; //OUTPUT
    TRISBbits.TRISB14=0; //OUTPUT
    TRISBbits.TRISB15=0; //OUTPUT
    
    LCD_WR=1;
    LCD_RS=1;
    LCD_REST=1;    
    
    LCD_REST=1;
    delay_ms(5); 
    LCD_REST=0;
    delay_ms(15);
    LCD_REST=1;
    delay_ms(15);
    
    lcd_Write_Com(0x11);
    delay_ms(20);
    lcd_Write_Com(0xD0);
    lcd_Write_Data(0x07);
    lcd_Write_Data(0x42);
    lcd_Write_Data(0x18);

    lcd_Write_Com(0xD1);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x07);
    lcd_Write_Data(0x10);

    lcd_Write_Com(0xD2);
    lcd_Write_Data(0x01);
    lcd_Write_Data(0x02);

    lcd_Write_Com(0xC0);
    lcd_Write_Data(0x10);
    lcd_Write_Data(0x3B);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x02);
    lcd_Write_Data(0x11);

    lcd_Write_Com(0xC5);
    lcd_Write_Data(0x03);
    lcd_Write_Com(0x36);
    lcd_Write_Data(0x0A);

    lcd_Write_Com(0x3A);
    lcd_Write_Data(0x55);

    lcd_Write_Com(0x2A);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x01);
    lcd_Write_Data(0x3F);

    lcd_Write_Com(0x2B);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x00);
    lcd_Write_Data(0x01);
    lcd_Write_Data(0xE0);
    delay_ms(120);
    lcd_Write_Com(0x29);
    lcd_Write_Com(0x002c); 
}


int lcd_rgb(unsigned int r,unsigned int g,unsigned int b) // 0 -31
{
    return (r&(0x1F)) << 11 | ((g<<1)&(0x3F)) << 5 | (b&(0x1F));
}

void lcd_clear()                   
{	
    unsigned int i,m;
    lcd_Address_set(0,0,320,480);
    for(i=0;i<320;i++){
        for(m=0;m<480;m++)
        {
          lcd_Write_Data(0);
          lcd_Write_Data(0);
        }
    }
}
