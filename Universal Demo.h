﻿
// Universal Demo.h: Universal Demo 應用程式的主要標頭檔
//
#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 在包含此檔案前先包含 'pch.h'"
#endif

#include "resource.h"       // 主要符號


// CUniversalDemoApp:
// 查看 Universal Demo.cpp 以了解此類別的實作
//

class CUniversalDemoApp : public CWinApp
{
public:
	CUniversalDemoApp() noexcept;


// 覆寫
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 程式碼實作
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUniversalDemoApp theApp;
