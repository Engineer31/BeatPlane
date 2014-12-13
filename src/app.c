#include "includes.h"

/*--------设置任务堆栈--------*/
static OS_STK StartGame_task_stk[STARTGAME_TASK_STK_SIZE];		  	
static OS_STK Key_task_stk[KEY_TASK_STK_SIZE];	
static OS_STK MyPlane_task_stk[MYPLANE_TASK_STK_SIZE];	
static OS_STK LCD_task_stk[LCD_TASK_STK_SIZE];
static OS_STK Shot_task_stk[SHOT_TASK_STK_SIZE];
static OS_STK Enemy_task_stk[ENEMY_TASK_STK_SIZE];
static OS_STK Status_task_stk[SHOT_TASK_STK_SIZE];

/************ 用户事件定义 ******************************/
OS_EVENT *Key_Mbox;

/************定义个体***********************************/
MyPlaneStruct MyPlane;
EnemyStruct  Enemy;
U32 GameScore;
U8 Clash[240][320];

//启动任务
void Task_Start(void *p_arg)
{
          
    OSRunning=TRUE;	//begin OS
    uHALr_InstallSystemTimer();//初始化系统时钟
    
    (void)p_arg;// 'p_arg' 并没有用到，防止编译器提示警告
    
    /*创建开始游戏任务*/
    OSTaskCreate(Task_StartGame,(void *)0,
	 &StartGame_task_stk[STARTGAME_TASK_STK_SIZE-1],STARTGAME_TASK_PRIO);
    
    /*初始化个体*/
    MyPlane_Init(&MyPlane,95,0);
    Enemy_Init(&Enemy);
    GameScore=0;
    
    /*LCD任务*/
    OSTaskCreate(Task_LCD,(void *)0,
	&LCD_task_stk[LCD_TASK_STK_SIZE-1],LCD_TASK_PRIO);	
    
    /*Myplane 任务*/
    OSTaskCreate(Task_MyPlane,(void *)0,
	  &MyPlane_task_stk[MYPLANE_TASK_STK_SIZE-1],MYPLANE_TASK_PRIO);	
    
     Key_Mbox = OSMboxCreate((void *)0); 			//创建键盘消息 邮箱
     
    /*shot任务*/
    OSTaskCreate(Task_Shot,(void *)0,
	&Shot_task_stk[SHOT_TASK_STK_SIZE-1],SHOT_TASK_PRIO);	
    
    /*Enemy任务*/
    OSTaskCreate(Task_Enemy,(void *)0,
	&Enemy_task_stk[ENEMY_TASK_STK_SIZE-1],ENEMY_TASK_PRIO);	
     
    /*创建键盘任务*/
    OSTaskCreate(Task_Key,(void *)0,
	   &Key_task_stk[KEY_TASK_STK_SIZE-1],KEY_TASK_PRIO);
    
    /*个体状态计算任务*/
    OSTaskCreate(Task_Status,(void *)0,
	 &Status_task_stk[STATUS_TASK_STK_SIZE-1],STATUS_TASK_PRIO);
      
    while (1)
    {
        OSTimeDlyHMSM(0, 0,0,500);
    }
}

//开始游戏任务
void Task_StartGame(void *p_arg)
{
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    LCD_ShowPic(gImage_BeatPlane_Text,(240-gImage_BeatPlane_Text_Witch)/2,100
                                      ,gImage_BeatPlane_Text_Witch+(240-gImage_BeatPlane_Text_Witch)/2
                                      ,gImage_BeatPlane_Text_Length+100);
    LCD_Refresh();
    hudelay(30000000);

    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    LCD_Refresh();
    OSTaskDel(OS_PRIO_SELF);
}

//主飞机任务
void Task_MyPlane(void *p_arg)
{
   //初始化飞机
   U8*  msg;
   U8 err;

   for(;;)
   {
     msg = (U8 *)OSMboxPend(Key_Mbox, 0, &err);		//接受邮箱消息{按键}
     
     if(OS_NO_ERR == err)
     {
          if(0 == msg[0])//左
          {
             MyPlane_Move(&MyPlane,MOVE_LEFT,1);
          }
          if(0 == msg[1])//下
          {
            MyPlane_Move(&MyPlane,MOVE_DOWN,1);
          }
          if(0 == msg[2])//上
          {
            MyPlane_Move(&MyPlane,MOVE_UP,1);
          }

          if(0 == msg[3])//右
          {
            MyPlane_Move(&MyPlane,MOVE_RIGHT,1);
          }

          if(0 == msg[4])//中
          {
          }
     } 
     
     OSTimeDlyHMSM(0, 0,0,10);
   }
   
}

//键盘任务
void Task_Key(void *p_arg)
{
  U8 key[5];
  
  for(;;)
  {   
    key[0] = read_gpio_bit(GPIO_F3);
    key[1] = read_gpio_bit(GPIO_F5);
    key[2] = read_gpio_bit(GPIO_G1);
    key[3] = read_gpio_bit(GPIO_G3);
    key[4] = read_gpio_bit(GPIO_G4);
    
    if((key[0]&&key[1]&&key[2]&&key[3]&&key[4])==0)
    {
       OSMboxPost(Key_Mbox, key);
    }
    OSTimeDly(10);
  }
}

/*LCD显示任务*/
void Task_LCD(void *p_arg)
{
  U16 x0,x1,y0,y1;
  U8 state;
  const U8* pic;
  U32 number;
  U16 nblocation;
  U8 nbchar[10];
  U8 nbcount;
  
  for(;;)
  {
    /*背景*/
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    
    /*子弹*/
    MyPlane_ShowShot(&MyPlane);
    
    /*敌对飞机*/
    Enemy_Move(&Enemy);
    Enemy_Show(&Enemy);
    
    /*主飞机*/
   // MyPlane_Dead(&MyPlane);
    pic=MyPlane_GetPic(&MyPlane,&x0,&x1,&y0,&y1);
    LCD_ShowPic(pic,x0,y0,x1,y1);
    MyPlane_AddAction(&MyPlane);
    
    /*分数*/
    LCD_ShowPic(gImage_Score_Text,5,295,5+gImage_Score_Text_Witch,295+gImage_Score_Text_Length);
    
    nblocation=60;
    number=Enemy_GetScore(&Enemy);;
    nbcount=0;
    do{
     nbchar[nbcount++]=number%10;
     number=number/10;
    }while(number);
    
    do{
     pic=GetNumberPic(nbchar[--nbcount]);
     LCD_ShowPic(pic,nblocation,295,nblocation+gImage_Number_Witch,295+gImage_Number_Length);
     nblocation+=gImage_Number_Witch;
    }while(nbcount);
    
    LCD_Refresh();
    
    OSTimeDlyHMSM(0, 0,0,80);
  }
}

//shot任务
void Task_Shot(void *p_arg)
{
    for(;;)
    {
       MyPlane_AddShot(&MyPlane);
       OSTimeDlyHMSM(0, 0,0,300);
    }
}

//Enemy任务
void Task_Enemy(void *p_arg)
{
    for(;;)
    {
       Enemy_AddSmallFoe(&Enemy);
       OSTimeDlyHMSM(0, 0,0,1000);
    }
}

//*状态计算任务
void Task_Status(void *p_arg)
{
    U16 i,j,k;
    
    U16 x0,x1,y0,y1;
   
    
    for(;;)
    {  
      for(i=0;i<240;i++)
      {
        for(j=0;j<320;j++)
        {
          Clash[i][j]=0x00;
        }
      }
     
      //MyPlane
      MyPlane_GetLocation(&MyPlane,&x0,&y0,&x1,&y1);
      for(i=x0;i<x1;i++)
      {
        for(j=y0;j<y1;j++)
        {
          Clash[i][j] |= (0x01)<<7;
        }
      }
      
      //子弹
      for(k=0;k<32;k++)
      {
          MyPlane_GetShotLocation(&MyPlane,k,&x0,&y0,&x1,&y1);
          for(i=x0;i<x1;i++)
          {
            for(j=y0;j<y1;j++)
            {
               Clash[i][j] |= (0x01)<<6;
            }
          }
      }
        
      //敌对飞机
      //小飞机
      for(k=0;k<32;k++)
      {
        Enemy_GetSmallFoeLocation(&Enemy,k,&x0,&y0,&x1,&y1);
        for(i=x0;i<x1;i++)
        {
              for(j=y0;j<y1;j++)
              {
                Clash[i][j] |= 0x01;
              }
        } 
       }
                        
      //运算状态
      for(i=0;i<240;i++)
      {
        for(j=0;j<320;j++)
        {
         
          if((Clash[i][j]&(0x81))==0x81)
          {
            /*死亡*/
          }
           
          if((Clash[i][j]&(0x41))==0x41)//子弹打到飞机
          {
             //寻找相应子弹和飞机并改变状态    
            //子弹
             MyPlane_DestoryShot(&MyPlane,i,j);
            
            //飞机
             Enemy_DestorySmallFoe(&Enemy,i,j);
             
            //结束寻找
         }   
        }
      }
      OSTimeDlyHMSM(0, 0,0,5);
    }
}