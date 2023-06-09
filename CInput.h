#pragma once
#include "afxdialogex.h"


// CInput 對話方塊

class CInput : public CDialogEx
{
	DECLARE_DYNAMIC(CInput)

public:
	CInput(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~CInput();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	CString InputStr;
	CString title;
	virtual BOOL OnInitDialog();
};
