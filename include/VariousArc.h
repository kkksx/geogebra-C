#ifndef _VA_H
#define _VA_H

/*
* 文件：VariousArc.h
* 功能：各种圆、半圆、弧、扇面的绘制
*/

#include "BasicGraphics.h"
#include "AdvanceGraphics.h"


/* 圆 = 圆心 + 圆上一个点 */
BG_Arc* VA_circleCenterPoint(double x1, double y1, double x2, double y2);

/* 圆 = 圆心 + 半径 */
BG_Arc* VA_circleCenterRadius(double x, double y, double r);

/* 圆 = 圆上三个点 */
BG_Arc* VA_circle3Point(double x1, double y1, double x2, double y2, double x3, double y3);

/* 半圆 = 两端点 */
BG_Arc* VA_halfPointPoint(double x1, double y1, double x2, double y2);

/* 弧 = 圆心 + 两端点 （第二个端点是不是端点无所谓）*/
BG_Arc* VA_arcCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3);

/* 弧 = 弧上三个点，1、3点是端点*/
BG_Arc* VA_arc3Point(double x1, double y1, double x2, double y2, double x3, double y3);

/* 扇面 = 圆心 + 两端点（第二个端点是不是端点无所谓）*/
AG_Sector* VA_sectorCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3, string color);

/* 扇面 = 扇面的弧上三个点*/
AG_Sector* VA_sector3Point(double x1, double y1, double x2, double y2, double x3, double y3, string color);


#endif