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

/*
* �����˻��������ݽṹ
* �������㡢�ߡ���������
*/


typedef struct
{
	double x, y;  // ����
}BG_Point;


typedef struct
{
	BG_Point point[2];   // �˵�
	int type;            // 0�߶Ρ�1���ߡ�2ֱ��

}BG_Line;


typedef struct
{
	BG_Point point[2];   // �˵�
}BG_Vector;






#endif