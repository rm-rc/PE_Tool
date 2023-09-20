
// MFC-PEDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <map>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include<set>
#include "PeLoad.h"

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

struct SystemProcess
{
	CString csProcessName;		//������
	CString dwBit;				//����λ��
	DWORD dwPID;				//����PID
	DWORD ImageBase;			//�ڴ�λ��
	DWORD ImageSize;			//�ڴ��С
	HICON  hIcon;				//ICON
	CString szPath;				//

};

struct ProcessMode
{
	CString szModeName;			//ģ����
	DWORD ImageBase;			//�ڴ�λ��
	DWORD ImageSize;			//�ڴ��С
};

class myCompare
{
public:
	bool operator()( SystemProcess p1,  SystemProcess p2) const//��const�޶������޸Ĵ��������
	{
		if (p1.csProcessName.Compare(p2.csProcessName) == 1)
		{
			return false;
		}
		else if (p1.csProcessName.Compare(p2.csProcessName) == 0)
		{
			return false;
		}
		else if (p1.csProcessName.Compare(p2.csProcessName) == -1)
		{
			return true;
		}
		//return  0;//p1.csProcessName.Compare(p2.csProcessName);
		//return p1.dwPID < p2.dwPID;
	}


};


// CMFCPEDlg �Ի���
class CMFCPEDlg : public CDialogEx
{
// ����
public:
	CMFCPEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListTop(NMHDR *pNMHDR, LRESULT *pResult);
	// ������б�
	CListCtrl m_UpList;
	POINT old;	//��ȡ�ϵ���Ļ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Resize();
	// ������б�
	CListCtrl m_DwonList;
	CImageList m_IconList;

	//��ȡ��ǰϵͳ�����н��̲�չʾ��LIST��
	VOID GetProcessList();
	BOOL EnableDebugPrivilege();
	std::map<int, SystemProcess> m_mapProcess;
	std::set<SystemProcess, myCompare> m_setProcess;
	std::vector<ProcessMode> m_ProcessMode;
	DWORD m_dwPid;
	TCHAR* OnGetFileName();
	//��ȡ��ǰѡ�еĽ���ģ��
	VOID GetModuleList(DWORD pid);
	afx_msg void OnNMClickListTop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickListTop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListTop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRemoteThreadInjection();
	afx_msg void OnRclickListDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAPC_Inject();
	afx_msg void OnEIP_Inject();
	afx_msg void OnBnClickedButton2();
};
