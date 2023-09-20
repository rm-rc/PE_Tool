
// MFC-PEDlg.h : 头文件
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
	CString csProcessName;		//进程名
	CString dwBit;				//程序位数
	DWORD dwPID;				//进程PID
	DWORD ImageBase;			//内存位置
	DWORD ImageSize;			//内存大小
	HICON  hIcon;				//ICON
	CString szPath;				//

};

struct ProcessMode
{
	CString szModeName;			//模块名
	DWORD ImageBase;			//内存位置
	DWORD ImageSize;			//内存大小
};

class myCompare
{
public:
	bool operator()( SystemProcess p1,  SystemProcess p2) const//加const限定不能修改传入的数据
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


// CMFCPEDlg 对话框
class CMFCPEDlg : public CDialogEx
{
// 构造
public:
	CMFCPEDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListTop(NMHDR *pNMHDR, LRESULT *pResult);
	// 上面的列表
	CListCtrl m_UpList;
	POINT old;	//获取老的屏幕坐标
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Resize();
	// 下面的列表
	CListCtrl m_DwonList;
	CImageList m_IconList;

	//获取当前系统的所有进程并展示在LIST上
	VOID GetProcessList();
	BOOL EnableDebugPrivilege();
	std::map<int, SystemProcess> m_mapProcess;
	std::set<SystemProcess, myCompare> m_setProcess;
	std::vector<ProcessMode> m_ProcessMode;
	DWORD m_dwPid;
	TCHAR* OnGetFileName();
	//获取当前选中的进程模块
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
