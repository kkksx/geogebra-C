/*
* 文件：NameLib.c
* 
* 分配名字，如果名字出现次数过多则在string后面添加数字进行区分
*/

#include "NameLib.h"
#include "boolean.h"

#include <string.h>


#define MAX_GREEK 4		// 希腊字母数

static string  s_greekLetter[] = { "α", "β", "γ", "δ" };  // 希腊字母表

static bool    s_greekLetterMap[1000];	    // 希腊字母使用情况
static bool    s_lowerCaseMap[1000];		// 小写字母使用情况
static bool    s_upperCaseMap[1000];		// 大写字母使用情况

static int s_getID(bool* a);						// 取得一个还没使用过的ID
static bool s_deleteID(bool* a, int id);			// 回收一个ID



//----------------------NameLib.h接口实现--------------------------


string NL_getLowerCase()
{
	string name = malloc(sizeof(char) * 5);
	int id = s_getID(s_lowerCaseMap);
	if (id >= 26) sprintf(name, "%c%d", 'a' + id % 26, id / 26);
	else sprintf(name, "%c", 'a' + id % 26);
	return name;
}

string NL_getUpperCase()
{
	string name = malloc(sizeof(char) * 5);
	int id = s_getID(s_upperCaseMap);
	if (id >= 26) sprintf(name, "%c%d", 'A' + id % 26, id / 26);
	else sprintf(name, "%c", 'A' + id % 26);
	return name;
}

string NL_getGreekLetter()
{
	string name = malloc(sizeof(char) * 5);
	int id = s_getID(s_greekLetterMap);
	name = s_greekLetter[id % MAX_GREEK];
	if (id >= MAX_GREEK) name = Concat(name, IntegerToString(id / MAX_GREEK));
	return name;
}

bool NL_deleteName(string name, int type)
{
	if (strlen(name) < 1) return FALSE;

	switch (type)
	{
	case 0:	 // 小写字母
		if (strlen(name) == 1) s_deleteID(s_lowerCaseMap, name[0] - 'a');
		else s_deleteID(s_lowerCaseMap, atoi(name + 1) * 26 + name[0] - 'a');
		break;
	case 1:  // 大写字母
		if (strlen(name) == 1) s_deleteID(s_upperCaseMap, name[0] - 'A');
		else s_deleteID(s_upperCaseMap, atoi(name + 1) * 26 + name[0] - 'A');
		break;
	case 2:  // 希腊字母
		if (strlen(name) == strlen(s_greekLetter[0]))
			s_deleteID(s_greekLetterMap, name[1] - s_greekLetter[0][1]);
		else s_deleteID(s_greekLetterMap, 
			atoi(name + strlen(s_greekLetter[0])) * MAX_GREEK + name[1] - s_greekLetter[0][1]);
		break;
	}
	return TRUE;
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










