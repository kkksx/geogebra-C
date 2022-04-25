/*
* 文件：BasicGraphics.c
* 
* 实现了
*	BasicGraphics.h
*	ReferenceAxis.h
* 中定义的接口
* 
* 提供绘图功能
*/

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"

#include "BasicGraphics.h"
#include "ReferenceAxis.h"


// 坐标轴的属性
static double s_axisSize = 5;      // 坐标轴的量度
static double s_axisCalibration;   // 坐标轴的刻度，该值应由s_axisSize计算出





