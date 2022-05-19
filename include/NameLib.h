#ifndef _NAMELIB_H
#define _NAMELIB_H

/*
* 文件：NameLib.h
* 
* 功能：为各个图形结构分配合适的名字
*/

#include "genlib.h"
#include "strlib.h"
#include "boolean.h"

/*
* 接口：NL_getLowerCase
* 功能：取得小写字母名
*/
string NL_getLowerCase();


/*
* 接口：NL_getUpperCase
* 功能：取得大写字母名
*/
string NL_getUpperCase();


/*
* 接口：NL_getGreekLetter
* 功能：取得希腊字母名
*/
string NL_getGreekLetter();


/*
* 接口：NL_deleteName
* 功能：回收一个名字
*		0:小写，1:大写，2:希腊字母
*		如 NL_deleteName("β1", 2) 回收希腊字符串β1用于下一次使用
*/
bool NL_deleteName(string name, int type);


#endif