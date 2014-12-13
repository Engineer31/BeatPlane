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

    enemy->SmallFoe[i].picwitch=gImage_SmallFoe_Witch;

    enemy->SmallFoe[i].piclength=gImage_SmallFoe_Length;
    enemy->SmallFoe[i].livepic[0]=gImage_SmallFoe;
    enemy->SmallFoe[i].deadpic[0]=&gImage_SmallFoeD[0];
    enemy->SmallFoe[i].deadpic[1]=&gImage_SmallFoeD[gImage_SmallFoe_Witch*gImage_SmallFoe_Length*2];
    enemy->SmallFoe[i].deadpic[2]=&gImage_SmallFoeD[gImage_SmallFoe_Witch*gImage_SmallFoe_Length*4];
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
     
      enemy->SmallFoe[i].x0 = rand()%(240-gImage_SmallFoe_Witch);
      enemy->SmallFoe[i].y0 = 320-gImage_SmallFoe_Length;
      enemy->SmallFoe[i].x1 = enemy->SmallFoe[i].x0+gImage_SmallFoe_Witch;
      enemy->SmallFoe[i].y1 = enemy->SmallFoe[i].y0+gImage_SmallFoe_Length;//location
      enemy->SmallFoe[i].speed=rand()%10+1;  
      
      x0=enemy->SmallFoe[i].x0;
      y0=enemy->SmallFoe[i].y0;
      x1=enemy->SmallFoe[i].x1;
      y1=enemy->SmallFoe[i].y1;

      enemy->SmallFoe_State |= ((0x0001)<<i);
      break;
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
    }
}