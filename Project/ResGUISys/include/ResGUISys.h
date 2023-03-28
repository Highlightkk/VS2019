/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        ResGUISys.h
*  @author      pedria
*  @date        2022-11-12
*  @brief       QT上位机框架头文件
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
* @brief    APP GUI类
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
    /***************成员变量***************/
    QScreen* screen = Q_NULLPTR; // 屏幕对象
    qint16 appWidth;  // 应用程序宽度
    qint16 appHeight; // 应用程序高度
    qreal screenFactor;  // 屏幕缩放因子
    WinTimer* timerPlot = Q_NULLPTR; // 绘图定时器对象
    ResWave* resWave = Q_NULLPTR; // 呼吸波形对象
    QDateTime* currentTime = Q_NULLPTR;
    qint16 resMode; // 呼吸模式
    /***************成员函数***************/
    void GUISetup(); // 控件布局，样式初始化函数
    void LoadCSS(const QString& sheetName); // 读取.CSS样式函数
    void LayoutSetup(); // 控件布局初始化函数
    void GraphSetup(); // 绘图初始化函数
private slots:
    /***************槽函数***************/
    void TimeoutHandler(); // 绘图定时器响应函数
    void ResWaveGenHandler(); // 呼吸波形模板相应函数
    void DisplayDataPoints(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event); // 曲线数据点显示函数b
    void ResParamsInit(qint16 mode); // 呼吸参数相关控件状态值初始化函数
};
#endif
