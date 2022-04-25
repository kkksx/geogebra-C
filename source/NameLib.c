/*
* 文件：NameLib.c
* 
* 分配名字，如果名字出现次数过多则在string后面添加数字进行区分
*/

#include "NameLib.h"
#include "boolean.h"

#define MAX_GREEK 6		// 希腊字母数

static string  s_greekLetter[] = { "α", "β", "γ", "δ", "θ", "ε" };  // 希腊字母表

static bool    s_greekLetterMap[1000];	// 希腊字母使用情况
static bool    s_lowerCaseMap[1000];		// 小写字母使用情况
static bool    s_upperCaseMap[1000];		// 大写字母使用情况

static int s_getID(bool* a);						// 取得一个还没使用过的ID
static bool s_deleteID(bool* a, int id);			// 回收一个ID


/*
* 函数：NL_getLowerCase()
* 作用：返回一个小写字母名，如a，d1
*/
string NL_getLowerCase()
{
	string name = "";
	int id = s_getID(s_lowerCaseMap);
	if (id >= 26) sprintf(name, "%c%d", 'a' + id % 26, id / 26);
	else sprintf(name, "%c", 'a' + id % 26);
	return name;
}

/*
* 函数：NL_getUpperCase()
* 作用：返回一个大写字母名，如B，C2
*/
string NL_getUpperCase()
{
	string name = "";
	int id = s_getID(s_upperCaseMap);
	if (id >= 26) sprintf(name, "%c%d", 'A' + id % 26, id / 26);
	else sprintf(name, "%c", 'A' + id % 26);
	return name;
}

/*
* 函数：NL_getGreekLetter()
* 作用：返回一个希腊字母名
*/
string NL_getGreekLetter()
{
	string name = "";
	int id = s_getID(s_greekLetterMap);
	name = s_greekLetter[id % MAX_GREEK];
	if (id >= MAX_GREEK) name = Concat(name, IntegerToString(id / 6));
	return name;
}



//---------------------静态函数部分-----------------------


static int s_getID(bool* a)
{
	int i;
	for (i = 0; i < 1000; ++i)
	{
		if (!a[i])
		{
			a[i] = 1;
			return i;
		}
	}
	return -1;  // ERROR
}

static bool s_deleteID(bool* a, int id)
{
	if (id < 0 || id > 999) return FALSE;
	a[id] = 0;
	return TRUE;
}










