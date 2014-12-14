#include "enemy.h"
#include <stdlib.h>
#include "../src/Pic/Pic.h"
#include "../src/drv/lcd320.h"

void Enemy_Init(EnemyStruct* enemy)
{
  U16 i,j;
  
  enemy->Score=0;//分数
  
  enemy->SmallFoe_State=0x0000;
  enemy->MiddleFoe_State=0x0000;
  enemy->BigFoe_State=0x0000;
  
  for(i=0;i<32;i++)
  {
    //小飞机初始化
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
    
    //中飞机初始化
    enemy->MiddleFoe[i].life=5;     //生命值
    enemy->MiddleFoe[i].status=ENEMY_NORMAL;    //状态
    enemy->MiddleFoe[i].action=0;   //动作
    enemy->MiddleFoe[i].speed=1;   //速度

    enemy->MiddleFoe[i].picwitch=gImage_MiddleFoe1_Witch;
    enemy->MiddleFoe[i].piclength=gImage_MiddleFoe1_Length;
    
    enemy->MiddleFoe[i].livepic[0]=gImage_MiddleFoe1;//一般状态图片
    enemy->MiddleFoe[i].livepic[1]=gImage_MiddleFoe2;//被射击时的图片
    
    enemy->MiddleFoe[i].deadpic[0]=&gImage_MiddleFoeD[0];
    enemy->MiddleFoe[i].deadpic[1]=&gImage_MiddleFoeD[gImage_MiddleFoe1_Witch*gImage_MiddleFoe1_Length*2];
    enemy->MiddleFoe[i].deadpic[2]=&gImage_MiddleFoeD[gImage_MiddleFoe1_Witch*gImage_MiddleFoe1_Length*4];
    enemy->MiddleFoe[i].deadpic[3]=&gImage_MiddleFoeD[gImage_MiddleFoe1_Witch*gImage_MiddleFoe1_Length*6];
    
    //大飞机初始化
    enemy->BigFoe[i].life=10;     //生命值
    enemy->BigFoe[i].status=ENEMY_NORMAL;    //状态
    enemy->BigFoe[i].action=0;   //动作
    enemy->BigFoe[i].speed=1;   //速度

    enemy->BigFoe[i].picwitch=gImage_BigFoe1_Witch;
    enemy->BigFoe[i].piclength=gImage_BigFoe1_Length;
    
    enemy->BigFoe[i].livepic[0]=gImage_BigFoe1;//一般状态图片
    enemy->BigFoe[i].livepic[1]=gImage_BigFoe2;//被射击时的图片
    
    enemy->BigFoe[i].deadpic[0]=&gImage_BigFoeD[0];
    enemy->BigFoe[i].deadpic[1]=&gImage_BigFoeD[gImage_BigFoe1_Witch*gImage_BigFoe1_Length*2];
    enemy->BigFoe[i].deadpic[2]=&gImage_BigFoeD[gImage_BigFoe1_Witch*gImage_BigFoe1_Length*4];
    enemy->BigFoe[i].deadpic[3]=&gImage_BigFoeD[gImage_BigFoe1_Witch*gImage_BigFoe1_Length*6];
    enemy->BigFoe[i].deadpic[4]=&gImage_BigFoeD[gImage_BigFoe1_Witch*gImage_BigFoe1_Length*8];
    enemy->BigFoe[i].deadpic[5]=&gImage_BigFoeD[gImage_BigFoe1_Witch*gImage_BigFoe1_Length*10];
  }
  
  //小飞机
  enemy->SmallFoe_Dead=0x0000;
  for(i=0;i<32;i++)
  {
      enemy->SmallFoe_DeadAction[i]=0;
  }
  
  //中飞机
  enemy->MiddleFoe_Dead=0x0000;
  for(i=0;i<32;i++)
  {
      enemy->MiddleFoe_DeadAction[i]=0;
  }
  
   //大飞机
  enemy->BigFoe_Dead=0x0000;
  for(i=0;i<32;i++)
  {
      enemy->BigFoe_DeadAction[i]=0;
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
      /*小飞机*/
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
     
     /*中飞机*/
      if(((enemy->MiddleFoe_State)>>i)&(0x0001)==0x0001)
      {
        y0=enemy->MiddleFoe[i].y0;
        y1=enemy->MiddleFoe[i].y1;
        y0=y0-enemy->MiddleFoe[i].speed;
        y1=y1-enemy->MiddleFoe[i].speed;
      if(y1<0)
      {
        enemy->MiddleFoe_State ^=(0x0001)<<i;
      }
      else
      {
        enemy->MiddleFoe[i].y0=y0;
        enemy->MiddleFoe[i].y1=y1;      
      }
     }
     
     /*大飞机*/
      if(((enemy->BigFoe_State)>>i)&(0x0001)==0x0001)
      {
        y0=enemy->BigFoe[i].y0;
        y1=enemy->BigFoe[i].y1;
        y0=y0-enemy->BigFoe[i].speed;
        y1=y1-enemy->BigFoe[i].speed;
      if(y1<0)
      {
        enemy->BigFoe_State ^=(0x0001)<<i;
      }
      else
      {
        enemy->BigFoe[i].y0=y0;
        enemy->BigFoe[i].y1=y1;      
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
      
      /*小飞机*/
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
     
    /*中飞机*/
     if((((enemy->MiddleFoe_State>>i))&(0x0001))==0x0001)
     {
       
        x0=enemy->MiddleFoe[i].x0;
        y0=enemy->MiddleFoe[i].y0;
        x1=enemy->MiddleFoe[i].x1;
        y1=enemy->MiddleFoe[i].y1;
        LCD_ShowPic(enemy->MiddleFoe[i].livepic[enemy->MiddleFoe[i].action],x0,y0,x1,y1);
        if(enemy->MiddleFoe[i].action==1)
        {
          enemy->MiddleFoe[i].action=0;
        }
     }
       
     //死亡状态飞机
     if((((enemy->MiddleFoe_Dead>>i))&(0x0001))==0x0001)
     {
        x0=enemy->MiddleFoe[i].x0;
        y0=enemy->MiddleFoe[i].y0;
        x1=enemy->MiddleFoe[i].x1;
        y1=enemy->MiddleFoe[i].y1;
        LCD_ShowPic(enemy->MiddleFoe[i].deadpic[(enemy->MiddleFoe_DeadAction[i])++],x0,y0,x1,y1);
        if(enemy->MiddleFoe_DeadAction[i]>=4)
        {
          enemy->MiddleFoe_Dead ^=(0x0001)<<i;
          enemy->MiddleFoe_DeadAction[i]=0;
          
          //死亡后加分
          enemy->Score+=6000;
        }
     }
     
     /*大飞机*/
     if((((enemy->BigFoe_State>>i))&(0x0001))==0x0001)
     {
       
        x0=enemy->BigFoe[i].x0;
        y0=enemy->BigFoe[i].y0;
        x1=enemy->BigFoe[i].x1;
        y1=enemy->BigFoe[i].y1;
        LCD_ShowPic(enemy->BigFoe[i].livepic[enemy->BigFoe[i].action],x0,y0,x1,y1);
        if(enemy->BigFoe[i].action==1)
        {
          enemy->BigFoe[i].action=0;
        }
     }
       
     //死亡状态飞机
     if((((enemy->BigFoe_Dead>>i))&(0x0001))==0x0001)
     {
        x0=enemy->BigFoe[i].x0;
        y0=enemy->BigFoe[i].y0;
        x1=enemy->BigFoe[i].x1;
        y1=enemy->BigFoe[i].y1;
        LCD_ShowPic(enemy->BigFoe[i].deadpic[(enemy->BigFoe_DeadAction[i])++],x0,y0,x1,y1);
        if(enemy->BigFoe_DeadAction[i]>=6)
        {
          enemy->BigFoe_Dead ^=(0x0001)<<i;
          enemy->BigFoe_DeadAction[i]=0;
          
          //死亡后加分
          enemy->Score+=10000;
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


//中飞机
void Enemy_AddMiddleFoe(EnemyStruct* enemy)
{
  U8 i;
  U16 x0,x1,y0,y1;
  
  U16 j,k;

  for(i=0;i<32;i++)
  {
    if((((enemy->MiddleFoe_State)>>i)&(0x0001))==0x0000)
    {
     
     if((((enemy->MiddleFoe_Dead>>i))&(0x0001))==0x0000)
     {
      enemy->MiddleFoe[i].x0 = rand()%(240-gImage_MiddleFoe1_Witch);
      enemy->MiddleFoe[i].y0 = 320-gImage_MiddleFoe1_Length;
      enemy->MiddleFoe[i].x1 = enemy->MiddleFoe[i].x0+gImage_MiddleFoe1_Witch;
      enemy->MiddleFoe[i].y1 = enemy->MiddleFoe[i].y0+gImage_MiddleFoe1_Length;//location
      enemy->MiddleFoe[i].speed=rand()%4+1;  
      
      x0=enemy->MiddleFoe[i].x0;
      y0=enemy->MiddleFoe[i].y0;
      x1=enemy->MiddleFoe[i].x1;
      y1=enemy->MiddleFoe[i].y1;

      enemy->MiddleFoe_State |= ((0x0001)<<i);
      break;
     }
    }
  }
}

void Enemy_GetMiddleFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1)
{
    U8 vx; 
    
    vx=7;
    
    if(((enemy->MiddleFoe_State)>>i)&(0x0001)==0x0001)
    {
      *x0=enemy->MiddleFoe[i].x0+vx;
      *x1=enemy->MiddleFoe[i].x1-vx;
      *y0=enemy->MiddleFoe[i].y0;
      *y1=enemy->MiddleFoe[i].y1;
    }
    else
    {
      *x0=0;
      *y0=0;
      *x1=0;
      *y1=0;
    }
}


void Enemy_DestoryMiddleFoe(EnemyStruct * enemy,U16 lox,U16 loy)
{
  U8 k;
  U16 x0,x1,y0,y1;
  U8 vx;
  U8 i;
    
  vx=7;

  for(k=0;k<32;k++)
  {
               
    if((((enemy->MiddleFoe_State)>>k)&(0x0001))==0x0001)
    { 
      x0=(enemy->MiddleFoe[k]).x0+vx;
      y0=(enemy->MiddleFoe[k]).y0;
      x1=(enemy->MiddleFoe[k]).x1-vx;
      y1=(enemy->MiddleFoe[k]).y1;
               
      if((x0<=lox)&&(x1>=lox)&&(y0<=loy)&&(y1>=loy))
      {
        ((enemy->MiddleFoe[k]).life)--;
        if(((enemy->MiddleFoe[k]).life)<=0)
        {
          enemy->MiddleFoe_State ^=(0x0001)<<k;             
          enemy->MiddleFoe_Dead |= (0x0001)<<k;
          enemy->MiddleFoe_DeadAction[i]=0;
        }
        else
        {
          (enemy->MiddleFoe[k]).action=1;
        }
      }
    }
  }
}


//大飞机
void Enemy_AddBigFoe(EnemyStruct* enemy)
{
  U8 i;
  U16 x0,x1,y0,y1;
  
  U16 j,k;

  for(i=0;i<32;i++)
  {
    if((((enemy->BigFoe_State)>>i)&(0x0001))==0x0000)
    {
     
     if((((enemy->BigFoe_Dead>>i))&(0x0001))==0x0000)
     {
      enemy->BigFoe[i].x0 = rand()%(240-gImage_BigFoe1_Witch);
      enemy->BigFoe[i].y0 = 320-gImage_BigFoe1_Length;
      enemy->BigFoe[i].x1 = enemy->BigFoe[i].x0+gImage_BigFoe1_Witch;
      enemy->BigFoe[i].y1 = enemy->BigFoe[i].y0+gImage_BigFoe1_Length;//location
      enemy->BigFoe[i].speed=rand()%4+1;  
      
      x0=enemy->BigFoe[i].x0;
      y0=enemy->BigFoe[i].y0;
      x1=enemy->BigFoe[i].x1;
      y1=enemy->BigFoe[i].y1;

      enemy->BigFoe_State |= ((0x0001)<<i);
      break;
     }
    }
  }
}

void Enemy_GetBigFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1)
{
    U8 vx; 
    
    vx=0;
    
    if(((enemy->BigFoe_State)>>i)&(0x0001)==0x0001)
    {
      *x0=enemy->BigFoe[i].x0+vx;
      *x1=enemy->BigFoe[i].x1-vx;
      *y0=enemy->BigFoe[i].y0;
      *y1=enemy->BigFoe[i].y1;
    }
    else
    {
      *x0=0;
      *y0=0;
      *x1=0;
      *y1=0;
    }
}


void Enemy_DestoryBigFoe(EnemyStruct * enemy,U16 lox,U16 loy)
{
  U8 k;
  U16 x0,x1,y0,y1;
  U8 vx;
  U8 i;
    
  vx=0;

  for(k=0;k<32;k++)
  {
               
    if((((enemy->BigFoe_State)>>k)&(0x0001))==0x0001)
    { 
      x0=(enemy->BigFoe[k]).x0+vx;
      y0=(enemy->BigFoe[k]).y0;
      x1=(enemy->BigFoe[k]).x1-vx;
      y1=(enemy->BigFoe[k]).y1;
               
      if((x0<=lox)&&(x1>=lox)&&(y0<=loy)&&(y1>=loy))
      {
        ((enemy->BigFoe[k]).life)--;
        if(((enemy->BigFoe[k]).life)==0)
        {
          enemy->BigFoe_State ^=(0x0001)<<k;             
          enemy->BigFoe_Dead |= (0x0001)<<k;
          enemy->BigFoe_DeadAction[i]=0;
        }
        else
        {
          (enemy->BigFoe[k]).action=1;
        }
      }
    }
  }
}
