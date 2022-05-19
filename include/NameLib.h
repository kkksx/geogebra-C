#ifndef _NAMELIB_H
#define _NAMELIB_H

/*
* �ļ���NameLib.h
* 
* ���ܣ�Ϊ����ͼ�νṹ������ʵ�����
*/

#include "genlib.h"
#include "strlib.h"
#include "boolean.h"

/*
* �ӿڣ�NL_getLowerCase
* ���ܣ�ȡ��Сд��ĸ��
*/
string NL_getLowerCase();


/*
* �ӿڣ�NL_getUpperCase
* ���ܣ�ȡ�ô�д��ĸ��
*/
string NL_getUpperCase();


/*
* �ӿڣ�NL_getGreekLetter
* ���ܣ�ȡ��ϣ����ĸ��
*/
string NL_getGreekLetter();


/*
* �ӿڣ�NL_deleteName
* ���ܣ�����һ������
*		0:Сд��1:��д��2:ϣ����ĸ
*		�� NL_deleteName("��1", 2) ����ϣ���ַ�����1������һ��ʹ��
*/
bool NL_deleteName(string name, int type);


#endif