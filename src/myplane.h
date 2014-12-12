#ifndef _MY_PLANE_H
#define _MY_PLANE_H

#include "macro.h"

struct myplanestruct
{
  U8 life;     //生命值
  U8 status;    //状态
  U8 action;   //动作
  U8 speed;   //速度
  
  U16 x0,y0,x1,y1; //location
  
  U16 picwitch;

  U16 piclength;
  const U8* livepic[2];
  const U8* deadpic[4];
};

typedef struct myplanestruct MyPlaneStruct;

void MyPlane_Init(MyPlaneStruct * plane,U16 x0,U16 y0);//x0，y0为初始图片左下角位置
void MyPlane_Move(MyPlaneStruct * plane,U8 or,U8 step);
void MyPlane_AddAction(MyPlaneStruct * plane);
const U8* MyPlane_GetPic(MyPlaneStruct * plane,U16* x0,U16* x1,U16* y0,U16* y1);
void MyPlane_Dead(MyPlaneStruct * plane);


#define PLANE_NORMAL 1
#define PLANE_DEAD 0

#define MOVE_LEFT 0
#define MOVE_DOWN 1
#define MOVE_RIGHT 3
#define MOVE_UP 5

#endif