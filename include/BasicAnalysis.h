#ifndef _BA_H
#define _BA_H

/*
*
* 文件名：BasicAnalysis.h
* 作者：kkksx
*
* 功能：基础图形关系和分析计算
*
*/

#include "BasicGraphics.h"
#include "boolean.h"


/*
* 接口：BA_disPointLine
* 功能：点到线的距离
		线不论类型一律当做直线处理
*/
double BA_disPointLine(BG_Point* point, BG_Line* line);

/*
* 接口：BA_disPointPoint
* 功能：两点之间的距离
*/
double BA_disPointPoint(BG_Point* point1, BG_Point* point2);


#endif // !_BA_H