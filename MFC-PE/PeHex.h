#pragma once
#include  "HexCtrl/HexCtrl.h"
// PeHex �Ի���
class PeHex : public CDialogEx
{
	DECLARE_DYNAMIC(PeHex)

public:
	PeHex(TCHAR* pFilePe, DWORD dwFile_Size, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PeHex();
	TCHAR* m_pFilePe;
	DWORD m_dwFile_Size;
// �Ի�������
	enum { IDD = IDD_DIALOG2 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HEXCTRL::IHexCtrlPtr m_myHex{ HEXCTRL::CreateHexCtrl() };
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
