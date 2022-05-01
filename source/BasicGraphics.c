/*
* �ļ���BasicGraphics.c
* 
* ʵ����
*	BasicGraphics.h
*	ReferenceAxis.h
* �ж���Ľӿ�
* 
* ��ģ��Ӱ��ķ�Χ�����ڻ�ͼ����
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
#include "linkedlist.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "BasicAnalysis.h"
#include "NameLib.h"

#define EPSILON   0.00000001
#define Pi        3.1415926535
#define LINESIZE  3

// ��������
static double s_windowWidth;	// ��ͼ������
static double s_windowHeight;	// ��ͼ����߶�

// �����������
static double s_axisCalibration = 1;		// ������Ŀ̶ȣ�ԭ�㵽��һ���̶��ߵ���ֵ���룬��ֵֻ����pow(2, n)
static double s_axisInches = 1;				// ԭ�㵽��һ���̶��ߵ�ʵ�ʾ��룬Ӧ�ñ�֤��ֵ��[1, 2)�ķ�Χ
static double s_axisX = 0, s_axisY = 0;		// �������ĵ����ֵ����

// ��¼ͼ����������==�Ѿ������
static linkedlistADT s_listPoint;	// ��-����
static linkedlistADT s_listLine;    // ��-����
static linkedlistADT s_listVector;  // ����-����
static linkedlistADT s_listArc;  // Բ-����

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

// ��װһЩ����ĺ���
static void s_drawACircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);
static BG_Line* s_vectorToLine(BG_Vector* vect);
static double Radians(double degrees);

// RA����õľ�̬����
static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();






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
	s_windowWidth = GetWindowWidth();		// ע�����������ϵ�Ĵ���
	s_windowHeight = GetWindowHeight();		// ��һ�����������ڷ�Χ������ֻ��һ��ʾ��

	// ��ʼ������
	s_listPoint = NewLinkedList();
	s_listLine = NewLinkedList();
	s_listVector = NewLinkedList();
	s_listArc = NewLinkedList();

}

void BG_repaint()
{
	linkedlistADT now;
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

void BG_addPoint(double x, double y)
{
	// ��һ�����������
	BG_Point* point = New(BG_Point*);
	point->x = x; point->y = y;
	point->name = NL_getUpperCase();
	InsertNode(s_listPoint, NULL, point);

	// ���������
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
		// ����ʲô���Ͷ�ת��BG_Point����Ϊ����name��Ա���ǵ�һ��
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

	// ��
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
		if (StringEqual(((BG_Point*)NodeObj(s_listPoint, now))->name, name)) return 0;

	// ��
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
		if (StringEqual(((BG_Line*)NodeObj(s_listLine, now))->name, name)) return 1;

	// ����
	for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
		if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	// ��
	for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
		if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	return -1;
}

string BG_getGraphic(double x, double y)
{
	linkedlistADT now;

	// ��
	for (now = NextNode(s_listPoint, s_listPoint); now; now = NextNode(s_listPoint, now))
	{
		BG_Point* point = NodeObj(s_listPoint, now);
		if (fabs(BG_axisToInchX(point->x) - x) < 0.1 &&
			fabs(BG_axisToInchY(point->y) - y) < 0.1)
			return point->name;
	}

	// ��
	for (now = NextNode(s_listLine, s_listLine); now; now = NextNode(s_listLine, now))
	{

	}

	//// ����
	//for (now = NextNode(s_listVector, s_listVector); now; now = NextNode(s_listVector, now))
	//	if (StringEqual(((BG_Vector*)NodeObj(s_listVector, now))->name, name)) return 2;

	//// ��
	//for (now = NextNode(s_listArc, s_listArc); now; now = NextNode(s_listArc, now))
	//	if (StringEqual(((BG_Arc*)NodeObj(s_listArc, now))->name, name)) return 3;

	return "";
}





//----------------------BasicAnalysis.h�ӿ�ʵ��------------------


double BA_disPointLine(BG_Point* point, BG_Line* line)
{
	double x1 = line->point[0].x, x2 = line->point[1].x;
	double y1 = line->point[0].y, y2 = line->point[1].y;
	double a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;  // һ��ʽ
	
	return (a * point->x + b * point->y + c) / sqrt(a * a + b * b);
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

	MovePen(x + r * cos(Radians(arc->start)), y + r * sin(Radians(arc->start)));
	DrawArc(r, arc->start, arc->end - arc->start);
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
* ������Radians
* ���ܣ��Ƕ�ת����
*/
static double Radians(double degrees)
{
	return (degrees * Pi / 180);
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



















