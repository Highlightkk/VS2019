#include "ResGUISys.h"
#include <typeinfo>
/**
* @func          ResGUISys
* 构造函数
* @param        parent
* @return   
* @type          [int]
*/
ResGUISys::ResGUISys(QWidget* parent)
    : QMainWindow(parent)
{
    timerPlot = new WinTimer(parent); // 定时器初始化
    currentTime = new QDateTime();
  /*qDebug() << typeid(resWave->tSeq).name() << endl;
    qDebug() << resWave->tSeq.size()<< endl;
    qDebug() << resWave->resSeq.size()<< endl;*/
    GUISetup();
    /***************槽函数***************/
	// 显示坐标点数据
    connect(ui.resWaveGraph, &QCustomPlot::plottableClick, this, &ResGUISys::DisplayDataPoints);
	// 呼吸波形生成
    connect(ui.gen_Btn, &QPushButton::clicked, this, &ResGUISys::ResWaveGenHandler);
	// 禁用波形生成按钮
	connect(ui.start_Btn, &QPushButton::clicked, this, [&]{ ui.gen_Btn->setDisabled(true); });
	// 开启定时器 
	connect(ui.start_Btn, &QPushButton::clicked, this, [&] { timerPlot->start(50);
															 *currentTime = QDateTime::currentDateTime(); }); 
	// 关闭定时器
    connect(ui.stop_Btn, &QPushButton::clicked, this, [&] { ui.gen_Btn->setEnabled(true);
															timerPlot->stop(); }); 
	// 清除图窗
    connect(ui.clear_Btn, &QPushButton::clicked, this, [&] { ui.resWaveGraph->graph(0)->data().data()->clear();
                                                            ui.resWaveGraph->replot(QCustomPlot::rpQueuedReplot); }); 
	// 定时器中断
	connect(timerPlot, &WinTimer::timeout, this, &ResGUISys::TimeoutHandler);
	// 滑条数据与文本框数据同步
	connect(ui.freq_HSlider,  &QSlider::valueChanged, this, [&] { ui.freq_LEdit->setText(QString::number(qreal(ui.freq_HSlider->value() / 100.0))); });
	connect(ui.depth_HSlider, &QSlider::valueChanged, this, [&] { ui.depth_LEdit->setText(QString::number(qreal(ui.depth_HSlider->value() / 10.0))); });
	connect(ui.IER_HSlider,   &QSlider::valueChanged, this, [&] { ui.IER_LEdit->setText(QString::number(qreal(ui.IER_HSlider->value() / 10.0))); });
	connect(ui.holdR_HSlider, &QSlider::valueChanged, this, [&] { ui.holdR_LEdit->setText(QString::number(qreal(ui.holdR_HSlider->value() / 100.0))); });
	connect(ui.freq_LEdit,    &QLineEdit::editingFinished, this, [&] { ui.freq_HSlider->setValue(ui.freq_LEdit->text().toDouble() * 100.0); });
	connect(ui.depth_LEdit,   &QLineEdit::editingFinished, this, [&] { ui.depth_HSlider->setValue(ui.depth_LEdit->text().toDouble() * 10.0); });
	connect(ui.IER_LEdit,     &QLineEdit::editingFinished, this, [&] { ui.IER_HSlider->setValue(ui.IER_LEdit->text().toDouble() * 10.0); });
	connect(ui.holdR_LEdit,   &QLineEdit::editingFinished, this, [&] { ui.holdR_HSlider->setValue(ui.holdR_LEdit->text().toDouble() * 100.0); });
	// 根据选择的呼吸方法设置默认值
	connect(ui.resMode_ComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ResGUISys::ResParamsInit);
}
/**
* @func          ~ResGUISys
* 析构函数
* @type          [void]
*/
ResGUISys::~ResGUISys()
{
    qDebug()<< "ResGUISys Deleted." << endl;
}
/**
* @func          LoadCSS
* CSS加载函数
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
        QString styleSheet = this->styleSheet(); // 读取当前UI样式表
        styleSheet += QLatin1String(file.readAll()); // 读取本地CSS样式
        this->setStyleSheet(styleSheet); // 加载本地CSS样式
    }
}
/**
* @func          TimeoutHandler
* 定时器响应函数, 完成绘图工作 
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
* 控件布局初始化函数
* @type          [void]
*/
void ResGUISys::LayoutSetup() 
{
	screen = qApp->primaryScreen(); // 获取屏幕大小信息
	screenFactor = 0.75; // 屏幕比例因子
	appWidth = (qint16)screen->size().width() * screenFactor; // 根据屏幕大小调整GUI窗口大小
	appHeight = (qint16)screen->size().height() * screenFactor; // 若1920x1080，则窗口大小为1440x810
	resize(appWidth, appHeight); // 调整APP大小
    
 //   QGridLayout* mainGridLayout = new QGridLayout(ui.mainWidget); // 创建网格布局管理器
 //   QGridLayout* subGridLayout = new QGridLayout(); // 创建网格布局管理器1
 //   QVBoxLayout* subVBoxLayout1 = new QVBoxLayout(); // 创建垂直布局管理器1

 //   QVBoxLayout* subVBoxLayout2 = new QVBoxLayout(); // 创建垂直布局管理器2
 //   QFormLayout* subFormLayout1 = new QFormLayout(); // 创建表单布局管理器1
 //   /*******************表单布局管理器1*******************/
 //   subFormLayout1->setContentsMargins(0, 0, 0, 0);
 //   //subFormLayout1->addRow(QStringLiteral("频率Hz"), ui.freq_HSlider, ui.freq_LEdit);
 //   /*******************垂直布局管理器1*******************/
 //   subVBoxLayout1->setContentsMargins(0, 0, 0, 0); // 布局管理器间隔(左上右下)
 //   subVBoxLayout1->setSpacing(5); // 控件之间的间隔
 //   subVBoxLayout1->addWidget(ui.resMode_ComboBox, 0, 0);
 //   subVBoxLayout1->addWidget(ui.freq_HSlider, 0, 0);
 //   subVBoxLayout1->addWidget(ui.depth_HSlider, 0, 0);
 //   subVBoxLayout1->addWidget(ui.IER_HSlider,  0, 0);
 //   subVBoxLayout1->addWidget(ui.holdR_HSlider,  0, 0);
 //   /*******************垂直布局管理器2*******************/
 //   subVBoxLayout2->setContentsMargins(0, 0, 0, 0); // 布局管理器间隔(左上右下)
 //   subVBoxLayout2->setSpacing(5); // 控件之间的间隔
 //   ui.start_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // 设置控件规则
 //   ui.stop_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   ui.clear_Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   subVBoxLayout2->addWidget(ui.start_Btn, 0, 0);
 //   subVBoxLayout2->addWidget(ui.stop_Btn,  0, 0);
 //   subVBoxLayout2->addWidget(ui.clear_Btn, 0, 0);
 //   /*******************网格布局管理器1*******************/
 //   subGridLayout->setContentsMargins(0, 0, 0, 0);
 //   subGridLayout->setSpacing(5);
 //   subGridLayout->addWidget(ui.resGuide_TBrowser, 0, 0, 3, 6);
 //   subGridLayout->addLayout(subVBoxLayout1, 6, 0, 3, 6);
 //   subGridLayout->addLayout(subVBoxLayout2, 9, 1, 2, 4);
 //   subGridLayout->setSizeConstraint(QLayout::SetMinimumSize); // 设置布局伸缩规则
 //   /*******************网格布局管理器*******************/
 //   ui.resWaveGraph->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
 //   mainGridLayout->addWidget(ui.resWaveGraph, 0,  0,   90, 140);
 //   mainGridLayout->addLayout(subGridLayout, 2, 144, 60, 12);
 //   mainGridLayout->setGeometry(QRect(0, 0, appWidth, appHeight));
}
/**
* @func          GraphSetup
* 绘图初始化函数
* @type          [void]
*/
void ResGUISys::GraphSetup() 
{
	ui.resWaveGraph->addGraph(); // 添加图标
	ui.resWaveGraph->setBackground(QBrush(QColor::fromRgb(0, 0, 0, 0))); // 设置背景颜色-透明
    ui.resWaveGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 鼠标拖拽、放大缩小、选中曲线加粗
	ui.resWaveGraph->axisRect()->setBackground(QBrush(QColor::fromRgb(249, 251, 253, 255))); // 设置绘图区颜色
	ui.resWaveGraph->xAxis->setRange(0, resWave->resLen);
	ui.resWaveGraph->yAxis->setRange(-resWave->resDepth/0.75, resWave->resDepth/0.75);
	ui.resWaveGraph->xAxis->setPadding(0); // 设置标签与边界间隔-0
	ui.resWaveGraph->yAxis->setPadding(0);
    ui.resWaveGraph->xAxis->setLabelPadding(0); // 设置标签与坐标轴间隔-0
    ui.resWaveGraph->yAxis->setLabelPadding(0);
	ui.resWaveGraph->xAxis->grid()->setVisible(false); // 隐藏网格
	ui.resWaveGraph->yAxis->grid()->setVisible(false);
	ui.resWaveGraph->xAxis->setLabel(QStringLiteral("时间(s)")); // QStringLiteral解决中文乱码问题
	ui.resWaveGraph->yAxis->setLabel(QStringLiteral("呼吸幅度"));
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
    //ui.resGuide_TBrowser->append(QStringLiteral("训练开始!"));
}
/**
* @func          ResParamsInit
* 根据选择的呼吸方法设置相应默认值 
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
		ui.holdR_LEdit->setDisabled(true); // 根据呼吸方法的特点禁用某项呼吸参数调节
		ui.holdR_HSlider->setDisabled(true);
		break;
	}
	case ResWave::PursedLip: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
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
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::ACBT: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	default: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
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
* 显示曲线坐标函数
* @param        plottable：绘制曲线状态类？
* @param        dataIndex：曲线数据下标索引
* @param        event：鼠标事件
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
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::PursedLip: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::Box: {
		BoxBreathing* resObj = new BoxBreathing(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::Buteyko: {
		ButeykoBreathing* resObj = new ButeykoBreathing(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::Paradoxical: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	case ResWave::ACBT: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
		resWave = &(*resObj);
		break;
	}
	default: {
		AbdominalRespiration* resObj = new AbdominalRespiration(); // 指向具体的子对象
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
		ui.resGuide_TBrowser->setText(QStringLiteral("参数设置错误."));
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
* 布局与样式初始化函数
* @type          [void]
*/
void ResGUISys::GUISetup() {
    ui.setupUi(this);
    /***************GUI尺寸调节***************/
    LayoutSetup();
	/***************控件状态值初始化***************/
    //WidgetInit();
    /***************绘图初始化***************/
    //GraphSetup();
    /***************样式设计***************/
    LoadCSS("SZU_Style.css");
}
