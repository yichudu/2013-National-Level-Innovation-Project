
// mfc_timer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_timerApp:
// �йش����ʵ�֣������ mfc_timer.cpp
//

class Cmfc_timerApp : public CWinApp
{
public:
	Cmfc_timerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_timerApp theApp;