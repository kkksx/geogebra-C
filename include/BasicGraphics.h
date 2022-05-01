#ifndef _BG_H
#define _BG_H

/*
* 
* �ļ�����BasicGraphics.h
* ���ߣ�kkksx
* 
* ���ܣ����������ͼ���йصĽӿں����ݽṹ
* 	
*	0:�㣬1:ֱ�ߣ�2:������3:��
* 
* 
*/

#include "boolean.h"
#include "genlib.h"
#include "strlib.h"


/*
* �ṹ����
*/
typedef struct
{
	string name;  // ����
	double x, y;  // ����
}BG_Point;


/*
* �ṹ����
*/
typedef struct
{
	string	 name;		 // ����
	BG_Point point[2];   // �˵�
	int		 type;       // 0�߶Ρ�1���ߡ�2ֱ��
}BG_Line;


/*
* �ṹ������
*/
typedef struct
{
	string   name;       // ����
	BG_Point point[2];   // �˵�
}BG_Vector;

/*
* �ṹ��Բ��������Բ�Σ�
*/
typedef struct
{
	string   name;   // ����
	BG_Point point;  // Բ��
	double   r;      // �뾶
	double   start;  // ���Ƕ�
	double   end;    // �յ�Ƕ�
}BG_Arc;


/*
* �ӿڣ�BG_init
* ���ܣ�����һЩ��ʼ��
*		�ú���Ӧ�������ȵ���
*/
void BG_init();

/*
* �ӿڣ�BG_repaint
* ���ܣ������е�ͼ������ػ�
*/
void BG_repaint();


/*
* �ӿڣ�BG_addPoint
* ���ܣ���һ������Ϊ(x, y)�ĵ�
*		����x��y����ֵ����
*/
void BG_addPoint(double x, double y);

/*
* �ӿڣ�BG_addLine
* ���ܣ������������������ߣ��Դ˴���һ���ߣ���ָ������
*/
void BG_addLine(double x1, double y1, double x2, double y2, int type);

/*
* �ӿڣ�BG_addVector
* ���ܣ�����һ����a��b������
*/
void BG_addVector(double x1, double y1, double x2, double y2);

/*
* �ӿڣ�BG_addArc
* ���ܣ�����һ��Բ��Ϊ(x,y)���뾶Ϊr�����Ƕ�Ϊstart���յ�Ƕ�Ϊend��Բ��
*/
void BG_addArc(double x, double y, double r, double start, double end);

/*
* �ӿڣ�BG_deleteGraphic
* ���ܣ����ա����֡�ɾ��һ��ͼ�Σ�����type����������
*		ͼ�λ����´�ˢ��ʱ��ɾ��
*/
void BG_deleteGraphic(string name, int type);


/*
* �ӿڣ�BG_axisToInchX / BG_axisToInchY
* ���ܣ���ȡ����ϵ�ں�����x / ������y�ڴ����ڵ�λ��
*/
double BG_axisToInchX(double x);
double BG_axisToInchY(double y);

/*
* �ӿڣ�BG_inchToAxisX / BG_inchToAxisY
* ���ܣ���ȡ������λ��������ϵ�ڵĺ�����x / ������y
*/
double BG_inchToAxisX(double x);
double BG_inchToAxisY(double y);

/*
* �ӿڣ�BG_getGraphicType
* ���ܣ����һ��ͼ�ε�����
*		����-1��ʾû�ҵ�
*/
int BG_getGraphicType(string name);

/*
* �ӿڣ�BG_getGraphic
* ���ܣ���ȡ��긽����һ��ͼ�Σ�����������
*		����Ϊ������ڵ�Ӣ������		
*		���ؿ��ַ�����ʾû�ҵ�
*/
string BG_getGraphic(double x, double y);



#endif