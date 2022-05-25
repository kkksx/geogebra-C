#ifndef _AG_H
#define _AG_H

/*
* �ļ�����AdvanceGraphics.h
* 
* ͼ�����ࣺ
*		4������Ρ�5������
*		6����Բ��7��˫���ߡ�8��������
* 
* 
* ���⺯��ͼ��
*
* 
*/

#include "BasicGraphics.h"
#include "linkedlist.h"
#include "genlib.h"
#include "boolean.h"

/*
* �ṹ������
*/
typedef struct
{
	string   name;   // ����
	BG_Point point;  // Բ��
	double   r;      // �뾶
	double   start;  // ���Ƕ�
	double   end;    // �յ�Ƕ�
	string   color;  // ������ɫ

	// updated on 20220525��Ϊ�˸�����������
	BG_Arc*  arc;		// ��Ӧ�Ļ�������ͬ����
	BG_Line* line[2];   // ��Ӧ������ֱ��
}AG_Sector;

/*
* �ṹ�������
*	��㰴˳���г�
*/
typedef struct
{
	string		  name;		  // ����
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
	string   name;		  // ����
	BG_Point point;       // Բ��
	BG_Point direction;   // ���᷽������
	double   a, b;		  // �볤�� & �����
}AG_Ellipse;

/*
* �ṹ��˫����
*		�����ĺ�ʵ��˵��ʾ	
*/
typedef struct
{
	string   name;		  // ����
	BG_Point point;		  // ����
	BG_Point direction;   // ʵ�᷽������
	double   a, b;		  // ��ʵ�� & ������
}AG_Hyperbola;

/*
* �ṹ��������
*/
typedef struct
{
	string   name;		  // ����
	BG_Point point;       // ����
	BG_Point direction;   // �Գ���ķ�������������ָ�򰼵ķ���
	double   f;			  // ����OF
}AG_Parabola;




/*
* �ӿڣ�AG_addSector
* ���ܣ�����һ�����Σ���color�������
*/
AG_Sector* AG_addSector(double x, double y, double r, double start, double end, string color);



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




//--------------�Ǳ�׼���Բ׶���ߵĽӿ�-------------------

/*
* �ӿڣ�AG_addEllipseBy3Point
* ���ܣ����㶨��Բ��ǰ������������
*/
AG_Ellipse* AG_addEllipseBy3Point(BG_Point a, BG_Point b, BG_Point c);

/*
* �ӿڣ�AG_addHyperbolaBy3Point
* ���ܣ����㶨˫���ߣ�ǰ������������
*/
AG_Hyperbola* AG_addHyperbolaBy3Point(BG_Point a, BG_Point b, BG_Point c);

/*
* �ӿڣ�AG_addParabolaByPointLine
* ���ܣ������׼�߶�������
*/
AG_Parabola* AG_addParabolaByPointLine(BG_Point a, BG_Line l);



/*
* �ӿڣ�AG_drawFunction
* ���ܣ��ö��ȷֵ�ķ�����һ�����Ⱥ���
	����������֤�ú����ڶ�����������
*/
void AG_drawFunction(string function);





#endif // !AG