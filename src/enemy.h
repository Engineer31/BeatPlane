#ifndef _ENEMY_H
#define _ENEMY_H

#include "macro.h"

#define ENEMY_NORMAL 0
#define ENEMY_DEAD  1

struct enemyplanestruct
{
  S8 life;     //生命值
  U8 status;    //状态
  U8 action;   //动作
  U8 speed;   //速度
  
  U16 x0,y0,x1,y1; //location
  
  U16 vx0,vy0,vx1,vy1; //V碰撞体积
  
  U16 picwitch;

  U16 piclength;
  const U8* livepic[5];
  const U8* deadpic[6];
  
};

typedef struct enemyplanestruct EnemyPlaneStruct;

#define MAX_SMALLFOE 32
#define MAX_MIDDLEFOE 6
#define MAX_BIGFOE  8

struct enemystruct
{
 
  EnemyPlaneStruct SmallFoe[MAX_SMALLFOE];
  U32 SmallFoe_State;
  
  EnemyPlaneStruct MiddleFoe[MAX_SMALLFOE];
  U32 MiddleFoe_State;
  
  EnemyPlaneStruct BigFoe[MAX_SMALLFOE];
  U32 BigFoe_State;
  
  U32 SmallFoe_Dead;
  U32 SmallFoe_DeadAction[32];
  
  U32 MiddleFoe_Dead;
  U32 MiddleFoe_DeadAction[32];
  
  U32 BigFoe_Dead;
  U32 BigFoe_DeadAction[32];
  
  U32 Score;
};

typedef struct enemystruct EnemyStruct;

void Enemy_Init(EnemyStruct* enemy);
void Enemy_Move(EnemyStruct* enemy);
void Enemy_Show(EnemyStruct* enemy);
U32 Enemy_GetScore(EnemyStruct * enemy);

void Enemy_AddSmallFoe(EnemyStruct* enemy);
void Enemy_GetSmallFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1);
void Enemy_DestorySmallFoe(EnemyStruct * enemy,U16 lox,U16 loy);

void Enemy_AddBigFoe(EnemyStruct* enemy);
void Enemy_GetBigFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1);
void Enemy_DestoryBigFoe(EnemyStruct * enemy,U16 lox,U16 loy);

void Enemy_AddMiddleFoe(EnemyStruct* enemy);
void Enemy_GetMiddleFoeLocation(EnemyStruct * enemy,U8 i,U16* x0,U16* y0,U16* x1,U16* y1);
void Enemy_DestoryMiddleFoe(EnemyStruct * enemy,U16 lox,U16 loy);

#endif