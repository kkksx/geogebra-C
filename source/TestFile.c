#include "graphics.h"
#include "extgraph.h"
#include "boolean.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "BasicAnalysis.h"
#include "AdvanceGraphics.h"
#include "VariousArc.h"
#include "VariousLine.h"
#include "NameLib.h"
#include "SimpleGUI.h"

#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>

static bool lineMode = 0;   // 加线模式
static bool pointMode = 0;	// 加点模式
static bool leftMouseDown = 0;	// 鼠标左键按下

// extern void test();

void Display()
{
	DisplayClear();
	RA_createAxis();
	BG_repaint();
	//test();
	GUI_addBitmap(5, 5, 1, 1, "../resource/Cbitmap/arrow.bmp");  // 加位图
	GUI_update();
}

void MouseEventProcess(int x, int y, int key, int event)
{
	static double lastDx = -1;	// 记录上一次鼠标事件的位置
	static double lastDy = -1;  // 同上
	double dx = ScaleXInches(x);
	double dy = ScaleYInches(y);

	switch (event)
	{
	case BUTTON_DOWN:
		if (key == LEFT_BUTTON && pointMode)
			BG_addPoint(BG_inchToAxisX(dx), BG_inchToAxisY(dy));
		else if (key == LEFT_BUTTON) leftMouseDown = 1;
		
		if (key == RIGHT_BUTTON)
		{
			string name = BG_getGraphicName(dx, dy);
			if (name != "")
			{
				printf("%lf %lf %s  type:%d\n", dx, dy, name, BG_getGraphicType(name));
				BG_deleteGraphic(name);
				Display();
			}
		}

		break;

	case BUTTON_UP:
		leftMouseDown = 0;

		break;
		
	case ROLL_UP:
		RA_roll(1);
		Display();
		break;

	case ROLL_DOWN:
		RA_roll(0);
		Display();
		break;

	case MOUSEMOVE:
		if (leftMouseDown)
		{
			lastDx < 0 ? RA_move(0, 0) :
				RA_move(dx - lastDx, dy - lastDy);
			Display();
		}
		else
		{
		//	printf("%s\n", BG_getGraphic(dx, dy));	// 检验是否可以get到鼠标附近的图形
		}
		break;

	default:
		return;
	}
	lastDx = dx;
	lastDy = dy;
}

void Main()
{
	SetWindowTitle("test");
	SetWindowSize(20, 10);
	InitGraphics();
	BG_init();

	InitConsole();

	registerMouseEvent(MouseEventProcess);

	//-----------------------一些测试----------------------
	Display();

	GUI_addButton(2, 2, 1, 1, "../resource/Cbitmap/arrow.bmp");
	GUI_addButton(4, 2, 1, 1, "../resource/Cbitmap/point.bmp");
	GUI_addButton(6, 2, 1, 1, "../resource/Cbitmap/line.bmp");

/*
	BG_addPoint(BG_inchToAxisX(2), BG_inchToAxisY(3));	// 加个实际坐标为(2, 3)的点
	BG_addPoint(1, 1);  // 寄一个坐标为(1, 1)的点

	pointMode = 0;


	BG_addLine(2, 2, 3, 4, 0);	 // 直线
	BG_addLine(2, 2, -1, 5, 1);   // 射线
	BG_addLine(-1, 2, 0, 2, 2);  // 线段
	BG_addPoint(2, 2);

	BG_addArc(1, 1, 2, 30, 330);

	BG_addVector(0, 0, 1, -2);
	BG_addVector(0, 0, -3, -3);
	BG_addVector(0, 0, -3, 0);
	BG_addVector(0, 0, 0, -3);


	// 测试分析
	printf("%lf\n", BA_disPointLine(BG_getGraphic("A"), BG_getGraphic("b")));  // A点到b线的距离
*/

	// 多边形测试
	BG_Point* pt[3] = { BG_addPoint(0, 0), BG_addPoint(0,1), BG_addPoint(1,1) };
	
	AG_Polygon* poly1 = AG_addPolygon(pt, 3, "red");
	//AG_addPoint(poly1, BG_addPoint(2, -3));

	//MovePen(10, 5);
	//DrawEllipticalArc(1.0, 2.0, 90, 180);

	NL_occupyName("a", 0);
	NL_occupyName("b", 0);
	// 椭圆 & 双曲线
	BG_Point a, b;
	a.x = 2; a.y = 0;
	b.x = -2; b.y = 0;
	//AG_addEllipse(a, b, 2, 1);
	//AG_addHyperbola(a, b, 4, 5);
	//AG_addParabola(a, b, 2);

	AG_addEllipseBy3Point(a, b, (BG_Point){ "", 1, 1 });
	AG_addHyperbolaBy3Point(a, b, (BG_Point) { "", 1, 1 });
	AG_addParabolaByPointLine(a, (BG_Line) { "", (BG_Point) { "", -2, 0 }, (BG_Point) { "", -2, 1 } });

	BG_Point point = BA_getPedal(&a, BG_addLine(1, 1, 3, 4, 0));
	BG_addPoint(a.x, a.y);
	BG_addPoint(point.x, point.y);

	//AG_addSector(1, 2, 2, 90, 180, "red");
	
	//BG_Point p = BA_crossLine(BG_addLine(1, 1, 2, 2, 0), BG_addLine(-4, 3, -2, -1, 0));
	//BG_addPoint(p.x, p.y);

	//BG_Line line = BA_midperpendicular(BG_addPoint(1, 1), BG_addPoint(2, 3));
	//BG_addLine(line.point[0].x, line.point[0].y, line.point[1].x, line.point[1].y, 0);
	

	///* 圆 = 圆 + 圆上一点 */
	//VA_circleCenterPoint(-2, -2, -3, -3);

	///* 圆 = 圆心 + 半径 */
	//VA_circleCenterRadius(-2, -2, 0.5);

	///* 圆 = 圆上三个点 */
	//VA_circle3Point(-4, 0, -5, 0, -4.5, 2);

	///* 半圆 = 两端点 */
	//VA_halfPointPoint(-6, 0, -7, 2);

	///* 弧 = 圆心 + 两端点 （第二个端点是不是端点无所谓）*/
	//VA_arcCenterPoint(1, -1, 3, -3, 3, -1);

	///* 弧 = 弧上三个点，1、3点是端点*/
	//VA_arc3Point(5, -5, 4, -4, 0, -3);

	/* 扇面 = 圆心 + 两端点（第二个端点是不是端点无所谓）*/
	VA_sectorCenterPoint(1, 1, 2, 1, 3, 3, "red");

	/* 扇面 = 扇面的弧上三个点*/
	VA_sector3Point(4, 4, 5, 5, 6, 4, "blue");


	BG_Point* pp = BG_addPoint(-3, 4);
	BG_Line line = VL_tangent(BG_addArc(-3, -3, 4, 40, 400), pp, 0);
	BG_addLine(line.point[0].x, line.point[0].y, line.point[1].x, line.point[1].y, 0);





}