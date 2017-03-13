/**
 *	@file		main.cpp
 *	@brief		�A�v���Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/03/14
 *	@version	1.0
 */

// include
#include "QtDirect3D12.h"
#include <QtWidgets/QApplication>

// �f�o�b�O(���������[�N)�p
#if defined(_WIN32) && !defined(QT_NO_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int main(int argc, char *argv[])
{
#if defined(_WIN32) && !defined(QT_NO_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	QApplication a(argc, argv);
	QtDirect3D12 w;
	w.show();
	return a.exec();
}
