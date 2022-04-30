/*
* �ļ���NameLib.c
* 
* �������֣�������ֳ��ִ�����������string����������ֽ�������
*/

#include "NameLib.h"
#include "boolean.h"

#define MAX_GREEK 6		// ϣ����ĸ��

static string  s_greekLetter[] = { "��", "��", "��", "��", "��", "��" };  // ϣ����ĸ��

static bool    s_greekLetterMap[1000];	    // ϣ����ĸʹ�����
static bool    s_lowerCaseMap[1000];		// Сд��ĸʹ�����
static bool    s_upperCaseMap[1000];		// ��д��ĸʹ�����

static int s_getID(bool* a);						// ȡ��һ����ûʹ�ù���ID
static bool s_deleteID(bool* a, int id);			// ����һ��ID



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
	if (id >= MAX_GREEK) name = Concat(name, IntegerToString(id / 6));
	return name;
}



//---------------------��̬��������-----------------------


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










