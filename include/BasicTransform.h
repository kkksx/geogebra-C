#ifndef _BT_H
#define _BT_H

/*
* �ļ���BasicTransform.h
* 
* ��Գơ����ĶԳơ���ת
* 
*/

#include "BasicGraphics.h"
#include "BasicAnalysis.h"
#include "boolean.h"

/*
* �ӿڣ�BT_drawAxisymmetry
* ���ܣ���һ��ͼ�ι���ֱ�ߵĶԳ�ͼ��
*		����˶Գ�ͼ�β��ҷ�����ָ��
*/
void* BT_drawAxisymmetry(void* graph, BG_Line* line);

/*
* �ӿڣ�BT_drawCentralSymmetry
* ���ܣ���һ��ͼ�ι��ڵ�����ĶԳ�ͼ��
*		����Գ�ͼ�β�������ָ��
*/
void* BT_drawCentralSymmetry(void* graph, BG_Point* point);

/*
* �ӿڣ�BT_drawRotate
* ���ܣ���һ��ͼ�ι���һ������ʱ����תangle�Ⱥ��ͼ��
*		�����ת��ͼ�β�������ָ��
*/
void* BT_drawRotate(void* graph, BG_Point* point, double angle);


#endif