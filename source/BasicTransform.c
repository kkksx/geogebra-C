/*
* �ļ���BasicTransform.c
* 
* ʵ�ֶ�Ӧ�ӿڣ�����ÿһ������������д���ʽ
* 
*/

#include <math.h>
#include "BasicTransform.h"

#define PI 3.1415926535897  // Բ���ʦ�

static double s_angle(double x1, double y1, double x2, double y2);

/*
* �ӿڣ�BT_drawAxisymmetry
* ���ܣ���һ��ͼ�ι���ֱ�ߵĶԳ�ͼ��
*		����˶Գ�ͼ�β��ҷ�����ָ��
*/
void* BT_drawAxisymmetry(void* graph, BG_Line* line)
{
	int type = BG_getGraphicType(graph);
	switch (type)
	{

	case ID_Point:
	{
		BG_Point* pt = graph;
		BG_Point pt2 = BA_getPedal(pt, line);
		return BG_addPoint(2 * pt2.x - pt->x, 2 * pt2.y - pt->y);
	}

	case ID_Line:
	{
		BG_Line* l = graph;
		BG_Point pt1 = BA_getPedal(&l->point[0], line);
		BG_Point pt2 = BA_getPedal(&l->point[1], line);
		return BG_addLine(2 * pt1.x - l->point[0].x,
			2 * pt1.y - l->point[0].y,
			2 * pt2.x - l->point[1].x,
			2 * pt2.y - l->point[1].y,
			l->type
		);
	}

	case ID_Vector:
	{
		BG_Vector* vector = graph;
		BG_Point pt1 = BA_getPedal(&vector->point[0], line);
		BG_Point pt2 = BA_getPedal(&vector->point[1], line);
		return BG_addVector(
			2 * pt1.x - vector->point[0].x,
			2 * pt1.y - vector->point[0].y,
			2 * pt2.x - vector->point[1].x,
			2 * pt2.y - vector->point[1].y
		);
	}

	case ID_Arc:
	{
		BG_Arc* arc = graph;
		BG_Point pt = BA_getPedal(&arc->point, line);
		double angle = s_angle(line->point[0].x, line->point[0].y, line->point[1].x, line->point[1].y);

		return BG_addArc(
			2 * pt.x - arc->point.x,
			2 * pt.y - arc->point.y,
			arc->r,
			2 * angle - arc->end,
			2 * angle - arc->start
		);
	}

	}

	return NULL;
}


/*
* �ӿڣ�BT_drawCentralSymmetry
* ���ܣ���һ��ͼ�ι��ڵ�����ĶԳ�ͼ��
*		����Գ�ͼ�β�������ָ��
*/
void* BT_drawCentralSymmetry(void* graph, BG_Point* point)
{
	int type = BG_getGraphicType(graph);
	switch (type)
	{

	case ID_Point:
	{
		BG_Point* pt = graph;
		return BG_addPoint(2 * point->x - pt->x, 2 * point->y - pt->y);
	}

	case ID_Line:
	{
		BG_Line* l = graph;
		return BG_addLine(2 * point->x - l->point[0].x,
			2 * point->y - l->point[0].y,
			2 * point->x - l->point[1].x,
			2 * point->y - l->point[1].y,
			l->type
		);
	}

	case ID_Vector:
	{
		BG_Vector* l = graph;
		return BG_addVector(2 * point->x - l->point[0].x,
			2 * point->y - l->point[0].y,
			2 * point->x - l->point[1].x,
			2 * point->y - l->point[1].y
		);
	}

	case ID_Arc:
	{
		BG_Arc* arc = graph;
		return BG_addArc(
			2 * point->x - arc->point.x,
			2 * point->y - arc->point.y,
			arc->r,
			arc->start + 180,
			arc->end + 180
		);
	}

	}

	return NULL;
}

/*
* �ӿڣ�BT_drawRotate
* ���ܣ���һ��ͼ�ι���һ������ʱ����תangle�Ⱥ��ͼ��
*		�����ת��ͼ�β�������ָ��
*/
void* BT_drawRotate(void* graph, BG_Point* point, double angle)
{
	int type = BG_getGraphicType(graph);
	double cosB = cos(angle);
	double sinB = sin(angle);

	switch (type)
	{

	case ID_Point:
	{
		BG_Point* pt = graph;
		double x = pt->x - point->x;
		double y = pt->y - point->y;
		return BG_addPoint(
			x * cosB - y * sinB + point->x,
			y * cosB + x * sinB + point->y
		);
	}

	case ID_Line:
	{
		BG_Line* l = graph;
		double x1 = l->point[0].x - point->x;
		double y1 = l->point[0].y - point->y;
		double x2 = l->point[1].x - point->x;
		double y2 = l->point[1].y - point->y;

		return BG_addLine(
			x1 * cosB - y1 * sinB + point->x,
			y1 * cosB + x1 * sinB + point->y,
			x2 * cosB - y2 * sinB + point->x,
			y2 * cosB + x2 * sinB + point->y,
			l->type
		);
	}

	case ID_Vector:
	{
		BG_Vector* l = graph;
		double x1 = l->point[0].x - point->x;
		double y1 = l->point[0].y - point->y;
		double x2 = l->point[1].x - point->x;
		double y2 = l->point[1].y - point->y;

		return BG_addVector(
			x1 * cosB - y1 * sinB + point->x,
			y1 * cosB + x1 * sinB + point->y,
			x2 * cosB - y2 * sinB + point->x,
			y2 * cosB + x2 * sinB + point->y
		);
	}

	case ID_Arc:
	{
		BG_Arc* arc = graph;
		double x = arc->point.x - point->x;
		double y = arc->point.y - point->y;
			
		return BG_addArc(
			x * cosB - y * sinB + point->x,
			y * cosB + x * sinB + point->y,
			arc->r,
			arc->start + angle,
			arc->end + angle
		);
	}

	}

	return NULL;
}



/*
* ������s_angle
* ���ܣ�����ת�ǣ����ؽǶ�ֵ
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