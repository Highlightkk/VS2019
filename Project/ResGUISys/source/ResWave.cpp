#include "ResWave.h"

/**
* ResWave类
* @func          ResWave
* 构造函数
* @type          [void]
*/
ResWave::ResWave(){
	qDebug() << "ResWave Class Was Constructed." << endl;
	ResWave::resSam = 0.05; // 采样间隔-50ms
	ResWave::resLen = 60; // 呼吸序列长度-60s
	ResWave::resFreq = 0.3; // 呼吸频率
	ResWave::resDepth = 10; // 呼吸深度(幅度)
	ResWave::IERatio = 1; // 吸呼比(吸气基数固定为1，呼气基数±0.5变化，如1:1.5、1:0.5) 
	ResWave::holdRatio = 0; // 屏息时间比(屏息基数固定为1，呼吸时间基数±0.25，范围[0，3])
	ResWave::nSamples = ResWave::resLen / ResWave::resSam; // 采样点-1200
	ResWave::tSeq.resize(ResWave::nSamples); // 时间序列
	ResWave::resSeq.resize(ResWave::nSamples); // 呼吸波形序列
}
/**
* ResWave类
* @func          ~ResWave
* 析构函数
* @type          [void]
*/
ResWave::~ResWave()
{
	qDebug() << "ResWave Class Was Deleted." << endl;
}
/**
* AbdominalRespiration类
* @func          AbdominalRespiration
* 构造函数 
* @type          [void]
*/
AbdominalRespiration::AbdominalRespiration()
{
	qDebug() << "AbdmoinalRespiration Class Was Constructed." << endl;
	AbdominalRespiration::IERatio = 2; // 吸呼比默认值-2
	AbdominalRespiration::holdRatio = 0; // 屏息比默认值-0
}
/**
* AbdmoinalRespiration类
* @func          ~AbdominalRespiration
* 析构函数
* @type          [void]
*/
AbdominalRespiration::~AbdominalRespiration()
{
	qDebug() << "AbdominalRespiration Class Was Deleted." << endl;
}
/**
* AbdominalRespiration类
* @func         CallResWaveGen
* 槽函数-调用私有的波形模板生成函数
* @param        resFeq
* @param        resDepth
* @param        IERatio 
* @return		函数调用成功与否
* @type          [bool]
*/
bool AbdominalRespiration::CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold) 
{
	if ((f <= 0) || (A <= 0) || (IER <= 0) || (hold < 0)) {
		qDebug() << "参数输入错误." << endl;
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
* AbdominalRespiration类
* @func          ResWaveGen
* 波形生成函数
* @return		 呼吸波形生成成功与否	
* @type          [bool]
*/
bool AbdominalRespiration::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("腹式呼吸波形生成函数调用.");
	qreal fIn = (f * (1 + IER)) / 2;				   // 吸气频率
	qreal fOut = (f * (1 + IER)) / (2*IER);			   // 呼气频率
	qreal TIn = 1 / fIn;							   // 吸气周期
	qreal TOut = 1 / fOut;							   // 呼气周期
	qreal Ts = 0.001;								   // 采样周期设为1ms，仿真"模拟信号"，再下采样至20Hz
	qint32 nChirp = AbdominalRespiration::resLen / Ts; // 采样点数 
	qreal T = 1 / f;								   // 预设周期
	qint16 n = AbdominalRespiration::resLen * f;	   // 完整周期数
	qint16 lenT = T / Ts;							   // 单个周期长度
	qint16 lenT_in = lenT / (1 + IER); // 吸气长度

	if (!GSL_IS_EVEN(lenT_in)) { // 确保吸气长度为偶数
		if (IER >= 1) { // 根据不同吸呼比改变吸气长度
			lenT_in -= 1; 
		}
		else {
			lenT_in += 1;
		}
	}
	qint32 i = 0;					  // 序列下标
	qint16 lenT_out = lenT - lenT_in; // 呼气长度(保证吸气呼气长度为整数，牺牲些许IER精度)
	QVector<qreal> tIn(qint16(TIn/Ts)); // 吸气时间序列 
	std::generate(tIn.begin(), tIn.end(), [Ts, &i]() { // 生成时间序列
		return (i++) * Ts;
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> xIn(qint16(TIn/Ts));
	std::generate(xIn.begin(), xIn.end(), [=, &i]() { // 构建吸气波形
		return A * gsl_sf_sin(2 * M_PI * fIn * tIn[i++]);
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> tOut(qint16(TOut/Ts)); // 呼气时间序列 
	std::generate(tOut.begin(), tOut.end(), [Ts, &i]() { // 生成时间序列
		return (i++) * Ts;
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> xOut(qint16(TOut/Ts));
	std::generate(xOut.begin(), xOut.end(), [=, &i]() { // 构建呼气波形
		return A * gsl_sf_cos(2 * M_PI * fOut * tOut[i++]);
		});
	QVector<qreal>::const_iterator iter_xIn = xIn.begin(); // 迭代器xIn
	QVector<qreal>::const_iterator iter_xOut = xOut.begin(); // 迭代器xOut
	QVector<qreal> x1(iter_xIn, iter_xIn + lenT_in/2); // 拼接波形
	QVector<qreal> x2(iter_xOut, iter_xOut + lenT_out); 
	QVector<qreal> x3(x1.size());
	QVector<qreal>::const_iterator iter_x1 = x1.begin(); // 迭代器x1
	QVector<qreal>::const_iterator iter_x2 = x2.begin(); // 迭代器x2
	QVector<qreal>::iterator iter_x3 = x3.begin();		 // 迭代器x3

	std::reverse_copy(x1.begin(), x1.end(), x3.begin()); // 逆序
	for (; iter_x3 != x3.end(); iter_x3++) {
		*iter_x3 = -(*iter_x3); // 取反
	}
	QVector<qreal> xT(x1); // 单个完整周期
	xT.append(x2);
	xT.append(x3);
	QVector<qreal> xBreathe(xT); // 呼吸序列
	for (int j = 0; j < n-1; j++) { // 连接n个完整周期
		xBreathe.append(xT);
	}
	qint16 resLen = nChirp - n * lenT; // 计算剩余长度
	if (resLen > 0) {
		QVector<qreal> x4;
		if (resLen <= lenT_in / 2) { // 一般情况下剩余长度不会超过吸气过程1的长度
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
		xBreathe.append(x4); // 拼接剩余长度，与时间轴对齐
	}
	qDebug() << "xBreathe Size is:" << xBreathe.size();
	i = 0;
	std::generate(AbdominalRespiration::tSeq.begin(), AbdominalRespiration::tSeq.end(), [this, &i]() { // lambda表达式生成时间序列 
		return (i++) * AbdominalRespiration::resSam;
		});
	qint16 M = AbdominalRespiration::resSam / Ts; // 数组下标从0开始
	i = -M;
	std::generate(AbdominalRespiration::resSeq.begin(), AbdominalRespiration::resSeq.end(), [this, M, xBreathe, &i]() { // lambda表达式生成波形序列 
		i += M;
		return xBreathe.at(i); // 下采样
		});
	if (AbdominalRespiration::resSeq.size() == 0) { // 若波形生成失败，返回false
		return false;
	}
	return true;
}
/**
* BoxBreathing类
* @func          BoxBreathing
* 
* @type          [void]
*/
BoxBreathing::BoxBreathing()
{
	qDebug() << "Boxbreathing Was Constructed.";
}
/**
* BoxBreathing类
* @func          ~BoxBreathing
* 
* @type          [void]
*/
BoxBreathing::~BoxBreathing()
{
	qDebug() << "BoxBreathing Was Deleted.";
}
/**
* BoxBreathing类
* @func          CallResWaveGen
* 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return       波形生成成功与否
* @type          [bool]
*/
bool BoxBreathing::CallResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	if ((f <= 0) || (A <= 0) || (IER <= 0) || (hold < 0)) {
		qDebug() << "参数输入错误." << endl;
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
* BoxBreathing类
* @func          ResWaveGen
* 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return       波形生成成功与否
* @type          [bool]
*/
bool BoxBreathing::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("箱式呼吸波形生成函数调用.");
	qreal fBreathe = 2 * f;					   // 呼吸频率
	qreal fHold = fBreathe;					   // 屏息频率	
	qreal TBreathe = 1 / fBreathe;			   // 呼吸周期
	qreal THold = 1 / fHold;			       // 屏息周期
	qreal Ts = 0.001;						   // 采样周期设为1ms，仿真"模拟信号"，再下采样至20Hz
	qint32 nChirp = BoxBreathing::resLen / Ts; // 采样点数 
	qreal T = 1 / f;						   // 预设周期
	qint16 n = BoxBreathing::resLen * f;	   // 完整周期数
	/*****************************分段长度确定*********************************/
	qint16 lenT = T / Ts;			    	   // 单个周期长度
	qint16 lenT_Breathe = lenT / (1 + hold);   // 呼吸长度
	qint16 lenT_Hold = lenT - lenT_Breathe;    // 屏息长度
	qint16 lenT_in = lenT_Breathe / (1 + IER); // 吸气长度

	if (!GSL_IS_EVEN(lenT_in)) { // 确保吸气长度为偶数
		if (IER >= 1) { // 根据不同吸呼比改变吸气长度
			lenT_in -= 1; 
		}
		else {
			lenT_in += 1;
		}
	}
	qint16 lenT_out = lenT_Breathe - lenT_in; // 呼气长度(保证吸气呼气长度为整数，牺牲些许IER精度)
	qint16 lenT_Hold1 = lenT_Hold / (1 + IER); // 屏息过程1
	if (!GSL_IS_EVEN(lenT_Hold1)) { // 确保吸气长度为偶数
		if (IER >= 1) { // 根据不同吸呼比改变吸气长度
			lenT_Hold1 += 1; 
		}
		else {
			lenT_Hold1 -= 1;
		}
	}
	qint16 lenT_Hold2 = lenT_Hold - lenT_Hold1; // 屏息过程2
	/*****************************波形序列生成*********************************/
	qint32 i = 0;					  // 序列下标
	QVector<qreal> tBreathe(qint16(TBreathe/Ts)); // 呼吸时间序列 
	std::generate(tBreathe.begin(), tBreathe.end(), [Ts, &i]() { // 生成时间序列
		return (i++) * Ts;
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> xIn(qint16(TBreathe/Ts));
	std::generate(xIn.begin(), xIn.end(), [=, &i]() { // 构建吸气波形
		return A * gsl_sf_sin(2 * M_PI * fBreathe * tBreathe[i++]);
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> xOut(qint16(TBreathe/Ts));
	std::generate(xOut.begin(), xOut.end(), [=, &i]() { // 构建呼气波形
		return A * gsl_sf_cos(2 * M_PI * fBreathe * tBreathe[i++]);
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> tHold(qint16(THold / Ts)); // 屏息时间序列
	std::generate(tHold.begin(), tHold.end(), [=, &i]() {
		return (i++) * Ts;
		});
	QVector<qreal> xHold(qint16(THold/Ts), A); // 构建屏息波形
	/*****************************分段波形拼接***********************************/
	QVector<qreal>::const_iterator iter_xIn = xIn.begin(); // 迭代器xIn
	QVector<qreal>::const_iterator iter_xOut = xOut.begin(); // 迭代器xOut
	QVector<qreal>::const_iterator iter_xHold = xHold.begin(); // 迭代器xHold
	QVector<qreal> x1(iter_xIn, iter_xIn + lenT_in/2); // 拼接波形
	QVector<qreal> x2(iter_xHold, iter_xHold + lenT_Hold1); 
	QVector<qreal> x3(iter_xOut, iter_xOut + lenT_out);
	QVector<qreal> x4(iter_xHold, iter_xHold + lenT_Hold2);
	QVector<qreal> x5(x1);

	QVector<qreal>::const_iterator iter_x1 = x1.begin(); // 迭代器x1
	QVector<qreal>::const_iterator iter_x2 = x2.begin(); // 迭代器x2
	QVector<qreal>::const_iterator iter_x3 = x3.begin(); // 迭代器x3
	QVector<qreal>::iterator iter_x4 = x4.begin(); // 迭代器x4
	QVector<qreal>::iterator iter_x5 = x5.begin(); // 迭代器x5

	for (; iter_x4 != x4.end(); iter_x4++) {
		*iter_x4 = -(*iter_x4); // 取反
	}
	std::reverse_copy(x1.begin(), x1.end(), x5.begin()); // 逆序
	for (; iter_x5 != x5.end(); iter_x5++) {
		*iter_x5 = -(*iter_x5); // 取反
	}
	QVector<qreal> xT(x1); // 单个完整周期
	xT.append(x2);
	xT.append(x3);
	xT.append(x4);
	xT.append(x5);
	QVector<qreal> xBreathe(xT); // 呼吸序列
	for (int j = 0; j < n-1; j++) { // 连接n个完整周期
		xBreathe.append(xT);
	}
	qint16 resLen = nChirp - n * lenT; // 计算剩余长度
	if (resLen > 0) {
		QVector<qreal> x6;
		if (resLen <= lenT_in / 2) { // 一般情况下剩余长度不会超过吸气过程1的长度
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
		xBreathe.append(x6); // 拼接剩余长度，与时间轴对齐
	}
	qDebug() << "xBreathe Size is:" << xBreathe.size();
	i = 0;
	std::generate(BoxBreathing::tSeq.begin(), BoxBreathing::tSeq.end(), [this, &i]() { // lambda表达式生成时间序列 
		return (i++) * BoxBreathing::resSam;
		});
	qint16 M = BoxBreathing::resSam / Ts; // 数组下标从0开始
	i = -M;
	std::generate(BoxBreathing::resSeq.begin(), BoxBreathing::resSeq.end(), [this, M, xBreathe, &i]() { // lambda表达式生成波形序列 
		i += M;
		return xBreathe.at(i); // 下采样
		});
	if (BoxBreathing::resSeq.size() == 0) { // 若波形生成失败，返回false
		return false;
	}
	return true;
}
/**
* ButeykoBreathing类
* @func          ButeykoBreathing
* 构造函数 
* @type          [void]
*/
ButeykoBreathing::ButeykoBreathing()
{
	qDebug() << "ButeykoBreathing Class Was Constructed.";
}
/**
* ButeykoBreathing类
* @func          ~ButeykoBreathing
* 析构函数 
* @type          [void]
*/
ButeykoBreathing::~ButeykoBreathing()
{
	qDebug() << "ButeykoBreathing Class Was Deleted.";
}
/**
* ButeykoBreathing类
* @func          CallResWaveGen
* Burteyko呼吸波形生成接口函数 
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
		qDebug() << "参数输入错误." << endl;
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
* ButeykoBreathing类
* @func          ResWaveGen
* Burteyko呼吸生成函数 
* @param        f
* @param        A
* @param        IER
* @param        hold
* @return   
* @type          [bool]
*/
bool ButeykoBreathing::ResWaveGen(qreal f, qreal A, qreal IER, qreal hold)
{
	qDebug() << QStringLiteral("Burteyko呼吸波形生成函数调用.");
	qreal Ts = 0.001;						   // 采样周期设为1ms，仿真"模拟信号"，再下采样至20Hz
	/*****************************分段长度确定*********************************/
	qint16 nT = ButeykoBreathing::resLen;
	qint32 lenT = nT / Ts;			    	   // 单个周期长度
	qint32 lenT_Breathe = lenT / (1 + hold);   // 总呼吸长度

	if ((lenT_Breathe % 3) != 0) { // 确保呼吸长度能被3整除
		lenT_Breathe -= (lenT_Breathe % 3);
	}
	qint32 lenT_Hold = lenT - lenT_Breathe; // 屏息长度
	qint32 lenT_Breathe1 = lenT_Breathe / 3; // 单次呼吸长度
	qreal T_Breathe1 = Ts * lenT_Breathe1;
	qreal f_Breathe = 1 / T_Breathe1; // 呼吸频率
	/*****************************波形序列生成*********************************/
	qint32 i = 0;					  // 序列下标
	QVector<qreal> tBreathe(lenT_Breathe1); // 呼吸时间序列 
	std::generate(tBreathe.begin(), tBreathe.end(), [Ts, &i]() { // 生成时间序列
		return (i++) * Ts;
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> xBreathe(lenT_Breathe1);
	std::generate(xBreathe.begin(), xBreathe.end(), [=, &i]() { // 构建吸气波形
		return A * gsl_sf_sin(2 * M_PI * f_Breathe * tBreathe[i++]);
		});
	i = 0; // 每次均需要重置i
	QVector<qreal> tHold(lenT_Hold); // 屏息时间序列
	std::generate(tHold.begin(), tHold.end(), [=, &i]() {
		return (i++) * Ts;
		});
	QVector<qreal> xHold(lenT_Hold, 0.0); // 构建屏息波形
	/*****************************分段波形拼接***********************************/
	QVector<qreal>::const_iterator iter_xBreathe = xBreathe.begin(); // 迭代器xBreathe
	QVector<qreal>::const_iterator iter_xHold = xHold.begin(); // 迭代器xHold
	QVector<qreal> x1(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // 拼接波形
	QVector<qreal> x2(iter_xHold, iter_xHold + lenT_Hold); 
	QVector<qreal> x3(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // 拼接波形
	QVector<qreal> x4(iter_xBreathe, iter_xBreathe + lenT_Breathe1); // 拼接波形

	QVector<qreal> xT(x1); // 单个完整周期
	xT.append(x2);
	xT.append(x3);
	xT.append(x4);
	qDebug() << "xT Size is:" << xT.size();
	i = 0;
	std::generate(ButeykoBreathing::tSeq.begin(), ButeykoBreathing::tSeq.end(), [this, &i]() { // lambda表达式生成时间序列 
		return (i++) * ButeykoBreathing::resSam;
		});
	qint16 M = ButeykoBreathing::resSam / Ts; // 数组下标从0开始
	i = -M;
	std::generate(ButeykoBreathing::resSeq.begin(), ButeykoBreathing::resSeq.end(), [this, M, xT, &i]() { // lambda表达式生成波形序列 
		i += M;
		return xT.at(i); // 下采样
		});
	if (ButeykoBreathing::resSeq.size() == 0) { // 若波形生成失败，返回false
		return false;
	}
	return true;
}
