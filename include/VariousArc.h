#ifndef _VA_H
#define _VA_H

/*
* �ļ���VariousArc.h
* ���ܣ�����Բ����Բ����������Ļ���
*/

#include "BasicGraphics.h"
#include "AdvanceGraphics.h"


/* Բ = Բ�� + Բ��һ���� */
BG_Arc* VA_circleCenterPoint(double x1, double y1, double x2, double y2);

/* Բ = Բ�� + �뾶 */
BG_Arc* VA_circleCenterRadius(double x, double y, double r);

/* Բ = Բ�������� */
BG_Arc* VA_circle3Point(double x1, double y1, double x2, double y2, double x3, double y3);

/* ��Բ = ���˵� */
BG_Arc* VA_halfPointPoint(double x1, double y1, double x2, double y2);

/* �� = Բ�� + ���˵� ���ڶ����˵��ǲ��Ƕ˵�����ν��*/
BG_Arc* VA_arcCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3);

/* �� = ���������㣬1��3���Ƕ˵�*/
BG_Arc* VA_arc3Point(double x1, double y1, double x2, double y2, double x3, double y3);

/* ���� = Բ�� + ���˵㣨�ڶ����˵��ǲ��Ƕ˵�����ν��*/
AG_Sector* VA_sectorCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3, string color);

/* ���� = ����Ļ���������*/
AG_Sector* VA_sector3Point(double x1, double y1, double x2, double y2, double x3, double y3, string color);


#endif