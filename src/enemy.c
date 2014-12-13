#include "enemy.h"
#include <stdlib.h>
#include "../src/Pic/Pic.h"
#include "../src/drv/lcd320.h"

void Enemy_Init(EnemyStruct* enemy)
{
  U16 i,j;
  
  enemy->SmallFoe_State=0x0000;
  for(i=0;i<32;i++)
  {
    enemy->SmallFoe[i].life=1;     //生命值
    enemy->SmallFoe[i].status=ENEMY_NORMAL;    //状态
    enemy->SmallFoe[i].action=0;   //动作
    enemy->SmallFoe[i].speed=1;   //速度
    enemy->Score=0;//分数

    enemy->SmallFoe[i].picwitch=gImage_SmallFoe_Witch;

    enemy->SmallFoe[i].piclength=gImage_SmallFoe_Length;
    enemy->SmallFoe[i].livepic[0]=gImage_SmallFoe;
    enemy->SmallFoe[i].deadpic[0]=&gImage_SmallFoeD[0];
    enemy->SmallFoe[i].deadpic[1]=&gImage_SmallFoeD[gImage_SmallFoe_Witch*gImage_SmallFoe_Length*2];
    enemy->SmallFoe[i].deadpic[2]=&gImage_SmallFoeD[gImage_SmallFoe_Witch*gImage_SmallFoe_Length*4];
  }
  
  enemy->SmallFoe_Dead=0x0000;
  for(i=0;i<32;i++)
  {
      enemy->SmallFoe_DeadAction[i]=0;
  }

}

void Enemy_AddSmallFoe(EnemyStruct* enemy)
{
  U8 i;
  U16 x0,x1,y0,y1;
  
  U16 j,k;

  for(i=0;i<32;i++)
  {
    if((((enemy->SmallFoe_State)>>i)&(0x0001))==0x0000)
    {
     
     if((((enemy->SmallFoe_Dead>>i))&(0x0001))==0x0000)
     {
      enemy->SmallFoe[i].x0 = rand()%(240-gImage_SmallFoe_Witch);
      enemy->SmallFoe[i].y0 = 320-gImage_SmallFoe_Length;
      enemy->SmallFoe[i].x1 = enemy->SmallFoe[i].x0+gImage_SmallFoe_Witch;
      enemy->SmallFoe[i].y1 = enemy->SmallFoe[i].y0+gImage_SmallFoe_Length;//location
      enemy->SmallFoe[i].speed=rand()%8+1;  
      
      x0=enemy->SmallFoe[i].x0;
      y0=enemy->SmallFoe[i].y0;
      x1=enemy->SmallFoe[i].x1;
      y1=enemy->SmallFoe[i].y1;

      enemy->SmallFoe_State |= ((0x0001)<<i);
      break;
     }
    }
  }
}

void Enemy_Move(EnemyStruct* enemy)
{
    S16 y0,y1;
    U8 i;
    
    for(i=0;i<32;i++)
    {
      if(((enemy->SmallFoe_State)>>i)&(0x0001)==0x0001)
      {
        y0=enemy->SmallFoe[i].y0;
        y1=enemy->SmallFoe[i].y1;
        y0=y0-enemy->SmallFoe[i].speed;
        y1=y1-enemy->SmallFoe[i].speed;
      if(y1<0)
      {
        enemy->SmallFoe_State ^=(0x0001)<<i;
      }
      else
      {
        enemy->SmallFoe[i].y0=y0;
        enemy->SmallFoe[i].y1=y1;      
      }
     }
    }
}

void Enemy_Show(EnemyStruct* enemy)
{
    U8 i;
    U16 x0,x1,y0,y1;
    
    for(i=0;i<32;i++)
    {
     if((((enemy->SmallFoe_State>>i))&(0x0001))==0x0001)
     {
       
        x0=enemy->SmallFoe[i].x0;
        y0=enemy->SmallFoe[i].y0;
        x1=enemy->SmallFoe[i].x1;
        y1=enemy->SmallFoe[i].y1;
        LCD_ShowPic(enemy->SmallFoe[i].livepic[0],x0,y0,x1,y1);
     }
       
     //死亡状态飞机
     if((((enemy->SmallFoe_Dead>>i))&(0x0001))==0x0001)
     {
        x0=enemy->SmallFoe[i].x0;
        y0=enemy->SmallFoe[i].y0;
        x1=enemy->SmallFoe[i].x1;
        y1=enemy->SmallFoe[i].y1;
        LCD_ShowPic(enemy->SmallFoe[i].deadpic[(enemy->SmallFoe_DeadAction[i])++],x0,y0,x1,y1);
        if(enemy->SmallFoe_DeadAction[i]>=3)
        {
          enemy->SmallFoe_Dead ^=(0x0001)<<i;
          enemy->SmallFoe_DeadAction[i]=0;
          
          //死亡后加分
          enemy->Score+=1000;
        }
     }
    }
}

void Enemy_GetSmallFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1)
{
    U8 vx; 
    
    vx=8;
    
    if(((enemy->SmallFoe_State)>>i)&(0x0001)==0x0001)
    {
      *x0=enemy->SmallFoe[i].x0+vx;
      *x1=enemy->SmallFoe[i].x1-vx;
      *y0=enemy->SmallFoe[i].y0;
      *y1=enemy->SmallFoe[i].y1;
    }
    else
    {
      *x0=0;
      *y0=0;
      *x1=0;
      *y1=0;
    }
}

void Enemy_DestorySmallFoe(EnemyStruct * enemy,U16 lox,U16 loy)
{
    U8 k;
    U16 x0,x1,y0,y1;
    U8 vx;
    U8 i;
    
    vx=8;

  for(k=0;k<32;k++)
  {
               
    if((((enemy->SmallFoe_State)>>k)&(0x0001))==0x0001)
    { 
      x0=(enemy->SmallFoe[k]).x0+vx;
                 y0=(enemy->SmallFoe[k]).y0;
                 x1=(enemy->SmallFoe[k]).x1-vx;
                 y1=(enemy->SmallFoe[k]).y1;
               
                 if((x0<=lox)&&(x1>=lox)&&(y0<=loy)&&(y1>=loy))
                 {
                      enemy->SmallFoe_State ^=(0x0001)<<k;
                      
                      enemy->SmallFoe_Dead |= (0x0001)<<k;
                      enemy->SmallFoe_DeadAction[i]=0;

                 }
    }
  }
}

U32 Enemy_GetScore(EnemyStruct * enemy)
{
  return enemy->Score;
}