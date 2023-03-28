/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        WinTimer.h
*  @author      pedria
*  @date        2022-11-13
*  @brief       https://blog.csdn.net/iliukunpeng/article/details/79211201
*  @namespace    
*  @note        �߾��ȶ�ý�嶨ʱ����
*  @version     -v  2022-11-13
*/
#ifndef _WINTIMER_H
#define _WINTIMER_H
#include <QObject>
#include <windows.h>
/**
* @class    WinTimer
* @brief	windows�߾��ȶ�ý���� 
* @note
* @author    pedria
*/
class WinTimer : public QObject
{
	Q_OBJECT
public:
	explicit WinTimer(QObject* parent = 0);
	~WinTimer();
	int m_cnt; // ��ʱ������
signals:
	void timeout();
public slots:
	void start(int timeInterval);
	void stop();
	friend void WINAPI CALLBACK PeriodCycle(uint, uint, DWORD_PTR, DWORD_PTR, DWORD_PTR);
private:
	int m_interval; // ��ʱ�����
	int m_id; // ��ʱ��id
};
#endif
