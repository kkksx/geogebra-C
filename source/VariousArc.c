/*
* 文件：VariousArc.c
* 
* 作用是实现弧按钮的一系列接口，简化操作
* 
*/

#include <math.h>

#include "VariousArc.h"
#include "BasicAnalysis.h"

static double s_angle(double x1, double y1, double x2, double y2);


BG_Arc* VA_circleCenterPoint(double x1, double y1, double x2, double y2)
{
	return BG_addArc(
		x1, y1,
		sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)),
		0, 360
	);
}

BG_Arc* VA_circleCenterRadius(double x, double y, double r)
{
	return BG_addArc(x, y, r, 0, 360);
}

BG_Arc* VA_circle3Point(double x1, double y1, double x2, double y2, double x3, double y3)
{
	BG_Point a = (BG_Point){ "", x1, y1 };
	BG_Point b = (BG_Point){ "", x2, y2 };
	BG_Point c = (BG_Point){ "", x3, y3 };
	BG_Line line1 = BA_midperpendicular(&a, &b);
	BG_Line line2 = BA_midperpendicular(&a, &c);

	BG_Point p = BA_crossPoint(&line1, &line2);
	return BG_addArc(p.x, p.y, BA_disPointPoint(&p, &a), 0, 360);
}

BG_Arc* VA_halfPointPoint(double x1, double y1, double x2, double y2)
{
	BG_Point a = (BG_Point){ "", x1, y1 };
	BG_Point b = (BG_Point){ "", x2, y2 };
	BG_Point mid = BA_midPoint(&a, &b);

	double start = s_angle(mid.x, mid.y, a.x, a.y);
	
	return BG_addArc(mid.x, mid.y, BA_disPointPoint(&mid, &a), start, start + 180);
}

BG_Arc* VA_arcCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double start = s_angle(x1, y1, x2, y2);
	double end = s_angle(x1, y1, x3, y3);

	return BG_addArc(x1, y1, sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)), start, end);
}

BG_Arc* VA_arc3Point(double x1, double y1, double x2, double y2, double x3, double y3)
{
	BG_Point a = (BG_Point){ "", x1, y1 };
	BG_Point b = (BG_Point){ "", x2, y2 };
	BG_Point c = (BG_Point){ "", x3, y3 };
	BG_Line line1 = BA_midperpendicular(&a, &b);
	BG_Line line2 = BA_midperpendicular(&b, &c);

	BG_Point p = BA_crossPoint(&line1, &line2);
	double start = s_angle(p.x, p.y, x1, y1);
	double end = s_angle(p.x, p.y, x3, y3);
	if ((x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1) < 0)
	{
		double temp = start;
		start = end;
		end = temp;
	}

	return BG_addArc(p.x, p.y, BA_disPointPoint(&p, &a), start, end);
}

AG_Sector* VA_sectorCenterPoint(double x1, double y1, double x2, double y2, double x3, double y3, string color)
{
	double start = s_angle(x1, y1, x2, y2);
	double end = s_angle(x1, y1, x3, y3);

	return AG_addSector(x1, y1, sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)), start, end, color);
}

AG_Sector* VA_sector3Point(double x1, double y1, double x2, double y2, double x3, double y3, string color)
{
	BG_Point a = (BG_Point){ "", x1, y1 };
	BG_Point b = (BG_Point){ "", x2, y2 };
	BG_Point c = (BG_Point){ "", x3, y3 };
	BG_Line line1 = BA_midperpendicular(&a, &b);
	BG_Line line2 = BA_midperpendicular(&b, &c);

	BG_Point p = BA_crossPoint(&line1, &line2);
	double start = s_angle(p.x, p.y, x1, y1);
	double end = s_angle(p.x, p.y, x3, y3);
	if ((x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1) < 0)
	{
		double temp = start;
		start = end;
		end = temp;
	}

	return AG_addSector(p.x, p.y, BA_disPointPoint(&p, &a), start, end, color);
}

// 求旋转角，返回角度值
static double s_angle(double x1, double y1, double x2, double y2)
{
	x2 -= x1;  y2 -= y1;
	double angle = atan(y2 / x2) * 180 / 3.1415926535897;
	if (x2 < 0 && y2 >= 0) angle = 180 + angle;
	else if (x2 < 0 && y2 < 0) angle = 180 + angle;
	else if (x2 >= 0 && y2 < 0) angle = 360 + angle;

	return angle;
}