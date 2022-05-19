#ifndef _AG_H
#define _AG_H

/*
* �ļ�����AdvanceGraphics.h
* 
* ���ܣ����ƽϸ��ӵ�ͼ�Σ�����Ҫ����
*
* 
*/

#include "BasicGraphics.h"
#include "linkedlist.h"
#include "genlib.h"
#include "boolean.h"

/*
* �ṹ�������
*	��㰴˳���г�
*/
typedef struct
{
	linkedlistADT vertice;    // �������
	linkedlistADT edge;       // ����������������б�ˢ�£��յ�->��㲻�ڸñ��У�
	string		  color;      // ������ɫ
	bool          closed;     // �ö�����Ƿ�պϣ�����յ��Ƿ����ӣ�
}AG_Polygon;

/*
* �ṹ����Բ
*		�����ĺͳ�����˵��ʾ
*/
typedef struct
{
	BG_Point point;       // Բ��
	BG_Point direction;   // ���᷽������
	double a, b;		  // �볤�� & �����
}AG_Ellipse;

/*
* �ṹ��˫����
*		�����ĺ�ʵ��˵��ʾ	
*/
typedef struct
{
	BG_Point point;		  // ����
	BG_Point direction;   // ʵ�᷽������
	double a, b;		  // ��ʵ�� & ������
}AG_Hyperbola;

/*
* �ṹ��������
*/
typedef struct
{
	BG_Point point;       // ����
	BG_Point direction;   // �Գ���ķ�������������ָ�򰼵ķ���
	double f;			  // ����OF
}AG_Parabola;



/*
* �ӿڣ�AG_addPolygon
* ���ܣ�����һ���ڵ����飬�������½�һ������Σ�n�ǽڵ����
*		���ֻ��һ�����ʼ����Ҳ����&point
*       �½��Ķ����Ĭ�ϲ��պ�
*/
AG_Polygon* AG_addPolygon(BG_Point** vertice, int n, string color);

/*
* �ӿڣ�AG_addPoint
* ���ܣ�Ϊ��������һ���ڵ㣬��������֮��صı���Ϣ
*/
AG_Polygon* AG_addPoint(AG_Polygon* polygon, BG_Point* point);

/*
* �ӿڣ�AG_switchClose
* ���ܣ�ת���պ����������ת�����ֵ
*/
bool AG_switchClose(AG_Polygon* polygon);




/* 
* �ӿڣ�AG_addEllipse
* ���ܣ������Բ
*		��Բ��ʾ������Բ�ģ����᷽���������볤�ᣬ�����
*/
AG_Ellipse* AG_addEllipse(BG_Point point, BG_Point direction, double a, double b);

/*
* �ӿڣ�AG_addHyperbola
* ���ܣ����˫����
*		˫���߱�ʾ������Բ�ģ�ʵ�᷽����������ʵ�ᣬ������
*/
AG_Hyperbola* AG_addHyperbola(BG_Point point, BG_Point direction, double a, double b);

/*
* �ӿڣ�AG_addParabola
* ���ܣ����������
*		�����߱�ʾ���������㣬�Գ��᷽������������f
*		��ע�⣺������������Ҫָ�������߰���һ��
*/
AG_Parabola* AG_addParabola(BG_Point point, BG_Point direction, double f);






#endif // !AG