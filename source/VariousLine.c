/*
* 文件名：VariousLine.c
* 
* VariousLine.h接口实现
*/

#include <math.h>
#include "VariousLine.h"
#include "BasicAnalysis.h"


BG_Line VL_perpendicular(BG_Line* line, BG_Point* point)
{
	BG_Line l;
	double dx = line->point[1].x - line->point[0].x;
	double dy = line->point[1].y - line->point[0].y;

	l.type = 0;
	l.point[0] = *point;
	l.point[1].x = l.point[0].x - dy;
	l.point[1].y = l.point[0].y + dx;
	
	return l;
}


BG_Line VL_parallel(BG_Line* line, BG_Point* point)
{
	BG_Line l;
	double dx = line->point[1].x - line->point[0].x;
	double dy = line->point[1].y - line->point[0].y;
	double d = sqrt(dx * dx + dy * dy);
	double dis = BA_disPointLine(point, line);

	l.point[0].x = line->point[0].x - dy * dis / d;
	l.point[0].y = line->point[0].y + dx * dis / d;
	l.point[1].x = line->point[1].x - dy * dis / d;
	l.point[1].y = line->point[1].y + dx * dis / d;
	l.type = 0;

	return l;
}


BG_Line VL_midperpendicular(BG_Point* pt1, BG_Point* pt2)
{
	return BA_midperpendicular(pt1, pt2);
}


BG_Line VL_angleBisector(BG_Point* ptc, BG_Point* pt1, BG_Point* pt2)
{
	BG_Line l;
	double dx = pt2->x - ptc->x;
	double dy = pt2->y - ptc->y;
	double d = sqrt(dx * dx + dy * dy);
	double dis = BA_disPointPoint(ptc, pt1);

	l.type = 0;
	l.point[0] = *ptc;
	l.point[1].x = (ptc->x + dx * dis / d + pt1->x) / 2;
	l.point[1].y = (ptc->y + dy * dis / d + pt1->y) / 2;

	return l;
}


BG_Line VL_tangent(BG_Arc* arc, BG_Point* point, bool left)
{
	BG_Line l;  l.type = -1;
	double disPP = BA_disPointPoint(&arc->point, point);
	if (disPP <= arc->r) return l;

	l.type = 0;
	double dis = sqrt(disPP * disPP - arc->r * arc->r);

	double x = (arc->point.x - point->x);
	double y = (arc->point.y - point->y);  // 方向向量

	double cosB = dis / disPP, sinB = arc->r / disPP;  // 选择角
	if (!left) sinB = -sinB;  // 逆时针

	double xx = x * cosB - y * sinB;
	double yy = y * cosB + x * sinB;  // 旋转公式

	l.point[0] = *point;
	l.point[1].x = point->x + xx;
	l.point[1].y = point->y + yy;

	return l;
}

BG_Line VL_bestMatch(linkedlistADT* point)
{
	BG_Line l;  l.type = -1;
	linkedlistADT now = NextNode(NULL, point);
	if (!now) return l;

	int n = 0;
	double averX = 0, averY = 0;
	double a, b, sum1 = 0, sum2 = 0;

	for (; now; now = NextNode(NULL, now))
	{
		++n;
		BG_Point* pt = NodeObj(NULL, now);
		averX += pt->x;
		averY += pt->y;
	}
	averX /= n;	averY /= n;

	for (; now; now = NextNode(NULL, now))
	{
		BG_Point* pt = NodeObj(NULL, now);
		sum1 += (pt->x - averX) * (pt->y - averY);
		sum2 += (pt->x - averX) * (pt->x - averX);
	}
	a = sum1 / sum2;
	b = averY - sum1 * averX / sum2;

	l.point[0].x = 0;
	l.point[0].y = b;
	l.point[1].x = 1;
	l.point[1].y = a + b;
	l.type = 0;

	return l;
}






