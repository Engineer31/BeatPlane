#include "myplane.h"
#include "../src/Pic/Pic.h"
void MyPlane_Init(MyPlaneStruct* plane,U16 x0,U16 y0)
{
  (plane->life)=1;
  plane->status=PLANE_NORMAL;
  plane->action=0;   //动作
  plane->speed=1;   //速度
  
  plane->x0=x0;
  plane->y0=y0;
  plane->x1=x0+gImage_MyPlane1_Witch;
  plane->y1=y0+gImage_MyPlane1_Length; //location
  
  plane->picwitch=gImage_MyPlane1_Witch;
  plane->piclength=gImage_MyPlane1_Length;
  plane->livepic[0]=gImage_MyPlane1;
  plane->livepic[1]=gImage_MyPlane2;
  
  plane->deadpic[0]=&gImage_MyPlaneD[0];

  plane->deadpic[1]=&gImage_MyPlaneD[gImage_MyPlane1_Witch*gImage_MyPlane1_Length*2];
  plane->deadpic[2]=&gImage_MyPlaneD[gImage_MyPlane1_Witch*gImage_MyPlane1_Length*4];
  plane->deadpic[3]=&gImage_MyPlaneD[gImage_MyPlane1_Witch*gImage_MyPlane1_Length*6];
}

void MyPlane_Move(MyPlaneStruct * plane,U8 or,U8 step)
{
   S16 x0,y0,x1,y1;
   x0= plane->x0;
   y0= plane->y0;
   x1= plane->x1;

   y1= plane->y1;
   if(or==MOVE_LEFT)
   {
     x0-=step*(plane->speed);
     x1-=step*(plane->speed);
   }
   else if(or==MOVE_RIGHT)
   {
     x0+=step*(plane->speed);
     x1+=step*(plane->speed);
   }
   else if(or==MOVE_UP)
   {
     y0+=step*(plane->speed);
     y1+=step*(plane->speed);
   }
   else if(or==MOVE_DOWN)
   {
     y0-=step*(plane->speed);
     y1-=step*(plane->speed);
   }
   else
   {
   }
   if(x0>0&&x0<240&&x1>0&&x1<240&&y0>0&&y0<320&&y1>0&&y1<320)
   {
     plane->x0=x0;
     plane->y0=y0;
     plane->x1=x1;

     plane->y1=y1;
   }
}

const U8* MyPlane_GetPic(MyPlaneStruct * plane,U16* x0,U16* x1,U16* y0,U16* y1)
{
  const U8 * pic;
  
  *x0=plane->x0;
  *x1=plane->x1;
  *y0=plane->y0;
  *y1=plane->y1;
  if(plane->status==PLANE_NORMAL)
  {
    pic=plane->livepic[plane->action];
  }
  else if(plane->status==PLANE_DEAD)
  {
    pic=plane->deadpic[plane->action];
  }
  else
  {
  }
  return pic;
}

void MyPlane_AddAction(MyPlaneStruct * plane)
{
  if(plane->status==PLANE_NORMAL)
  {
    plane->action+=1;
    if((plane->action)>=2)
    {
      plane->action=0;
    }
  }
  else if(plane->status==PLANE_DEAD)
  {
    plane->action+=1;
    if((plane->action)>=4)
    {
      plane->action=0;
    }
  }
  else
  {
  }
}


void MyPlane_Dead(MyPlaneStruct * plane)
{
  U8 temp=plane->status;
  if((plane->status)!=PLANE_DEAD)
  {
    plane->status=PLANE_DEAD;
    plane->action=0;
  }
}