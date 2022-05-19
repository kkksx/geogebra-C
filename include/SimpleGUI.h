#ifndef _SIMPLE_GUI_H
#define _SIMPLE_GUI_H

/*
* 
* 功能：资源加载
*	由graphics.c实现
*
*/

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


/*
* GUI_addBitmap
* 功能：添加一张bmp位图，参数同GUI_addButton
*/
void GUI_addBitmap(double x, double y, double w, double h, char* file);


/*
* 函数：GUI_update
* 功能：更新窗口，在display里面最后调用
*/
inline void GUI_update();


#endif