#include "drivers.h"
#include "lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> //Ϊ���ṩ��������������Ŀ�
#include <math.h>
#include "lcd320.h"
#include "macro.h"
#include "reg2410.h"
#include "gpio.h"
#include "Serial.h"
//#include "global_varible.h"
#include "../uhal/isr.h"
#include "../uhal/uhal.h"
#include "uart.h"

#define LCDCON1_CLKVAL	(1<<8)   //ȷ��VCLK��Ƶ��
#define LCDCON1_MMODE	(0<<7)   //ȷ��VM����������  
#define LCDCON1_PNRMODE	(0x3<<5)       //ѡ����ʾģʽ ��11ΪTFT��ʾ��
#define LCDCON1_BPPMODE (0xc<<1)  //ѡ��bpp(bits per pixel)ģʽ��1100ΪTFT16λ
#define LCDCON1_ENVID	(1)   //ʹ����Ƶ�����LCD�����ź�   


#define LCDCON2_VBPD  9  //9
#define LCDCON2_LINEVAL 240
#define LCDCON2_VFPD 4
#define LCDCON2_VSPW 2
#define LCDCON3_HBPD  49   //49
#define LCDCON3_HOZVAL 320
#define LCDCON3_HFPD 15
#define LCDCON4_HSPW 20

#define LCDCON5_FRM565 1           
#define LCDCON5_INVVCLK 0          
#define LCDCON5_INVVLINE 1        
#define LCDCON5_INVVFRAME 1        
#define LCDCON5_INVVD 0            
#define LCDCON5_INVVDEN 0         
#define LCDCON5_INVPWREN 0         
#define LCDCON5_INVLEND 0          
#define LCDCON5_PWREN 1            
#define LCDCON5_ENLEND 0          
#define LCDCON5_BSWP 0    
#define LCDCON5_HWSWP 1     

#define BPP24BL 0     
#define TPALEN 1    
#define LPC_EN 1      
#define FIWSEL 0
#define INT_FrSyn 1  
#define INT_FiCnt 1   
#define MVAL 13         

U16 LCDBufferII1[153600];  
U32 LCDBufferII2[LCD_Y_MAX][LCD_X_MAX];      //�ɸı���ʾ����
U16* pLCDBuffer16I1 = (U16*)I1ADDR;
U16* pLCDBuffer16I2 = (U16*)I2ADDR;

#define LCD_LENGTH 320  //��
#define LCD_WITCH 240  //��
/*RGB565 ��������*/
U16 LCDBuffer565[LCD_WITCH][LCD_LENGTH];
//��ʾĬ�ϴ���Ϊ͸��

void LCD_Init()
{
	U32 i;
	U32 LCDBASEU,LCDBASEL,LCDBANK;

    	rGPCUP=0xffffffff; // Disable Pull-up register
    	rGPCCON=0xaaaaaaaa; //Initialize VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 

    	rGPDUP=0xffffffff; // Disable Pull-up register
    	rGPDCON=0xaaaaaaaa; //Initialize VD[23:8]
    	rLCDCON1=0|LCDCON1_BPPMODE|LCDCON1_PNRMODE|LCDCON1_MMODE|LCDCON1_CLKVAL;//|LCDCON1_MMODE;    
	    // disable  rLCDCON1��ΪLCD���ƼĴ���1
	rLCDCON2=(LCDCON2_VBPD<<24)|((LCDCON2_LINEVAL-1)<<14)|(LCDCON2_VFPD<<6)|LCDCON2_VSPW;
	rLCDCON3=(LCDCON3_HBPD<<19)|((LCDCON3_HOZVAL-1)<<8)|LCDCON3_HFPD;
       rLCDCON4=LCDCON4_HSPW|(MVAL<<8);
	rLCDCON5=(BPP24BL<<12)|(LCDCON5_FRM565<<11)|(LCDCON5_INVVCLK<<10)|(LCDCON5_INVVLINE<<9)|(LCDCON5_INVVFRAME<<8)|(LCDCON5_INVVD<<7)|\
		     (LCDCON5_INVVDEN<<6)|(LCDCON5_INVPWREN<<5)|(LCDCON5_INVLEND<<4)|(LCDCON5_PWREN<<3)|(LCDCON5_ENLEND<<2)|(LCDCON5_BSWP<<1)|LCDCON5_HWSWP;
	
	LCDBANK=0x31000000>>22;  
	LCDBASEU=0x0;            
        LCDBASEL=LCDBASEU+(240)*320;
       rLCDADDR1= (LCDBANK<<21)|LCDBASEU;    
	rLCDADDR2=LCDBASEL;	             
       rLCDADDR3= (320)|(0<<11);          
     	rLCDINTMSK=(INT_FrSyn<<1)|INT_FiCnt;
	rLCDLPCSEL=0;
	rTPAL=(0<<24);	  
         
       for(i=0;i<240*320;i++) 
	 *(pLCDBuffer16I1+i)=(0xf8<<11);/*show bgcolor red 0xf8<<11*/
    	rLCDCON1+=LCDCON1_ENVID;	
    	for(i=0;i<10000000;i++);
}


void LCD_Test(void)
{
  int i;
  int h,w;
  unsigned int temp;
  U8* pbuf=(U8*)LCDBufferII2[0];   // by sprife
  U32 LCDBASEU,LCDBASEL,LCDBANK; 
  LCDBANK=I2ADDR>>22;
  LCDBASEU = (I2ADDR>>1)&0x1FFFFF;       
  LCDBASEL = LCDBASEU+(240)*320;
  rLCDADDR1 = (LCDBANK<<21)|LCDBASEU;   //+ 320;    
  rLCDADDR2 = LCDBASEL;     //+ 320;	
  rLCDADDR3 = (320)|(0<<11); 
  
   extern const unsigned char gImage_BackGround[153600];
   for(i = 0;i < LCDWIDTH * LCDHEIGHT;i++){	
       *(pLCDBuffer16I2+i) = (U16)gImage_BackGround[2*i]+(U16)(gImage_BackGround[2*i+1]<<8);
  }
  /*
  extern const unsigned char gImage_play2[6200];
  for(h=0;h<50;h++)
    for(w=0;w<62;w++)
    {
      i=h*62+w;
      temp=(U16)gImage_play2[2*i]+(U16)(gImage_play2[2*i+1]<<8);
      if(temp!=0xFFFF)
        *(pLCDBuffer16I2+h*320+w) = (U16)gImage_play2[2*i]+(U16)(gImage_play2[2*i+1]<<8);
    }
  */
}

void LCD_ShowPic(const unsigned char *pic,U16 x0,U16 y0,U16 x1,U16 y1)
{
    U16 i,j;
    U16 temp;
    U16 tempcolor;
    
    for(i=x0;i<x1;i++)
      for(j=y0;j<y1;j++)
      {
         temp=(i-x0)*(y1-y0)+j-y0;
         tempcolor=(U16)(pic[2*temp])+((U16)(pic[2*temp+1])<<8);
         if(tempcolor!=0xffff)//����0xFFFFΪ͸��
         {
           LCDBuffer565[i-x0][j-y0]=tempcolor;
         }
      }
}


void LCD_Refresh()
{
  U16 i,j;
  U8* pbuf=(U8*)LCDBufferII2[0];   // by sprife
  U32 LCDBASEU,LCDBASEL,LCDBANK; 
  LCDBANK=I2ADDR>>22;
  LCDBASEU = (I2ADDR>>1)&0x1FFFFF;       
  LCDBASEL = LCDBASEU+(240)*320;
  rLCDADDR1 = (LCDBANK<<21)|LCDBASEU;   //+ 320;    
  rLCDADDR2 = LCDBASEL;     //+ 320;	
  rLCDADDR3 = (320)|(0<<11); 
  
  for(i=0;i<LCD_WITCH;i++)
     for(j=0;j<LCD_LENGTH;j++)
      {
         pLCDBuffer16I2[i*LCD_LENGTH+j]=LCDBuffer565[i][j];
      }
  /*
    for(i=0;i<240*320;i++)
         pLCDBuffer16I2[i]=0xFFFF;*/
   
  /*
  for(i=0;i<240;i++)
     for(j=0;j<320;j++)
      {
         pLCDBuffer16I2[i*LCD_LENGTH+j]=0xF100;
      }*/
}

/****************************************************************************************
��������void LCD_ScreenFill(COLOR stColour)
˵���������Ļ
*******************************************************************************************/
//extern static const unsigned int BackGroundPic[86400];
void LCD_ScreenFill(COLOR stColour)
{
    U16 uiXPos,uiYPos;
    /*
    for(uiYPos = 0; uiYPos< LCD_Y_MAX; uiYPos++)
        for(uiXPos =0; uiXPos<LCD_X_MAX; uiXPos++)
            LCDBufferII2[uiYPos][uiXPos] = stColour; */ //�ı���Ļ��ɫ
    
    /*
    for(uiYPos = 0; uiYPos< 240; uiYPos++)
        for(uiXPos =0; uiXPos<320; uiXPos++)
     
      LCDBufferII2[uiYPos][uiXPos] = BackGroundPic[240*uiYPos+uiXPos]; */ //�ı���Ļ��ɫ
    
    
    for(uiYPos = 20; uiYPos< 70; uiYPos++)
        for(uiXPos =20; uiXPos<82; uiXPos++)
          ;
    LCD_Refresh();
}

/***********************************************************************************************
��������void LCD_DrawPixel(U32 uiXPos,U32 uiYPos,COLOR stColour)
˵��������һ����
***********************************************************************************************/
void LCD_DrawPixel(U32 uiXPos,U32 uiYPos,COLOR stColour)
{
    if(uiXPos<LCD_X_MAX && uiYPos<LCD_Y_MAX)
        LCDBufferII2[uiYPos][uiXPos] = stColour;
}

/********************************************************************************************
��������void LCD_DrawHLine(U32 uiLeft,U32 uiRight,U32 uiYPos,COLOR stColour)
˵��;����һ��ˮƽֱ��
***********************************************************************************************/
void LCD_DrawHLine(U32 uiLeft,U32 uiRight,U32 uiYPos,COLOR stColour)
{
  U32 uiCount;
  if(uiLeft>=LCD_X_MAX||uiRight>=LCD_X_MAX||uiYPos>=LCD_Y_MAX)
    return;
  if(uiLeft<=uiRight)
  {
    for(uiCount=uiLeft;uiCount<=uiRight;uiCount++)
      LCDBufferII2[uiYPos][uiCount]=stColour;
  }
  else
  {
    for(uiCount=uiLeft;uiCount>=uiRight;uiCount--)
      LCDBufferII2[uiYPos][uiCount]=stColour;
  }
}
/************************************************************************************************
��������void LCD_DrawVLine(U32 uiTop,U32 uiBottom,U32 uiXPos,COLOR stColour)
˵��������һ����ֱ����
***********************************************************************************************/
void LCD_DrawVLine(U32 uiTop,U32 uiBottom,U32 uiXPos,COLOR stColour)
{
  U32 uiCount;
  if(uiTop>=LCD_Y_MAX||uiBottom>LCD_Y_MAX||uiXPos>LCD_X_MAX)
    return;
  if(uiTop<=uiBottom)
  {
    for(uiCount=uiTop;uiCount<=uiBottom;uiCount++)
      LCDBufferII2[uiCount][uiXPos]=stColour;
  }
  else
  {
    for(uiCount=uiTop;uiCount>=uiBottom;uiCount--)
      LCDBufferII2[uiCount][uiXPos]=stColour;
  }
}

/**********************************************************************************************
��������void LCD_DrawLine(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
˵�������������һ����
*********************************************************************************************/
void LCD_DrawLine(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
{
    S32 siDx, siDy, e;
    
    if(uiLeft>=LCD_X_MAX||uiRight>=LCD_X_MAX||uiTop>=LCD_Y_MAX||uiBottom>=LCD_Y_MAX)
    	return;
    	
    siDx = uiRight - uiLeft; 
    siDy = uiBottom - uiTop;
    
    if(siDx >= 0)
    {
        if(siDy >= 0)        // siDy>=0
        {
            if(siDx >= siDy)   // 1/8 octant
            {
                e = siDy-siDx/2;
                while(uiLeft <= uiRight)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e > 0) {uiTop+=1; e-=siDx;}
                    uiLeft += 1;
                    e += siDy;
                }
            }
            else        // 2/8 octant
            {
                e = siDx-siDy/2;
                while(uiTop <= uiBottom)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e > 0) {uiLeft+=1; e-=siDy;}
                    uiTop += 1;
                    e += siDx;
                }
            }
        }
        else           // siDy<0
        {
            siDy = -siDy;   // siDy=abs(siDy)
            
            if(siDx >= siDy) // 8/8 octant
            {
                e = siDy-siDx/2;
                while(uiLeft <= uiRight)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e > 0) {uiTop-=1; e-=siDx;}
                    uiLeft += 1;
                    e += siDy;
                }
            }
            else        // 7/8 octant
            {
                e = siDx-siDy/2;
                while(uiTop >= uiBottom)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e > 0) {uiLeft+=1; e-=siDy;}
                    uiTop -= 1;
                    e += siDx;
                }
            }
        }
    }
    else //siDx<0
    {
        siDx=-siDx;     //siDx=abs(siDx)
        if(siDy >= 0) // siDy>=0
        {
            if(siDx>=siDy) // 4/8 octant
            {
                e=siDy-siDx/2;
                while(uiLeft>=uiRight)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e>0){uiTop+=1;e-=siDx;}	
                    uiLeft-=1;
                    e+=siDy;
                }
            }
            else        // 3/8 octant
            {
                e=siDx-siDy/2;
                while(uiTop<=uiBottom)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e>0){uiLeft-=1;e-=siDy;}	
                    uiTop+=1;
                    e+=siDx;
                }
            }
        }
        else       // siDy<0
        {
            siDy=-siDy;   // siDy=abs(siDy)
            
            if(siDx>=siDy) // 5/8 octant
            {
                e=siDy-siDx/2;
                while(uiLeft>=uiRight)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e>0){uiTop-=1;e-=siDx;}	
                    uiLeft-=1;
                    e+=siDy;
                }
            }
            else        // 6/8 octant
            {
                e=siDx-siDy/2;
                while(uiTop>=uiBottom)
                {
                    LCDBufferII2[uiTop][uiLeft]=stColour;
                    if(e>0){uiLeft-=1;e-=siDy;}	
                    uiTop-=1;
                    e+=siDx;
                }
            }
        }
    }
}



/********************************************************************************************
��������void LCD_DrawRect(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
˵�������������һ��ֱ��
*********************************************************************************************/
void LCD_DrawRect(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
{
  LCD_DrawHLine(uiLeft,uiRight,uiTop,stColour);
  LCD_DrawHLine(uiLeft,uiRight,uiBottom,stColour);
  LCD_DrawVLine(uiTop,uiBottom,uiLeft,stColour);
  LCD_DrawVLine(uiTop,uiBottom,uiRight,stColour);
}


/**********************************************************************************************
��������void LCD_DrawRectFill(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
˵�����������
***********************************************************************************************/
void LCD_DrawRectFill(U32 uiLeft,U32 uiTop,U32 uiRight,U32 uiBottom,COLOR stColour)
{
  U32 uiXCount,uiYCount;
  for(uiXCount=uiLeft;uiXCount<=uiRight;uiXCount++)
    for(uiYCount=uiTop;uiYCount<=uiBottom;uiYCount++)
      LCDBufferII2[uiYCount][uiXCount]=stColour;
}
/**********************************************************************************************
��������void LCD_PaintBmp(U32 uiXPos,U32 uiYPos,U32 uiWidth,U32 uiHigh,COLOR stColour)           
***********************************************************************************************/
void LCD_PaintBmp(U32 uiXPos,U32 uiYPos,U32 uiWidth,U32 uiHigh,COLOR stColour)
{
  U32 uiXCount,uiYCount;
  if((uiXPos+uiWidth)>=LCD_X_MAX||(uiYPos+uiHigh)>=LCD_Y_MAX)
    return;
  for(uiYCount=0;uiYCount<uiHigh;uiYCount++)
    for(uiXCount=0;uiXCount<uiWidth;uiXCount++)
    {
      LCDBufferII2[uiYPos+uiYCount][uiXPos+uiXCount] =stColour;
    }
}
