#ifndef _NAMELIB_H
#define _NAMELIB_H

/*
* 文件：NameLib.h
* 作者：kkksx
* 功能：为各个图形结构分配合适的名字
*/

#include "genlib.h"
#include "strlib.h"


/*
* 接口：NL_getLowerCase()
* 功能：取得小写字母名
*/
string NL_getLowerCase();


/*
* 接口：NL_getUpperCase()
* 功能：取得大写字母名
*/
string NL_getUpperCase();


/*
* 接口：NL_getGreekLetter()
* 功能：取得希腊字母名
*/
string NL_getGreekLetter();




#endif