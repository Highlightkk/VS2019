/**
*  Copyright (c) 2022, Shenzhen University
*  All rights reserved.
*
*  @file        main.cpp
*  @author      pedria
*  @date        2022-11-12
*  @brief       ��λ�����������
*  @note        
*  @version     -v  2022-11-12
*/
#include "ResGUISys.h"
#include <QtWidgets/QApplication>
#pragma comment(lib,"Winmm.lib") // ��ý��߾��ȶ�ʱ��������

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ResGUISys w;
	w.show();

	return a.exec();
}
