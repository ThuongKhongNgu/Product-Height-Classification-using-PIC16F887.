
// ***************************************************
// DESCRIPTION: Code cua tap lenh dieu khien Character LCD.
// TOOL COMPILE: Hitech C (Ver 9.65)
// CHIP: PIC16F887
// HISTORY: 28/03/2013
// AUTHOR: Pham Quang Tri.
// *************************************************** 
#include <stdlib.h>
#include <ctype.h>
#include "lcd.h"

// ***************************************************
// Ten CTC:          lcd_busy()  
// Thong so dau vao:       
// Thong so dau ra:  1: LCD ban, 0: LCD hoan tat thao tac.   
// Cong dung:        Kiem tra va tra ve thong bao LCD dang xu ly du lieu.   
// ***************************************************
unsigned char lcd_busy()
{
// Khai bao bien.
unsigned char busy;

// Dinh nghia ham.
	LCD_DATA4_TRIS = 1;		// Cau hinh cac chan ket noi LCD.
	LCD_DATA5_TRIS = 1;
	LCD_DATA6_TRIS = 1;
	LCD_DATA7_TRIS = 1;
	
	LCD_RW = 1;
	LCD_RS = 0;
	__delay_us(20);
	LCD_EN = 1;
	__delay_us(20);
	
	busy = LCD_DATA7;	// Doc bit bao ban.
	
	LCD_EN = 0;
	__delay_us(20);
	LCD_EN = 1;
	__delay_us(20);
	LCD_EN = 0;
	
	return busy;	// Tra ve ket qua.
}


// ***************************************************
// Ten CTC:          lcd_get_byte(rs)  
// Thong so dau vao: rs: Qui uoc doc lenh hay du lieu (0: Lenh, 1: Du lieu).      
// Thong so dau ra:  Gia tri doc duoc tu LCD.   
// Cong dung:        Doc mot byte du lieu tu LCD.   
// ***************************************************
unsigned char lcd_get_byte(unsigned char rs)
{
// Khai bao bien.
	BYTE_VAL b;

// Dinh nghia ham.
	LCD_DATA4_TRIS = 1;		// Cau hinh cac chan ket noi LCD.
	LCD_DATA5_TRIS = 1;
	LCD_DATA6_TRIS = 1;
	LCD_DATA7_TRIS = 1;
	
	LCD_RW = 1;

	if(rs) 
		LCD_RS = 1;
	else
		LCD_RS = 0;	
	__delay_us(20);

	LCD_EN = 1;
	__delay_us(20);
	
	b.bits.b7 = LCD_DATA7;	// Doc 4 bit cao.
	b.bits.b6 = LCD_DATA6;
	b.bits.b5 = LCD_DATA5;
	b.bits.b4 = LCD_DATA4;
	
	LCD_EN = 0;
	__delay_us(20);

	LCD_EN = 1;
	__delay_us(20);

	b.bits.b3 = LCD_DATA7;	// Doc 4 bit thap.
	b.bits.b2 = LCD_DATA6;
	b.bits.b1 = LCD_DATA5;
	b.bits.b0 = LCD_DATA4;

	LCD_EN = 0;
	__delay_us(20);
	
	return b.Val;	// Tra ve ket qua doc duoc.
}

// ***************************************************
// Ten CTC:          lcd_put_nibble(b)  
// Thong so dau vao: b: Du lieu can gui di.   
// Thong so dau ra:     
// Cong dung:        Ghi 4 bit thap cua du lieu den LCD.   
// ***************************************************
void lcd_put_nibble(unsigned char b)
{
// Khai bao bien.
	BYTE_VAL temp;

// Dinh nghia ham.	
	LCD_DATA4_TRIS = 0;		// Cau hinh cac chan ket noi LCD.
	LCD_DATA5_TRIS = 0;
	LCD_DATA6_TRIS = 0;
	LCD_DATA7_TRIS = 0;
	
	temp.Val = b;
	
	LCD_DATA4 = temp.bits.b0;	// Gui 4 bit thap.
	LCD_DATA5 = temp.bits.b1;
	LCD_DATA6 = temp.bits.b2;
	LCD_DATA7 = temp.bits.b3;

	__delay_us(20);
	LCD_EN =  1;
	__delay_us(20);
	LCD_EN = 0;
}

// ***************************************************
// Ten CTC:          lcd_put_byte(rs,b)  
// Thong so dau vao: rs: Qui uoc doc lenh hay du lieu (0: Lenh, 1: Du lieu).   
//					 b: 8 bit du lieu/lenh can ghi den LCD.   
// Thong so dau ra:     
// Cong dung:        Ghi mot byte du lieu/lenh den LCD.   
// ***************************************************
void lcd_put_byte(unsigned char rs, unsigned char b)
{
// Khai bao bien.

// Dinh nghia ham.	
	LCD_DATA4_TRIS = 0;		// Cau hinh cac chan ket noi LCD.
	LCD_DATA5_TRIS = 0;
	LCD_DATA6_TRIS = 0;
	LCD_DATA7_TRIS = 0;
		
	if(rs) 
		LCD_RS = 1;
	else
		LCD_RS = 0;	
	__delay_us(20);

	LCD_RW = 0;
	__delay_us(20);

	LCD_EN = 0;
	
	lcd_put_nibble(b >> 4);		// Gui 4 bit cao.

	lcd_put_nibble(b & 0xf);	// Gui 4 bit thap.
}

// ***************************************************
// Ten CTC:          lcd_gotoxy(col,row)  
// Thong so dau vao: col: So thu tu cua cot (0..15).
//					 row: So thu tu cua dong (0..1).   
// Thong so dau ra:     
// Cong dung:        Dinh toa do tren LCD.   
// ***************************************************
void lcd_gotoxy(unsigned char col, unsigned char row)
{
    unsigned char address;

    if (row == 0)
        address = 0x80 + col;       // Dòng 1
    else if (row == 1)
        address = 0xC0 + col;       // Dòng 2
    else if (row == 2)
        address = 0x94 + col;       // Dòng 3
    else if (row == 3)
        address = 0xD4 + col;       // Dòng 4

    lcd_put_byte(0, address);  
    while (lcd_busy());
}

// ***************************************************
// Ten CTC:          lcd_putc(c)  
// Thong so dau vao: c: Ma ASCII cua ky tu can ghi.   
// Thong so dau ra:     
// Cong dung:        Ghi mot ky tu den LCD.   
// ***************************************************
void lcd_putc(char c)
{
// Khai bao bien.

// Dinh nghia ham.
	switch(c)
		{
		case '\f':				// Xoa man hinh.
			lcd_put_byte(0,1);
			while(lcd_busy());
			break;
		case '\n':				// Xuong dong va dua con tro ve dau dong.
			lcd_gotoxy(0,1);
			break;
		case '\b': 
			lcd_put_byte(0,0x10);
			while(lcd_busy()); 
			break;
		default:	// Hien thi tat ca ky tu mac dinh trong LCD (c: dia chi HEX cua ky tu)
			 // if(isprint(c))	// Dong lenh dung khi chi muon hien thi nhung ky tu tren ban phim PC?
				{
				lcd_put_byte(1,c);
				while(lcd_busy());
				}
			// break;
		}
}

// ***************************************************
// Ten CTC:          lcd_puts(*s)  
// Thong so dau vao: s: Dia chi noi chua chuoi thong tin can hien thi.   
// Thong so dau ra:     
// Cong dung:        Ghi mot chuoi den LCD.   
// ***************************************************
void lcd_puts(const char* s)
{
// Khai bao bien.

// Dinh nghia ham.
	while(*s)
		{
		lcd_putc(*s++);
		}
}

// ***************************************************
// Ten CTC:          lcd_init()  
// Thong so dau vao:       
// Thong so dau ra:     
// Cong dung:        Khoi dong va cau hinh LCD.   
// ***************************************************
void lcd_init()
{
// Khai bao bien.
unsigned char i;

// Dinh nghia ham.
	LCD_EN_TRIS = 0;		// Cau hinh cac chan ket noi LCD.
	LCD_RS_TRIS = 0;
	LCD_RW_TRIS = 0;
	LCD_DATA4_TRIS = 0;
	LCD_DATA5_TRIS = 0;
	LCD_DATA6_TRIS = 0;
	LCD_DATA7_TRIS = 0;

	__delay_ms(15);		// Tao tre cho cap nguon LCD on dinh.

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	
	lcd_put_nibble(3);	// Reset LCD
	__delay_ms(10);
	lcd_put_nibble(3);
	__delay_ms(10);
	lcd_put_nibble(3);
	__delay_ms(10);

	lcd_put_nibble(2);
	while(lcd_busy());	

	lcd_put_byte(0,FOUR_BIT & LINES_5X10);		// Loai LCD.
	while(lcd_busy());
	
	lcd_put_byte(0,DON & CURSOR_OFF & BLINK_OFF);	// Bat hien thi, tat con tro, tat chop tat con tro.
	while(lcd_busy());
	
	lcd_put_byte(0,0x01);						// Xoa man hinh va dua con tro ve dau dong.
	while(lcd_busy());

	lcd_put_byte(0,SHIFT_DISP_LEFT);			// Che do dich chuyen con tro.
	while(lcd_busy());
}

// ***************************************************
// Ten CTC:            lcd_ShiftLeft()
// Thong so dau vao:       
// Thong so dau ra:     
// Cong dung:          Thiet lap lenh dich chuyen noi dung hien thi sang trai.
// ***************************************************
void lcd_ShiftLeft(void)
{
// Khai bao bien.

// Dinh nghia ham.
     lcd_put_byte(0,0x18);
}

// ***************************************************
// Ten CTC:            lcd_ShiftRight()
// Thong so dau vao:       
// Thong so dau ra:     
// Cong dung:          Thiet lap lenh dich chuyen noi dung hien thi sang phai.
// ***************************************************
void lcd_ShiftRight(void)
{
// Khai bao bien.

// Dinh nghia ham.
     lcd_put_byte(0,0x1C);
}

// ***************************************************
// Ten CTC:          lcd_MoveRight(p)  
// Thong so dau vao: p: so lan di chuyen sang phai.      
// Thong so dau ra:     
// Cong dung:        Di chuyen sang phai p vi tri.  
// ***************************************************
void lcd_MoveRight(unsigned char p)
{
// Khai bao bien.
unsigned char i;

// Dinh nghia ham.
	for(i=0;i<p;i++)
		{
		lcd_ShiftRight();
        __delay_ms(100);
     	}
}

// ***************************************************
// Ten CTC:           lcd_MoveLeft(p) 
// Thong so dau vao:  p: so lan di chuyen sang trai.     
// Thong so dau ra:     
// Cong dung:         Di chuyen sang trai p vi tri. 
// ***************************************************
void lcd_MoveLeft(unsigned char p)
{
// Khai bao bien.
unsigned char i;

// Dinh nghia ham.
	for(i=0;i<p;i++)
		{
    	lcd_ShiftLeft();
        __delay_ms(100);
		}
}

// ***************************************************
// Ten CTC:          lcd_String_Delay(*s,dly)  
// Thong so dau vao:       
// Thong so dau ra:     
// Cong dung:        // Hien thi tung ky tu trong chuoi theo thoi gian.  
// ***************************************************
void lcd_String_Delay(unsigned char*s, unsigned int dly) 
	{
// Khai bao bien.

// Dinh nghia ham.
	while(*s!=0) 
		{
		lcd_putc(*s++);
//        __delay_ms(dly);
    	}
}

void putch(char c)
{
    lcd_putc(c);
}