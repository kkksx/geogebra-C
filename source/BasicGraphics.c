/*
* 文件：BasicGraphics.c
* 
* 实现了
*	BasicGraphics.h
*	ReferenceAxis.h
* 中定义的接口
* 
* 此模块影响的范围仅限于绘图区域
* 
* 数值距离：以数字形式写出来的距离，即在坐标轴内的距离
* 实际距离：占窗口的英寸
* 
* 以下颜色可用
	DefineColor("my Blue", .4, .65, 1);				用于给点染色
	DefineColor("nearly white", 0.9, 0.9, 1);		2类型坐标轴
	DefineColor("somehow white", 0.8, 0.8, 0.9);	1类型坐标轴
* 
* 
* 
*/
#include <math.h>

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "strlib.h"
#include "linkedlist.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "BasicAnalysis.h"
#include "NameLib.h"

#define EPSILON   0.00000001
#define Pi        3.1415926535
#define LINESIZE  3

// 窗口属性
static double s_windowWidth;	// 绘图区域宽度
static double s_windowHeight;	// 绘图区域高度

// 坐标轴的属性
static double s_axisCalibration = 1;		// 坐标轴的刻度，原点到第一条刻度线的数值距离，该值只能是pow(2, n)
static double s_axisInches = 1;				// 原点到第一条刻度线的实际距离，应该保证该值在[1, 2)的范围
static double s_axisX = 0, s_axisY = 0;		// 窗口中心点的数值坐标

// 记录图像，在链表中==已经被绘出
static linkedlistADT s_listPoint;	// 点-链表
static linkedlistADT s_listLine;    // 线-链表
static linkedlistADT s_listVector;  // 向量-链表
static linkedlistADT s_listArc;  // 圆-链表

// 画笔状态
static int    s_penSize;			// 画笔大小
static int    s_pointSize;			// 文字大小
static string s_penColor;			// 画笔颜色



// -----------------------静态函数声明---------------------

static void s_savePenStatus();
static void s_loadPenStatus();
static bool s_inAxis(double x, double y);

static void s_drawPoint(BG_Point* point);
static void s_drawLine(BG_Line* line);
static void s_drawVector(BG_Vector* vect);
static void s_drawArc(BG_Arc* arc);

// 封装一些方便的函数
static void s_drawACircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);
static BG_Line* s_vectorToLine(BG_Vector* vect);
static double Radians(double degrees);

// RA会调用的静态函数
static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();






//-----------------ReferenceAxis.h接口实现-----------------------

void RA_createAxis()
{
	double centerX = s_windowWidth / 2;
	double centerY = s_windowHeight / 2;
	double scale = s_axisCalibration / s_axisInches;  // 单位英寸的数值长度

	double nearX = s_axisCalibration * floor(s_axisX / s_axisCalibration);	// 离中心点最近的坐标
	double nearY = s_axisCalibration * floor(s_axisY / s_axisCalibration);	// 该坐标是数值坐标
	
	int j;

	SetPenSize(1);

	// 浅色part

	double x = nearX;
	double inchesX = centerX + (nearX - s_axisX) / scale;
	for (; inchesX <= s_windowWidth; x += s_axisCalibration, inchesX += s_axisInches)
	{
		for (j = 1; j <= 4; ++j)
		{
			double xx = inchesX + s_axisInches * j / 5;
			if (xx <= s_windowWidth) s_RA_drawLine(xx, nearY, 1, 2);
		}
	}

	x = nearX;
	inchesX = centerX + (nearX - s_axisX) / scale;
	for (; inchesX >= 0; x -= s_axisCalibration, inchesX -= s_axisInches)
	{
		for (j = 1; j <= 4; ++j)
		{
			double xx = inchesX - s_axisInches * j / 5;
			if (xx >= 0) s_RA_drawLine(xx, nearY, 1, 2);
		}
	}

	double y = nearY;
	double inchesY = centerY + (nearY - s_axisY) / scale;
	for (; inchesY <= s_windowHeight; y += s_axisCalibration, inchesY += s_axisInches)
	{
		for (j = 1; j <= 4; ++j)
		{
			double yy = inchesY + s_axisInches * j / 5;
			if (yy <= s_windowHeight) s_RA_drawLine(nearX, yy, 0, 2);
		}
	}

	y = nearY;
	inchesY = centerY + (nearY - s_axisY) / scale;
	for (; inchesY >= 0; y -= s_axisCalibration, inchesY -= s_axisInches)
	{
		for (j = 1; j <= 4; ++j)
		{
			double yy = inchesY - s_axisInches * j / 5;
			if (yy >= 0) s_RA_drawLine(nearX, yy,0 , 2);
		}
	}

	// 深色part

	double X = s_RA_findX();  // 水平数字的位置
	X = X < GetFontHeight() ? GetFontHeight() :	X - GetFontHeight();
	double Y = s_RA_findY();  // 竖直数字的位置

	x = nearX;
	inchesX = centerX + (nearX - s_axisX) / scale;
	for (; inchesX <= s_windowWidth; x += s_axisCalibration, inchesX += s_axisInches)
	{
		if (fabs(x) <= EPSILON) s_RA_drawLine(inchesX, nearY, 1, 0);
		else
		{
			s_RA_drawLine(inchesX, nearY, 1, 1);
			char str[20]; sprintf(str, "%g", x);
			s_drawDouble(inchesX - TextStringWidth(str) / 2, X, x);
		}
	}

	x = nearX;
	inchesX = centerX + (nearX - s_axisX) / scale;
	for (; inchesX >= 0; x -= s_axisCalibration, inchesX -= s_axisInches)
	{
		if (fabs(x) <= EPSILON) s_RA_drawLine(inchesX, nearY, 1, 0);
		else
		{
			s_RA_drawLine(inchesX, nearY, 1, 1);
			char str[20]; sprintf(str, "%g", x);
			s_drawDouble(inchesX - TextStringWidth(str) / 2, X, x);
		}
	}

	y = nearY;
	inchesY = centerY + (nearY - s_axisY) / scale;
	for (; inchesY <= s_windowHeight; y += s_axisCalibration, inchesY += s_axisInches)
	{
		if (fabs(y) <= EPSILON) s_RA_drawLine(nearX, inchesY, 0, 0);
		else
		{
			s_RA_drawLine(nearX, inchesY, 0, 1);
			char str[20]; sprintf(str, "%g", y);
			double width = TextStringWidth(str);
			s_drawDouble(Y > width ? Y - width : 0, inchesY - GetFontHeight() / 3, y);
		}
	}

	y = nearY;
	inchesY = centerY + (nearY - s_axisY) / scale;
	for (; inchesY >= 0; y -= s_axisCalibration, inchesY -= s_axisInches)
	{
		if (fabs(y) <= EPSILON) s_RA_drawLine(nearX, inchesY, 0, 0);
		else
		{
			s_RA_drawLine(nearX, inchesY, 0, 1);
			char str[20]; sprintf(str, "%g", y);
			double width = TextStringWidth(str);
			s_drawDouble(Y > width ? Y - width : 0, inchesY - GetFontHeight() / 3, y);
		}
	}

}

void RA_roll(int up)
{
	if (up)
	{
		if (s_axisCalibration < 0.001) return;
		s_axisInches += 0.1;
		if (fabs(s_axisInches - 2) <= EPSILON)
		{
			s_axisCalibration /= 2;
			s_axisInches = 1;
		}
	}
	else
	{
		if (s_axisCalibration > 100000000.0) return;
		s_axisInches -= 0.1;
		if (fabs(s_axisInches - 0.9) <= EPSILON)
		{
			s_axisCalibration *= 2;
			s_axisInches = 1.9;
		}
	}
}

void RA_move(double x, double y)
{
	s_axisX -= x / s_axisInches * s_axisCalibration;
	s_axisY -= y / s_axisInches * s_axisCalibration;
}


//-----------------BasicGraphics.h接口实现-----------------------

// !!初始化!!
void BG_init()
{
	// 定义一些颜色
	DefineColor("my Blue", .4, .65, 1);
	DefineColor("nearly white", 0.9, 0.9, 1);
	DefineColor("somehow white", 0.8, 0.8, 0.9);

	// 初始化窗口信息
	s_windowWidth = GetWindowWidth();		// 注意这里，画坐标系的窗口
	s_windowHeight = GetWindowHeight();		// 不一定是整个窗口范围，这里只是一个示范

	// 初始化链表
	s_listPoint = NewLinkedList();
	s_listLine = NewLinkedList();
	s_listVector = NewLinkedList();
	s_listArc = NewLinkedList();

}

void BG_repaint()
{
	linkedlistADT now;
	// 线
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{
		s_drawLine(NodeObj(s_listLine, now));
	}

	// 弧
	for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	{
		s_drawArc(NodeObj(s_listArc, now));
	}

	// 向量
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	{
		s_drawVector(NodeObj(s_listVector, now));
	}

	// 点
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		s_drawPoint(NodeObj(s_listPoint, now));
	}
}

void BG_addPoint(double x, double y)
{
	// 加一个点进入链表
	BG_Point* point = New(BG_Point*);
	point->x = x; point->y = y;
	point->name = NL_getUpperCase();
	InsertNode(s_listPoint, NULL, point);

	// 画出这个点
	s_drawPoint(point);
}

void BG_addLine(double x1, double y1, double x2, double y2, int type)
{
	BG_Line* line = New(BG_Line*);
	line->point[0] = (BG_Point){ "", x1, y1 };
	line->point[1] = (BG_Point){ "", x2, y2 };
	line->type = type;
	line->name = NL_getLowerCase();
	InsertNode(s_listLine, NULL, line);

	s_drawLine(line);
}

void BG_addVector(double x1, double y1, double x2, double y2)
{
	BG_Vector* vect = New(BG_Vector*);
	vect->point[0] = (BG_Point){ "", x1, y1 };
	vect->point[1] = (BG_Point){ "", x2, y2 };
	vect->name = NL_getLowerCase();
	InsertNode(s_listVector, NULL, vect);

	s_drawVector(vect);
}

void BG_addArc(double x, double y, double r, double start, double end)
{
	BG_Arc* arc = New(BG_Arc*);
	arc->point = (BG_Point){ "", x, y };
	arc->r = r;
	arc->start = start;
	arc->end = end;
	arc->name = NL_getLowerCase();
	InsertNode(s_listArc, NULL, arc);

	s_drawArc(arc);
}

void BG_deleteGraphic(string name, int type)
{
	int nameType = -1;
	linkedlistADT head = NULL, now = NULL;
	switch (type)
	{
	case 0:
		head = s_listPoint;
		nameType = 1;
		break;
	case 1:
		head = s_listLine;
		nameType = 0;
		break;
	case 2:
		head = s_listVector;
		nameType = 0;
		break;
	case 3:
		head = s_listArc;
		nameType = 0;
		break;
	}
	
	for (now = NextNode(head, head); now; now = NextNode(head, now))
	{
		// 不管什么类型都转成BG_Point，因为它们name成员都是第一个
		BG_Point* data = NodeObj(head, now);
		if (StringEqual(data->name, name))
		{
			DeleteNode(head, data, StringEqual);
			NL_deleteName(name, nameType);
		}
	}
}



double BG_axisToInchX(double x)
{
	double scale = s_axisCalibration / s_axisInches;
	return s_windowWidth / 2 + (x - s_axisX) / scale;
}

double BG_axisToInchY(double y)
{
	double scale = s_axisCalibration / s_axisInches;
	return s_windowHeight / 2 + (y - s_axisY) / scale;
}

double BG_inchToAxisX(double x)
{
	double scale = s_axisCalibration / s_axisInches;
	return s_axisX + (x - s_windowWidth / 2) * scale;
}

double BG_inchToAxisY(double y)
{
	double scale = s_axisCalibration / s_axisInches;
	return s_axisY + (y - s_windowHeight / 2) * scale;
}

int BG_getGraphicType(string name)
{
	linkedlistADT now;

	// 点
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
		if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return 0;

	// 线
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
		if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return 1;

	// 向量
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
		if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	// 弧
	for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
		if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	return -1;
}

string BG_getGraphic(double x, double y)
{
	linkedlistADT now;

	// 点
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		BG_Point* point = NodeObj(s_listPoint, now);
		if (fabs(BG_axisToInchX(point->x) - x) < 0.1 &&
			fabs(BG_axisToInchY(point->y) - y) < 0.1)
			return point->name;
	}

	// 线
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{

	}

	//// 向量
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	//// 弧
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	return "";
}





//----------------------BasicAnalysis.h接口实现------------------


double BA_disPointLine(BG_Point* point, BG_Line* line)
{
	double x1 = line->point[0].x, x2 = line->point[1].x;
	double y1 = line->point[0].y, y2 = line->point[1].y;
	double a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;  // 一般式
	
	return (a * point->x + b * point->y + c) / sqrt(a * a + b * b);
}





//------------------------静态函数实现----------------------------

/*
* 函数：s_inAxis
* 功能：判断实际坐标为(x, y)的点是否在坐标系内
*		坐标系的具体范围应另外确定
*/
static bool s_inAxis(double x, double y)
{
	return (0 <= x && x <= s_windowWidth && 0 <= y && y <= s_windowHeight);
}

/*
* 函数：s_savePenStatus
* 功能：保存画笔的一些状态
* !!! 注意，该函数不应该在嵌套函数里面连续调用
*/
static void s_savePenStatus()
{
	s_penSize = GetPenSize();
	s_pointSize = GetPointSize();
	s_penColor = GetPenColor();
}

/*
* 函数：s_loadPenStatus
* 功能：加载前面保存的画笔状态
*/
static void s_loadPenStatus()
{
	SetPenSize(s_penSize);
	SetPointSize(s_pointSize);
	SetPenColor(s_penColor);
}

/*
* 函数：s_drawName
* 功能：在(x,y)点写名字
*/
static void s_drawName(double x, double y, string name)
{
	int pointSize = GetPointSize();

	SetPointSize(1);
	MovePen(x, y);
	DrawTextString(name);
	SetPointSize(pointSize);
}

/*
* 函数：s_drawPoint
* 功能：绘出一个点
*/
static void s_drawPoint(BG_Point* point)
{
	if (point == NULL) return;
	//s_savePenStatus();

	double x = BG_axisToInchX(point->x);
	double y = BG_axisToInchY(point->y);

	SetPenSize(1);

	// 画圆
	StartFilledRegion(1);
	SetPenColor("my blue");
	s_drawACircle(x, y, 0.08);
	EndFilledRegion();
	
	SetPenColor("black");
	s_drawACircle(x, y, 0.09);
	
	// 编号
	s_drawName(x + 0.1, y + 0.1, point->name);

	//s_loadPenStatus();
}

/*
* 函数：s_drawLine
* 功能：绘出一条线（仅画线，不画点）
*/
static void s_drawLine(BG_Line* line)
{
	if (line == NULL) return;
	//s_savePenStatus();

	SetPenSize(LINESIZE);
	SetPenColor("black");
	
	double x1 = BG_axisToInchX(line->point[0].x);
	double y1 = BG_axisToInchY(line->point[0].y);
	double x2 = BG_axisToInchX(line->point[1].x);
	double y2 = BG_axisToInchY(line->point[1].y);

	switch (line->type)
	{
	case 0:   // 直线 
		if (fabs(x2 - x1) <= EPSILON) s_drawALine(x1, 0, x1, s_windowHeight);
		else s_drawALine(0, -x1 * (y2 - y1) / (x2 - x1) + y1,
			 s_windowWidth, (s_windowWidth - x1) * (y2 - y1) / (x2 - x1) + y1);
		break;
	case 1:	  // 射线
		if (fabs(x2 - x1) <= EPSILON) s_drawALine(x1, y1, x1, s_windowHeight);
		else if(x1 < x2) s_drawALine(x1, y1, s_windowWidth, (s_windowWidth - x1) * (y2 - y1) / (x2 - x1) + y1);
		else s_drawALine(x1, y1, 0, -x1 * (y2 - y1) / (x2 - x1) + y1);
		break;
	default:  // 线段
		s_drawALine(x1, y1, x2, y2);
	}

	//s_loadPenStatus();
}

/*
* 函数：s_drawVector
* 功能：画一个向量（带箭头的线段）
*/
static void s_drawVector(BG_Vector* vect)
{
	if (vect == NULL) return;
	//s_savePenStatus();

	SetPenSize(LINESIZE);
	SetPenColor("black");

	double x1 = BG_axisToInchX(vect->point[0].x);
	double y1 = BG_axisToInchY(vect->point[0].y);
	double x2 = BG_axisToInchX(vect->point[1].x);
	double y2 = BG_axisToInchY(vect->point[1].y);

	s_drawALine(x1, y1, x2, y2);

	// 画小三角形
	double k, q;
	double x3 = 0, x4 = 0, x5 = 0, y3 = 0, y4 = 0, y5 = 0;

	if (fabs(x2 - x1) <= EPSILON)
	{
		x4 = x2 - 0.06;	
		x5 = x2 + 0.06;
		y4 = y5 = y2 + (y2 > y1 ? -0.2 : 0.2);
	}
	else if (fabs(y2 - y1) <= EPSILON)
	{
		y4 = y2 - 0.06;
		y5 = y2 + 0.06;
		x4 = x5 = x2 + (x2 > x1 ? -0.2 : 0.2);
	}
	else
	{
		k = (y2 - y1) / (x2 - x1);
		x3 = x2, y3 = y2, q = sqrt(1 + k * k);
		x3 += (x2 > x1 ? -1 : 1) * 0.2 / q;
		y3 += (x2 > x1 ? -1 : 1) * 0.2 * k / q;

		k = (x1 - x2) / (y2 - y1);
		q = sqrt(1 + k * k);
		x4 = x3 + 0.06 / q;
		x5 = x3 - 0.06 / q;
		y4 = y3 + 0.06 * k / q;
		y5 = y3 - 0.06 * k / q;
	}


	StartFilledRegion(1);

	s_drawALine(x2, y2, x4, y4);
	DrawLine(x5 - x4, y5 - y4);
	DrawLine(x2 - x5, y2 - y5);

	EndFilledRegion();
	//s_loadPenStatus();
}

/*
* 函数：s_drawArc
* 功能：画一个圆弧
*/
static void s_drawArc(BG_Arc* arc)
{
	if (arc == NULL) return;
	SetPenColor("black");
	SetPenSize(LINESIZE);

	double x = BG_axisToInchX((arc->point).x);
	double y = BG_axisToInchY((arc->point).y);
	double r = arc->r / s_axisCalibration * s_axisInches;

	MovePen(x + r * cos(Radians(arc->start)), y + r * sin(Radians(arc->start)));
	DrawArc(r, arc->start, arc->end - arc->start);
}




/*
* 函数：s_vectorToLine
* 功能：生成一条和向量重合的线段
*/
static BG_Line* s_vectorToLine(BG_Vector* vect)
{
	BG_Line* line = New(BG_Line*);
	line->point[0] = vect->point[0];
	line->point[1] = vect->point[1];
	line->name = vect->name;
	line->type = 2;
	return line;
}

/*
* 函数：s_drawACircle
* 功能：以(x,y)为圆心画半径为r的圆
*/
static void s_drawACircle(double x, double y, double r)
{
	MovePen(x + r, y);
	DrawArc(r, 0, 360);
}

/*
* 函数：s_drawDouble
* 功能：在指定位置输出一个数
*/
static void s_drawDouble(double x, double y, double number)
{
	SetPenColor("black");
	char str[20];
	sprintf(str, "%g", number);
	MovePen(x, y);
	DrawTextString(str);
}

/*
* 函数：s_drawALine
* 功能：画一条线段
*/
static void s_drawALine(double x1, double y1, double x2, double y2)
{
	MovePen(x1, y1);
	DrawLine(x2 - x1, y2 - y1);
}

/*
* 函数：s_RA_drawLine
* 功能：在实际坐标(x,y)画一条直线
*		type=0：横线， type=1：竖线
*		style=0：纯实线， style=1：半实线， style=2：灰线
*/
static void s_RA_drawLine(double x, double y, int type, int style)
{
	//s_savePenStatus();

	double x1, x2, y1, y2;
	
	if (type == 0)
	{
		y1 = y2 = y;
		x1 = 0;	x2 = s_windowWidth;
	}
	else
	{
		x1 = x2 = x;
		y1 = 0; y2 = s_windowHeight;
	}

	switch (style)
	{
	case 0:
		SetPenColor("black");
		break;
	case 1:
		SetPenColor("somehow white");
		break;
	default:
		SetPenColor("nearly white");
	}

	s_drawALine(x1, y1, x2, y2);

	// 画个小箭头
	if (type == 0 && style == 0)
	{
		s_drawALine(s_windowWidth, y1, s_windowWidth - 0.1, y1 - 0.1);
		s_drawALine(s_windowWidth, y1, s_windowWidth - 0.1, y1 + 0.1);
	}
	if (type == 1 && style == 0)
	{
		s_drawALine(x1, s_windowHeight, x1 - 0.1, s_windowHeight - 0.1);
		s_drawALine(x1, s_windowHeight, x1 + 0.1, s_windowHeight - 0.1);
	}
	//s_loadPenStatus();
}

/*
* 函数：Radians
* 功能：角度转弧度
*/
static double Radians(double degrees)
{
	return (degrees * Pi / 180);
}




/*
* 函数：s_RA_findX
* 功能：求出X轴的实际位置
*		如果低于0，返回0；如果高于窗口，则返回窗口高度
*/
static double s_RA_findX()
{
	double scale = s_axisCalibration / s_axisInches;
	double Y = s_windowHeight / 2 - s_axisY / scale;
	Y = Y > 0 ? Y : 0;
	Y = Y < s_windowHeight ? Y : s_windowHeight;
	return Y;
}

/*
* 函数：s_RA_findY
* 功能：求出Y轴的实际位置
*		如果低于0，返回0；如果高于窗口，则返回窗口宽度
*/
static double s_RA_findY()
{
	double scale = s_axisCalibration / s_axisInches;
	double X = s_windowWidth / 2 - s_axisX / scale;
	X = X > 0 ? X : 0;
	X = X < s_windowWidth ? X : s_windowWidth;
	return X;
}



















