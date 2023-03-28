#include "ResWave.h"

/**
* ResWave��
* @func          ResWave
* ���캯��
* @type          [void]
*/
ResWave::ResWave(){
	qDebug() << "ResWave Class Was Constructed." << endl;
	ResWave::resSam = 0.05; // �������-50ms
	ResWave::resLen = 60; // �������г���-60s
	ResWave::resFreq = 0.3; // ����Ƶ��
	ResWave::resDepth = 10; // �������(����)
	ResWave::IERatio = 1; // ������(���������̶�Ϊ1������������0.5�仯����1:1.5��1:0.5) 
	ResWave::holdRatio = 0; // ��Ϣʱ���(��Ϣ�����̶�Ϊ1������ʱ�������0.25����Χ[0��3])
	ResWave::nSamples = ResWave::resLen / ResWave::resSam; // ������-1200
	ResWave::tSeq.resize(ResWave::nSamples); // ʱ������
	ResWave::resSeq.resize(ResWave::nSamples); // ������������
}
/**
* ResWave��
* @func          ~ResWave
* ��������
* @type          [void]
*/
ResWave::~ResWave()
{
	qDebug() << "ResWave Class Was Deleted." << endl;
}
/**
* AbdominalRespiration��
* @func          AbdominalRespiration
* ���캯�� 
* @type          [void]
*/
AbdominalRespiration::AbdominalRespiration()
{
	qDebug() << "AbdmoinalRespiration Class Was Constructed." << endl;
	AbdominalRespiration::IERatio = 2; // ������Ĭ��ֵ-2
	AbdominalRespiration::holdRatio = 0; // ��Ϣ��Ĭ��ֵ-0
}
/**
* AbdmoinalRespiration��
* @func          ~AbdominalRespiration
* ��������
* @type          [void]
*/
AbdominalRespiration::~AbdominalRespiration()
{
	qDebug() << "AbdominalRespiration Class Was Deleted." << endl;
}
/**
* AbdominalRespiration��
* @func         CallResWaveGen
* �ۺ���-����˽�еĲ���ģ�����ɺ���
* @param        resFeq
* @param        resDepth
* @param        IERatio 
* @return		�������óɹ����
* @type          [bool]
*/
bool AbdominalRespiration::CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold) 
{
	if ((f <= 0) || (A <= 0) || (IER <= 0) || (hold < 0)) {
		qDebug() << "�����������." << endl;
		return false;
	}
	else {
		AbdominalRespiration::resFreq = f;
		AbdominalRespiration::resDepth = A;
		AbdominalRespiration::IERatio = IER;
		AbdominalRespiration::holdRatio = hold;
		return AbdominalRespiration::ResWaveGen(f, A, IER, hold);
	}
}
/**
* AbdominalRespiration��
* @func          ResWaveGen
* �������ɺ���
* @return		 �����������ɳɹ����	
* @type          [bool]
*/
bool AbdominalRespiration::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("��ʽ�����������ɺ�������.");
	qreal fIn = (f * (1 + IER)) / 2;				   // ����Ƶ��
	qreal fOut = (f * (1 + IER)) / (2*IER);			   // ����Ƶ��
	qreal TIn = 1 / fIn;							   // ��������
	qreal TOut = 1 / fOut;							   // ��������
	qreal Ts = 0.001;								   // ����������Ϊ1ms������"ģ���ź�"�����²�����20Hz
	qint32 nChirp = AbdominalRespiration::resLen / Ts; // �������� 
	qreal T = 1 / f;								   // Ԥ������
	qint16 n = AbdominalRespiration::resLen * f;	   // ����������
	qint16 lenT = T / Ts;							   // �������ڳ���
	qint16 lenT_in = lenT / (1 + IER); // ��������

	if (!GSL_IS_EVEN(lenT_in)) { // ȷ����������Ϊż��
		if (IER >= 1) { // ���ݲ�ͬ�����ȸı���������
			lenT_in -= 1; 
		}
		else {
			lenT_in += 1;
		}
	}
	qint32 i = 0;					  // �����±�
	qint16 lenT_out = lenT - lenT_in; // ��������(��֤������������Ϊ����������Щ��IER����)
	QVector<qreal> tIn(qint16(TIn/Ts)); // ����ʱ������ 
	std::generate(tIn.begin(), tIn.end(), [Ts, &i]() { // ����ʱ������
		return (i++) * Ts;
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> xIn(qint16(TIn/Ts));
	std::generate(xIn.begin(), xIn.end(), [=, &i]() { // ������������
		return A * gsl_sf_sin(2 * M_PI * fIn * tIn[i++]);
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> tOut(qint16(TOut/Ts)); // ����ʱ������ 
	std::generate(tOut.begin(), tOut.end(), [Ts, &i]() { // ����ʱ������
		return (i++) * Ts;
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> xOut(qint16(TOut/Ts));
	std::generate(xOut.begin(), xOut.end(), [=, &i]() { // ������������
		return A * gsl_sf_cos(2 * M_PI * fOut * tOut[i++]);
		});
	QVector<qreal>::const_iterator iter_xIn = xIn.begin(); // ������xIn
	QVector<qreal>::const_iterator iter_xOut = xOut.begin(); // ������xOut
	QVector<qreal> x1(iter_xIn, iter_xIn + lenT_in/2); // ƴ�Ӳ���
	QVector<qreal> x2(iter_xOut, iter_xOut + lenT_out); 
	QVector<qreal> x3(x1.size());
	QVector<qreal>::const_iterator iter_x1 = x1.begin(); // ������x1
	QVector<qreal>::const_iterator iter_x2 = x2.begin(); // ������x2
	QVector<qreal>::iterator iter_x3 = x3.begin();		 // ������x3

	std::reverse_copy(x1.begin(), x1.end(), x3.begin()); // ����
	for (; iter_x3 != x3.end(); iter_x3++) {
		*iter_x3 = -(*iter_x3); // ȡ��
	}
	QVector<qreal> xT(x1); // ������������
	xT.append(x2);
	xT.append(x3);
	QVector<qreal> xBreathe(xT); // ��������
	for (int j = 0; j < n-1; j++) { // ����n����������
		xBreathe.append(xT);
	}
	qint16 resLen = nChirp - n * lenT; // ����ʣ�೤��
	if (resLen > 0) {
		QVector<qreal> x4;
		if (resLen <= lenT_in / 2) { // һ�������ʣ�೤�Ȳ��ᳬ����������1�ĳ���
			QVector<qreal> tmp(iter_x1, iter_x1 + resLen);
			x4.append(tmp);
		}
		else if (resLen <= (lenT_in / 2 + lenT_out)) {
			x4.append(x1);
			QVector<qreal> tmp(iter_x2, iter_x2 + resLen - lenT_in / 2);
			x4.append(tmp);
		}
		else {
			x4.append(x1);
			x4.append(x2);
			QVector<qreal> tmp(iter_x3, iter_x3 + resLen - lenT_in / 2 - lenT_out);
			x4.append(tmp);
		}
		xBreathe.append(x4); // ƴ��ʣ�೤�ȣ���ʱ�������
	}
	qDebug() << "xBreathe Size is:" << xBreathe.size();
	i = 0;
	std::generate(AbdominalRespiration::tSeq.begin(), AbdominalRespiration::tSeq.end(), [this, &i]() { // lambda���ʽ����ʱ������ 
		return (i++) * AbdominalRespiration::resSam;
		});
	qint16 M = AbdominalRespiration::resSam / Ts; // �����±��0��ʼ
	i = -M;
	std::generate(AbdominalRespiration::resSeq.begin(), AbdominalRespiration::resSeq.end(), [this, M, xBreathe, &i]() { // lambda���ʽ���ɲ������� 
		i += M;
		return xBreathe.at(i); // �²���
		});
	if (AbdominalRespiration::resSeq.size() == 0) { // ����������ʧ�ܣ�����false
		return false;
	}
	return true;
}
/**
* BoxBreathing��
* @func          BoxBreathing
* 
* @type          [void]
*/
BoxBreathing::BoxBreathing()
{
	qDebug() << "Boxbreathing Was Constructed.";
}
/**
* BoxBreathing��
* @func          ~BoxBreathing
* 
* @type          [void]
*/
BoxBreathing::~BoxBreathing()
{
	qDebug() << "BoxBreathing Was Deleted.";
}
/**
* BoxBreathing��
* @func          CallResWaveGen
* 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return       �������ɳɹ����
* @type          [bool]
*/
bool BoxBreathing::CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	if ((f <= 0) || (A <= 0) || (IER <= 0) || (hold < 0)) {
		qDebug() << "�����������." << endl;
		return false;
	}
	else {
		BoxBreathing::resFreq = f;
		BoxBreathing::resDepth = A;
		BoxBreathing::IERatio = IER;
		BoxBreathing::holdRatio = hold;
		return BoxBreathing::ResWaveGen(f, A, IER, hold);
	}
}
/**
* BoxBreathing��
* @func          ResWaveGen
* 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return       �������ɳɹ����
* @type          [bool]
*/
bool BoxBreathing::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("��ʽ�����������ɺ�������.");
	qreal fBreathe = 2 * f;					   // ����Ƶ��
	qreal fHold = fBreathe;					   // ��ϢƵ��	
	qreal TBreathe = 1 / fBreathe;			   // ��������
	qreal THold = 1 / fHold;			       // ��Ϣ����
	qreal Ts = 0.001;						   // ����������Ϊ1ms������"ģ���ź�"�����²�����20Hz
	qint32 nChirp = BoxBreathing::resLen / Ts; // �������� 
	qreal T = 1 / f;						   // Ԥ������
	qint16 n = BoxBreathing::resLen * f;	   // ����������
	/*****************************�ֶγ���ȷ��*********************************/
	qint16 lenT = T / Ts;			    	   // �������ڳ���
	qint16 lenT_Breathe = lenT / (1 + hold);   // ��������
	qint16 lenT_Hold = lenT - lenT_Breathe;    // ��Ϣ����
	qint16 lenT_in = lenT_Breathe / (1 + IER); // ��������

	if (!GSL_IS_EVEN(lenT_in)) { // ȷ����������Ϊż��
		if (IER >= 1) { // ���ݲ�ͬ�����ȸı���������
			lenT_in -= 1; 
		}
		else {
			lenT_in += 1;
		}
	}
	qint16 lenT_out = lenT_Breathe - lenT_in; // ��������(��֤������������Ϊ����������Щ��IER����)
	qint16 lenT_Hold1 = lenT_Hold / (1 + IER); // ��Ϣ����1
	if (!GSL_IS_EVEN(lenT_Hold1)) { // ȷ����������Ϊż��
		if (IER >= 1) { // ���ݲ�ͬ�����ȸı���������
			lenT_Hold1 += 1; 
		}
		else {
			lenT_Hold1 -= 1;
		}
	}
	qint16 lenT_Hold2 = lenT_Hold - lenT_Hold1; // ��Ϣ����2
	/*****************************������������*********************************/
	qint32 i = 0;					  // �����±�
	QVector<qreal> tBreathe(qint16(TBreathe/Ts)); // ����ʱ������ 
	std::generate(tBreathe.begin(), tBreathe.end(), [Ts, &i]() { // ����ʱ������
		return (i++) * Ts;
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> xIn(qint16(TBreathe/Ts));
	std::generate(xIn.begin(), xIn.end(), [=, &i]() { // ������������
		return A * gsl_sf_sin(2 * M_PI * fBreathe * tBreathe[i++]);
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> xOut(qint16(TBreathe/Ts));
	std::generate(xOut.begin(), xOut.end(), [=, &i]() { // ������������
		return A * gsl_sf_cos(2 * M_PI * fBreathe * tBreathe[i++]);
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> tHold(qint16(THold / Ts)); // ��Ϣʱ������
	std::generate(tHold.begin(), tHold.end(), [=, &i]() {
		return (i++) * Ts;
		});
	QVector<qreal> xHold(qint16(THold/Ts), A); // ������Ϣ����
	/*****************************�ֶβ���ƴ��***********************************/
	QVector<qreal>::const_iterator iter_xIn = xIn.begin(); // ������xIn
	QVector<qreal>::const_iterator iter_xOut = xOut.begin(); // ������xOut
	QVector<qreal>::const_iterator iter_xHold = xHold.begin(); // ������xHold
	QVector<qreal> x1(iter_xIn, iter_xIn + lenT_in/2); // ƴ�Ӳ���
	QVector<qreal> x2(iter_xHold, iter_xHold + lenT_Hold1); 
	QVector<qreal> x3(iter_xOut, iter_xOut + lenT_out);
	QVector<qreal> x4(iter_xHold, iter_xHold + lenT_Hold2);
	QVector<qreal> x5(x1);

	QVector<qreal>::const_iterator iter_x1 = x1.begin(); // ������x1
	QVector<qreal>::const_iterator iter_x2 = x2.begin(); // ������x2
	QVector<qreal>::const_iterator iter_x3 = x3.begin(); // ������x3
	QVector<qreal>::iterator iter_x4 = x4.begin(); // ������x4
	QVector<qreal>::iterator iter_x5 = x5.begin(); // ������x5

	for (; iter_x4 != x4.end(); iter_x4++) {
		*iter_x4 = -(*iter_x4); // ȡ��
	}
	std::reverse_copy(x1.begin(), x1.end(), x5.begin()); // ����
	for (; iter_x5 != x5.end(); iter_x5++) {
		*iter_x5 = -(*iter_x5); // ȡ��
	}
	QVector<qreal> xT(x1); // ������������
	xT.append(x2);
	xT.append(x3);
	xT.append(x4);
	xT.append(x5);
	QVector<qreal> xBreathe(xT); // ��������
	for (int j = 0; j < n-1; j++) { // ����n����������
		xBreathe.append(xT);
	}
	qint16 resLen = nChirp - n * lenT; // ����ʣ�೤��
	if (resLen > 0) {
		QVector<qreal> x6;
		if (resLen <= lenT_in / 2) { // һ�������ʣ�೤�Ȳ��ᳬ����������1�ĳ���
			QVector<qreal> tmp(iter_x1, iter_x1 + resLen);
			x6.append(tmp);
		}
		else if (resLen <= (lenT_in / 2 + lenT_Hold1)) {
			x6.append(x1);
			QVector<qreal> tmp(iter_x2, iter_x2 + resLen - lenT_in / 2);
			x6.append(tmp);
		}
		else if (resLen <= (lenT_in / 2 + lenT_Hold1 + lenT_out)) {
			x6.append(x1);
			x6.append(x2);
			QVector<qreal> tmp(iter_x3, iter_x3 + resLen - lenT_in / 2 - lenT_Hold1);
			x6.append(tmp);
		}
		else if (resLen <= (lenT_in / 2 + lenT_Hold1 + lenT_out + lenT_Hold2)) {
			x6.append(x1);
			x6.append(x2);
			x6.append(x3);
			QVector<qreal> tmp(iter_x4, iter_x4 + resLen - lenT_in/2 - lenT_Hold1 - lenT_out);
			x6.append(tmp);
		}
		else { 
			x6.append(x1);
			x6.append(x2);
			x6.append(x3);
			x6.append(x4);
			QVector<qreal> tmp(iter_x5, iter_x5 + resLen - lenT_in/2 - lenT_Hold1 - lenT_out - lenT_in / 2);
			x6.append(tmp);
		}
		xBreathe.append(x6); // ƴ��ʣ�೤�ȣ���ʱ�������
	}
	qDebug() << "xBreathe Size is:" << xBreathe.size();
	i = 0;
	std::generate(BoxBreathing::tSeq.begin(), BoxBreathing::tSeq.end(), [this, &i]() { // lambda���ʽ����ʱ������ 
		return (i++) * BoxBreathing::resSam;
		});
	qint16 M = BoxBreathing::resSam / Ts; // �����±��0��ʼ
	i = -M;
	std::generate(BoxBreathing::resSeq.begin(), BoxBreathing::resSeq.end(), [this, M, xBreathe, &i]() { // lambda���ʽ���ɲ������� 
		i += M;
		return xBreathe.at(i); // �²���
		});
	if (BoxBreathing::resSeq.size() == 0) { // ����������ʧ�ܣ�����false
		return false;
	}
	return true;
}
/**
* ButeykoBreathing��
* @func          ButeykoBreathing
* ���캯�� 
* @type          [void]
*/
ButeykoBreathing::ButeykoBreathing()
{
	qDebug() << "ButeykoBreathing Class Was Constructed.";
}
/**
* ButeykoBreathing��
* @func          ~ButeykoBreathing
* �������� 
* @type          [void]
*/
ButeykoBreathing::~ButeykoBreathing()
{
	qDebug() << "ButeykoBreathing Class Was Deleted.";
}
/**
* ButeykoBreathing��
* @func          CallResWaveGen
* Burteyko�����������ɽӿں��� 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return   
* @type          [bool]
*/
bool ButeykoBreathing::CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	if ((f <= 0) || (A <= 0) || (IER <= 0) || (hold < 0)) {
		qDebug() << "�����������." << endl;
		return false;
	}
	else {
		ButeykoBreathing::resFreq = f;
		ButeykoBreathing::resDepth = A;
		ButeykoBreathing::IERatio = IER;
		ButeykoBreathing::holdRatio = hold;
		return ButeykoBreathing::ResWaveGen(f, A, IER, hold);
	}
}
/**
* ButeykoBreathing��
* @func          ResWaveGen
* Burteyko�������ɺ��� 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return   
* @type          [bool]
*/
bool ButeykoBreathing::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("Burteyko�����������ɺ�������.");
	qreal Ts = 0.001;						   // ����������Ϊ1ms������"ģ���ź�"�����²�����20Hz
	/*****************************�ֶγ���ȷ��*********************************/
	qint16 nT = ButeykoBreathing::resLen;
	qint32 lenT = nT / Ts;			    	   // �������ڳ���
	qint32 lenT_Breathe = lenT / (1 + hold);   // �ܺ�������

	if ((lenT_Breathe % 3) != 0) { // ȷ�����������ܱ�3����
		lenT_Breathe -= (lenT_Breathe % 3);
	}
	qint32 lenT_Hold = lenT - lenT_Breathe; // ��Ϣ����
	qint32 lenT_Breathe1 = lenT_Breathe / 3; // ���κ�������
	qreal T_Breathe1 = Ts * lenT_Breathe1;
	qreal f_Breathe = 1 / T_Breathe1; // ����Ƶ��
	/*****************************������������*********************************/
	qint32 i = 0;					  // �����±�
	QVector<qreal> tBreathe(lenT_Breathe1); // ����ʱ������ 
	std::generate(tBreathe.begin(), tBreathe.end(), [Ts, &i]() { // ����ʱ������
		return (i++) * Ts;
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> xBreathe(lenT_Breathe1);
	std::generate(xBreathe.begin(), xBreathe.end(), [=, &i]() { // ������������
		return A * gsl_sf_sin(2 * M_PI * f_Breathe * tBreathe[i++]);
		});
	i = 0; // ÿ�ξ���Ҫ����i
	QVector<qreal> tHold(lenT_Hold); // ��Ϣʱ������
	std::generate(tHold.begin(), tHold.end(), [=, &i]() {
		return (i++) * Ts;
		});
	QVector<qreal> xHold(lenT_Hold, 0.0); // ������Ϣ����
	/*****************************�ֶβ���ƴ��***********************************/
	QVector<qreal>::const_iterator iter_xBreathe = xBreathe.begin(); // ������xBreathe
	QVector<qreal>::const_iterator iter_xHold = xHold.begin(); // ������xHold
	QVector<qreal> x1(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // ƴ�Ӳ���
	QVector<qreal> x2(iter_xHold, iter_xHold + lenT_Hold); 
	QVector<qreal> x3(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // ƴ�Ӳ���
	QVector<qreal> x4(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // ƴ�Ӳ���

	QVector<qreal> xT(x1); // ������������
	xT.append(x2);
	xT.append(x3);
	xT.append(x4);
	qDebug() << "xT Size is:" << xT.size();
	i = 0;
	std::generate(ButeykoBreathing::tSeq.begin(), ButeykoBreathing::tSeq.end(), [this, &i]() { // lambda���ʽ����ʱ������ 
		return (i++) * ButeykoBreathing::resSam;
		});
	qint16 M = ButeykoBreathing::resSam / Ts; // �����±��0��ʼ
	i = -M;
	std::generate(ButeykoBreathing::resSeq.begin(), ButeykoBreathing::resSeq.end(), [this, M, xT, &i]() { // lambda���ʽ���ɲ������� 
		i += M;
		return xT.at(i); // �²���
		});
	if (ButeykoBreathing::resSeq.size() == 0) { // ����������ʧ�ܣ�����false
		return false;
	}
	return true;
}
