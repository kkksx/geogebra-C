#ifndef _BT_H
#define _BT_H

/*
* 文件：BasicTransform.h
* 
* 轴对称、中心对称、旋转
* 
*/

#include "BasicGraphics.h"
#include "BasicAnalysis.h"
#include "boolean.h"

/*
* 接口：BT_drawAxisymmetry
* 功能：求一个图形关于直线的对称图形
*		绘出此对称图形并且返回其指针
*/
void* BT_drawAxisymmetry(void* graph, BG_Line* line);

/*
* 接口：BT_drawCentralSymmetry
* 功能：求一个图形关于点的中心对称图形
*		绘出对称图形并返回其指针
*/
void* BT_drawCentralSymmetry(void* graph, BG_Point* point);

/*
* 接口：BT_drawRotate
* 功能：求一个图形关于一个点逆时针旋转angle度后的图形
*		绘出旋转后图形并返回其指针
*/
void* BT_drawRotate(void* graph, BG_Point* point, double angle);


#endif