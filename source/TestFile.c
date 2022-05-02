#include "graphics.h"
#include "extgraph.h"
#include "boolean.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "BasicAnalysis.h"
#include "NameLib.h"
#include "SimpleGUI.h"

#include <Windows.h>
#include <WinUser.h>


static bool pointMode = 0;	// �������ӵ�ģʽ
static bool mouseDown = 0;	// ���������£����Ǽӵ�ģʽ

void Display()
{
	DisplayClear();
	RA_createAxis();
	BG_repaint();
}

void MouseEventProcess(int x, int y, int key, int event)
{
	static double lastDx = -1;	// ��¼��һ������¼���λ��
	static double lastDy = -1;  // ͬ��
	double dx = ScaleXInches(x);
	double dy = ScaleYInches(y);

	switch (event)
	{
	case BUTTON_DOWN:
		if (key == LEFT_BUTTON && pointMode)
			BG_addPoint(BG_inchToAxisX(dx), BG_inchToAxisY(dy));
		else if (key == LEFT_BUTTON) mouseDown = 1;
		
		if (key == RIGHT_BUTTON)
		{
			pointMode ^= 1;
			string name = BG_getGraphicName(dx, dy);
			if (name != "")
			{
				BG_deleteGraphic(name);
				printf("%lf %lf %s\n", dx, dy, name);
				Display();
			}
		}

		break;

	case BUTTON_UP:
		mouseDown = 0;
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
		if (mouseDown)
		{
			lastDx < 0 ? RA_move(0, 0) :
				RA_move(dx - lastDx, dy - lastDy);
			Display();
		}
		else
		{
		//	printf("%s\n", BG_getGraphic(dx, dy));	// �����Ƿ����get����긽����ͼ��
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

	//-----------------------һЩ����----------------------
	RA_createAxis();
	
	GUI_addButton(2, 2, 1, 1, "../resource/Cbitmap/arrow.bmp");
	GUI_addButton(4, 2, 1, 1, "../resource/Cbitmap/point.bmp");
	GUI_addButton(6, 2, 1, 1, "../resource/Cbitmap/line.bmp");

	BG_addPoint(BG_inchToAxisX(2), BG_inchToAxisY(3));	// �Ӹ�ʵ������Ϊ(2, 3)�ĵ�
	BG_addPoint(1, 1);  // ��һ������Ϊ(1, 1)�ĵ�

	pointMode = 0;


	BG_addLine(2, 2, 3, 4, 0);	 // ֱ��
	BG_addLine(2, 2, -1, 5, 1);   // ����
	BG_addLine(-1, 2, 0, 2, 2);  // �߶�
	BG_addPoint(2, 2);

	BG_addArc(1, 1, 2, 30, 330);

	BG_addVector(0, 0, 1, -2);
	BG_addVector(0, 0, -3, -3);
	BG_addVector(0, 0, -3, 0);
	BG_addVector(0, 0, 0, -3);

	printf("%lf\n", BA_disPointLine(BG_getGraphic("A"), BG_getGraphic("b")));  // A�㵽b�ߵľ���

}