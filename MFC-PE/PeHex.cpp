// PeHex.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC-PE.h"
#include "PeHex.h"
#include "afxdialogex.h"



// PeHex �Ի���

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


// PeHex ��Ϣ�������


BOOL PeHex::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_myHex->CreateDialogCtrl(IDC_CUSTOM1, m_hWnd);
	
	HEXCTRL::HEXDATA hds;
	hds.spnData = { reinterpret_cast<std::byte*>(m_pFilePe), m_dwFile_Size};
	m_myHex->SetData(hds);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
