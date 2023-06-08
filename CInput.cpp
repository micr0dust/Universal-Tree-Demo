// CInput.cpp: 實作檔案
//

#include "pch.h"
#include "Universal Demo.h"
#include "afxdialogex.h"
#include "CInput.h"


// CInput 對話方塊

IMPLEMENT_DYNAMIC(CInput, CDialogEx)

CInput::CInput(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, InputStr(_T(""))
{

}

CInput::~CInput()
{
}

void CInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, InputStr);
}


BEGIN_MESSAGE_MAP(CInput, CDialogEx)
END_MESSAGE_MAP()


// CInput 訊息處理常式


BOOL CInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此加入額外的初始化
	SetDlgItemText(IDC_EDIT1, InputStr);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
