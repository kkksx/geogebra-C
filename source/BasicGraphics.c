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

	DefineColor("my Blue", .4, .65, 1);				���ڸ���Ⱦɫ
	DefineColor("nearly white", 0.95, 0.95, 1);		2����������
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
#include "NameLib.h"

#define EPSILON 0.00000001


// ��������
static double s_windowWidth;	// ��ͼ������
static double s_windowHeight;	// ��ͼ����߶�

// �����������
static double s_axisCalibration = 1;		// ������Ŀ̶ȣ�ԭ�㵽��һ���̶��ߵ���ֵ���룬��ֵֻ����pow(2, n)
static double s_axisInches = 2;				// ԭ�㵽��һ���̶��ߵ�ʵ�ʾ��룬Ӧ�ñ�֤��ֵ��[1.3, 2.6)�ķ�Χ
static double s_axisX = 0, s_axisY = 0;		// �������ĵ����ֵ����

// ��¼ͼ����������==�Ѿ������
static linkedlistADT s_listPoint;	// ��-����
static linkedlistADT s_listLine;    // ��-����
static linkedlistADT s_listVector;  // ����-����

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

// ��װһЩ����ĺ���
static void s_drawCircle(double x, double y, double r);
static void s_drawName(double x, double y, string name);
static void s_drawDouble(double x, double y, double number);
static void s_drawALine(double x1, double y1, double x2, double y2);

static void s_RA_drawLine(double x, double y, int type, int style);
static double s_RA_findX();
static double s_RA_findY();




// ��ʼ��
void BG_init()
{
	// ��ʼ��������Ϣ
	s_windowWidth = GetWindowWidth();		// ע�����������ϵ�Ĵ���
	s_windowHeight = GetWindowHeight();		// ��һ�����������ڷ�Χ������ֻ��һ��ʾ��

	// ��ʼ������
	s_listPoint = NewLinkedList();
	s_listLine = NewLinkedList();
	s_listVector = NewLinkedList();

}

//-----------------ReferenceAxis.h�ӿ�ʵ��-----------------------

void RA_createAxis()
{
	double centerX = s_windowWidth / 2;
	double centerY = s_windowHeight / 2;
	double scale = s_axisCalibration / s_axisInches;  // ��λӢ�����ֵ����

	double nearX = s_axisCalibration * floor(s_axisX / s_axisCalibration);	// �����ĵ����������
	double nearY = s_axisCalibration * floor(s_axisY / s_axisCalibration);	// ����������ֵ����
	
	int j;

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


//-----------------BasicGraphics.h�ӿ�ʵ��-----------------------

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
	
}

void BG_addVector(double x1, double y1, double x2, double y2)
{

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
	s_savePenStatus();

	SetPenSize(1);

	// ��Բ
	StartFilledRegion(1);
	SetPenColor("my blue");
	s_drawCircle(point->x, point->y, 0.08);
	EndFilledRegion();
	
	SetPenColor("black");
	s_drawCircle(point->x, point->y, 0.09);
	
	// ���
	s_drawName(point->x + 0.1, point->y + 0.1, point->name);

	s_loadPenStatus();
}

/*
* ������s_drawCircle
* ���ܣ���(x,y)ΪԲ�Ļ��뾶Ϊr��Բ
*/
static void s_drawCircle(double x, double y, double r)
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
	s_loadPenStatus();
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



















