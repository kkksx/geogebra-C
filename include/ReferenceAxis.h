#ifndef _REFERENCEAXIS_H
#define _REFERENCEAXIS_H

/*
* �ļ���ReferenceAxis.h
* ���ߣ�kkksx
* 
* ���ܣ�����������
*/



/*
* �ӿڣ�RA_createAxis
* ���ܣ��������������ԣ�����������
*/
void RA_createAxis();

/*
* �ӿڣ�RA_move
* ���ܣ���ס����ƶ�ʱ���ı佹�㣨�������ĵ㣩
* ���룺���λ�ƣ���λ��Ӣ�磩
*/
void RA_move(double x, double y);

/*
* �ӿڣ�RA_roll
* ���ܣ�ʹ��������ʱ�����Ŵ��ڣ�ע������ˮƽ���ޣ���ʱ�����Դ�������Ϊ���ĵ����ţ�
* ���룺������£�0-�£�1-�ϣ�
*/
void RA_roll(int up);



#endif