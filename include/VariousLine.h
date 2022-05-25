#ifndef _VL_H
#define _VL_H

/*
* �ļ���VariousLine.h
* 
* �����ߵĻ��ƽӿ�
* 
* 
*/

#include "linkedlist.h"
#include "BasicGraphics.h"
#include "BasicAnalysis.h"
#include "boolean.h"


/*
* �ӿڣ�VL_perpendicular
* ���ܣ�����һ�����һ���ߣ��������
*/
BG_Line VL_perpendicular(BG_Line* line, BG_Point* point);

/*
* �ӿڣ�VL_parallel
* ���ܣ�����һ�����һ���ߣ����ƽ����
*/
BG_Line VL_parallel(BG_Line* line, BG_Point* point);

/*
* �ӿڣ�VL_midperpendicular
* ���ܣ����������㣬����д���
*/
BG_Line VL_midperpendicular(BG_Point* pt1, BG_Point* pt2);

/*
* �ӿڣ�VL_angleBisector
* ���ܣ����������㣬�����ƽ����
*		��һ�����Ƕ��㣨�м��Ǹ��㣩
*		������pt1->ptc->pt2
*/
BG_Line VL_angleBisector(BG_Point* ptc, BG_Point* pt1, BG_Point* pt2);

/*
* �ӿڣ�VL_tangent
* ���ܣ�����һ���㣬������Բ������
*		������������ߣ������ߵ�typeΪ-1
*		left��ʾ�Ƿ�Ϊ�������
*/
BG_Line VL_tangent(BG_Arc* arc, BG_Point* point, bool left);

/*
* �ӿڣ�VL_bestMatch
* ���ܣ�����С���˷���������ֱ��
*/
BG_Line VL_bestMatch(linkedlistADT* point);

 





#endif