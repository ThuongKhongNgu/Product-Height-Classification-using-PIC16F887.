#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <htc.h>
#include"lcd.h"
// CONFIG1
#pragma config FOSC = HS       // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
//KICH HOAT CHAN RESET RC3
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 20000000
#define sensor_cao RC0
#define sensor_thap RC1
#define sensor_vua RC2
//MACRO "DATA" dung kieu du lieu uint8 de giam kich thuoc chuong trinh (0-255)
#define DATA uint8_t
DATA sp_cao = 0;
DATA sp_thap = 0;
DATA sp_vua = 0;
DATA tong = 0;
//co hieu cho phep cap nhat lcd
DATA flag_cao = 0;
DATA flag_thap = 0;
DATA flag_vua = 0;
DATA flag_tong = 0;
//Tao 1 ky tu "trai tim"
const unsigned char trai_tim[8] ={0x00, 0x0A, 0x1F, 0x1F, 0x1F, 0x0E, 0x04, 0x00};
//----------------------------------HAM DELAY---------------------------------//
void delayTimer0()
{
    //2,7 seconds, 2700 milis 
    int t = 500;
    while(t--)
 {
        TMR0 = 100; //set gia tri ban dau cho timer0
        INTCONbits.T0IF = 0; // set co ngat timer0 ve 0
        while(!INTCONbits.T0IF); //giong nhu 1 ham cho, neu ko co ngat timer0 thi
 }
}
//-------------------------------KET THUC HAM DELAY---------------------------//
//-------------------------------SERVO CAO------------------------------------//
void servo_xoay_P(void){
    PORTBbits.RB7=1;
    __delay_us(1000);
    PORTBbits.RB7=0;
    __delay_us(19000);
}
void servo_xoay_G(void){
    PORTBbits.RB7=1;
    __delay_us(1800);
    PORTBbits.RB7=0;
    __delay_us(23000);
}
//---------------------------------END SERVO CAO------------------------------//
//------------------------------SERVO THAP------------------------------------//
void servo_xoay_P2(void){
    PORTBbits.RB6=1;
    __delay_us(1000);
    PORTBbits.RB6=0;
    __delay_us(19000);
}
void servo_xoay_G2(void){
    PORTBbits.RB6=1;
    __delay_us(1800);
    PORTBbits.RB6=0;
    __delay_us(23000);
}
//---------------------------------END SERVO THAP-----------------------------//
//--------------------------------SERVO TRUNG---------------------------------//
void servo_xoay_P3(void){
    PORTBbits.RB5=1;
    __delay_us(1000);
    PORTBbits.RB5=0;
    __delay_us(19000);
}
void servo_xoay_G3(void){
    PORTBbits.RB5=1;
    __delay_us(1800);
    PORTBbits.RB5=0;
    __delay_us(23000);
}
//-------------------------------END SERVO TRUNG------------------------------//
void SP_CAO()
{
            if (sensor_cao == 1) 
            { 
                //Neu sensor nhan, quay servo 
                servo_xoay_P();
                sp_cao++;
                delayTimer0();
                servo_xoay_G();
            }
    flag_cao = 1;
    flag_tong =1;
}
void SP_THAP()
{
            if (sensor_thap == 1) 
            { 
                //Neu sensor nhan, quay servo 
                servo_xoay_P2();
                delayTimer0();
                sp_thap++;
                servo_xoay_G2();
            }
    flag_thap = 1;
    flag_tong =1;
}
void SP_VUA()
{
            if (sensor_vua == 1) 
            {  
                //Neu sensor nhan, quay servo 
                servo_xoay_P3(); 
                delayTimer0();
                sp_vua++;
                servo_xoay_G3();
            }
    flag_vua =1;
    flag_tong =1;
}
void PIC_Config()
{
    ANSEL = ANSELH = 0;
    PORTC=0x00;
    TRISC=0x07;
    PORTB=0x00;
    TRISB=0x07;
    OPTION_REGbits.T0CS = 0;// chon nguon xung clock 
    OPTION_REGbits.PSA = 0; // xac dinh bo chia tan WDT hay la TIMER0
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0; //
    OPTION_REGbits.PS0 = 0;// set bo chia truoc PRESCALER 1/32 (100)
}

void main(void)
{
    PIC_Config();
    lcd_init();
    //Tao 1 bien buffer "tam thoi" chua 1 chuoi de dinh dang gia tri so
    char buffer[16]; 
    //Chon vi tri cho CGRAM, thuong la 0x40
    //CGRAM la 1 bo nho dac biet cua LCD dung luu tru cac ki tu 
    //parameter "0" o day la GUI LENH, parameter "0x40" la dia chi CGRAM
    lcd_put_byte(0, 0x40); 
    for (int i = 0; i < 8; i++) 
    {
        // Ghi tung byte du lieu cua "trai tim" vao CGRAM
        //parameter "1" o day la GUI DATA
        //parameter "trai_tim[i]" doc gia tri tung byte cua "trai tim"
        //co 8 byte nen can doc 8 lan
        lcd_put_byte(1, trai_tim[i]); 
    }
    while(1) 
    {       
        SP_VUA();
        SP_CAO();
        SP_THAP();
        tong = sp_cao + sp_thap + sp_vua;
        //Dong 1, cot 0, in text
        lcd_gotoxy(0, 0);  
        lcd_puts("DO AN PLSP CHIEU CAO");
        //Dong 2, in cac loai san pham
        lcd_gotoxy(0, 1); 
        lcd_puts("CAO");
        lcd_gotoxy(6, 1); 
        lcd_puts("T.BINH");
        lcd_gotoxy(15, 1); 
        lcd_puts("THAP");
        //Dong 3, in gia tri cac san pham
        lcd_gotoxy(1, 2); 
        //Sprintf chuyen doi gia tri cua bien "sp_cao" luu vao bien "buffer"
        //Cuoi cung, in noi dung cua sprintf
        sprintf(buffer, "%d", sp_cao);
        lcd_puts(buffer);
        lcd_gotoxy(8, 2); 
        sprintf(buffer, "%d", sp_vua);
        lcd_puts(buffer);       
        lcd_gotoxy(16, 2); 
        sprintf(buffer, "%d", sp_thap);
        lcd_puts(buffer);
        //Dong 4, in tong so luong san pham dem duoc
        lcd_gotoxy(2, 3); 
        sprintf(buffer, "TONG SO LUONG:%d", tong);
        lcd_puts(buffer);
        //In ky tu trai tim
        lcd_gotoxy(0, 3); 
        //lcd_putc chay den dia chi 0x40 cua CGRAM, in ra trai tim 
        lcd_putc(0);
        lcd_gotoxy(19, 3); 
        lcd_putc(0);
        }
}
