#ifndef	_APP_H_
#define	_APP_H_

/************ �û��������� ******************************/
void Task_Start(void *p_arg);
void Task_StartGame(void *p_arg);
void Task_MyPlane(void *p_arg);
void Task_Key(void *Id);
void Task_LCD(void *p_arg);

/************ �û��¼����� ******************************/
extern OS_EVENT *Key_Mbox;

#endif	//_APP_H_