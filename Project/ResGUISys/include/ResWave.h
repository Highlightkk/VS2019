/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        ResWave.h
*  @author      pedria
*  @date        2022-11-12
*  @brief       呼吸训练波形参数类头文件
*  @namespace   std
*  @note        
*  @version     -v  2022-11-12
*/
#ifndef _RESWAVE_H
#define _RESWAVE_H
#include <QtGlobal>
#include <QVector>
#include <QObject>
#include <QDebug>
#include <algorithm>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_trig.h>
/**
* @class    ResWave
* @brief    呼吸波形抽象类
* @note
* @author    pedria
*/
class ResWave: public QObject
{
	Q_OBJECT // 继承QObject以使用信号槽机制
public:// 呼吸波形参数成员变量
	qreal resFreq;			// 呼吸频率(±0.1变化，范围[0.1，1.5])
	qreal resDepth;			// 呼吸深度(幅度)
	qreal resSam;			// 呼吸序列采样间隔
	qreal resLen;			// 呼吸序列长度 
	qreal IERatio;			// 吸呼比(吸气基数固定为1，呼气基数±0.5变化，如1:1.5、1:0.5，范围[0.5，3]) 
	qreal holdRatio;		// 屏息时间比(屏息基数固定为1，呼吸时间基数±0.25，范围[0，3])
	qint16 nSamples;		// 呼吸序列数据点数
	QVector<qreal> tSeq;    // 时间序列
	QVector<qreal> resSeq;  // 呼吸序列
	enum {
		Abdominal,
		PursedLip,
		Box,
		Norstril,
		Paradoxical,
		Buteyko,
		Valsalva,
		ACBT
	};
	ResWave();
	virtual ~ResWave(); // 虚析构函数
public:
	virtual bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold) = 0; // 接口函数(纯虚函数)
private:
	virtual bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold) = 0; // 呼吸波形生成函数(纯虚函数)
};
/**
* @class    AbdominalRespiration
* @brief    腹式呼吸子类
* @note
* @author    pedria
*/
class AbdominalRespiration: public ResWave { 
public:
	AbdominalRespiration();
	~AbdominalRespiration();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 接口函数
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 腹式呼吸波形生成函数
};
/**
* @class    BoxBreathing 
* @brief    箱式呼吸子类 
* @note
* @author    pedria
*/
class BoxBreathing: public ResWave { 
public:
	BoxBreathing();
	~BoxBreathing();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 接口函数
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 箱式呼吸波形生成函数
};
/**
* @class   ButeykoBreathing
* @brief   Buteyko呼吸子类 
* @note
* @author    pedria
*/
class ButeykoBreathing: public ResWave { 
public:
	ButeykoBreathing();
	~ButeykoBreathing();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 接口函数
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // 箱式呼吸波形生成函数
};
#endif
