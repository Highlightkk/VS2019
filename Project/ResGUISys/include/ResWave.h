/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        ResWave.h
*  @author      pedria
*  @date        2022-11-12
*  @brief       ����ѵ�����β�����ͷ�ļ�
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
* @brief    �������γ�����
* @note
* @author    pedria
*/
class ResWave: public QObject
{
	Q_OBJECT // �̳�QObject��ʹ���źŲۻ���
public:// �������β�����Ա����
	qreal resFreq;			// ����Ƶ��(��0.1�仯����Χ[0.1��1.5])
	qreal resDepth;			// �������(����)
	qreal resSam;			// �������в������
	qreal resLen;			// �������г��� 
	qreal IERatio;			// ������(���������̶�Ϊ1������������0.5�仯����1:1.5��1:0.5����Χ[0.5��3]) 
	qreal holdRatio;		// ��Ϣʱ���(��Ϣ�����̶�Ϊ1������ʱ�������0.25����Χ[0��3])
	qint16 nSamples;		// �����������ݵ���
	QVector<qreal> tSeq;    // ʱ������
	QVector<qreal> resSeq;  // ��������
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
	virtual ~ResWave(); // ����������
public:
	virtual bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold) = 0; // �ӿں���(���麯��)
private:
	virtual bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold) = 0; // �����������ɺ���(���麯��)
};
/**
* @class    AbdominalRespiration
* @brief    ��ʽ��������
* @note
* @author    pedria
*/
class AbdominalRespiration: public ResWave { 
public:
	AbdominalRespiration();
	~AbdominalRespiration();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // �ӿں���
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // ��ʽ�����������ɺ���
};
/**
* @class    BoxBreathing 
* @brief    ��ʽ�������� 
* @note
* @author    pedria
*/
class BoxBreathing: public ResWave { 
public:
	BoxBreathing();
	~BoxBreathing();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // �ӿں���
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // ��ʽ�����������ɺ���
};
/**
* @class   ButeykoBreathing
* @brief   Buteyko�������� 
* @note
* @author    pedria
*/
class ButeykoBreathing: public ResWave { 
public:
	ButeykoBreathing();
	~ButeykoBreathing();
	bool CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // �ӿں���
private:
	bool ResWaveGen(qreal f, qreal A, qreal IER, qreal hold); // ��ʽ�����������ɺ���
};
#endif
