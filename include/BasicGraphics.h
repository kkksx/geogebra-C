#ifndef _BG_H
#define _BG_H

/*
* 
* 文件名：BasicGraphics.h
* 作者：kkksx
* 
* 功能：包含与具体图形有关的接口和数据结构
* 
*/

#include "boolean.h"

/*
* 定义了基础的数据结构
* 包括：点、线、弧、向量
*/


typedef struct
{
	double x, y;  // 坐标
}BG_Point;


typedef struct
{
	BG_Point point[2];   // 端点
	int type;            // 0线段、1射线、2直线

}BG_Line;


typedef struct
{
	BG_Point point[2];   // 端点
}BG_Vector;






#endif