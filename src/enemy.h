#ifndef _ENEMY_H
#define _ENEMY_H

#include "macro.h"

#define ENEMY_NORMAL 0
#define ENEMY_DEAD  1

struct enemyplanestruct
{
  U8 life;     //����ֵ
  U8 status;    //״̬
  U8 action;   //����
  U8 speed;   //�ٶ�
  
  U16 x0,y0,x1,y1; //location
  
  U16 vx0,vy0,vx1,vy1; //V��ײ���
  
  U16 picwitch;

  U16 piclength;
  const U8* livepic[5];
  const U8* deadpic[5];
  
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
  U16 MiddleFoe_State;
  
  EnemyPlaneStruct BigFoe[MAX_SMALLFOE];
  U8 BigFoe_State;
};

typedef struct enemystruct EnemyStruct;

void Enemy_Init(EnemyStruct* enemy);
void Enemy_AddSmallFoe(EnemyStruct* enemy);
void Enemy_AddMiddleFoe(EnemyStruct* enemy);
void Enemy_AddBigFoe(EnemyStruct* enemy);

void Enemy_Move(EnemyStruct* enemy);

void Enemy_Show(EnemyStruct* enemy);
#endif