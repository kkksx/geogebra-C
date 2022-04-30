#ifndef _SIMPLE_GUI_H
#define _SIMPLE_GUI_H

/*
* 功能：资源加载
*/

void GUI_addBitmap();

/*
* 接口：GUI_addButton
* 功能：向窗口加一个按钮
* 输入参数：
*	(x,y)  左下角
*	w	   宽
*	h      高
*	file   图片位置
*/
void GUI_addButton(double x, double y, double w, double h, char* file);




#endif