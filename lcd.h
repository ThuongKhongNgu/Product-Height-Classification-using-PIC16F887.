
// ***************************************************
// DESCRIPTION: Khai bao ket noi va tap lenh dieu khien Character LCD.
// TOOL COMPILE: Hitech C (Ver 9.65)
// CHIP: PIC16F887
// HISTORY: 28/03/2013
// AUTHOR: Pham Quang Tri.
// *************************************************** 
#include <htc.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ	20000000
#endif

/* Cac khai bao dieu khien bat/tat hien thi */
	#define DON         0b00001111  /* Bat hien thi            */
	#define DOFF        0b00001011  /* Tat hien thi            */
	#define CURSOR_ON   0b00001111  /* Bat con tro             */
	#define CURSOR_OFF  0b00001101  /* Tat con tro             */
	#define BLINK_ON    0b00001111  /* Nhap nhay con tro       */
	#define BLINK_OFF   0b00001110  /* Khong nhap nhay con tro */

/* Cac khai bao dich chuyen hien thi va con tro */
	#define SHIFT_CUR_LEFT    0b00000100  /* Con tro dich sang trai  */
	#define SHIFT_CUR_RIGHT   0b00000101  /* Con tro dich sang phai  */
	#define SHIFT_DISP_LEFT   0b00000110  /* Hien thi dich sang trai */
	#define SHIFT_DISP_RIGHT  0b00000111  /* Hien thi dich sang phai */

/* Cac khai bao thiet lap chuc nang */
	#define FOUR_BIT   0b00101100  /* Giao dien 4-bit       */
	#define EIGHT_BIT  0b00111100  /* Giao dien 8-bit       */
	#define LINE_5X7   0b00110000  /* Ky tu 5x7, mot hang   */
	#define LINE_5X10  0b00110100  /* Ky tu 5x10       	    */
	#define LINES_5X7  0b00111000  /* Ky tu 5x7, nhieu hang */
    #define LINES_5X10 0b00111100  /* Ky tu 5x10,nhieu hang */


/* Cac khai bao ket noi chan LCD voi PIC */
	#define	LCD_RS		RD0
	#define	LCD_EN		RD2
	#define	LCD_RW		RD1
	#define	LCD_DATA4	RD4
	#define	LCD_DATA5	RD5
	#define	LCD_DATA6	RD6
	#define	LCD_DATA7	RD7
	
	#define	LCD_RS_TRIS	TRISD0
	#define	LCD_EN_TRIS	TRISD2
	#define	LCD_RW_TRIS	TRISD1
	#define	LCD_DATA4_TRIS	TRISD4
	#define	LCD_DATA5_TRIS	TRISD5
	#define	LCD_DATA6_TRIS	TRISD6
	#define	LCD_DATA7_TRIS	TRISD7

/* Khai bao bien du lieu 8-bit */
typedef union _BYTE_VAL
	{
    unsigned char Val;
    struct
    	{
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    	} bits;
	} BYTE_VAL;

/* Khai bao cac lenh dung cho LCD */
	void lcd_init(void);								// Khoi dong LCD. 
	unsigned char lcd_busy(void);						// Thong bao LCD ban xu ly thong tin.
	unsigned char lcd_get_byte(unsigned char rs);	// Doc mot byte du lieu tu LCD.
	void lcd_put_byte(unsigned char a,unsigned char b);		// Ghi mot byte du lieu/lenh den LCD.
	void lcd_gotoxy(unsigned char col, unsigned char row);	// Dinh toa do tren LCD.
	void lcd_putc(char c);							// Ghi mot ky tu den LCD.
	void lcd_puts(const char* s);					// Ghi mot chuoi den LCD.

	void lcd_ShiftLeft(void);				// Dich chuyen noi dung sang trai.
	void lcd_ShiftRight(void);				// Dich chuyen noi dung sang phai.
	void lcd_MoveRight(unsigned char p);	// Di chuyen sang phai p vi tri.
	void lcd_MoveLeft(unsigned char p);		// Di chuyen sang trai p vi tri.
	void lcd_String_Delay(unsigned char*s,unsigned int dly);	// Hien thi tung ky tu trong chuoi theo thoi gian.