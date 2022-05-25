#ifndef _VL_H
#define _VL_H

/*
* 文件：VariousLine.h
* 
* 各种线的绘制接口
* 
* 
*/

#include "linkedlist.h"
#include "BasicGraphics.h"
#include "BasicAnalysis.h"
#include "boolean.h"


/*
* 接口：VL_perpendicular
* 功能：给出一个点和一条线，求出垂线
*/
BG_Line VL_perpendicular(BG_Line* line, BG_Point* point);

/*
* 接口：VL_parallel
* 功能：给出一个点和一条线，求出平行线
*/
BG_Line VL_parallel(BG_Line* line, BG_Point* point);

/*
* 接口：VL_midperpendicular
* 功能：给出两个点，求出中垂线
*/
BG_Line VL_midperpendicular(BG_Point* pt1, BG_Point* pt2);

/*
* 接口：VL_angleBisector
* 功能：给出三个点，求出角平分线
*		第一个点是顶点（中间那个点）
*		方向是pt1->ptc->pt2
*/
BG_Line VL_angleBisector(BG_Point* ptc, BG_Point* pt1, BG_Point* pt2);

/*
* 接口：VL_tangent
* 功能：给出一个点，求出其对圆的切线
*		如果不存在切线，则令线的type为-1
*		left表示是否为左侧切线
*/
BG_Line VL_tangent(BG_Arc* arc, BG_Point* point, bool left);

/*
* 接口：VL_bestMatch
* 功能：用最小二乘法求最佳拟合直线
*/
BG_Line VL_bestMatch(linkedlistADT* point);

 





#endif