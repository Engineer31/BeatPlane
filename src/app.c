#include "includes.h"

/*--------���������ջ--------*/
static OS_STK StartGame_task_stk[STARTGAME_TASK_STK_SIZE];		  	
static OS_STK Key_task_stk[KEY_TASK_STK_SIZE];	
static OS_STK MyPlane_task_stk[MYPLANE_TASK_STK_SIZE];	
static OS_STK LCD_task_stk[LCD_TASK_STK_SIZE];
static OS_STK Shot_task_stk[SHOT_TASK_STK_SIZE];
static OS_STK Enemy_task_stk[ENEMY_TASK_STK_SIZE];
static OS_STK Status_task_stk[SHOT_TASK_STK_SIZE];

/************ �û��¼����� ******************************/
OS_EVENT *Key_Mbox;

/*��������*/
void Wait_Restart(void);

/************�������***********************************/
MyPlaneStruct MyPlane;
EnemyStruct  Enemy;
U32 GameScore;
U8 GameOver;
U8 Clash[240][320];

//��������
void Task_Start(void *p_arg)
{
          
    OSRunning=TRUE;	//begin OS
    uHALr_InstallSystemTimer();//��ʼ��ϵͳʱ��
    
    (void)p_arg;// 'p_arg' ��û���õ�����ֹ��������ʾ����
   while(1)
   {
    /*������ʼ��Ϸ����*/
    OSTaskCreate(Task_StartGame,(void *)0,
	 &StartGame_task_stk[STARTGAME_TASK_STK_SIZE-1],STARTGAME_TASK_PRIO);
    
    /*��ʼ������*/
    MyPlane_Init(&MyPlane,95,0);
    Enemy_Init(&Enemy);
    GameScore=0;
    GameOver=0;
    
    /*LCD����*/
    OSTaskCreate(Task_LCD,(void *)0,
	&LCD_task_stk[LCD_TASK_STK_SIZE-1],LCD_TASK_PRIO);	
    
    /*Myplane ����*/
    OSTaskCreate(Task_MyPlane,(void *)0,
	  &MyPlane_task_stk[MYPLANE_TASK_STK_SIZE-1],MYPLANE_TASK_PRIO);	
    
     Key_Mbox = OSMboxCreate((void *)0); 			//����������Ϣ ����
     
    /*shot����*/
    OSTaskCreate(Task_Shot,(void *)0,
	&Shot_task_stk[SHOT_TASK_STK_SIZE-1],SHOT_TASK_PRIO);	
    
    /*Enemy����*/
    OSTaskCreate(Task_Enemy,(void *)0,
	&Enemy_task_stk[ENEMY_TASK_STK_SIZE-1],ENEMY_TASK_PRIO);	
     
    /*������������*/
    OSTaskCreate(Task_Key,(void *)0,
	   &Key_task_stk[KEY_TASK_STK_SIZE-1],KEY_TASK_PRIO);
    
    /*����״̬��������*/
    OSTaskCreate(Task_Status,(void *)0,
	 &Status_task_stk[STATUS_TASK_STK_SIZE-1],STATUS_TASK_PRIO);
      
    while (1)
    {
        if(GameOver==1)//��Ϸ��������
        {
          OSTaskDel(KEY_TASK_PRIO);
          OSTaskDel(STATUS_TASK_PRIO);
          OSTaskDel(ENEMY_TASK_PRIO);
          OSTaskDel(SHOT_TASK_PRIO);

          OSTaskDel(MYPLANE_TASK_PRIO);
          OSTaskDel(LCD_TASK_PRIO);
          Wait_Restart();
          break;
        }
        OSTimeDlyHMSM(0, 0,0,100);
    }
    
   }
}

//��ʼ��Ϸ����
void Task_StartGame(void *p_arg)
{
    U8 key[5];
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    LCD_ShowPic(gImage_BeatPlane_Text,(240-gImage_BeatPlane_Text_Witch)/2,180
                                      ,gImage_BeatPlane_Text_Witch+(240-gImage_BeatPlane_Text_Witch)/2
                                      ,gImage_BeatPlane_Text_Length+180);
    LCD_Refresh();
    LCD_ShowPic(gImage_Plane_BK1,(240-gImage_Plane_BK1_Witch)/2,100,gImage_Plane_BK1_Witch+(240-gImage_Plane_BK1_Witch)/2,gImage_Plane_BK1_Length+100);
    hudelay(10000000);
    LCD_Refresh();
    LCD_ShowPic(gImage_Plane_BK2,(240-gImage_Plane_BK1_Witch)/2,100,gImage_Plane_BK1_Witch+(240-gImage_Plane_BK1_Witch)/2,gImage_Plane_BK1_Length+100);
    hudelay(10000000);
    LCD_Refresh();
    LCD_ShowPic(gImage_Plane_BK3,(240-gImage_Plane_BK1_Witch)/2,100,gImage_Plane_BK1_Witch+(240-gImage_Plane_BK1_Witch)/2,gImage_Plane_BK1_Length+100);
    hudelay(10000000);
    LCD_Refresh();
    hudelay(10000000);
    
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    LCD_ShowPic(gImage_BeatPlane_Text,(240-gImage_BeatPlane_Text_Witch)/2,180
                                      ,gImage_BeatPlane_Text_Witch+(240-gImage_BeatPlane_Text_Witch)/2
                                      ,gImage_BeatPlane_Text_Length+180);
     //Button 
    LCD_ShowPic(gImage_Button_ST,80,108,80+gImage_Button_BK_Witch,108+gImage_Button_BK_Length);
    LCD_Refresh();
    for(;;)
    {   
      key[0] = read_gpio_bit(GPIO_F3);
      key[1] = read_gpio_bit(GPIO_F5);
      key[2] = read_gpio_bit(GPIO_G1);
      key[3] = read_gpio_bit(GPIO_G3);
      key[4] = read_gpio_bit(GPIO_G4);
    
      if((key[0]&&key[1]&&key[2]&&key[3]&&key[4])==0)
      {
        break;
      }
  }

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
   U32 count;
   
   count=0;
   
   for(;;)
   {
     count++;
     
     msg = (U8 *)OSMboxPend(Key_Mbox, 1, &err);		//����������Ϣ{����}
     
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
  U8 state;
  const U8* pic;
  U32 number;
  U16 nblocation;
  U8 nbchar[10];
  U8 nbcount;
  
  for(;;)
  {
    /*����*/
    LCD_ShowPic(gImage_BackGround,0,0,gImage_BackGround_Witch+0,gImage_BackGround_Length+0);
    
    /*�ӵ�*/
    MyPlane_ShowShot(&MyPlane);
    
    /*�жԷɻ�*/
    Enemy_Show(&Enemy);
    
    /*���ɻ�*/
    pic=MyPlane_GetPic(&MyPlane,&x0,&x1,&y0,&y1);
    LCD_ShowPic(pic,x0,y0,x1,y1);
    
    /*���Ӷ���*/
    GameOver=MyPlane_AddAction(&MyPlane);//��Ӷ�������ȡ���ɻ�״̬������Ϊ1ʱ GameOver
    
    /*����*/
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

//shot����
void Task_Shot(void *p_arg)
{
    for(;;)
    {
       MyPlane_AddShot(&MyPlane);
       OSTimeDlyHMSM(0, 0,0,300);
    }
}

//Enemy����
void Task_Enemy(void *p_arg)
{
    U32 count;
    count=0;
    for(;;)
    {
      count++;
      Enemy_Move(&Enemy);
      OSTimeDlyHMSM(0, 0,0,80);
      if(count%100==0)
      {
        Enemy_AddBigFoe(&Enemy);
      }
      
      if(count%10==0)
      {
       Enemy_AddSmallFoe(&Enemy);
      }
      
      if(count%50==0)
      {
        Enemy_AddMiddleFoe(&Enemy); 
      }
      
    }
}

//*״̬��������
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
      
      //�ӵ�
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
        
      //�жԷɻ�
      //С�ɻ�
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
      
      //�зɻ�
      for(k=0;k<32;k++)
      {
        Enemy_GetMiddleFoeLocation(&Enemy,k,&x0,&y0,&x1,&y1);
        for(i=x0;i<x1;i++)
        {
              for(j=y0;j<y1;j++)
              {
                Clash[i][j] |= (0x01)<<1;
              }
        } 
       }
      
      //��ɻ�
      for(k=0;k<32;k++)
      {
        Enemy_GetBigFoeLocation(&Enemy,k,&x0,&y0,&x1,&y1);
        for(i=x0;i<x1;i++)
        {
              for(j=y0;j<y1;j++)
              {
                Clash[i][j] |= (0x01)<<2;
              }
        } 
       }
                        
      //����״̬
      for(i=0;i<240;i++)
      {
        for(j=0;j<320;j++)
        {
         
          if(((Clash[i][j]&(0x81))==0x81)||((Clash[i][j]&(0x82))==0x82)||((Clash[i][j]&(0x84))==0x84))
          {
            /*����*/
            MyPlane_Destory(&MyPlane);
          }
          
          //�ӵ���С�ɻ�
          if((Clash[i][j]&(0x41))==0x41)
          {
             //Ѱ����Ӧ�ӵ��ͷɻ����ı�״̬    
            //�ӵ�
             MyPlane_DestoryShot(&MyPlane,i,j);
            
            //�ɻ�
             Enemy_DestorySmallFoe(&Enemy,i,j);
             
             break;
            //����Ѱ��
          }
          
          //�ӵ����зɻ�
          if((Clash[i][j]&(0x42))==0x42)
          {
             //Ѱ����Ӧ�ӵ��ͷɻ����ı�״̬    
            //�ӵ�
             MyPlane_DestoryShot(&MyPlane,i,j);
            
            //�ɻ�
             Enemy_DestoryMiddleFoe(&Enemy,i,j);
             
             break;
            //����Ѱ��
         }
         
         //�ӵ��򵽴�ɻ�
          if((Clash[i][j]&(0x44))==0x44)
          {
             //Ѱ����Ӧ�ӵ��ͷɻ����ı�״̬    
            //�ӵ�
             MyPlane_DestoryShot(&MyPlane,i,j);
            
            //�ɻ�
             Enemy_DestoryBigFoe(&Enemy,i,j);
             
             break;
            //����Ѱ��
         }
         
         
        }
      }
      OSTimeDlyHMSM(0, 0,0,50);
    }
}

//�ȴ����¿�ʼ
void Wait_Restart(void)
{
    const U8* pic;
    U32 number;
    U16 nblocation;
    U8 nbchar[10];
    U8 nbcount;
    U8 key[5];
    
    LCD_ShowPic(gImage_EndPic,40,100,gImage_EndPic_Witch+40,gImage_EndPic_Length+100);
    
    //Number
    number=Enemy_GetScore(&Enemy);
    nbcount=0;
    do{
     nbchar[nbcount++]=number%10;
     number=number/10;
    }while(number);
    
    nblocation=(240-nbcount*16)/2;
    do{
     pic=GetNumberPic(nbchar[--nbcount]);
     LCD_ShowPic(pic,nblocation,148,nblocation+gImage_Number_Witch,148+gImage_Number_Length);
     nblocation+=gImage_Number_Witch;
    }while(nbcount);
    
    //Button 
    LCD_ShowPic(gImage_Button_BK,80,108,80+gImage_Button_BK_Witch,108+gImage_Button_BK_Length);
    
    LCD_Refresh();
    for(;;)
    {   
      key[0] = read_gpio_bit(GPIO_F3);
      key[1] = read_gpio_bit(GPIO_F5);
      key[2] = read_gpio_bit(GPIO_G1);
      key[3] = read_gpio_bit(GPIO_G3);
      key[4] = read_gpio_bit(GPIO_G4);
    
      if((key[0]&&key[1]&&key[2]&&key[3]&&key[4])==0)
      {
        break;
      }
  }
}