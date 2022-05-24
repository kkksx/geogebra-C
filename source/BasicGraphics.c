/*
* �ļ���BasicGraphics.c
* 
* ʵ����
*	BasicGraphics.h
*	ReferenceAxis.h
*	BasicAnalysis.h
*	AdvanceGraphics.h
* �ж���Ľӿ�
* 
* ͼ�����ࣺ0:�㣬1:ֱ�ߣ�2:������3:��
		   4������Σ�5������
		   6����Բ��7��˫���ߣ�8��������
* 
* ��ģ��Ӱ���ͼ����
* 
* ��ֵ���룺��������ʽд�����ľ��룬�����������ڵľ���
* ʵ�ʾ��룺ռ���ڵ�Ӣ��
* 
* ������ɫ����
	DefineColor("my Blue", .4, .65, 1);				���ڸ���Ⱦɫ
	DefineColor("nearly white", 0.9, 0.9, 1);		2����������
	DefineColor("somehow white", 0.8, 0.8, 0.9);	1����������
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

#define EPSILON   0.00000001		// double����
#define PI        3.1415926535897	// Բ���ʦ�
#define INF		  1145141919.810	// ����ֵ
#define LINESIZE  2					// �ߵĴ�ϸ

#define Max(x, y) (((x) > (y)) ? (x) : (y))
#define Min(x, y) (((x) < (y)) ? (x) : (y))

// ��������
static double s_windowWidth;	// ��ͼ������
static double s_windowHeight;	// ��ͼ����߶�

// �����������
static double s_axisCalibration = 1;		// ������Ŀ̶ȣ�ԭ�㵽��һ���̶��ߵ���ֵ���룬��ֵֻ����pow(2, n)
static double s_axisInches = 1;				// ԭ�㵽��һ���̶��ߵ�ʵ�ʾ��룬Ӧ�ñ�֤��ֵ��[1, 2)�ķ�Χ
static double s_axisX = 0, s_axisY = 0;		// �������ĵ����ֵ����

// ��¼ͼ����������==�Ѿ������
// ʹ�ö�ά������ٴ�����( ? )
static linkedlistADT s_list;			 // ���������ڵ�����Ϊ���¸��������ͷ
static linkedlistADT s_listPoint;		 // ��-����
static linkedlistADT s_listLine;		 // ��-����
static linkedlistADT s_listVector;		 // ����-����
static linkedlistADT s_listArc;			 // ��-����
static linkedlistADT s_listPolygon;		 // �����-����
static linkedlistADT s_listSector;		 // ����-����
static linkedlistADT s_listEllipse;		 // ��Բ-����
static linkedlistADT s_listHyperbola;    // ˫����-����
static linkedlistADT s_listParabola;     // ������-����



// ����״̬
static int    s_penSize;			// ���ʴ�С
static int    s_pointSize;			// ���ִ�С
static string s_penColor;			// ������ɫ



// -----------------------��̬��������---------------------

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

// ��װһЩ����ĺ���
static void s_drawACircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);
static void s_drawAllLine(BG_Point* point, int n, bool closed);
static BG_Line* s_vectorToLine(BG_Vector* vect);

// RA����õľ�̬����
static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();

// һЩ��ѧ����
static int s_dcmp(double x);
static double s_angle(double x1, double y1, double x2, double y2);
static double s_Radians(double degrees);
static double s_Degrees(double radians);
static double s_DotProduct(double x1, double y1, double x2, double y2);
static double s_CrossProduct(double x1, double y1, double x2, double y2);




//-----------------ReferenceAxis.h�ӿ�ʵ��-----------------------

void RA_createAxis()
{
	double centerX = s_windowWidth / 2;
	double centerY = s_windowHeight / 2;
	double scale = s_axisCalibration / s_axisInches;  // ��λӢ�����ֵ����

	double nearX = s_axisCalibration * floor(s_axisX / s_axisCalibration);	// �����ĵ����������
	double nearY = s_axisCalibration * floor(s_axisY / s_axisCalibration);	// ����������ֵ����
	
	int j;

	SetPenSize(1);

	// ǳɫpart

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

	// ��ɫpart

	double X = s_RA_findX();  // ˮƽ���ֵ�λ��
	X = X < GetFontHeight() ? GetFontHeight() :	X - GetFontHeight();
	double Y = s_RA_findY();  // ��ֱ���ֵ�λ��

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


//-----------------BasicGraphics.h�ӿ�ʵ��-----------------------

// !!��ʼ��!!
void BG_init()
{
	// ����һЩ��ɫ
	DefineColor("my Blue", .4, .65, 1);
	DefineColor("nearly white", 0.9, 0.9, 1);
	DefineColor("somehow white", 0.8, 0.8, 0.9);

	// ��ʼ��������Ϣ
	s_windowWidth = GetWindowWidth();			// ע�����������ϵ�Ĵ���
	s_windowHeight = GetWindowHeight() - 1;		// ��һ�����������ڷ�Χ������ֻ��һ��ʾ��

	// ��ʼ������
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

	// �����
	for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	{
		s_drawPolygon(NodeObj(s_listPolygon, now));
	}

	// ����
	for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	{
		s_drawSector(NodeObj(s_listSector, now));
	}

	// ��
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{
		s_drawLine(NodeObj(s_listLine, now));
	}

	// ��
	for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	{
		s_drawArc(NodeObj(s_listArc, now));
	}

	// ��Բ
	for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	{
		s_drawEllipse(NodeObj(s_listEllipse, now));
	}

	// ˫����
	for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	{
		s_drawHyperbola(NodeObj(s_listHyperbola, now));
	}

	// ������
	for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	{
		s_drawParabola(NodeObj(s_listParabola, now));
	}

	// ����
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	{
		s_drawVector(NodeObj(s_listVector, now));
	}

	// ��
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		s_drawPoint(NodeObj(s_listPoint, now));
	}
}


BG_Point* BG_addPoint(double x, double y)
{
	// ��һ�����������
	BG_Point* point = New(BG_Point*);
	point->x = x; point->y = y;
	point->name = NL_getUpperCase();
	InsertNode(s_listPoint, NULL, point);

	// ���������
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
			nameType = (listCount == 0);	// ֻ�е��Ǵ�д��ĸ
		}
	}

	for (now = NextNode(head, head); now; now = NextNode(head, now))
	{
		// ����ʲô���Ͷ�ת��BG_Point����Ϊ����name��Ա���ǵ�һ��
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

	double range = 0.05 * s_axisCalibration / s_axisInches;  // ������ж�Բ������0.05Ӣ��

	// ��
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		BG_Point* point = NodeObj(s_listPoint, now);
		if (BA_disPointPoint(pt, point) < range) return point->name;
	}

	// ��
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{
		BG_Line* line = NodeObj(s_listLine, now);
		BG_Point* point[2] = { &line->point[0], &line->point[1] };

		double dis = BA_disPointLine(pt, line);
		if (dis >= range) continue;

		if (line->type == 0) return line->name;
		else if (line->type == 1)	// ����
		{
			if (s_DotProduct(point[1]->x - point[0]->x, point[1]->y - point[0]->y,
				pt->x - point[0]->x, pt->y - point[0]->y) > 0) return line->name;
			else if (BA_disPointPoint(pt, point[0]) < range) return line->name;
		}
		else if (line->type == 2)	// �߶����жϴ����Ƿ������߶���
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

	// ����
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	{
		BG_Vector* vect = NodeObj(s_listVector, now);
		BG_Point* point[2] = { &vect->point[0], &vect->point[1] };

		double dis = BA_disPointLine(pt, (BG_Line*)vect);	// �˴�ǿת��ǰ��������ǰ����Ԫ����ͬ
		if (dis >= range) continue;

		if (BA_disPointPoint(pt, point[0]) < range) return vect->name;
		if (BA_disPointPoint(pt, point[1]) < range) return vect->name;
		if (s_DotProduct(point[1]->x - point[0]->x, point[1]->y - point[0]->y,
			pt->x - point[0]->x, pt->y - point[0]->y) > 0 &&
			s_DotProduct(point[0]->x - point[1]->x, point[0]->y - point[1]->y,
				pt->x - point[1]->x, pt->y - point[1]->y) > 0)
			return vect->name;
	}

	// ��
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
	
	// �����
	// �жϱ�׼�����߷� �� ��������������
	for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	{
		linkedlistADT firstPoint = NextNode(NULL, ((AG_Polygon*)(now->dataptr))->vertice), point;
		int count = 0;	// ����
		int flag = 0;   // ������
		for (point = firstPoint; point; point = NextNode(NULL, point))
		{
			++count;
			BG_Point* p1 = NodeObj(NULL, point);
			BG_Point* p2 = NULL;
			if (point->next == NULL) p2 = NodeObj(NULL, firstPoint);
			else p2 = NodeObj(NULL, point->next);

			// ���ж��Ƿ���������
			if (fabs(s_CrossProduct(p2->x - p1->x, p2->y - p1->y, pt->x - p1->x, pt->y - p1->y)) < EPSILON) 
				return ((AG_Polygon*)now->dataptr)->name;

			// ����(pt->x, pt->y)���ҵ�һ��������(p1->x, p1->y)--(p2->x, p2->y)�Ľ�������
			// ��һ���жϱ�֤��pt->y������y֮��
			// �ڶ����жϱ�֤�˵��������
			// �������жϱ�֤�˽��㲻��yֵ��С�ĵ�
			if ((s_dcmp(p1->y - pt->y) > 0 != s_dcmp(p2->y - pt->y) > 0) 
				&& s_dcmp(pt->x - (pt->y - p1->y) * (p1->x - p2->x) / (p1->y - p2->y) - p1->x) < 0
				&& s_dcmp(pt->y - Min(p1->y, p2->y)) > 0)
					++flag;
		}
		if (count >= 3 && flag % 2) return ((AG_Polygon*)now->dataptr)->name;
	}

	// ����
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

	// ��Բ
	// �жϱ�׼������������ = 2a
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

	// ˫����
	// �жϱ�׼������������ = 2a
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

	// ������
	// �жϱ�׼����������� = ��׼�߾���
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

	//// ��
	//for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	//	if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return 0;

	//// ��
	//for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	//	if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return 1;

	//// ����
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	//// ��
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	//// �����
	//for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	//	if (StringEqual(((AG_Polygon*)NodeObj(s_listPolygon, now))->name, name)) return 4;

	//// ����
	//for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	//	if (StringEqual(((AG_Sector*)NodeObj(s_listSector, now))->name, name)) return 5;

	//// ��Բ
	//for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	//	if (StringEqual(((AG_Ellipse*)NodeObj(s_listEllipse, now))->name, name)) return 6;

	//// ˫����
	//for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	//	if (StringEqual(((AG_Hyperbola*)NodeObj(s_listHyperbola, now))->name, name)) return 7;

	//// ������
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

	//// ��
	//for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	//	if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return now->dataptr;

	//// ��
	//for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	//	if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return now->dataptr;

	//// ����
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return now->dataptr;

	//// ��
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return now->dataptr;

	//// �����
	//for (now = NextNode(s_listPolygon, s_listPolygon); now; now = NextNode(s_listPolygon, now))
	//	if (StringEqual(((AG_Polygon*)NodeObj(s_listPolygon, now))->name, name)) return now->dataptr;

	//// ����
	//for (now = NextNode(s_listSector, s_listSector); now; now = NextNode(s_listSector, now))
	//	if (StringEqual(((AG_Sector*)NodeObj(s_listSector, now))->name, name)) return now->dataptr;

	//// ��Բ
	//for (now = NextNode(s_listEllipse, s_listEllipse); now; now = NextNode(s_listEllipse, now))
	//	if (StringEqual(((AG_Ellipse*)NodeObj(s_listEllipse, now))->name, name)) return now->dataptr;

	//// ˫����
	//for (now = NextNode(s_listHyperbola, s_listHyperbola); now; now = NextNode(s_listHyperbola, now))
	//	if (StringEqual(((AG_Hyperbola*)NodeObj(s_listHyperbola, now))->name, name)) return now->dataptr;

	//// ������
	//for (now = NextNode(s_listParabola, s_listParabola); now; now = NextNode(s_listParabola, now))
	//	if (StringEqual(((AG_Parabola*)NodeObj(s_listParabola, now))->name, name)) return now->dataptr;

	return NULL;
}




//----------------------AdvanceGraphics.h�ӿ�ʵ��---------------------

// ����

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



// �����

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
		// ��һ�����������
		BG_Point* point = vertice[i];
		InsertNode(polygon->vertice, NULL, point);
	
		if (i + 1 == n) break;
		BG_Point* point2 = vertice[i + 1];
		InsertNode(polygon->edge, NULL,
			BG_addLine(point->x, point->y, point2->x, point2->y, 2));
	}

	// ����ö���β����
	InsertNode(s_listPolygon, NULL, polygon);
	s_drawPolygon(polygon);
	return polygon;
}

AG_Polygon* AG_addPoint(AG_Polygon* polygon, BG_Point* point)
{
	if (polygon->vertice->next == NULL)  // һ���㶼û��
	{
		InsertNode(polygon->vertice, NULL, point);
		return polygon;
	}
	// ��һ���ߣ�һ����
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


// ����Բ׶���߻���

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


// Բ׶�����������ƽӿ�

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








//----------------------BasicAnalysis.h�ӿ�ʵ��-----------------------


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
	double a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;  // һ��ʽ
	
	return fabs(a * point->x + b * point->y + c) / sqrt(a * a + b * b);
}

BG_Point BA_getPedal(BG_Point* point, BG_Line* line)
{
	double dis = BA_disPointLine(point, line);
	double x = line->point[1].x - line->point[0].x;
	double y = line->point[1].y - line->point[0].y;
	double d = sqrt(x * x + y * y);
	double dx = -y / d, dy = x / d;  // ��������
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
	if (BA_isParallel(line1, line2))  // ʵ����Ӧ���ڵ��øú���ǰ���ж�ƽ��
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




//------------------------��̬����ʵ��----------------------------

/*
* ������s_inAxis
* ���ܣ��ж�ʵ������Ϊ(x, y)�ĵ��Ƿ�������ϵ��
*		����ϵ�ľ��巶ΧӦ����ȷ��
*/
static bool s_inAxis(double x, double y)
{
	return (0 <= x && x <= s_windowWidth && 0 <= y && y <= s_windowHeight);
}

/*
* ������s_savePenStatus
* ���ܣ����滭�ʵ�һЩ״̬
* !!! ע�⣬�ú�����Ӧ����Ƕ�׺���������������
*/
static void s_savePenStatus()
{
	s_penSize = GetPenSize();
	s_pointSize = GetPointSize();
	s_penColor = GetPenColor();
}

/*
* ������s_loadPenStatus
* ���ܣ�����ǰ�汣��Ļ���״̬
*/
static void s_loadPenStatus()
{
	SetPenSize(s_penSize);
	SetPointSize(s_pointSize);
	SetPenColor(s_penColor);
}

/*
* ������s_drawName
* ���ܣ���(x,y)��д����
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
* ������s_drawPoint
* ���ܣ����һ����
*/
static void s_drawPoint(BG_Point* point)
{
	if (point == NULL) return;
	//s_savePenStatus();

	double x = BG_axisToInchX(point->x);
	double y = BG_axisToInchY(point->y);

	SetPenSize(1);

	// ��Բ
	StartFilledRegion(1);
	SetPenColor("my blue");
	s_drawACircle(x, y, 0.08);
	EndFilledRegion();
	
	SetPenColor("black");
	s_drawACircle(x, y, 0.09);
	
	// ���
	s_drawName(x + 0.1, y + 0.1, point->name);

	//s_loadPenStatus();
}

/*
* ������s_drawLine
* ���ܣ����һ���ߣ������ߣ������㣩
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
	case 0:   // ֱ�� 
		if (fabs(x2 - x1) <= EPSILON) s_drawALine(x1, 0, x1, s_windowHeight);
		else s_drawALine(0, -x1 * (y2 - y1) / (x2 - x1) + y1,
			 s_windowWidth, (s_windowWidth - x1) * (y2 - y1) / (x2 - x1) + y1);
		break;
	case 1:	  // ����
		if (fabs(x2 - x1) <= EPSILON) s_drawALine(x1, y1, x1, s_windowHeight);
		else if(x1 < x2) s_drawALine(x1, y1, s_windowWidth, (s_windowWidth - x1) * (y2 - y1) / (x2 - x1) + y1);
		else s_drawALine(x1, y1, 0, -x1 * (y2 - y1) / (x2 - x1) + y1);
		break;
	default:  // �߶�
		s_drawALine(x1, y1, x2, y2);
	}

	s_drawName((x1 + x2) / 2 + 0.1, (y1 + y2) / 2 + 0.1, line->name);
	//s_loadPenStatus();
}

/*
* ������s_drawVector
* ���ܣ���һ������������ͷ���߶Σ�
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

	// ��С������
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
* ������s_drawArc
* ���ܣ���һ��Բ��
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
* ������s_drawPolygon
* ���ܣ���һ������Σ����Ҫ��͸���ȣ�ֻ��Ҫ���������
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

	a = NodeObj(NULL, lasPoint);  // ��һ����
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

	// ���һ���պ���
	if (polygon->closed)
	{
		a = NodeObj(NULL, lasPoint);
		b = NodeObj(NULL, NextNode(polygon->vertice, polygon->vertice));
		DrawLine(BG_axisToInchX(b->x) - BG_axisToInchX(a->x),
				 BG_axisToInchY(b->y) - BG_axisToInchY(a->y));
	}

	EndFilledRegion();


	// ���»��㣬��֤������������
	nowPoint = NextNode(polygon->vertice, polygon->vertice);
	for (; nowPoint; nowPoint = NextNode(polygon->vertice, nowPoint))
	{
		s_drawPoint((BG_Point*)NodeObj(polygon->vertice, nowPoint));
	}

}

/*
* ������s_drawEllipse
* ���ܣ��ò������̷��߶λ���Բ
*/
static void s_drawEllipse(AG_Ellipse* ellipse)
{
	BG_Point pt[360];
	double cx = ellipse->point.x, cy = ellipse->point.y;
	double dx = ellipse->direction.x, dy = ellipse->direction.y;

	double a = ellipse->a;
	double b = ellipse->b;  // ������

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // ��ת��

	int i;
	for (i = 0; i < 360; ++i)
	{
		double x = a * cos(i * PI / 180);
		double y = b * sin(i * PI / 180);  // ��׼��Բ��������

		double xx = x * cosB - y * sinB;
		double yy = y * cosB + x * sinB;  // ��ת��ʽ

		pt[i].x = xx + cx;
		pt[i].y = yy + cy;
	}

	s_drawAllLine(pt, 360, 1);
}

/*
* ������s_drawHyperbola
* ���ܣ���˫���ߣ�����ͬ��Բ
*/
static void s_drawHyperbola(AG_Hyperbola* hyperbola)
{
	BG_Point pt[360];
	double cx = hyperbola->point.x, cy = hyperbola->point.y;
	double dx = hyperbola->direction.x, dy = hyperbola->direction.y;

	double a = hyperbola->a;
	double b = hyperbola->b;  // ������

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // ��ת��

	int i;
	for (i = 0; i < 360; ++i)
	{
		if (i == 0 || i == 180) continue;
		double x = a / cos((i + 270) * PI / 180);
		double y = b * tan((i + 270) * PI / 180);  // ��׼��Բ��������

		double xx = x * cosB - y * sinB;
		double yy = y * cosB + x * sinB;  // ��ת��ʽ

		pt[i].x = xx + cx;
		pt[i].y = yy + cy;
	}

	s_drawAllLine(pt + 1, 179, 0);
	s_drawAllLine(pt + 181, 179, 0);

	// ���������С���ĸ������޷������ӳ��������ߴ���
	BG_Line line;
	line.name = "";
	line.type = 1;
	line.point[0] = pt[2]; line.point[1] = pt[1]; s_drawLine(&line);
	line.point[0] = pt[178]; line.point[1] = pt[179]; s_drawLine(&line);
	line.point[0] = pt[182]; line.point[1] = pt[181]; s_drawLine(&line);
	line.point[0] = pt[358]; line.point[1] = pt[359]; s_drawLine(&line);
}

/*
* ������s_drawParabola
* ���ܣ���������
*/
static void s_drawParabola(AG_Parabola* parabola)
{
	double f = parabola->f;
	double cx = parabola->point.x, cy = parabola->point.y;
	double dx = parabola->direction.x, dy = parabola->direction.y;

	double leftX = BG_inchToAxisX(0), rightX = BG_inchToAxisX(s_windowWidth);
	double downY = BG_inchToAxisY(0), upY = BG_inchToAxisY(s_windowHeight);	 // ���ڱ߽����ֵ����

	double minX = INF, maxX = -INF;  // �����ܡ��ڴ����г��ֵ�˫����x��Χ
	double X[4] = { leftX, rightX, leftX, rightX };
	double Y[4] = { downY, downY, upY, upY };	// �ĸ���
	BG_Line line;

	int i;
	for (i = 0; i < 4; ++i)
	{
		line.point[0] = (BG_Point){ "", X[i], Y[i] };
		line.point[1] = (BG_Point){ "", X[i] - dy, Y[i] + dx };
		double dis = BA_disPointLine(&parabola->point, &line);
		if (s_DotProduct(dx, dy, X[i] - cx, Y[i] - cy) < 0) dis = 0;  // �۽�˵�������������ߺ���
		minX = Min(minX, dis);  maxX = Max(maxX, dis);
	}

	// ֻ����x in [minX, maxX]�����Ҳ�����ɷ�
	// ��ʼ������ߣ�������������Բ׶����

	int sign = 1, j;
	BG_Point pt[201];

	double sinB = dy / sqrt(dx * dx + dy * dy);
	double cosB = dx / sqrt(dx * dx + dy * dy);  // ��ת��
	double part = (maxX - minX) / 200, nowX;

	for (j = 0; j < 2; ++j, sign *= -1)	 // ͬʱ��������y
	{
		for (i = 0, nowX = minX; i <= 200; ++i, nowX += part)
		{
			double x = nowX;
			double y = 2 * sqrt(f * x) * sign;

			double xx = x * cosB - y * sinB;
			double yy = y * cosB + x * sinB;  // ��ת��ʽ
			pt[i].x = xx + cx;
			pt[i].y = yy + cy;
		}
		s_drawAllLine(pt, 201, 0);
	}
}

/*
* ������s_drawSector
* ���ܣ�������
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

	// ���
	SetPenColor("black");
	DrawLine(r * cos(s_Radians(sector->start)), r * sin(s_Radians(sector->start)));
	DrawArc(r, sector->start, sector->end - sector->start);
	DrawLine(-r * cos(s_Radians(sector->end)), -r * sin(s_Radians(sector->end)));
}






/*
* ������s_vectorToLine
* ���ܣ�����һ���������غϵ��߶�
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
* ������s_drawACircle
* ���ܣ���(x,y)ΪԲ�Ļ��뾶Ϊr��Բ
*/
static void s_drawACircle(double x, double y, double r)
{
	MovePen(x + r, y);
	DrawArc(r, 0, 360);
}

/*
* ������s_drawDouble
* ���ܣ���ָ��λ�����һ����
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
* ������s_drawALine
* ���ܣ���һ���߶�
*/
static void s_drawALine(double x1, double y1, double x2, double y2)
{
	MovePen(x1, y1);
	DrawLine(x2 - x1, y2 - y1);
}



/*
* ������s_drawAllLine
* ���ܣ�ʹ��һ�������黭ͼ��
		���ƶ���Σ��������������������
		closed�����պ����
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
* ������s_RA_drawLine
* ���ܣ���ʵ������(x,y)��һ��ֱ��
*		type=0�����ߣ� type=1������
*		style=0����ʵ�ߣ� style=1����ʵ�ߣ� style=2������
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

	// ����С��ͷ
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
* ������s_RA_findX
* ���ܣ����X���ʵ��λ��
*		�������0������0��������ڴ��ڣ��򷵻ش��ڸ߶�
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
* ������s_RA_findY
* ���ܣ����Y���ʵ��λ��
*		�������0������0��������ڴ��ڣ��򷵻ش��ڿ��
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
* ������s_dcmp
* ���ܣ���̬��������ȥ���ֻ����x������0����
*/
static int s_dcmp(double x) 
{
	if (fabs(x) < EPSILON) return 0;
	return x < 0 ? -1 : 1;
}

/*
* ������s_Radians
* ���ܣ��Ƕ�ת����
*/
static double s_Radians(double degrees)
{
	return (degrees * PI / 180);
}

/*
* ������s_Degrees
* ���ܣ�����ת�Ƕ�
*/
static double s_Degrees(double radians)
{
	return (radians * 180 / PI);
}

/*
* ������s_DotProduct
* ���ܣ����
*/
static double s_DotProduct(double x1, double y1, double x2, double y2)
{
	return x1 * x2 + y1 * y2;
}

/*
* ������s_CrossProduct
* ���ܣ����
*/
static double s_CrossProduct(double x1, double y1, double x2, double y2)
{
	return x1 * y2 - x2 * y1;
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







