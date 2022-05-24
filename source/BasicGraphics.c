/*
* 文件：BasicGraphics.c
* 
* 实现了
*	BasicGraphics.h
*	ReferenceAxis.h
*	BasicAnalysis.h
*	AdvanceGraphics.h
* 中定义的接口
* 
* 图形种类：0:点，1:直线，2:向量，3:弧
		   4：多边形，5：扇面
		   6：椭圆，7：双曲线，8：抛物线
* 
* 此模块影响绘图区域
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
#include "boolean.h"
#include "linkedlist.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "AdvanceGraphics.h"
#include "BasicAnalysis.h"
#include "NameLib.h"

#define EPSILON   0.00000001		// double精度
#define PI        3.1415926535897	// 圆周率π
#define INF		  1145141919.810	// 极大值
#define LINESIZE  2					// 线的粗细

#define Max(x, y) (((x) > (y)) ? (x) : (y))
#define Min(x, y) (((x) < (y)) ? (x) : (y))

// 窗口属性
static double s_windowWidth;	// 绘图区域宽度
static double s_windowHeight;	// 绘图区域高度

// 坐标轴的属性
static double s_axisCalibration = 1;		// 坐标轴的刻度，原点到第一条刻度线的数值距离，该值只能是pow(2, n)
static double s_axisInches = 1;				// 原点到第一条刻度线的实际距离，应该保证该值在[1, 2)的范围
static double s_axisX = 0, s_axisY = 0;		// 窗口中心点的数值坐标

// 记录图像，在链表中==已经被绘出
// 使用二维链表减少代码量( ? )
static linkedlistADT s_list;			 // 超级链表，节点内容为以下各个链表的头
static linkedlistADT s_listPoint;		 // 点-链表
static linkedlistADT s_listLine;		 // 线-链表
static linkedlistADT s_listVector;		 // 向量-链表
static linkedlistADT s_listArc;			 // 弧-链表
static linkedlistADT s_listPolygon;		 // 多边形-链表
static linkedlistADT s_listSector;		 // 扇面-链表
static linkedlistADT s_listEllipse;		 // 椭圆-链表
static linkedlistADT s_listHyperbola;    // 双曲线-链表
static linkedlistADT s_listParabola;     // 抛物线-链表



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
static void s_drawPolygon(AG_Polygon* polygon);
static void s_drawEllipse(AG_Ellipse* ellipse);
static void s_drawHyperbola(AG_Hyperbola* hyperbola);
static void s_drawParabola(AG_Parabola* parabola);
static void s_drawSector(AG_Sector* sector);

// 封装一些方便的函数
static void s_drawACircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);
static void s_drawAllLine(BG_Point* point, int n, bool closed);
static BG_Line* s_vectorToLine(BG_Vector* vect);

// RA会调用的静态函数
static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();

// 一些数学函数
static int s_dcmp(double x);
static double s_angle(double x1, double y1, double x2, double y2);
static double s_Radians(double degrees);
static double s_Degrees(double radians);
static double s_DotProduct(double x1, double y1, double x2, double y2);
static double s_CrossProduct(double x1, double y1, double x2, double y2);




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
	s_windowWidth = GetWindowWidth();			// 注意这里，画坐标系的窗口
	s_windowHeight = GetWindowHeight() - 1;		// 不一定是整个窗口范围，这里只是一个示范

	// 初始化链表
	s_list = NewLinkedList();
	s_listPoint = NewLinkedList();		 InsertNode(s_list, NULL, s_listPoint);
	s_listLine = NewLinkedList();		 InsertNode(s_list, NULL, s_listLine);
	s_listVector = NewLinkedList();		 InsertNode(s_list, NULL, s_listVector);
	s_listArc = NewLinkedList();		 InsertNode(s_list, NULL, s_listArc);
	s_listPolygon = NewLinkedList();	 InsertNode(s_list, NULL, s_listPolygon);
	s_listSector = NewLinkedList();		 InsertNode(s_list, NULL, s_listSector);
	s_listEllipse = NewLinkedList();	 InsertNode(s_list, NULL, s_listEllipse);
	s_listHyperbola = NewLinkedList();	 InsertNode(s_list, NULL, s_listHyperbola);
	s_listParabola = NewLinkedList();	 InsertNode(s_list, NULL, s_listParabola);
	
}

void BG_repaint()
{
	linkedlistADT now;

	// 多边形
	for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	{
		s_drawPolygon(NodeObj(s_listPolygon, now));
	}

	// 扇面
	for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	{
		s_drawSector(NodeObj(s_listSector, now));
	}

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

	// 椭圆
	for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	{
		s_drawEllipse(NodeObj(s_listEllipse, now));
	}

	// 双曲线
	for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	{
		s_drawHyperbola(NodeObj(s_listHyperbola, now));
	}

	// 抛物线
	for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	{
		s_drawParabola(NodeObj(s_listParabola, now));
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


BG_Point* BG_addPoint(double x, double y)
{
	// 加一个点进入链表
	BG_Point* point = New(BG_Point*);
	point->x = x; point->y = y;
	point->name = NL_getUpperCase();
	InsertNode(s_listPoint, NULL, point);

	// 画出这个点
	s_drawPoint(point);
	return point;
}

BG_Line* BG_addLine(double x1, double y1, double x2, double y2, int type)
{
	BG_Line* line = New(BG_Line*);
	line->point[0] = (BG_Point){ "", x1, y1 };
	line->point[1] = (BG_Point){ "", x2, y2 };
	line->type = type;
	line->name = NL_getLowerCase();
	InsertNode(s_listLine, NULL, line);

	s_drawLine(line);
	return line;
}

BG_Vector* BG_addVector(double x1, double y1, double x2, double y2)
{
	BG_Vector* vect = New(BG_Vector*);
	vect->point[0] = (BG_Point){ "", x1, y1 };
	vect->point[1] = (BG_Point){ "", x2, y2 };
	vect->name = NL_getLowerCase();
	InsertNode(s_listVector, NULL, vect);

	s_drawVector(vect);
	return vect;
}

BG_Arc* BG_addArc(double x, double y, double r, double start, double end)
{
	while (end < start) end += 360;	

	BG_Arc* arc = New(BG_Arc*);
	arc->point = (BG_Point){ "", x, y };
	arc->r = r;
	arc->start = start;
	arc->end = end;
	arc->name = NL_getLowerCase();
	InsertNode(s_listArc, NULL, arc);

	s_drawArc(arc);
	return arc;
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



void BG_deleteGraphic(string name)
{
	int nameType = -1, listCount = 0;
	int type = BG_getGraphicType(name);
	linkedlistADT listHead = NextNode(s_list, s_list);
	linkedlistADT head = NULL, now = NULL;

	for (; listHead; listHead = NextNode(NULL, listHead), ++listCount)
	{
		if (type == listCount)
		{
			head = listHead->dataptr;
			nameType = (listCount == 0);	// 只有点是大写字母
		}
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

string BG_getGraphicName(double x, double y)
{
	linkedlistADT now;

	BG_Point* pt = New(BG_Point*);
	pt->x = BG_inchToAxisX(x);
	pt->y = BG_inchToAxisY(y);

	double range = 0.05 * s_axisCalibration / s_axisInches;  // 鼠标点击判定圆形区域0.05英寸

	// 点
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		BG_Point* point = NodeObj(s_listPoint, now);
		if (BA_disPointPoint(pt, point) < range) return point->name;
	}

	// 线
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{
		BG_Line* line = NodeObj(s_listLine, now);
		BG_Point* point[2] = { &line->point[0], &line->point[1] };

		double dis = BA_disPointLine(pt, line);
		if (dis >= range) continue;

		if (line->type == 0) return line->name;
		else if (line->type == 1)	// 射线
		{
			if (s_DotProduct(point[1]->x - point[0]->x, point[1]->y - point[0]->y,
				pt->x - point[0]->x, pt->y - point[0]->y) > 0) return line->name;
			else if (BA_disPointPoint(pt, point[0]) < range) return line->name;
		}
		else if (line->type == 2)	// 线段需判断垂点是否落在线段上
		{
			if (BA_disPointPoint(pt, point[0]) < range) return line->name;
			if (BA_disPointPoint(pt, point[1]) < range) return line->name;
			if (s_DotProduct(point[1]->x - point[0]->x, point[1]->y - point[0]->y,
				pt->x - point[0]->x, pt->y - point[0]->y) > 0 && 
				s_DotProduct(point[0]->x - point[1]->x, point[0]->y - point[1]->y,
				pt->x - point[1]->x, pt->y - point[1]->y) > 0) 
					return line->name;
		}
	}

	// 向量
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	{
		BG_Vector* vect = NodeObj(s_listVector, now);
		BG_Point* point[2] = { &vect->point[0], &vect->point[1] };

		double dis = BA_disPointLine(pt, (BG_Line*)vect);	// 此处强转的前提是两者前几个元素相同
		if (dis >= range) continue;

		if (BA_disPointPoint(pt, point[0]) < range) return vect->name;
		if (BA_disPointPoint(pt, point[1]) < range) return vect->name;
		if (s_DotProduct(point[1]->x - point[0]->x, point[1]->y - point[0]->y,
			pt->x - point[0]->x, pt->y - point[0]->y) > 0 &&
			s_DotProduct(point[0]->x - point[1]->x, point[0]->y - point[1]->y,
				pt->x - point[1]->x, pt->y - point[1]->y) > 0)
			return vect->name;
	}

	// 弧
	for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	{
		BG_Arc* arc = NodeObj(s_listArc, now);
		double dx = pt->x - arc->point.x;
		double dy = pt->y - arc->point.y;

		if (fabs(BA_disPointPoint(pt, &arc->point) - arc->r) >= range) continue;

		double angle = s_angle(arc->point.x, arc->point.y, pt->x, pt->y);
		while (angle < arc->start) angle += 360;
		if (angle <= arc->end) return arc->name;
	}
	
	// 多边形
	// 判断标准：射线法 且 区域至少三个点
	for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	{
		linkedlistADT firstPoint = NextNode(NULL, ((AG_Polygon*)(now->dataptr))->vertice), point;
		int count = 0;	// 点数
		int flag = 0;   // 交点数
		for (point = firstPoint; point; point = NextNode(NULL, point))
		{
			++count;
			BG_Point* p1 = NodeObj(NULL, point);
			BG_Point* p2 = NULL;
			if (point->next == NULL) p2 = NodeObj(NULL, firstPoint);
			else p2 = NodeObj(NULL, point->next);

			// 先判断是否点击到线上
			if (fabs(s_CrossProduct(p2->x - p1->x, p2->y - p1->y, pt->x - p1->x, pt->y - p1->y)) < EPSILON) 
				return ((AG_Polygon*)now->dataptr)->name;

			// 处理(pt->x, pt->y)向右的一条射线与(p1->x, p1->y)--(p2->x, p2->y)的交点问题
			// 第一个判断保证了pt->y在两点y之间
			// 第二个判断保证了点在线左边
			// 第三个判断保证了交点不是y值较小的点
			if ((s_dcmp(p1->y - pt->y) > 0 != s_dcmp(p2->y - pt->y) > 0) 
				&& s_dcmp(pt->x - (pt->y - p1->y) * (p1->x - p2->x) / (p1->y - p2->y) - p1->x) < 0
				&& s_dcmp(pt->y - Min(p1->y, p2->y)) > 0)
					++flag;
		}
		if (count >= 3 && flag % 2) return ((AG_Polygon*)now->dataptr)->name;
	}

	// 扇面
	for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	{
		AG_Sector* sector = NodeObj(s_listSector, now);
		double dx = pt->x - sector->point.x;
		double dy = pt->y - sector->point.y;

		if (BA_disPointPoint(pt, &sector->point) >= sector->r) continue;
		
		double angle = s_angle(sector->point.x, sector->point.y, pt->x, pt->y);
		while (angle < sector->start) angle += 360;
		if (angle <= sector->end) return sector->name;
	}

	// 椭圆
	// 判断标准：到焦点距离和 = 2a
	for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	{
		AG_Ellipse* ellipse = NodeObj(s_listEllipse, now);
		BG_Point center = ellipse->point;
		BG_Point p1, p2;
		double dx = ellipse->direction.x, dy = ellipse->direction.y;
		double d = sqrt(pow(dx, 2) + pow(dy, 2));
		double a = ellipse->a, b = ellipse->b, c = sqrt(a * a - b * b);
		p1.x = center.x + c * dx / d;
		p1.y = center.y + c * dy / d;
		p2.x = center.x - c * dx / d;
		p2.y = center.y - c * dy / d;
		
		if (fabs(BA_disPointPoint(&p1, pt) +	BA_disPointPoint(&p2, pt) - 2 * a) < range) 
			return ellipse->name;
	}

	// 双曲线
	// 判断标准：到焦点距离差 = 2a
	for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	{
		AG_Hyperbola* hyperbola = NodeObj(s_listHyperbola, now);
		BG_Point center = hyperbola->point;
		BG_Point p1, p2;
		double dx = hyperbola->direction.x, dy = hyperbola->direction.y;
		double d = sqrt(pow(dx, 2) + pow(dy, 2));
		double a = hyperbola->a, b = hyperbola->b, c = sqrt(a * a + b * b);
		p1.x = center.x + c * dx / d;
		p1.y = center.y + c * dy / d;
		p2.x = center.x - c * dx / d;
		p2.y = center.y - c * dy / d;

		if (fabs(fabs(BA_disPointPoint(&p1, pt) - BA_disPointPoint(&p2, pt)) - 2 * a) < range)
			return hyperbola->name;
	}

	// 抛物线
	// 判断标准：到焦点距离 = 到准线距离
	for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	{
		AG_Parabola* parabola = NodeObj(s_listParabola, now);
		BG_Point center = parabola->point;
		BG_Point p;
		double dx = parabola->direction.x, dy = parabola->direction.y;
		double d = sqrt(pow(dx, 2) + pow(dy, 2));
		p.x = center.x + parabola->f * dx / d;
		p.y = center.y + parabola->f * dy / d;

		BG_Line line;
		line.point[0].x = center.x - parabola->f * dx / d;
		line.point[0].y = center.y - parabola->f * dy / d;
		line.point[1].x = line.point[0].x - dy;
		line.point[1].y = line.point[0].y + dx;

		if (fabs(BA_disPointLine(pt, &line) - BA_disPointPoint(pt, &p)) < range)
			return parabola->name;
	}

	FreeBlock(pt);
	
	return "";
}

int BG_getGraphicType(string name)
{
	int listCount = 0;
	linkedlistADT listHead = NextNode(s_list, s_list);
	linkedlistADT head = NULL, now = NULL;

	for (; listHead; listHead = NextNode(NULL, listHead), ++listCount)
	{
		for (now = NextNode(NULL, listHead->dataptr); now; now = NextNode(NULL, now))
		{
			if (StringEqual(((BG_Point*)NodeObj(NULL, now))->name, name))
				return listCount;
		}
	}

	//// 点
	//for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	//	if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return 0;

	//// 线
	//for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	//	if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return 1;

	//// 向量
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	//// 弧
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	//// 多边形
	//for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	//	if (StringEqual(((AG_Polygon*)NodeObj(s_listPolygon, now))->name, name)) return 4;

	//// 扇面
	//for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	//	if (StringEqual(((AG_Sector*)NodeObj(s_listSector, now))->name, name)) return 5;

	//// 椭圆
	//for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	//	if (StringEqual(((AG_Ellipse*)NodeObj(s_listEllipse, now))->name, name)) return 6;

	//// 双曲线
	//for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	//	if (StringEqual(((AG_Hyperbola*)NodeObj(s_listHyperbola, now))->name, name)) return 7;

	//// 抛物线
	//for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	//	if (StringEqual(((AG_Parabola*)NodeObj(s_listParabola, now))->name, name)) return 8;

	return -1;
}

void* BG_getGraphic(string name)
{
	int listCount = 0;
	linkedlistADT listHead = NextNode(s_list, s_list);
	linkedlistADT head = NULL, now = NULL;

	for (; listHead; listHead = NextNode(NULL, listHead), ++listCount)
	{
		for (now = NextNode(NULL, listHead->dataptr); now; now = NextNode(NULL, now))
		{
			if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) 
				return now->dataptr;
		}
	}

	//// 点
	//for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	//	if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return now->dataptr;

	//// 线
	//for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	//	if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return now->dataptr;

	//// 向量
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return now->dataptr;

	//// 弧
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return now->dataptr;

	//// 多边形
	//for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	//	if (StringEqual(((AG_Polygon*)NodeObj(s_listPolygon, now))->name, name)) return now->dataptr;

	//// 扇面
	//for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	//	if (StringEqual(((AG_Sector*)NodeObj(s_listSector, now))->name, name)) return now->dataptr;

	//// 椭圆
	//for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	//	if (StringEqual(((AG_Ellipse*)NodeObj(s_listEllipse, now))->name, name)) return now->dataptr;

	//// 双曲线
	//for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	//	if (StringEqual(((AG_Hyperbola*)NodeObj(s_listHyperbola, now))->name, name)) return now->dataptr;

	//// 抛物线
	//for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	//	if (StringEqual(((AG_Parabola*)NodeObj(s_listParabola, now))->name, name)) return now->dataptr;

	return NULL;
}




//----------------------AdvanceGraphics.h接口实现---------------------

// 扇形

AG_Sector* AG_addSector(double x, double y, double r, double start, double end, string color)
{
	while (end < start) end += 360;

	AG_Sector* sector = New(AG_Sector*);
	sector->point = (BG_Point){ "", x, y };
	sector->r = r;
	sector->start = start;
	sector->end = end;
	sector->color = color;
	sector->name = NL_getLowerCase();

	InsertNode(s_listSector, NULL, sector);
	s_drawSector(sector);

	return sector;
}



// 多边形

AG_Polygon* AG_addPolygon(BG_Point** vertice, int n, string color)
{
	AG_Polygon* polygon = New(AG_Polygon*);
	polygon->vertice = NewLinkedList();
	polygon->edge = NewLinkedList();
	polygon->color = color;
	polygon->closed = 0;
	polygon->name = NL_getLowerCase();

	int i;
	for (i = 0; i < n; ++i)
	{
		// 加一个点进入链表
		BG_Point* point = vertice[i];
		InsertNode(polygon->vertice, NULL, point);
	
		if (i + 1 == n) break;
		BG_Point* point2 = vertice[i + 1];
		InsertNode(polygon->edge, NULL,
			BG_addLine(point->x, point->y, point2->x, point2->y, 2));
	}

	// 插入该多边形并绘出
	InsertNode(s_listPolygon, NULL, polygon);
	s_drawPolygon(polygon);
	return polygon;
}

AG_Polygon* AG_addPoint(AG_Polygon* polygon, BG_Point* point)
{
	if (polygon->vertice->next == NULL)  // 一个点都没有
	{
		InsertNode(polygon->vertice, NULL, point);
		return polygon;
	}
	// 加一条边，一个点
	BG_Point* las = FindLastNode(polygon->vertice)->dataptr;
	InsertNode(polygon->edge, NULL,
		BG_addLine(las->x, las->y, point->x, point->y, 2));
	InsertNode(polygon->vertice, NULL, point);
	
	return polygon;
}

bool AG_switchClose(AG_Polygon* polygon)
{
	polygon->closed ^= 1;
	return polygon->closed;
}


// 基础圆锥曲线绘制

AG_Ellipse* AG_addEllipse(BG_Point point, BG_Point direction, double a, double b)
{
	AG_Ellipse* ellipse = New(AG_Ellipse*);
	ellipse->point = point;
	ellipse->direction = direction;
	ellipse->a = a;
	ellipse->b = b;
	ellipse->name = NL_getLowerCase();

	InsertNode(s_listEllipse, NULL, ellipse);
	s_drawEllipse(ellipse);

	return ellipse;
}

AG_Hyperbola* AG_addHyperbola(BG_Point point, BG_Point direction, double a, double b)
{
	AG_Hyperbola* hyperbola = New(AG_Hyperbola*);
	hyperbola->point = point;
	hyperbola->direction = direction;
	hyperbola->a = a;
	hyperbola->b = b;
	hyperbola->name = NL_getLowerCase();

	InsertNode(s_listHyperbola, NULL, hyperbola);
	s_drawHyperbola(hyperbola);

	return hyperbola;
}

AG_Parabola* AG_addParabola(BG_Point point, BG_Point direction, double f)
{
	AG_Parabola* parabola = New(AG_Parabola*);
	parabola->point = point;
	parabola->direction = direction;
	parabola->f = f;
	parabola->name = NL_getLowerCase();

	InsertNode(s_listParabola, NULL, parabola);
	s_drawParabola(parabola);

	return parabola;
}


// 圆锥曲线其他绘制接口

AG_Ellipse* AG_addEllipseBy3Point(BG_Point p1, BG_Point p2, BG_Point p3)
{
	double a = (BA_disPointPoint(&p1, &p3) + BA_disPointPoint(&p2, &p3)) / 2;
	double c = BA_disPointPoint(&p1, &p2) / 2;
	double b = sqrt(a * a - c * c);
	return AG_addEllipse(
		BA_midPoint(&p1, &p2),
		BA_minusPoint(&p2, &p1),
		a, b
	);
}

AG_Hyperbola* AG_addHyperbolaBy3Point(BG_Point p1, BG_Point p2, BG_Point p3)
{
	double a = fabs(BA_disPointPoint(&p1, &p3) - BA_disPointPoint(&p2, &p3)) / 2;
	double c = BA_disPointPoint(&p1, &p2) / 2;
	double b = sqrt(c * c - a * a);
	return AG_addHyperbola(
		BA_midPoint(&p1, &p2),
		BA_minusPoint(&p2, &p1),
		a, b
	);
}

AG_Parabola* AG_addParabolaByPointLine(BG_Point point, BG_Line line)
{
	BG_Point pedal = BA_getPedal(&point, &line);
	return AG_addParabola(
		BA_midPoint(&point, &pedal),
		BA_minusPoint(&point, &pedal),
		BA_disPointPoint(&point, &pedal) / 2
	);
}








//----------------------BasicAnalysis.h接口实现-----------------------


BG_Point BA_minusPoint(BG_Point* a, BG_Point* b)
{
	BG_Point ret;
	ret.x = a->x - b->x;
	ret.y = a->y - b->y;
	return ret;
}

BG_Point BA_addPoint(BG_Point* a, BG_Point* b)
{
	BG_Point ret;
	ret.x = a->x + b->x;
	ret.y = a->y + b->y;
	return ret;
}

double BA_disPointPoint(BG_Point* point1, BG_Point* point2)
{
	return sqrt(pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2));
}

BG_Point BA_midPoint(BG_Point* point1, BG_Point* point2)
{
	BG_Point mid;
	mid.x = (point1->x + point2->x) / 2;
	mid.y = (point1->y + point2->y) / 2;
	return mid;
}

BG_Line BA_midperpendicular(BG_Point* p1, BG_Point* p2)
{
	BG_Point a = BA_midPoint(p1, p2);
	double dx = p1->x - p2->x;
	double dy = p1->y - p2->y;
	BG_Point b = (BG_Point){ "", a.x - dy, a.y + dx };
	return (BG_Line) { "", a, b, 0 };
}



double BA_disPointLine(BG_Point* point, BG_Line* line)
{
	double x1 = line->point[0].x, x2 = line->point[1].x;
	double y1 = line->point[0].y, y2 = line->point[1].y;
	double a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;  // 一般式
	
	return fabs(a * point->x + b * point->y + c) / sqrt(a * a + b * b);
}

BG_Point BA_getPedal(BG_Point* point, BG_Line* line)
{
	double dis = BA_disPointLine(point, line);
	double x = line->point[1].x - line->point[0].x;
	double y = line->point[1].y - line->point[0].y;
	double d = sqrt(x * x + y * y);
	double dx = -y / d, dy = x / d;  // 方向向量
	return (BG_Point) { "", point->x + dx * dis, point->y + dy * dis };
}



bool BA_isParallel(BG_Line* line1, BG_Line* line2)
{
	double x1 = line1->point[1].x - line1->point[0].x;
	double y1 = line1->point[1].y - line1->point[0].y;
	double x2 = line2->point[1].x - line2->point[0].x;
	double y2 = line2->point[1].y - line2->point[0].y;

	return fabs(x1 * y2 - x2 * y1) < EPSILON;
}

bool BA_isVertical(BG_Line* line1, BG_Line* line2)
{
	double x1 = line1->point[1].x - line1->point[0].x;
	double y1 = line1->point[1].y - line1->point[0].y;
	double x2 = line2->point[1].x - line2->point[0].x;
	double y2 = line2->point[1].y - line2->point[0].y;

	return fabs(x1 * x2 + y1 * y2) < EPSILON;
}

BG_Point BA_crossPoint(BG_Line* line1, BG_Line* line2)
{
	if (BA_isParallel(line1, line2))  // 实际上应该在调用该函数前先判断平行
		return (BG_Point) { "", INF, INF };

	double a1 = line1->point[1].y - line1->point[0].y;
	double b1 = line1->point[0].x - line1->point[1].x;
	double c1 = line1->point[0].x * line1->point[1].y - line1->point[1].x * line1->point[0].y;

	double a2 = line2->point[1].y - line2->point[0].y;
	double b2 = line2->point[0].x - line2->point[1].x;
	double c2 = line2->point[0].x * line2->point[1].y - line2->point[1].x * line2->point[0].y;

	return (BG_Point) {	"",
		(c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1),
		(a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1)
	};
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

	s_drawName((x1 + x2) / 2 + 0.1, (y1 + y2) / 2 + 0.1, line->name);
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

	s_drawName((x1 + x2) / 2 + 0.1, (y1 + y2) / 2 + 0.1, vect->name);
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

	MovePen(x + r * cos(s_Radians(arc->start)), y + r * sin(s_Radians(arc->start)));
	DrawArc(r, arc->start, arc->end - arc->start);

	s_drawName(x + cos(s_Radians((arc->start + arc->end) / 2)) * (r - 0.1),
			   y + sin(s_Radians((arc->start + arc->end) / 2)) * (r - 0.1), arc->name);
}

/*
* 函数：s_drawPolygon
* 功能：画一个多边形，如果要改透明度，只需要改这里就行
*/
static void s_drawPolygon(AG_Polygon* polygon)
{
	if (polygon == NULL || polygon->vertice == NULL) return;

	linkedlistADT lasPoint = NextNode(polygon->vertice, polygon->vertice);
	linkedlistADT nowPoint = NextNode(polygon->vertice, lasPoint);
	BG_Point *a, *b;

	if (nowPoint == NULL) return;

	SetPenSize(LINESIZE);
	SetPenColor(polygon->color);

	a = NodeObj(NULL, lasPoint);  // 第一个点
	MovePen(BG_axisToInchX(a->x), BG_axisToInchY(a->y));
	StartFilledRegion(1);

	while (nowPoint != NULL)
	{
		a = NodeObj(NULL, lasPoint);
		b = NodeObj(NULL, nowPoint);

		DrawLine(BG_axisToInchX(b->x) - BG_axisToInchX(a->x), 
				 BG_axisToInchY(b->y) - BG_axisToInchY(a->y));

		lasPoint = nowPoint;
		nowPoint = NextNode(polygon->vertice, nowPoint);
	}

	// 最后一条闭合线
	if (polygon->closed)
	{
		a = NodeObj(NULL, lasPoint);
		b = NodeObj(NULL, NextNode(polygon->vertice, polygon->vertice));
		DrawLine(BG_axisToInchX(b->x) - BG_axisToInchX(a->x),
				 BG_axisToInchY(b->y) - BG_axisToInchY(a->y));
	}

	EndFilledRegion();


	// 重新画点，保证点在区域上面
	nowPoint = NextNode(polygon->vertice, polygon->vertice);
	for (; nowPoint; nowPoint = NextNode(polygon->vertice, nowPoint))
	{
		s_drawPoint((BG_Point*)NodeObj(polygon->vertice, nowPoint));
	}

}

/*
* 函数：s_drawEllipse
* 功能：用参数方程分线段画椭圆
*/
static void s_drawEllipse(AG_Ellipse* ellipse)
{
	BG_Point pt[360];
	double cx = ellipse->point.x, cy = ellipse->point.y;
	double dx = ellipse->direction.x, dy = ellipse->direction.y;

	double a = ellipse->a;
	double b = ellipse->b;  // 长短轴

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // 旋转角

	int i;
	for (i = 0; i < 360; ++i)
	{
		double x = a * cos(i * PI / 180);
		double y = b * sin(i * PI / 180);  // 标准椭圆参数方程

		double xx = x * cosB - y * sinB;
		double yy = y * cosB + x * sinB;  // 旋转公式

		pt[i].x = xx + cx;
		pt[i].y = yy + cy;
	}

	s_drawAllLine(pt, 360, 1);
}

/*
* 函数：s_drawHyperbola
* 功能：画双曲线，做法同椭圆
*/
static void s_drawHyperbola(AG_Hyperbola* hyperbola)
{
	BG_Point pt[360];
	double cx = hyperbola->point.x, cy = hyperbola->point.y;
	double dx = hyperbola->direction.x, dy = hyperbola->direction.y;

	double a = hyperbola->a;
	double b = hyperbola->b;  // 长短轴

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // 旋转角

	int i;
	for (i = 0; i < 360; ++i)
	{
		if (i == 0 || i == 180) continue;
		double x = a / cos((i + 270) * PI / 180);
		double y = b * tan((i + 270) * PI / 180);  // 标准椭圆参数方程

		double xx = x * cosB - y * sinB;
		double yy = y * cosB + x * sinB;  // 旋转公式

		pt[i].x = xx + cx;
		pt[i].y = yy + cy;
	}

	s_drawAllLine(pt + 1, 179, 0);
	s_drawAllLine(pt + 181, 179, 0);

	// 如果缩到很小，四个方向无法无限延长，用射线代替
	BG_Line line;
	line.name = "";
	line.type = 1;
	line.point[0] = pt[2]; line.point[1] = pt[1]; s_drawLine(&line);
	line.point[0] = pt[178]; line.point[1] = pt[179]; s_drawLine(&line);
	line.point[0] = pt[182]; line.point[1] = pt[181]; s_drawLine(&line);
	line.point[0] = pt[358]; line.point[1] = pt[359]; s_drawLine(&line);
}

/*
* 函数：s_drawParabola
* 功能：画抛物线
*/
static void s_drawParabola(AG_Parabola* parabola)
{
	double f = parabola->f;
	double cx = parabola->point.x, cy = parabola->point.y;
	double dx = parabola->direction.x, dy = parabola->direction.y;

	double leftX = BG_inchToAxisX(0), rightX = BG_inchToAxisX(s_windowWidth);
	double downY = BG_inchToAxisY(0), upY = BG_inchToAxisY(s_windowHeight);	 // 窗口边界的数值坐标

	double minX = INF, maxX = -INF;  // “可能”在窗口中出现的双曲线x范围
	double X[4] = { leftX, rightX, leftX, rightX };
	double Y[4] = { downY, downY, upY, upY };	// 四个角
	BG_Line line;

	int i;
	for (i = 0; i < 4; ++i)
	{
		line.point[0] = (BG_Point){ "", X[i], Y[i] };
		line.point[1] = (BG_Point){ "", X[i] - dy, Y[i] + dx };
		double dis = BA_disPointLine(&parabola->point, &line);
		if (s_DotProduct(dx, dy, X[i] - cx, Y[i] - cy) < 0) dis = 0;  // 钝角说明顶点在这条线后面
		minX = Min(minX, dis);  maxX = Max(maxX, dis);
	}

	// 只考虑x in [minX, maxX]，并且拆成若干份
	// 开始打点连线，类似另外两个圆锥曲线

	int sign = 1, j;
	BG_Point pt[201];

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // 旋转角
	double part = (maxX - minX) / 200, nowX;

	for (j = 0; j < 2; ++j, sign *= -1)	 // 同时考虑两种y
	{
		for (i = 0, nowX = minX; i <= 200; ++i, nowX += part)
		{
			double x = nowX;
			double y = 2 * sqrt(f * x) * sign;

			double xx = x * cosB - y * sinB;
			double yy = y * cosB + x * sinB;  // 旋转公式
			pt[i].x = xx + cx;
			pt[i].y = yy + cy;
		}
		s_drawAllLine(pt, 201, 0);
	}
}

/*
* 函数：s_drawSector
* 功能：画扇形
*/
static void s_drawSector(AG_Sector* sector)
{
	if (sector == NULL) return;

	SetPenSize(LINESIZE);
	SetPenColor(sector->color);

	double x = BG_axisToInchX(sector->point.x);
	double y = BG_axisToInchY(sector->point.y);
	double r = sector->r / s_axisCalibration * s_axisInches;

	MovePen(x, y);
	StartFilledRegion(1);
	DrawLine(r * cos(s_Radians(sector->start)), r * sin(s_Radians(sector->start)));
	DrawArc(r, sector->start, sector->end - sector->start);
	DrawLine(-r * cos(s_Radians(sector->end)), -r * sin(s_Radians(sector->end)));
	EndFilledRegion();

	// 描边
	SetPenColor("black");
	DrawLine(r * cos(s_Radians(sector->start)), r * sin(s_Radians(sector->start)));
	DrawArc(r, sector->start, sector->end - sector->start);
	DrawLine(-r * cos(s_Radians(sector->end)), -r * sin(s_Radians(sector->end)));
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
* 函数：s_drawAllLine
* 功能：使用一个点数组画图形
		类似多边形，不过可以用于拟合曲线
		closed决定闭合与否
*/
static void s_drawAllLine(BG_Point* point, int n, bool closed)
{
	if (!n) return;

	SetPenSize(LINESIZE);
	MovePen(BG_axisToInchX(point[0].x), BG_axisToInchY(point[0].y));
	for (int i = 1; i < n; ++i)
		DrawLine(BG_axisToInchX(point[i].x) - BG_axisToInchX(point[i - 1].x),
		  	BG_axisToInchY(point[i].y) - BG_axisToInchY(point[i - 1].y));
	if(closed) 
		DrawLine(BG_axisToInchX(point[0].x) - BG_axisToInchX(point[n - 1].x),
			BG_axisToInchY(point[0].y) - BG_axisToInchY(point[n - 1].y));
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


/*
* 函数：s_dcmp
* 功能：三态函数，除去误差只保留x的正负0属性
*/
static int s_dcmp(double x) 
{
	if (fabs(x) < EPSILON) return 0;
	return x < 0 ? -1 : 1;
}

/*
* 函数：s_Radians
* 功能：角度转弧度
*/
static double s_Radians(double degrees)
{
	return (degrees * PI / 180);
}

/*
* 函数：s_Degrees
* 功能：弧度转角度
*/
static double s_Degrees(double radians)
{
	return (radians * 180 / PI);
}

/*
* 函数：s_DotProduct
* 功能：点积
*/
static double s_DotProduct(double x1, double y1, double x2, double y2)
{
	return x1 * x2 + y1 * y2;
}

/*
* 函数：s_CrossProduct
* 功能：叉积
*/
static double s_CrossProduct(double x1, double y1, double x2, double y2)
{
	return x1 * y2 - x2 * y1;
}

/*
* 函数：s_angle
* 功能：求旋转角，返回角度值
*/
static double s_angle(double x1, double y1, double x2, double y2)
{
	x2 -= x1;  y2 -= y1;
	double angle = atan(y2 / x2) * 180 / PI;
	if (x2 < 0 && y2 >= 0) angle = 180 + angle;
	else if (x2 < 0 && y2 < 0) angle = 180 + angle;
	else if (x2 >= 0 && y2 < 0) angle = 360 + angle;

	return angle;
}







