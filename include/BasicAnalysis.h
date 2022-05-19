#ifndef _BA_H
#define _BA_H

/*
*
* �ļ�����BasicAnalysis.h
*
* ���ܣ�����ͼ�ι�ϵ�ͷ�������
*
*/

#include "BasicGraphics.h"
#include "boolean.h"


/*
* �ӿڣ�BA_disPointLine
* ���ܣ��㵽�ߵľ���
		�߲�������һ�ɵ���ֱ�ߴ���
*/
double BA_disPointLine(BG_Point* point, BG_Line* line);

/*
* �ӿڣ�BA_disPointPoint
* ���ܣ�����֮��ľ���
*/
double BA_disPointPoint(BG_Point* point1, BG_Point* point2);

/*
* �ӿڣ�BA_midPoint
* ���ܣ����е�
*/
BG_Point BA_midPoint(BG_Point* point1, BG_Point* point2);


#endif // !_BA_H