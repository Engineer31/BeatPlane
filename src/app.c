#include "includes.h"

/*--------���������ջ--------*/
static OS_STK StartGame_task_stk[STARTGAME_TASK_STK_SIZE];		  	
static OS_STK Key_task_stk[KEY_TASK_STK_SIZE];	
static OS_STK MyPlane_task_stk[MYPLANE_TASK_STK_SIZE];	
static OS_STK LCD_task_stk[LCD_TASK_STK_SIZE];

/************ �û��¼����� ******************************/
OS_EVENT *Key_Mbox;

/************�������***********************************/
MyPlaneStruct MyPlane;

//��������
void Task_Start(void *p_arg)
{
          
    OSRunning=TRUE;	//begin OS
    uHALr_InstallSystemTimer();//��ʼ��ϵͳʱ��
    
    (void)p_arg;// 'p_arg' ��û���õ�����ֹ��������ʾ����
    
    /*������ʼ��Ϸ����*/
    OSTaskCreate(Task_StartGame,(void *)0,
	   &StartGame_task_stk[STARTGAME_TASK_STK_SIZE-1],STARTGAME_TASK_PRIO);
    
    /*��ʼ������*/
    MyPlane_Init(&MyPlane,95,0);
    
    /*LCD����*/
    OSTaskCreate(Task_LCD,(void *)0,
	&LCD_task_stk[LCD_TASK_STK_SIZE-1],LCD_TASK_PRIO);	
    
    /*Myplane ����*/
    OSTaskCreate(Task_MyPlane,(void *)0,
	  &MyPlane_task_stk[MYPLANE_TASK_STK_SIZE-1],MYPLANE_TASK_PRIO);	
    
     Key_Mbox = OSMboxCreate((void *)0); 			//����������Ϣ ����
     
     
    /*������������*/
    OSTaskCreate(Task_Key,(void *)0,
	   &Key_task_stk[KEY_TASK_STK_SIZE-1],KEY_TASK_PRIO);
  
    /*
    OSTaskCreate(Task_Touch,(void *)0,
	   &Touch_task_stk[TOUCH_TASK_STK_SIZE-1],TOUCH_TASK_PRIO);	//������������

    OSTaskCreate(Task_Count,(void *)0,							//������������
	   &Count_task_stk[COUNT_TASK_STK_SIZE-1],COUNT_TASK_PRIO);
   */
      
    while (1)
    {
        OSTimeDlyHMSM(0, 0,0,500);
    }
}

//��ʼ��Ϸ����
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

//���ɻ�����
void Task_MyPlane(void *p_arg)
{

   //��ʼ���ɻ�
   U8*  msg;
   U8 err;
   static U16 Myplane_x0=95;
   static U16 Myplane_y0=0;


   for(;;)
   {
     msg = (U8 *)OSMboxPend(Key_Mbox, 0, &err);		//����������Ϣ{����}
     
     if(OS_NO_ERR == err)
     {
          if(0 == msg[0])//��
          {
             MyPlane_Move(&MyPlane,MOVE_LEFT,1);
          }
          if(0 == msg[1])//��
          {
            MyPlane_Move(&MyPlane,MOVE_DOWN,1);
          }
          if(0 == msg[2])//��
          {
            MyPlane_Move(&MyPlane,MOVE_UP,1);
          }

          if(0 == msg[3])//��
          {
            MyPlane_Move(&MyPlane,MOVE_RIGHT,1);
          }

          if(0 == msg[4])//��
          {
          }
    
     } 
     
     OSTimeDlyHMSM(0, 0,0,10);
   }
   
}


//��������
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

/*LCD��ʾ����*/
void Task_LCD(void *p_arg)
{
  U16 x0,x1,y0,y1;
  const U8* pic;
  
  for(;;)
  {
    /*����*/
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    
    /*�жԷɻ�*/
    /*��ɻ�*/
    /*�зɻ�*/
    /*С�ɻ�*/
    
    /*���ɻ�*/
   // MyPlane_Dead(&MyPlane);
    pic=MyPlane_GetPic(&MyPlane,&x0,&x1,&y0,&y1);
    LCD_ShowPic(pic,x0,y0,x1,y1);
    MyPlane_AddAction(&MyPlane);
    LCD_Refresh();
    
    OSTimeDlyHMSM(0, 0,0,80);
  }
}
