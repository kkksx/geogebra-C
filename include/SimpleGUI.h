#ifndef _SIMPLE_GUI_H
#define _SIMPLE_GUI_H

/*
* 
* ���ܣ���Դ����
*	��graphics.cʵ��
*
*/

/*
* �ӿڣ�GUI_addButton
* ���ܣ��򴰿ڼ�һ����ť
* ���������
*	(x,y)  ���½�
*	w	   ��
*	h      ��
*	file   ͼƬλ��
*/
void GUI_addButton(double x, double y, double w, double h, char* file);


/*
* GUI_addBitmap
* ���ܣ����һ��bmpλͼ������ͬGUI_addButton
*/
void GUI_addBitmap(double x, double y, double w, double h, char* file);


/*
* ������GUI_update
* ���ܣ����´��ڣ���display����������
*/
inline void GUI_update();


#endif