#include "graphics.h"
#include "extgraph.h"
#include "boolean.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "BasicAnalysis.h"
#include "AdvanceGraphics.h"
#include "NameLib.h"
#include "SimpleGUI.h"

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
		
		//if (key == RIGHT_BUTTON)
		//{
		//	pointMode ^= 1;
		//	string name = BG_getGraphicName(dx, dy);
		//	if (name != "")
		//	{
		//		BG_deleteGraphic(name);
		//		printf("%lf %lf %s\n", dx, dy, name);
		//		Display();
		//	}
		//}

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
	//BG_Point* pt[3] = { BG_addPoint(0, 0), BG_addPoint(0,1), BG_addPoint(1,1) };
	//
	//AG_Polygon* poly1 = AG_addPolygon(pt, 3, "red");
	//AG_addPoint(poly1, BG_addPoint(2, -3));

	//MovePen(10, 5);
	//DrawEllipticalArc(1.0, 2.0, 90, 180);


	// 椭圆 & 双曲线
	BG_Point a, b;
	a.x = 3; a.y = 2;
	b.x = 1; b.y = 1;
	AG_addEllipse(a, b, 2, 1);
	AG_addHyperbola(a, b, 4, 5);
	AG_addParabola(a, b, 2);

}