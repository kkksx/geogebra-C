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
    DefineColor("Black", 0, 0, 0);
    DefineColor("Dark Gray", .35, .35, .35);
    DefineColor("Gray", .6, .6, .6);
    DefineColor("Light Gray", .75, .75, .75);
    DefineColor("White", 1, 1, 1);
    DefineColor("Red", 1, 0, 0);
    DefineColor("Orange", 1, .40, .1);
    DefineColor("Yellow", 1, 1, 0);
    DefineColor("Green", 0, 1, 0);
    DefineColor("Blue", 0, 0, 1);
    DefineColor("Violet", .93, .5, .93);
    DefineColor("Magenta", 1, 0, 1);
    DefineColor("Cyan", 0, 1, 1);

	DefineColor("my Blue", .4, .65, 1);				用于给点染色
	DefineColor("nearly white", 0.95, 0.95, 1);		2类型坐标轴
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
#include "NameLib.h"

#define EPSILON 0.00000001


// 窗口属性
static double s_windowWidth;	// 绘图区域宽度
static double s_windowHeight;	// 绘图区域高度

// 坐标轴的属性
static double s_axisCalibration = 1;		// 坐标轴的刻度，原点到第一条刻度线的数值距离，该值只能是pow(2, n)
static double s_axisInches = 2;				// 原点到第一条刻度线的实际距离，应该保证该值在[1.3, 2.6)的范围
static double s_axisX = 0, s_axisY = 0;		// 窗口中心点的数值坐标

// 记录图像，在链表中==已经被绘出
static linkedlistADT s_listPoint;	// 点-链表
static linkedlistADT s_listLine;    // 线-链表
static linkedlistADT s_listVector;  // 向量-链表

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

// 封装一些方便的函数
static void s_drawCircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);

static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();




// 初始化
void BG_init()
{
	// 初始化窗口信息
	s_windowWidth = GetWindowWidth();		// 注意这里，画坐标系的窗口
	s_windowHeight = GetWindowHeight();		// 不一定是整个窗口范围，这里只是一个示范

	// 初始化链表
	s_listPoint = NewLinkedList();
	s_listLine = NewLinkedList();
	s_listVector = NewLinkedList();

}

//-----------------ReferenceAxis.h接口实现-----------------------

void RA_createAxis()
{
	double centerX = s_windowWidth / 2;
	double centerY = s_windowHeight / 2;
	double scale = s_axisCalibration / s_axisInches;  // 单位英寸的数值长度

	double nearX = s_axisCalibration * floor(s_axisX / s_axisCalibration);	// 离中心点最近的坐标
	double nearY = s_axisCalibration * floor(s_axisY / s_axisCalibration);	// 该坐标是数值坐标
	
	int j;

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
		if (fabs(s_axisInches - 2.6) <= EPSILON)
		{
			s_axisCalibration /= 2;
			s_axisInches = 1.3;
		}
	}
	else
	{
		if (s_axisCalibration > 100000000.0) return;
		s_axisInches -= 0.1;
		if (fabs(s_axisInches - 1.2) <= EPSILON)
		{
			s_axisCalibration *= 2;
			s_axisInches = 2.5;
		}
	}
}

void RA_move(double x, double y)
{
	s_axisX -= x / s_axisInches * s_axisCalibration;
	s_axisY -= y / s_axisInches * s_axisCalibration;
}


//-----------------BasicGraphics.h接口实现-----------------------

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
	
}

void BG_addVector(double x1, double y1, double x2, double y2)
{

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
	s_savePenStatus();

	SetPenSize(1);

	// 画圆
	StartFilledRegion(1);
	SetPenColor("my blue");
	s_drawCircle(point->x, point->y, 0.08);
	EndFilledRegion();
	
	SetPenColor("black");
	s_drawCircle(point->x, point->y, 0.09);
	
	// 编号
	s_drawName(point->x + 0.1, point->y + 0.1, point->name);

	s_loadPenStatus();
}

/*
* 函数：s_drawCircle
* 功能：以(x,y)为圆心画半径为r的圆
*/
static void s_drawCircle(double x, double y, double r)
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
	s_savePenStatus();

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
	SetPenSize(1);
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
	s_loadPenStatus();
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



















