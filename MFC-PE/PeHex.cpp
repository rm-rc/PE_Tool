// PeHex.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC-PE.h"
#include "PeHex.h"
#include "afxdialogex.h"



// PeHex 对话框

IMPLEMENT_DYNAMIC(PeHex, CDialogEx)

PeHex::PeHex(TCHAR* pFilePe, DWORD dwFile_Size, CWnd* pParent /*=NULL*/)
	: CDialogEx(PeHex::IDD, pParent)
{
	m_pFilePe = pFilePe;
	m_dwFile_Size = dwFile_Size;
}

PeHex::~PeHex()
{
}

void PeHex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PeHex, CDialogEx)
END_MESSAGE_MAP()


// PeHex 消息处理程序


BOOL PeHex::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_myHex->CreateDialogCtrl(IDC_CUSTOM1, m_hWnd);
	
	HEXCTRL::HEXDATA hds;
	hds.spnData = { reinterpret_cast<std::byte*>(m_pFilePe), m_dwFile_Size};
	m_myHex->SetData(hds);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
