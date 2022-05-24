#ifndef _BG_H
#define _BG_H

/*
* 
* 文件名：BasicGraphics.h
* 
* 功能：包含与具体图形有关的接口和数据结构
* 	
*	图形种类：0:点，1:直线，2:向量，3:弧
* 
* 
*/

#include "boolean.h"
#include "genlib.h"
#include "strlib.h"


/*
* 结构：点
*/
typedef struct
{
	string name;  // 命名
	double x, y;  // 坐标
}BG_Point;


/*
* 结构：线
*/
typedef struct
{
	string	 name;		 // 命名
	BG_Point point[2];   // 端点
	int		 type;       // 0直线、1射线、2线段
}BG_Line;


/*
* 结构：向量
*/
typedef struct
{
	string   name;       // 命名
	BG_Point point[2];   // 端点
}BG_Vector;

/*
* 结构：圆弧（包括圆形）
*/
typedef struct
{
	string   name;   // 名字
	BG_Point point;  // 圆心
	double   r;      // 半径
	double   start;  // 起点角度
	double   end;    // 终点角度
}BG_Arc;


/*
* 接口：BG_init
* 功能：进行一些初始化
*		该函数应当被首先调用
*/
void BG_init();

/*
* 接口：BG_repaint
* 功能：将所有的图像进行重画
*/
void BG_repaint();




/*
* 接口：BG_addPoint
* 功能：加一个坐标为(x, y)的点
*		其中x和y是数值坐标
*/
BG_Point* BG_addPoint(double x, double y);

/*
* 接口：BG_addLine
* 功能：将给定的两个点连线，以此创建一条线，并指定类型
*		0直线，1射线，2线段
*/
BG_Line* BG_addLine(double x1, double y1, double x2, double y2, int type);

/*
* 接口：BG_addVector
* 功能：构造一个从a到b的向量
*/
BG_Vector* BG_addVector(double x1, double y1, double x2, double y2);

/*
* 接口：BG_addArc
* 功能：构造一个圆心为(x,y)，半径为r，起点角度为start，终点角度为end的圆弧
*/
BG_Arc* BG_addArc(double x, double y, double r, double start, double end);




/*
* 接口：BG_axisToInchX / BG_axisToInchY
* 功能：获取坐标系内横坐标x / 纵坐标y在窗口内的位置
*/
double BG_axisToInchX(double x);
double BG_axisToInchY(double y);

/*
* 接口：BG_inchToAxisX / BG_inchToAxisY
* 功能：获取窗口内位置在坐标系内的横坐标x / 纵坐标y
*/
double BG_inchToAxisX(double x);
double BG_inchToAxisY(double y);




/*
* 接口：BG_deleteGraphic
* 功能：按照“名字”删除一个图形
*		图形会在下次刷新时被删除
*/
void BG_deleteGraphic(string name);

/*
* 接口：BG_getGraphicName
* 功能：获取鼠标附近的一个图形，返回其名字
*		输入为鼠标所在的英寸坐标
*		返回空字符串表示没找到
*/
string BG_getGraphicName(double x, double y);

/*
* 接口：BG_getGraphicType
* 功能：获得一个图形的种类
*		返回-1表示没找到
*/
int BG_getGraphicType(string name);

/*
* 接口：BG_getGraphic
* 功能：按照一个名字获取对应的图形，返回其指针
*		类型应该由BG_getGraphicType确定
*/
void* BG_getGraphic(string name);







#endif