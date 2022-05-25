#ifndef _AG_H
#define _AG_H

/*
* 文件名：AdvanceGraphics.h
* 
* 图形种类：
*		4：多边形、5：扇形
*		6：椭圆、7：双曲线、8：抛物线
* 
* 
* 任意函数图像
*
* 
*/

#include "BasicGraphics.h"
#include "linkedlist.h"
#include "genlib.h"
#include "boolean.h"

/*
* 结构：扇形
*/
typedef struct
{
	string   name;   // 名字
	BG_Point point;  // 圆心
	double   r;      // 半径
	double   start;  // 起点角度
	double   end;    // 终点角度
	string   color;  // 区域颜色

	// updated on 20220525，为了给扇形添加描边
	BG_Arc*  arc;		// 对应的弧，属性同扇形
	BG_Line* line[2];   // 对应的两条直线
}AG_Sector;

/*
* 结构：多边形
*	结点按顺序列出
*/
typedef struct
{
	string		  name;		  // 名字
	linkedlistADT vertice;    // 结点链表
	linkedlistADT edge;       // 边链表，该链表随点列表刷新（终点->起点不在该表中）
	string		  color;      // 区域颜色
	bool          closed;     // 该多边形是否闭合（起点终点是否连接）
}AG_Polygon;


/*
* 结构：椭圆
*		由中心和长短轴端点表示
*/
typedef struct
{
	string   name;		  // 名字
	BG_Point point;       // 圆心
	BG_Point direction;   // 长轴方向向量
	double   a, b;		  // 半长轴 & 半短轴
}AG_Ellipse;

/*
* 结构：双曲线
*		由中心和实虚端点表示	
*/
typedef struct
{
	string   name;		  // 名字
	BG_Point point;		  // 中心
	BG_Point direction;   // 实轴方向向量
	double   a, b;		  // 半实轴 & 半虚轴
}AG_Hyperbola;

/*
* 结构：抛物线
*/
typedef struct
{
	string   name;		  // 名字
	BG_Point point;       // 顶点
	BG_Point direction;   // 对称轴的方向向量，必须指向凹的方向
	double   f;			  // 焦距OF
}AG_Parabola;




/*
* 接口：AG_addSector
* 功能：创建一个扇形，以color进行填充
*/
AG_Sector* AG_addSector(double x, double y, double r, double start, double end, string color);



/*
* 接口：AG_addPolygon
* 功能：读入一个节点数组，按数组新建一个多边形，n是节点个数
*		如果只用一个点初始化，也得用&point
*       新建的多边形默认不闭合
*/
AG_Polygon* AG_addPolygon(BG_Point** vertice, int n, string color);

/*
* 接口：AG_addPoint
* 功能：为多边形添加一个节点，并更新与之相关的边信息
*/
AG_Polygon* AG_addPoint(AG_Polygon* polygon, BG_Point* point);

/*
* 接口：AG_switchClose
* 功能：转换闭合情况，返回转换后的值
*/
bool AG_switchClose(AG_Polygon* polygon);




/* 
* 接口：AG_addEllipse
* 功能：添加椭圆
*		椭圆表示方法：圆心，长轴方向向量，半长轴，半短轴
*/
AG_Ellipse* AG_addEllipse(BG_Point point, BG_Point direction, double a, double b);

/*
* 接口：AG_addHyperbola
* 功能：添加双曲线
*		双曲线表示方法：圆心，实轴方向向量，半实轴，半虚轴
*/
AG_Hyperbola* AG_addHyperbola(BG_Point point, BG_Point direction, double a, double b);

/*
* 接口：AG_addParabola
* 功能：添加抛物线
*		抛物线表示方法；顶点，对称轴方向向量，焦距f
*		！注意：方向向量必须要指向抛物线凹的一方
*/
AG_Parabola* AG_addParabola(BG_Point point, BG_Point direction, double f);




//--------------非标准添加圆锥曲线的接口-------------------

/*
* 接口：AG_addEllipseBy3Point
* 功能：三点定椭圆，前两点是两焦点
*/
AG_Ellipse* AG_addEllipseBy3Point(BG_Point a, BG_Point b, BG_Point c);

/*
* 接口：AG_addHyperbolaBy3Point
* 功能：三点定双曲线，前两点是两焦点
*/
AG_Hyperbola* AG_addHyperbolaBy3Point(BG_Point a, BG_Point b, BG_Point c);

/*
* 接口：AG_addParabolaByPointLine
* 功能：焦点和准线定抛物线
*/
AG_Parabola* AG_addParabolaByPointLine(BG_Point a, BG_Line l);



/*
* 接口：AG_drawFunction
* 功能：用定比分点的方法画一个初等函数
	（尽量）保证该函数在定义域内连续
*/
void AG_drawFunction(string function);





#endif // !AG