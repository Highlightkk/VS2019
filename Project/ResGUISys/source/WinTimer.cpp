#include "WinTimer.h"
#define TIME_KILL_SYNCHRONOUS 0x0100
/**
* @func          WinTimer
* ���캯�� 
* @param        parent
* @return   
* @type          [int]
*/
WinTimer::WinTimer(QObject* parent) : QObject(parent)
{
    m_id = 0;
    m_cnt = 0;
}
/**
* @func          ~WinTimer
* �������� 
* @type          [void]
*/
WinTimer::~WinTimer()
{
    stop();
}
/**
* @func          PeriodCycle
* ?
* @param        timerId
* @param        
* @param        user
* @param        
* @param        
* @type          [void]
*/
void  CALLBACK PeriodCycle(uint timerId, uint, DWORD_PTR user, DWORD_PTR, DWORD_PTR)
{
    WinTimer* t = reinterpret_cast<WinTimer*>(user);
    emit t->timeout(); // ������ʱ�ж�
}
/**
* @func          start
* ��ʱ���������� 
* @param        timeInterval
* @type          [void]
*/
void WinTimer::start(int timeInterval)
{
    m_cnt = 0;
    m_interval = timeInterval;
    m_id = timeSetEvent(timeInterval, 1, PeriodCycle, (DWORD_PTR)this,
                        TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
}
/**
* @func          stop
* ��ʱ��ֹͣ����
* @type          [void]
*/
void WinTimer::stop()
{
    if (m_id)
    {
        timeKillEvent(m_id);
        m_id = 0;
    }
}

