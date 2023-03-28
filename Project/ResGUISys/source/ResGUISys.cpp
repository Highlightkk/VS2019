#include "ResGUISys.h"
#include <typeinfo>
/**
* @func          ResGUISys
* ���캯��
* @param        parent
* @return   
* @type          [int]
*/
ResGUISys::ResGUISys(QWidget* parent)
    : QMainWindow(parent)
{
    timerPlot = new WinTimer(parent); // ��ʱ����ʼ��
    currentTime = new QDateTime();
  /*qDebug() << typeid(resWave->tSeq).name() << endl;
    qDebug() << resWave->tSeq.size()<< endl;
    qDebug() << resWave->resSeq.size()<< endl;*/
    GUISetup();
    /***************�ۺ���***************/
	// ��ʾ���������
    connect(ui.resWaveGraph, &QCustomPlot::plottableClick, this, &ResGUISys::DisplayDataPoints);
	// ������������
    connect(ui.gen_Btn, &QPushButton::clicked, this, &ResGUISys::ResWaveGenHandler);
	// ���ò������ɰ�ť
	connect(ui.start_Btn, &QPushButton::clicked, this, [&]{ ui.gen_Btn->setDisabled(true); });
	// ������ʱ�� 
	connect(ui.start_Btn, &QPushButton::clicked, this, [&] { timerPlot->start(50);
															 *currentTime = QDateTime::currentDateTime(); }); 
	// �رն�ʱ��
    connect(ui.stop_Btn, &QPushButton::clicked, this, [&] { ui.gen_Btn->setEnabled(true);
															timerPlot->stop(); }); 
	// ���ͼ��
    connect(ui.clear_Btn, &QPushButton::clicked, this, [&] { ui.resWaveGraph->graph(0)->data().data()->clear();
                                                            ui.resWaveGraph->replot(QCustomPlot::rpQueuedReplot); }); 
	// ��ʱ���ж�
	connect(timerPlot, &WinTimer::timeout, this, &ResGUISys::TimeoutHandler);
	// �����������ı�������ͬ��
	connect(ui.freq_HSlider,  &QSlider::valueChanged, this, [&] { ui.freq_LEdit->setText(QString::number(qreal(ui.freq_HSlider->value() / 100.0))); });
	connect(ui.depth_HSlider, &QSlider::valueChanged, this, [&] { ui.depth_LEdit->setText(QString::number(qreal(ui.depth_HSlider->value() / 10.0))); });
	connect(ui.IER_HSlider,   &QSlider::valueChanged, this, [&] { ui.IER_LEdit->setText(QString::number(qreal(ui.IER_HSlider->value() / 10.0))); });
	connect(ui.holdR_HSlider, &QSlider::valueChanged, this, [&] { ui.holdR_LEdit->setText(QString::number(qreal(ui.holdR_HSlider->value() / 100.0))); });
	connect(ui.freq_LEdit,    &QLineEdit::editingFinished, this, [&] { ui.freq_HSlider->setValue(ui.freq_LEdit->text().toDouble() * 100.0); });
	connect(ui.depth_LEdit,   &QLineEdit::editingFinished, this, [&] { ui.depth_HSlider->setValue(ui.depth_LEdit->text().toDouble() * 10.0); });
	connect(ui.IER_LEdit,     &QLineEdit::editingFinished, this, [&] { ui.IER_HSlider->setValue(ui.IER_LEdit->text().toDouble() * 10.0); });
	connect(ui.holdR_LEdit,   &QLineEdit::editingFinished, this, [&] { ui.holdR_HSlider->setValue(ui.holdR_LEdit->text().toDouble() * 100.0); });
	// ����ѡ��ĺ�����������Ĭ��ֵ
	connect(ui.resMode_ComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ResGUISys::ResParamsInit);
}
/**
* @func          ~ResGUISys
* ��������
* @type          [void]
*/
ResGUISys::~ResGUISys()
{
    qDebug()<< "ResGUISys Deleted." << endl;
}
/**
* @func          LoadCSS
* CSS���غ���
* @param        sheetName
* @type          [void]
*/
void ResGUISys::LoadCSS(const QString& sheetName)
{
    QString filePath = ".\\qss\\";
    QFile file(filePath+sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet(); // ��ȡ��ǰUI��ʽ��
        styleSheet += QLatin1String(file.readAll()); // ��ȡ����CSS��ʽ
        this->setStyleSheet(styleSheet); // ���ر���CSS��ʽ
    }
}
/**
* @func          TimeoutHandler
* ��ʱ����Ӧ����, ��ɻ�ͼ���� 
* @type          [void]
*/
void ResGUISys::TimeoutHandler() 
{
    //qDebug() << "TimeoutHandler Called!" << endl;
	timerPlot->m_cnt++;
	ui.resWaveGraph->graph(0)->addData(resWave->tSeq[timerPlot->m_cnt - 1], resWave->resSeq[timerPlot->m_cnt - 1]);
    if (timerPlot->m_cnt >= resWave->nSamples) {
        timerPlot->m_cnt = 0; 
        QDateTime tmpTime = QDateTime::currentDateTime();
        qDebug() << ui.resWaveGraph->graph(0)->dataCount() << endl;
		qDebug() << static_cast<qreal>((static_cast<qreal>(tmpTime.time().hour()) - static_cast<qreal>(currentTime->time().hour())) * 3600.0 + \
			(static_cast<qreal>(tmpTime.time().minute()) - static_cast<qreal>(currentTime->time().minute())) * 60.0 + \
			static_cast<qreal>(tmpTime.time().second()) - static_cast<qreal>(currentTime->time().second()) + \
			static_cast<qreal>((tmpTime.time().msec()) - static_cast<qreal>(currentTime->time().msec()))/1000.0) << endl;
        *currentTime = tmpTime;
        ui.resWaveGraph->graph(0)->data().data()->clear();
    }
	ui.resWaveGraph->replot(QCustomPlot::rpQueuedReplot);
}
/**
* @func          LayoutSetup
* �ؼ����ֳ�ʼ������
* @type          [void]
*/
void ResGUISys::LayoutSetup() 
{
	screen = qApp->primaryScreen(); // ��ȡ��Ļ��С��Ϣ
	screenFactor = 0.75; // ��Ļ��������
	appWidth = (qint16)screen->size().width() * screenFactor; // ������Ļ��С����GUI���ڴ�С
	appHeight = (qint16)screen->size().height() * screenFactor; // ��1920x1080���򴰿ڴ�СΪ1440x810
	resize(appWidth, appHeight); // ����APP��С
    
 //   QGridLayout* mainGridLayout = new QGridLayout(ui.mainWidget); // �������񲼾ֹ�����
 //   QGridLayout* subGridLayout = new QGridLayout(); // �������񲼾ֹ�����1
 //   QVBoxLayout* subVBoxLayout1 = new QVBoxLayout(); // ������ֱ���ֹ�����1

 //   QVBoxLayout* subVBoxLayout2 = new QVBoxLayout(); // ������ֱ���ֹ�����2
 //   QFormLayout* subFormLayout1 = new QFormLayout(); // ���������ֹ�����1
 //   /*******************�����ֹ�����1*******************/
 //   subFormLayout1->setContentsMargins(0, 0, 0, 0);
 //   //subFormLayout1->addRow(QStringLiteral("Ƶ��Hz"), ui.freq_HSlider, ui.freq_LEdit);
 //   /*******************��ֱ���ֹ�����1*******************/
 //   subVBoxLayout1->setContentsMargins(0, 0, 0, 0); // ���ֹ��������(��������)
 //   subVBoxLayout1->setSpacing(5); // �ؼ�֮��ļ��
 //   subVBoxLayout1->addWidget(ui.resMode_ComboBox, 0, 0);
 //   subVBoxLayout1->addWidget(ui.freq_HSlider, 0, 0);
 //   subVBoxLayout1->addWidget(ui.depth_HSlider, 0, 0);
 //   subVBoxLayout1->addWidget(ui.IER_HSlider,  0, 0);
 //   subVBoxLayout1->addWidget(ui.holdR_HSlider,  0, 0);
 //   /*******************��ֱ���ֹ�����2*******************/
 //   subVBoxLayout2->setContentsMargins(0, 0, 0, 0); // ���ֹ��������(��������)
 //   subVBoxLayout2->setSpacing(5); // �ؼ�֮��ļ��
 //   ui.start_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // ���ÿؼ�����
 //   ui.stop_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   ui.clear_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   subVBoxLayout2->addWidget(ui.start_Btn, 0, 0);
 //   subVBoxLayout2->addWidget(ui.stop_Btn,  0, 0);
 //   subVBoxLayout2->addWidget(ui.clear_Btn, 0, 0);
 //   /*******************���񲼾ֹ�����1*******************/
 //   subGridLayout->setContentsMargins(0, 0, 0, 0);
 //   subGridLayout->setSpacing(5);
 //   subGridLayout->addWidget(ui.resGuide_TBrowser, 0, 0, 3, 6);
 //   subGridLayout->addLayout(subVBoxLayout1, 6, 0, 3, 6);
 //   subGridLayout->addLayout(subVBoxLayout2, 9, 1, 2, 4);
 //   subGridLayout->setSizeConstraint(QLayout::SetMinimumSize); // ���ò�����������
 //   /*******************���񲼾ֹ�����*******************/
 //   ui.resWaveGraph->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   mainGridLayout->addWidget(ui.resWaveGraph, 0,  0,   90, 140);
 //   mainGridLayout->addLayout(subGridLayout, 2, 144, 60, 12);
 //   mainGridLayout->setGeometry(QRect(0, 0, appWidth, appHeight));
}
/**
* @func          GraphSetup
* ��ͼ��ʼ������
* @type          [void]
*/
void ResGUISys::GraphSetup() 
{
	ui.resWaveGraph->addGraph(); // ���ͼ��
	ui.resWaveGraph->setBackground(QBrush(QColor::fromRgb(0, 0, 0, 0))); // ���ñ�����ɫ-͸��
    ui.resWaveGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // �����ק���Ŵ���С��ѡ�����߼Ӵ�
	ui.resWaveGraph->axisRect()->setBackground(QBrush(QColor::fromRgb(249, 251, 253, 255))); // ���û�ͼ����ɫ
	ui.resWaveGraph->xAxis->setRange(0, resWave->resLen);
	ui.resWaveGraph->yAxis->setRange(-resWave->resDepth/0.75, resWave->resDepth/0.75);
	ui.resWaveGraph->xAxis->setPadding(0); // ���ñ�ǩ��߽���-0
	ui.resWaveGraph->yAxis->setPadding(0);
    ui.resWaveGraph->xAxis->setLabelPadding(0); // ���ñ�ǩ����������-0
    ui.resWaveGraph->yAxis->setLabelPadding(0);
	ui.resWaveGraph->xAxis->grid()->setVisible(false); // ��������
	ui.resWaveGraph->yAxis->grid()->setVisible(false);
	ui.resWaveGraph->xAxis->setLabel(QStringLiteral("ʱ��(s)")); // QStringLiteral���������������
	ui.resWaveGraph->yAxis->setLabel(QStringLiteral("��������"));
	ui.resWaveGraph->xAxis->setLabelFont(QFont("Microsoft Yahei", 14, QFont::Bold));
	ui.resWaveGraph->yAxis->setLabelFont(QFont("Microsoft Yahei", 14, QFont::Bold));
	ui.resWaveGraph->xAxis->setTickLabelFont(QFont("Microsoft Yahei", 12, QFont::Bold));
	ui.resWaveGraph->yAxis->setTickLabelFont(QFont("Microsoft Yahei", 12, QFont::Bold));
    QPen pen;
    pen.setWidth(2);
    ui.resWaveGraph->xAxis->setBasePen(pen);
    ui.resWaveGraph->yAxis->setBasePen(pen);
	ui.resWaveGraph->xAxis->setTickPen(pen);
	ui.resWaveGraph->xAxis->setSubTickPen(pen);
	ui.resWaveGraph->yAxis->setTickPen(pen);
	ui.resWaveGraph->yAxis->setSubTickPen(pen);
	ui.resWaveGraph->xAxis->setTickLengthIn(10);
	ui.resWaveGraph->yAxis->setTickLengthIn(10);
	ui.resWaveGraph->xAxis->setSubTickLengthIn(5);
	ui.resWaveGraph->yAxis->setSubTickLengthIn(5);
    //ui.resGuide_TBrowser->append(QStringLiteral("ѵ����ʼ!"));
}
/**
* @func          ResParamsInit
* ����ѡ��ĺ�������������ӦĬ��ֵ 
* @param        mode
* @type          [void]
*/
void ResGUISys::ResParamsInit(qint16 mode)
{
	ui.freq_LEdit->setEnabled(true);
	ui.freq_HSlider->setEnabled(true);
	ui.depth_LEdit->setEnabled(true);
	ui.depth_HSlider->setEnabled(true);
	ui.IER_LEdit->setEnabled(true);
	ui.IER_HSlider->setEnabled(true);
	ui.holdR_LEdit->setEnabled(true);
	ui.holdR_HSlider->setEnabled(true);

	switch (mode) {
	case ResWave::Abdominal: {
		ui.freq_LEdit->setText(QString("0.3"));
		ui.depth_LEdit->setText(QString("10.0"));
		ui.IER_LEdit->setText(QString("2.0"));
		ui.holdR_LEdit->setText(QString("0.0"));
		ui.holdR_LEdit->setDisabled(true); // ���ݺ����������ص����ĳ�������������
		ui.holdR_HSlider->setDisabled(true);
		break;
	}
	case ResWave::PursedLip: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::Box: {
		ui.freq_LEdit->setText(QString("0.3"));
		ui.depth_LEdit->setText(QString("10.0"));
		ui.IER_LEdit->setText(QString("1.0"));
		ui.holdR_LEdit->setText(QString("1.0"));
		ui.holdR_LEdit->setDisabled(true);
		ui.holdR_HSlider->setDisabled(true);
		ui.IER_LEdit->setDisabled(true);
		ui.IER_HSlider->setDisabled(true);
		break;
	}
	case ResWave::Buteyko: {
		ui.depth_LEdit->setText(QString("5.0"));
		ui.IER_LEdit->setText(QString("1.0"));
		ui.holdR_LEdit->setText(QString("3.0"));
		ui.freq_LEdit->setText(QString("0.2"));
		ui.freq_LEdit->setDisabled(true);
		ui.freq_HSlider->setDisabled(true);
		ui.IER_LEdit->setDisabled(true);
		ui.IER_HSlider->setDisabled(true);
		break;
	}
	case ResWave::Paradoxical: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::ACBT: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	default: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	}
	//ui.freq_LEdit->setText(QString("0.3"));
	//ui.depth_LEdit->setText(QString("10.0"));
	//ui.IER_LEdit->setText(QString("2.0"));
	//ui.holdR_LEdit->setText(QString("0.0"));
}
/**
* @func          DisplayDataPoints
* ��ʾ�������꺯��
* @param        plottable����������״̬�ࣿ
* @param        dataIndex�����������±�����
* @param        event������¼�
* @type          [void]
*/
void ResGUISys::DisplayDataPoints(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
		qDebug() << "data point is:" << dataIndex << endl;
    }
}
void ResGUISys::ResWaveGenHandler()
{
	qDebug() << "ResWaveGenHandler Called." << endl;
    switch (ui.resMode_ComboBox->currentIndex()) {
	case ResWave::Abdominal: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::PursedLip: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::Box: {
		BoxBreathing* resObj = new BoxBreathing(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::Buteyko: {
		ButeykoBreathing* resObj = new ButeykoBreathing(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::Paradoxical: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	case ResWave::ACBT: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	default: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // ָ�������Ӷ���
		resWave = &(*resObj);
		break;
	}
	}
	qreal f = ui.freq_LEdit->text().toDouble();
	qreal A = ui.depth_LEdit->text().toDouble();
	qreal IER = ui.IER_LEdit->text().toDouble();
	qreal holdR = ui.holdR_LEdit->text().toDouble();
	qDebug() << "f:" << ui.freq_LEdit->text().toDouble();
	if (resWave->CallResWaveGen(f, A, IER, holdR) == 0) {
		ui.resGuide_TBrowser->setText(QStringLiteral("�������ô���."));
		ui.start_Btn->setDisabled(true);
		ui.stop_Btn->setDisabled(true);
		ui.clear_Btn->setDisabled(true);
	}
	else {
		GraphSetup();
		ui.start_Btn->setEnabled(true);
		ui.stop_Btn->setEnabled(true);
		ui.clear_Btn->setEnabled(true);
	}
}
/**
* @func          GUISetup
* ��������ʽ��ʼ������
* @type          [void]
*/
void ResGUISys::GUISetup() {
    ui.setupUi(this);
    /***************GUI�ߴ����***************/
    LayoutSetup();
	/***************�ؼ�״ֵ̬��ʼ��***************/
    //WidgetInit();
    /***************��ͼ��ʼ��***************/
    //GraphSetup();
    /***************��ʽ���***************/
    LoadCSS("SZU_Style.css");
}
