#ifndef _REFERENCEAXIS_H
#define _REFERENCEAXIS_H

/*
* 文件：ReferenceAxis.h
* 作者：kkksx
* 
* 功能：创建坐标轴
*/



/*
* 接口：RA_createAxis
* 功能：按照坐标轴属性，画出坐标轴
*/
void RA_createAxis();

/*
* 接口：RA_move
* 功能：按住鼠标移动时，改变焦点（窗口中心点）
* 输入：鼠标位移（单位：英寸）
*/
void RA_move(double x, double y);

/*
* 接口：RA_roll
* 功能：使用鼠标滚轮时，缩放窗口（注：由于水平有限，暂时仅能以窗口中心为中心点缩放）
* 输入：鼠标上下（0-下，1-上）
*/
void RA_roll(int up);



#endif