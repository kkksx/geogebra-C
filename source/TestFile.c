#include "graphics.h"
#include "extgraph.h"
#include "boolean.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"
#include "NameLib.h"
#include "SimpleGUI.h"

#include <Windows.h>
#include <WinUser.h>
#include <sysinfoapi.h>

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
		
		if (key == RIGHT_BUTTON) pointMode ^= 1;

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

	registerMouseEvent(MouseEventProcess);

	
	
	//-----------------------һЩ����----------------------
	RA_createAxis();
	
	GUI_addButton(2, 2, 1, 1, "../resource/Cbitmap/arrow.bmp");
	GUI_addButton(4, 2, 1, 1, "../resource/Cbitmap/point.bmp");
	GUI_addButton(6, 2, 1, 1, "../resource/Cbitmap/line.bmp");

	SetPenSize(3);
	MovePen(2, 5);
	DrawLine(1, 1);

	BG_addPoint(BG_inchToAxisX(2), BG_inchToAxisY(3));	// �Ӹ�ʵ������Ϊ(2, 3)�ĵ�
	BG_addPoint(1, 1);  // ��һ������Ϊ(1, 1)�ĵ�

	pointMode = 1;	// �Ҽ����ش�ģʽ


	BG_addLine(2, 2, 3, 4, 0);	 // ֱ��
	BG_addLine(2, 2, -1, 5, 1);   // ����
	BG_addLine(-1, 0, 0, 2, 2);  // �߶�
	BG_addPoint(2, 2);

}