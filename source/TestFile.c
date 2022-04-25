#include "graphics.h"
#include "extgraph.h"

#include "BasicGraphics.h"
#include "NameLib.h"

void Main()
{
	SetWindowTitle("ţ");
	SetWindowSize(20, 10);
	InitGraphics();

	SetPenSize(3);

	MovePen(10, 5);
	DrawEllipticalArc(2, 1.5, 0, 180);
	

}