#ifndef _BA_H
#define _BA_H

/*
*
* �ļ�����BasicAnalysis.h
*
* ���ܣ�����ͼ�ι�ϵ�ͷ�������
*
* ��Ͳ������롢�����е㡢�д���
* ���߾��롢����ߵĴ���
* ����ƽ��/��ֱ�����߽���
*/

#include "BasicGraphics.h"
#include "boolean.h"


//------------����ϵ---------------

/*
* �ӿڣ�BA_minusPoint
* ���ܣ�������
*/
BG_Point BA_minusPoint(BG_Point* a, BG_Point* b);

/*
* �ӿڣ�BA_addPoint
* ���ܣ������
*/
BG_Point BA_addPoint(BG_Point* a, BG_Point* b);

/*
* �ӿڣ�BA_disPointPoint
* ���ܣ�����֮��ľ���
*/
double BA_disPointPoint(BG_Point* point1, BG_Point* point2);

/*
* �ӿڣ�BA_midPoint
* ���ܣ��������е�
*/
BG_Point BA_midPoint(BG_Point* point1, BG_Point* point2);

/*
* �ӿڣ�BA_midperpendicular
* ���ܣ��������������߶ε��д���
*/
BG_Line BA_midperpendicular(BG_Point* p1, BG_Point* p2);




//------------���߹�ϵ---------------

/*
* �ӿڣ�BA_disPointLine
* ���ܣ��㵽�ߵľ���
		�߲�������һ�ɵ���ֱ�ߴ���
*/
double BA_disPointLine(BG_Point* point, BG_Line* line);

/*
* �ӿڣ�BA_getPedal
* ���ܣ���ȡ�㵽�ߵĴ���
*/
BG_Point BA_getPedal(BG_Point* point, BG_Line* line);




//-------------���߹�ϵ--------------

/*
* �ӿڣ�BA_isParallel
* ���ܣ��ж��������Ƿ�ƽ��
*/
bool BA_isParallel(BG_Line* line1, BG_Line* line2);

/*
* �ӿڣ�BA_isVertical
* ���ܣ��ж��������Ƿ�ֱ
*/
bool BA_isVertical(BG_Line* line1, BG_Line* line2);

/*
* �ӿڣ�BA_crossPoint
* ���ܣ��������ߵĽ���
*/
BG_Point BA_crossPoint(BG_Line* line1, BG_Line* line2);






#endif // !_BA_H