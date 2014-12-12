#include "macro.h"

#define LCDWIDTH		320
#define LCDHEIGHT		240

#define LCD_X_MAX 	320
#define LCD_Y_MAX	240
#define LCD_HOZVAL	(LCD_X_MAX - 5)
#define LCD_LINEVA	(LCD_Y_MAX - 5)

//U32* pLCDBuffer16=(U32*)0x03000000;
/*#define LCDCON1		(*(volatile unsigned *)0X4D000000)
#define LCDCON2		(*(volatile unsigned *)0X4D000004)
#define LCDCON3		(*(volatile unsigned *)0X4D000008)
#define LCDCON4		(*(volatile unsigned *)0X4D00000C)
#define LCDCON5		(*(volatile unsigned *)0X4D000010)
#define LCDSADDR1		(*(volatile unsigned *)0X4D000014)
#define LCDSADDR2		(*(volatile unsigned *)0X4D000018)
#define LCDSADDR3	    (*(volatile unsigned *)0X4D00001C)*/
//#define GPGCON	    (*(volatile unsigned *)0x56000060)
#define I1ADDR  (0x31000000)
#define I2ADDR  (0x31096000)

#define RED_COLOR       0x000000f8
#define BLACK_COLOR     0x00000000
#define CHENGSE_COLOR   0x0000f0f8
#define YELLOW_COLOR    0x0000fcf8
#define GREEN_COLOR     0x0000fc00
#define QINGSE_COLOR    0x00f8fc00
#define BLUE_COLOR      0x00f80000
#define ZISE_COLOR      0x00f800f8
#define WHITE_COLOR     0x00f8fcf8




typedef U32 COLOR;

void LCD_ScreenFill(COLOR stColour);
void LCD_DrawPixel(U32 uiXPos,U32 uiYPos,COLOR stColour);
void LCD_DrawHLine(U32 uiLeft,U32 uiRight,U32 uiYPos,COLOR stColour);
void LCD_DrawVLine(U32 uiTop,U32 uiBottom,U32 uiXPos,COLOR stColour);
void LCD_DrawLine(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour);
void LCD_DrawRect(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour);
void LCD_DrawRectFill(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour);
void LCD_PaintBmp(U32 uiXPos,U32 uiYPos,U32 uiWidth,U32 uiHigh,COLOR stColour);
void LCD_Test(void);

void LCD_Init(void);
void LCD_ShowPic(const unsigned char *pic,U16 x0,U16 y0,U16 x1,U16 y1);
void LCD_Refresh(void);//Ë¢ÐÂÆÁÄ»

