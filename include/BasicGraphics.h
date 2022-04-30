#ifndef _BG_H
#define _BG_H

/*
* 
* �ļ�����BasicGraphics.h
* ���ߣ�kkksx
* 
* ���ܣ����������ͼ���йصĽӿں����ݽṹ
* 
*/

#include "boolean.h"
#include "genlib.h"
#include "strlib.h"

/*
* �����˻��������ݽṹ
* �������㡢�ߡ���������
*/


/*
* �ṹ����
*/
typedef struct
{
	double x, y;  // ����
	string name;  // ����
}BG_Point;


/*
* �ṹ����
*/
typedef struct
{
	BG_Point point[2];   // �˵�
	string	 name;		 // ����
	int		 type;       // 0�߶Ρ�1���ߡ�2ֱ��
}BG_Line;


/*
* �ṹ������
*/
typedef struct
{
	BG_Point point[2];   // �˵�
	string   name;       // ����
}BG_Vector;



/*
* �ӿڣ�BG_init
* ���ܣ�����һЩ��ʼ��
*		�ú���Ӧ�������ȵ���
*/
void BG_init();


/*
* �ӿڣ�BG_addPoint
* ���ܣ���һ������Ϊ(x, y)�ĵ�
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
* �ӿڣ�BG_axisToInchX / BG_axisToInchY
* ���ܣ���ȡ����ϵ�ں�����x / ������y�ڴ����ڵ�λ��
*	    ��������겻�ڴ����ڽ����ظ�ֵ
*/
double BG_axisToInchX(double x);
double BG_axisToInchY(double y);

/*
* �ӿڣ�BG_inchToAxisX / BG_inchToAxisY
* ���ܣ���ȡ������λ��������ϵ�ڵĺ�����x / ������y
*/
double BG_inchToAxisX(double x);
double BG_inchToAxisY(double y);



#endif