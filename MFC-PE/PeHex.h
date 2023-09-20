#pragma once
#include  "HexCtrl/HexCtrl.h"
// PeHex 对话框
class PeHex : public CDialogEx
{
	DECLARE_DYNAMIC(PeHex)

public:
	PeHex(TCHAR* pFilePe, DWORD dwFile_Size, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PeHex();
	TCHAR* m_pFilePe;
	DWORD m_dwFile_Size;
// 对话框数据
	enum { IDD = IDD_DIALOG2 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HEXCTRL::IHexCtrlPtr m_myHex{ HEXCTRL::CreateHexCtrl() };
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
