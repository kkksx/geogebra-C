#ifndef _BA_H
#define _BA_H

/*
*
* 文件名：BasicAnalysis.h
*
* 功能：基础图形关系和分析计算
*
* 点和差、两点距离、两点中点、中垂线
* 点线距离、点对线的垂足
* 线线平行/垂直、线线交点
*/

#include "BasicGraphics.h"
#include "boolean.h"


//------------点点关系---------------

/*
* 接口：BA_minusPoint
* 功能：点做差
*/
BG_Point BA_minusPoint(BG_Point* a, BG_Point* b);

/*
* 接口：BA_addPoint
* 功能：点求和
*/
BG_Point BA_addPoint(BG_Point* a, BG_Point* b);

/*
* 接口：BA_disPointPoint
* 功能：两点之间的距离
*/
double BA_disPointPoint(BG_Point* point1, BG_Point* point2);

/*
* 接口：BA_midPoint
* 功能：求两点中点
*/
BG_Point BA_midPoint(BG_Point* point1, BG_Point* point2);

/*
* 接口：BA_midperpendicular
* 功能：求两个点所连线段的中垂线
*/
BG_Line BA_midperpendicular(BG_Point* p1, BG_Point* p2);




//------------点线关系---------------

/*
* 接口：BA_disPointLine
* 功能：点到线的距离
		线不论类型一律当做直线处理
*/
double BA_disPointLine(BG_Point* point, BG_Line* line);

/*
* 接口：BA_getPedal
* 功能：获取点到线的垂足
*/
BG_Point BA_getPedal(BG_Point* point, BG_Line* line);




//-------------线线关系--------------

/*
* 接口：BA_isParallel
* 功能：判断两条线是否平行
*/
bool BA_isParallel(BG_Line* line1, BG_Line* line2);

/*
* 接口：BA_isVertical
* 功能：判断两条线是否垂直
*/
bool BA_isVertical(BG_Line* line1, BG_Line* line2);

/*
* 接口：BA_crossPoint
* 功能：求两条线的交点
*/
BG_Point BA_crossPoint(BG_Line* line1, BG_Line* line2);






#endif // !_BA_H