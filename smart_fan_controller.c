#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000

// CONFIG
#pragma config FOSC = HS        // High-speed oscillator
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config BOREN = OFF      // Brown-out Reset Enable bit
#pragma config LVP = OFF        // Low-Voltage Programming
#pragma config CPD = OFF        // Data EEPROM memory code protection
#pragma config WRT = OFF        // Flash Program Memory Write Enable
#pragma config CP = OFF         // Flash Program Memory Code Protection

// Pin mappings
#define RELAY   RC0     // Relay control (Pin 15)
#define BUZZER  RC1     // Buzzer (Pin 16)
#define BUTTON  RB0     // Push button (Pin 33)
#define RS      RD0     // LCD Register Select (Pin 19)
#define EN      RD1     // LCD Enable (Pin 20)

unsigned int temp_raw;
float temperature;
unsigned char threshold = 20;
unsigned char button_pressed = 0;

// Function declarations
void ADC_Init();
unsigned int Read_ADC(unsigned char channel);
float Read_Stable_Temperature();
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);
void LCD_Init();
void LCD_Clear();
void LCD_Set_Cursor(unsigned char row, unsigned char column);
void __interrupt() ISR();

void main() {
   TRISA = 0xFF;
   TRISB = 0x01;
   TRISC = 0x00;
   TRISD = 0x00;
   PORTC = 0x00;
   PORTD = 0x00;
   OPTION_REG &= 0x7F;
   ADC_Init();
   LCD_Init();
   LCD_Clear();
   INTEDG = 1;
   INTE = 1;
   GIE = 1;

   LCD_Set_Cursor(1, 1);
   LCD_String("System Ready");
   __delay_ms(2000);

   while (1) {
       temperature = Read_Stable_Temperature();
       LCD_Clear();
       LCD_Set_Cursor(1, 1);
       LCD_String("Temp:");
       char temp_buffer[8];
       sprintf(temp_buffer, "%.1fC", temperature);
       LCD_String(temp_buffer);

       LCD_Set_Cursor(1, 12);
       LCD_String("T:");
       char thresh_buffer[4];
       sprintf(thresh_buffer, "%d", threshold);
       LCD_String(thresh_buffer);

       LCD_Set_Cursor(2, 1);
       if (temperature >= threshold) {
           RELAY = 1;
           BUZZER = 1;
           LCD_String("Fan: ON ");
       } else {
           RELAY = 0;
           BUZZER = 0;
           LCD_String("Fan: OFF");
       }

       if (button_pressed) {
           threshold += 5;
           if (threshold > 35) {
               threshold = 20;
           }
           button_pressed = 0;
           LCD_Set_Cursor(2, 9);
           LCD_String("THRESH!");
           __delay_ms(500);
       }

       __delay_ms(1000);
   }
}

void __interrupt() ISR() {
   if (INTF) {
       __delay_ms(50);
       if (BUTTON == 1) {
           button_pressed = 1;
       }
       INTF = 0;
   }
}

float Read_Stable_Temperature() {
    float total = 0;
    for (unsigned char i = 0; i < 5; i++) {
        temp_raw = Read_ADC(0);
        total += (temp_raw * 5.0 * 100.0) / 1024.0;
        __delay_ms(20);
    }
    return total / 5.0;
}

void ADC_Init() {
   ADCON0 = 0x41;
   ADCON1 = 0x80;
   __delay_ms(10);
}

unsigned int Read_ADC(unsigned char channel) {
   ADCON0 = (ADCON0 & 0xC7) | (channel << 3);
   __delay_ms(2);
   GO_nDONE = 1;
   while (GO_nDONE);
   return ((ADRESH << 8) + ADRESL);
}

void LCD_Command(unsigned char cmd) {
   RS = 0;
   PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
   EN = 1;
   __delay_ms(2);
   EN = 0;
   PORTD = (PORTD & 0x0F) | ((cmd << 4) & 0xF0);
   EN = 1;
   __delay_ms(2);
   EN = 0;
   __delay_ms(2);
}

void LCD_Char(unsigned char data) {
   RS = 1;
   PORTD = (PORTD & 0x0F) | (data & 0xF0);
   EN = 1;
   __delay_ms(2);
   EN = 0;
   PORTD = (PORTD & 0x0F) | ((data << 4) & 0xF0);
   EN = 1;
   __delay_ms(2);
   EN = 0;
   __delay_ms(2);
}

void LCD_String(const char *str) {
   while (*str) {
       LCD_Char(*str++);
   }
}

void LCD_Init() {
   __delay_ms(50);
   PORTD = (PORTD & 0x0F) | 0x30;
   EN = 1; __delay_ms(5); EN = 0;
   PORTD = (PORTD & 0x0F) | 0x30;
   EN = 1; __delay_ms(1); EN = 0;
   PORTD = (PORTD & 0x0F) | 0x30;
   EN = 1; __delay_ms(1); EN = 0;
   PORTD = (PORTD & 0x0F) | 0x20;
   EN = 1; __delay_ms(1); EN = 0;

   LCD_Command(0x28);
   LCD_Command(0x0C);
   LCD_Command(0x06);
   LCD_Command(0x01);
   __delay_ms(2);
}

void LCD_Clear() {
   LCD_Command(0x01);
   __delay_ms(2);
}

void LCD_Set_Cursor(unsigned char row, unsigned char column) {
   unsigned char pos = (row == 1) ? (0x80 + column - 1) : (0xC0 + column - 1);
   LCD_Command(pos);
}
