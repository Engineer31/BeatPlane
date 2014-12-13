#include "myplane.h"
#include "../src/Pic/Pic.h"
#include "../src/drv/lcd320.h"
void MyPlane_Init(MyPlaneStruct* plane,U16 x0,U16 y0)
{
  U8 i;
  
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
  
  plane->shotstate=0x0000;//子弹状态
  for(i=0;i<32;i++)
  {
    (plane->shot[i]).picwitch=gImage_Shot_Witch;

    (plane->shot[i]).piclength=gImage_Shot_Length;
    (plane->shot[i]).pic=gImage_Shot;
  }

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
   if(x0>=0&&x0<=240&&x1>=0&&x1<=240&&y0>=0&&y0<=320&&y1>=0&&y1<=320)
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

U8 MyPlane_AddAction(MyPlaneStruct * plane)
{
   U16 x0,x1,y0,y1;
   U8 i;
   U8 state;
   
   state=0;
   
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
      state=1;
      plane->action=0;
    }
  }
  else
  {
  }
  
  //子弹运动

  for(i=0;i<32;i++)
  {
    if(((plane->shotstate)>>i)&(0x0001)==0x0001)
    {
      y0=(plane->shot[i]).y0;
      y1=(plane->shot[i]).y1;
      y0=y0+30;
      y1=y1+30;
      if(y1>320)
      {
        plane->shotstate ^=(0x0001)<<i;
      }
      else
      {
       (plane->shot[i]).y0=y0;
       (plane->shot[i]).y1=y1;
      }
    }
  }
  
  return state;
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

void MyPlane_AddShot(MyPlaneStruct * plane)
{
  U8 i;
  U16 x0,x1,y0,y1;
  
  for(i=0;i<32;i++)
  {
    if((((plane->shotstate)>>i)&(0x0001))==0x0000)
    {
       x0=plane->x0+((plane->picwitch)-(plane->shot[i]).picwitch)/2;
       y0=plane->y1;
       x1=x0+(plane->shot[i]).picwitch;
       y1=y0+(plane->shot[i]).piclength; 
       if(x0>=0&&x0<=240&&x1>=0&&x1<=240&&y0>=0&&y0<=320&&y1>=0&&y1<=320)
      {
      (plane->shot[i]).x0=x0;
      (plane->shot[i]).y0=y0;
      (plane->shot[i]).x1=x1;
      (plane->shot[i]).y1=y1;
      plane->shotstate |= ((0x0001)<<i);
      break;
      }
    }
  }
}

void MyPlane_ShowShot(MyPlaneStruct * plane)
{
  U8 i;
  U16 x0,x1,y0,y1;
  
    for(i=0;i<32;i++)
    {
     if((((plane->shotstate)>>i)&(0x0001))==0x0001)
     {
      x0=(plane->shot[i]).x0;
      y0=(plane->shot[i]).y0;
      x1=(plane->shot[i]).x1;
      y1=(plane->shot[i]).y1;
      LCD_ShowPic((plane->shot[i]).pic,x0,y0,x1,y1);
     }
    }
}

void MyPlane_GetLocation(MyPlaneStruct * plane,U16* x0,U16* y0,U16* x1,U16* y1)
{
    U8 myplane_vx;
    
    myplane_vx=15;
    
    *x0=plane->x0+myplane_vx;
    *x1=plane->x1-myplane_vx;
    *y0=plane->y0;
    *y1=plane->y1;
}

void MyPlane_GetShotLocation(MyPlaneStruct * plane,U8 i,U16* x0,U16* y0,U16* x1,U16* y1)
{
    if(((plane->shotstate)>>i)&(0x0001)==0x0001)
    {
      *x0=plane->shot[i].x0;
      *x1=plane->shot[i].x1;
      *y0=plane->shot[i].y0;
      *y1=plane->shot[i].y1;
    }
    else
    {
      *x0=0;
      *y0=0;
      *x1=0;
      *y1=0;
    }
}

void MyPlane_DestoryShot(MyPlaneStruct * plane,U16 lox,U16 loy)
{
    U8 k;
  U16 x0,x1,y0,y1;
  

  for(k=0;k<32;k++)
  {
               
           if((((plane->shotstate)>>k)&(0x0001))==0x0001)
              { 
                 x0=(plane->shot[k]).x0;
                 y0=(plane->shot[k]).y0;
                 x1=(plane->shot[k]).x1;
                 y1=(plane->shot[k]).y1;
               
                 if((x0<=lox)&&(x1>=lox)&&(y0<=loy)&&(y1>=loy))
                 {
                      plane->shotstate ^=(0x0001)<<k;
                 }
              }
  }
}

void MyPlane_Destory(MyPlaneStruct * plane)
{
    plane->status=PLANE_DEAD;
}



