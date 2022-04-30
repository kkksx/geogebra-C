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
#include "genlib.h"
#include "strlib.h"

/*
* 定义了基础的数据结构
* 包括：点、线、弧、向量
*/


/*
* 结构：点
*/
typedef struct
{
	double x, y;  // 坐标
	string name;  // 命名
}BG_Point;


/*
* 结构：线
*/
typedef struct
{
	BG_Point point[2];   // 端点
	string	 name;		 // 命名
	int		 type;       // 0线段、1射线、2直线
}BG_Line;


/*
* 结构：向量
*/
typedef struct
{
	BG_Point point[2];   // 端点
	string   name;       // 命名
}BG_Vector;



/*
* 接口：BG_init
* 功能：进行一些初始化
*		该函数应当被首先调用
*/
void BG_init();


/*
* 接口：BG_addPoint
* 功能：加一个坐标为(x, y)的点
*/
void BG_addPoint(double x, double y);

/*
* 接口：BG_addLine
* 功能：将给定的两个点连线，以此创建一条线，并指定类型
*/
void BG_addLine(double x1, double y1, double x2, double y2, int type);

/*
* 接口：BG_addVector
* 功能：构造一个从a到b的向量
*/
void BG_addVector(double x1, double y1, double x2, double y2);



/*
* 接口：BG_axisToInchX / BG_axisToInchY
* 功能：获取坐标系内横坐标x / 纵坐标y在窗口内的位置
*	    如果该坐标不在窗口内将返回负值
*/
double BG_axisToInchX(double x);
double BG_axisToInchY(double y);

/*
* 接口：BG_inchToAxisX / BG_inchToAxisY
* 功能：获取窗口内位置在坐标系内的横坐标x / 纵坐标y
*/
double BG_inchToAxisX(double x);
double BG_inchToAxisY(double y);



#endif