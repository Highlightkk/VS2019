/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        ResGUISys.h
*  @author      pedria
*  @date        2022-11-12
*  @brief       QT��λ�����ͷ�ļ�
*  @namespace   ??
*  @note        
*  @version     -v  2022-11-12
*/
#ifndef _RESGUISYS_H
#define _RESGUISYS_H

#include <QtWidgets/QMainWindow>
#include <QtGlobal>
#include <QDateTime>
#include <QString>
#include <QDebug>
#include <QScreen>
#include <QThread>
#include "ui_ResGUISys.h"
#include "WinTimer.h"
#include "ResWave.h"
/**
* @class    ResGUISys
* @brief    APP GUI��
* @note
* @author    pedria
*/
class ResGUISys : public QMainWindow
{
    Q_OBJECT

public:
    ResGUISys(QWidget* parent = nullptr);
    ~ResGUISys();

private:
    Ui::ResGUISysClass ui;
    /***************��Ա����***************/
    QScreen* screen = Q_NULLPTR; // ��Ļ����
    qint16 appWidth;  // Ӧ�ó�����
    qint16 appHeight; // Ӧ�ó���߶�
    qreal screenFactor;  // ��Ļ��������
    WinTimer* timerPlot = Q_NULLPTR; // ��ͼ��ʱ������
    ResWave* resWave = Q_NULLPTR; // �������ζ���
    QDateTime* currentTime = Q_NULLPTR;
    qint16 resMode; // ����ģʽ
    /***************��Ա����***************/
    void GUISetup(); // �ؼ����֣���ʽ��ʼ������
    void LoadCSS(const QString& sheetName); // ��ȡ.CSS��ʽ����
    void LayoutSetup(); // �ؼ����ֳ�ʼ������
    void GraphSetup(); // ��ͼ��ʼ������
private slots:
    /***************�ۺ���***************/
    void TimeoutHandler(); // ��ͼ��ʱ����Ӧ����
    void ResWaveGenHandler(); // ��������ģ����Ӧ����
    void DisplayDataPoints(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event); // �������ݵ���ʾ����b
    void ResParamsInit(qint16 mode); // ����������ؿؼ�״ֵ̬��ʼ������
};
#endif
