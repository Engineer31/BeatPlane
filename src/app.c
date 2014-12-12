#include "includes.h"

/*--------���������ջ--------*/
static OS_STK Count_task_stk[COUNT_TASK_STK_SIZE];		  		//����Count�����ջ
static OS_STK Touch_task_stk[TOUCH_TASK_STK_SIZE];		  		//����Touch�����ջ

//��������
void Task_Start(void *p_arg)
{
          
    OSRunning=TRUE;	//begin OS
    uHALr_InstallSystemTimer();//��ʼ��ϵͳʱ��
    
    (void)p_arg;// 'p_arg' ��û���õ�����ֹ��������ʾ����
    
    Led_off();
    /*
    Touch_Q=OSQCreate(&MsgGrp[0],N_MESSAGES); 					//������Ϣ����

    OSTaskCreate(Task_Touch,(void *)0,
	   &Touch_task_stk[TOUCH_TASK_STK_SIZE-1],TOUCH_TASK_PRIO);	//������������

    OSTaskCreate(Task_Count,(void *)0,							//������������
	   &Count_task_stk[COUNT_TASK_STK_SIZE-1],COUNT_TASK_PRIO);
   */
    
    while (1)
    {
        Led1_on();
        OSTimeDlyHMSM(0, 0,0,500);
        Led_off();
        OSTimeDlyHMSM(0, 0,0,500);
    }
}