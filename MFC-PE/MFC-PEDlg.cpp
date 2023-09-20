
// MFC-PEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC-PE.h"
#include "MFC-PEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnOpenPE();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_COMMAND(ID_32771, &CAboutDlg::OnOpenPE)
END_MESSAGE_MAP()


// CMFCPEDlg 对话框



CMFCPEDlg::CMFCPEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCPEDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCPEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOP, m_UpList);
	DDX_Control(pDX, IDC_LIST_DOWN, m_DwonList);
}

BEGIN_MESSAGE_MAP(CMFCPEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOP, &CMFCPEDlg::OnLvnItemchangedListTop)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_TOP, &CMFCPEDlg::OnNMClickListTop)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_TOP, &CMFCPEDlg::OnColumnclickListTop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TOP, &CMFCPEDlg::OnRclickListTop)
	ON_COMMAND(ID_32771, &CMFCPEDlg::OnRemoteThreadInjection)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DOWN, &CMFCPEDlg::OnRclickListDown)
	ON_COMMAND(ID_32772, &CMFCPEDlg::OnAPC_Inject)
	ON_COMMAND(ID_32773, &CMFCPEDlg::OnEIP_Inject)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCPEDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCPEDlg 消息处理程序

BOOL CMFCPEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	EnableDebugPrivilege();
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;



	CRect UpRectList;
	m_UpList.GetClientRect(&UpRectList);
	//获取编程语言列表视图的位置和大小
	m_UpList.SetExtendedStyle(m_UpList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_UpList.InsertColumn(0, L"图标", LVCFMT_CENTER, UpRectList.Width() / 6, 0);
	m_UpList.InsertColumn(1, L"进程", LVCFMT_CENTER, UpRectList.Width() / 6, 0);
	m_UpList.InsertColumn(2, L"architecture", LVCFMT_CENTER, UpRectList.Width() / 6, 1);
	m_UpList.InsertColumn(3, L"进程PID", LVCFMT_CENTER, UpRectList.Width() / 6, 2);
	m_UpList.InsertColumn(4, L"内存地址", LVCFMT_CENTER, UpRectList.Width() / 6, 3);
	m_UpList.InsertColumn(5, L"内存大小", LVCFMT_CENTER, UpRectList.Width() / 6, 4);

	CRect DwonRectList;
	m_DwonList.GetClientRect(&DwonRectList);
	//获取编程语言列表视图的位置和大小
	m_DwonList.SetExtendedStyle(m_DwonList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DwonList.InsertColumn(0, L"模块名", LVCFMT_CENTER, UpRectList.Width() / 3, 0);
	m_DwonList.InsertColumn(1, L"内存地址", LVCFMT_CENTER, UpRectList.Width() / 3, 1);
	m_DwonList.InsertColumn(2, L"内存大小", LVCFMT_CENTER, UpRectList.Width() / 3, 2);

	

	m_IconList.Create(25, 25, 1, 10, 4);
	GetProcessList();
	std::map<int, SystemProcess>::reverse_iterator iter;
	//DWORD dwSize = m_mapProcess.size();
	DWORD dwTolal = 0;


	m_UpList.SetImageList(&m_IconList, LVSIL_SMALL);
	for (iter = m_mapProcess.rbegin(); iter != m_mapProcess.rend(); iter++)
	{
		
		CString szString;
		CString szString1;
		CString szString2;
		//m_UpList.InsertItem(dwTolal, iter->second.csProcessName);
		m_UpList.InsertItem(dwTolal, NULL, dwTolal);
		m_UpList.SetItemText(dwTolal, 1, iter->second.csProcessName);
		m_UpList.SetItemText(dwTolal, 2, iter->second.dwBit);
		szString.Format(L"%x", iter->second.dwPID);
		m_UpList.SetItemText(dwTolal, 3, szString);
		szString1.Format(L"%x", iter->second.ImageBase);
		m_UpList.SetItemText(dwTolal, 4, szString1);
		szString2.Format(L"%x", iter->second.ImageSize);
		m_UpList.SetItemText(dwTolal, 5, szString2);
		if (iter->second.hIcon != 0)
		{
			m_IconList.Add(iter->second.hIcon);
		}
		else
		{
			m_IconList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
		}
		dwTolal++;
		
		//		 iter->second ;
	}

	
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCPEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCPEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCPEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCPEDlg::OnLvnItemchangedListTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCPEDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType != SIZE_MINIMIZED)
	{
		Resize();
	}
}
void CMFCPEDlg::Resize()
{
	float fsp[2];
	POINT Newp;	//获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);	//取客户端大小
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint;	//左上角
	CPoint OldBRPoint, BRPoint;	//右上角
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = LONG(OldTLPoint.x*fsp[0]);
		TLPoint.y = LONG(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = LONG(OldBRPoint.x*fsp[0]);
		BRPoint.y = LONG(OldBRPoint.y*fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}

#include <Windows.h>

CString IsProcess64Bit(HANDLE handle)
{
	BOOL bIsWow64 = FALSE;
	
	if (IsWow64Process(handle, &bIsWow64))
	{
		if (bIsWow64)
		{
			return L"32Bit"; // 32位进程
		}
		else
		{
			return L"64bit"; // 64位进程
		}
	}
	else
	{
		return L"N/A"; // 获取失败
	}
}

CString GetModeImageByPid(CString szExeFile, DWORD pid, DWORD &imagebsae, DWORD &imagesize)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		//wprintf(L"CreateToolhelp32Snapshot (of modules)");
		return L"";
	}

	me32.dwSize = sizeof(MODULEENTRY32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		//wprintf(TEXT("Module32First"));  // Show cause of failure 
		CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		return L"";
	}
	do
	{
		/*_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
		_tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
		_tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
		_tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
		_tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
		_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
		_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);*/
		if (szExeFile.Compare(me32.szModule) == 0)
		{
			imagesize = me32.modBaseSize;
			imagebsae = (DWORD)me32.modBaseAddr;
			return me32.szExePath;
		}

	} while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
	return L"";
}


VOID CMFCPEDlg::GetModuleList(DWORD pid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		int a = GetLastError();
		//wprintf(L"CreateToolhelp32Snapshot (of modules)");
		return;
	}

	me32.dwSize = sizeof(MODULEENTRY32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		//wprintf(TEXT("Module32First"));  // Show cause of failure 
		CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		return;
	}
	int nCount = 0;
	do
	{
		ProcessMode mode = { 0 };
		mode.szModeName = me32.szModule;
		mode.ImageSize = me32.modBaseSize;
		mode.ImageBase = (DWORD)me32.modBaseAddr;
		/*_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
		_tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
		_tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
		_tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
		_tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
		_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
		_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);*/
		m_ProcessMode.push_back(mode);
		//m_ProcessMode[nCount].ImageSize = me32.modBaseSize;
		//m_ProcessMode[nCount].ImageBase = (DWORD)me32.modBaseAddr;

	} while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
	return ;
}


VOID CMFCPEDlg::GetProcessList()
{
	HANDLE hPrcessSnap = NULL;
	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	m_mapProcess.clear();
	m_setProcess.clear();
	hPrcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hPrcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hPrcessSnap,&pe32))
	{
		int ncount = 0;
		do 
		{

			HMODULE moduleHandles[1024];
			HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
			DWORD dwImageBase = 0;
			DWORD dwImageSize = 0;
			SHFILEINFOW  sfi = { 0 };
			HICON hIcon;

			HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			CString szBit = IsProcess64Bit(handle);
			CString szExePath = GetModeImageByPid(pe32.szExeFile, pe32.th32ProcessID, dwImageBase, dwImageSize);
			if (szExePath.Compare(L"") == 0)
			{
				szExePath = pe32.szExeFile;
			}
			DWORD_PTR  dwRetVal = SHGetFileInfo((LPCTSTR)szExePath, 0, &sfi, sizeof(sfi), SHGFI_SMALLICON | SHGFI_ICON);
			/*if (dwRetVal > 0)
			{
				hIcon = sfi.hIcon;
			}*/
			
			SystemProcess process = { szExePath, szBit, pe32.th32ProcessID, dwImageBase, dwImageSize, sfi.hIcon };
			m_mapProcess[ncount] = process;
			m_setProcess.insert(process);
			ncount++;
			CloseHandle(handle);
		} while (Process32Next(hPrcessSnap,&pe32));
	}
	CloseHandle(hPrcessSnap);


}




void CMFCPEDlg::OnNMClickListTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//*pResult = 0;
	int nIndex = m_UpList.GetSelectionMark();
	CString s = m_UpList.GetItemText(nIndex, 3);
	DWORD Pid = _tcstoul(s, NULL, 16);
	//m_ProcessMode.swap(std::vector<ProcessMode>());
	GetModuleList(Pid);
	m_DwonList.DeleteAllItems();
	for (int i = 0; i < m_ProcessMode.size(); i++)
	{

		CString szString;
		CString szString1;
		m_DwonList.InsertItem(i, m_ProcessMode[i].szModeName);
		szString.Format(L"%x", m_ProcessMode[i].ImageBase);
		m_DwonList.SetItemText(i, 1, szString);
		szString1.Format(L"%x", m_ProcessMode[i].ImageSize);
		m_DwonList.SetItemText(i, 2, szString1);

		//		 iter->second ;
	}

}


void CMFCPEDlg::OnColumnclickListTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	GetProcessList();
	*pResult = 0;
	DWORD dwTolal = 0;
	m_UpList.DeleteAllItems();
	m_UpList.SetImageList(&m_IconList, LVSIL_SMALL);
	for (std::set<SystemProcess, myCompare>::iterator it = m_setProcess.begin(); it != m_setProcess.end(); it++)
	{

		CString szString;
		CString szString1;
		CString szString2;
		m_UpList.InsertItem(dwTolal, NULL, dwTolal);
		m_UpList.SetItemText(dwTolal, 1,  it->csProcessName);
		m_UpList.SetItemText(dwTolal, 2, it->dwBit);
		szString.Format(L"%x", it->dwPID);
		m_UpList.SetItemText(dwTolal, 3, szString);
		szString1.Format(L"%x", it->ImageBase);
		m_UpList.SetItemText(dwTolal, 4, szString1);
		szString2.Format(L"%x", it->ImageSize);
		m_UpList.SetItemText(dwTolal, 5, szString2);
		if (it->hIcon != 0)
		{
			m_IconList.Add(it->hIcon);
		}
		else
		{
			m_IconList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
		}
		dwTolal++;

		//		 iter->second ;
	}
}

void CMFCPEDlg::OnRclickListTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程
		menu.LoadMenu(IDR_MENU1);	//这是我们在1中定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		//下面的两行代码主要是为了后面的操作而准备的
		//获取列表视图控件中第一个被选择项的位置
		//POSITION m_pstion = GetFirstSelectedItemPosition();
		int nIndex = m_UpList.GetSelectionMark();
		CString s = m_UpList.GetItemText(nIndex, 3);
		m_dwPid = _tcstoul(s, NULL, 16);
	}
}


/*void CAboutDlg::OnOpenPE()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox(0, 0, 0);
}*/
TCHAR* CMFCPEDlg::OnGetFileName()
{
	//打开文件对话框
	TCHAR* szFileName = (TCHAR*)malloc(MAX_PATH);
	OPENFILENAMEW openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;//这个必须设置，不设置的话不会出现打开文件对话框  
	openFileName.lpstrFilter = L"文本文件（*.dll*）\0*.dll\0任何文件（*.*）\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&openFileName))// 如果保存则用GetSaveFileName()
	{
		return szFileName;

	}
	return nullptr;
}



void CMFCPEDlg::OnRemoteThreadInjection()
{
	// TODO:  在此添加命令处理程序代码
	TCHAR* szFileName = OnGetFileName();
	if (szFileName == nullptr)
	{
		return;
	}
	//TCHAR szFileName[MAX_PATH] = L"C:\\Users\\DELL\\Desktop\\破\\Dll\\Debug\\Dll.dll";
	//用PID打开进程 参数1：权限 参数2：是否继承 参数3：PID
	HANDLE  Handle = OpenProcess(PROCESS_ALL_ACCESS, false, m_dwPid);
	int a = GetLastError();
	//远程进程中申请空间
	//参数1：进程句柄 参数2：起始地址（nill 决定分配在哪）
	//参数3：要分配的大小（不够一页分一页）参数4：内存分配类型 参数5：内存保护
	LPVOID Address = VirtualAllocEx(Handle, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	int b = GetLastError();
	//3远程进程写入数据
	//也就是LoadLibrary的参数 即 DLL的地址
	SIZE_T RealWrite = 0; //实际写入的地址
	//参数1 ：进程句柄 参数2：写入数据的起始地址 参数3：写入的数据 参数4：写入字节数 参数5：实际写入字节数
	bool ret = WriteProcessMemory(Handle, Address, szFileName, MAX_PATH, &RealWrite);
	if (!ret)
	{
		VirtualFreeEx(Handle, Address, 0, MEM_RELEASE);
		CloseHandle(Handle);
		return;
	}
	int c = GetLastError();
	
	//4获取LoadLibrary函数地址
	//HINSTANCE LibHandle = LoadLibrary(L"kernel32");
	//DWORD ProcAdd = (DWORD)GetProcAddress(LibHandle, "LoadLibraryW");
	FARPROC ProcAdd = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryW");

	//5.在目标进程内创建远程线程
	//线程的起始位置是LoadLibrary 在Windows下所有windows API 在不同的进程中的函数地址都是相同的
	//参数1 进程句柄 参数2：安全描述符（为0默认 ） 参数3：堆栈大小（默认为0）
	//参数4 指向由线程执行的 类型为LPTHREAD_START_POUYINE 的应用程序定义的函数指针
	//参数5 函数的参数指针 参数6 控制线程创建的标志（为0立即执行） 参数7 指向接收线程标识符的变量指针（为0不接收）
	DWORD threadid = 0;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	hThread = CreateRemoteThread(Handle, NULL, NULL, (LPTHREAD_START_ROUTINE)ProcAdd, Address, NULL, &threadid);

	//5.等待远程线程执行完毕
	WaitForSingleObject(hThread, 42294967295);
	//6.释放空间
	//可以选择是否释放注入的模块
	//VirtualFreeEx(Handle, Address, NULL, MEM_RELEASE);

	CloseHandle(Handle);
	CloseHandle(hThread);
	return ;
	
}

// 提权函数：提升为DEBUG权限
BOOL CMFCPEDlg::EnableDebugPrivilege()
{
	HANDLE hToken;
	BOOL fOk = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}

void CMFCPEDlg::OnRclickListDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCPEDlg::OnAPC_Inject()
{
	// TODO:  在此添加命令处理程序代码
	TCHAR* szFileName = OnGetFileName();
	if (szFileName == nullptr)
	{
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwPid);
	if (!hProcess)
	{
		return;
	}
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, m_dwPid);
	if (snap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		return ;
	}
	THREADENTRY32 enty = { 0 };
	enty.dwSize = sizeof(THREADENTRY32);
	FARPROC ProcAdd = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryW");
	LPVOID new_addr = VirtualAllocEx(hProcess, 0, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, new_addr, szFileName, 0x1000, 0);
	Thread32First(snap, &enty);
	HANDLE htHread = 0;
	do 
	{
		if (enty.th32OwnerProcessID == m_dwPid)
		{
			htHread = OpenThread(THREAD_ALL_ACCESS, false, enty.th32ThreadID);
			if (htHread)
			{
				QueueUserAPC((PAPCFUNC)ProcAdd, htHread, (ULONG_PTR)new_addr);
				CloseHandle(htHread); 
			}
		}
	} while (Thread32Next(snap,&enty));
	CloseHandle(snap);
	CloseHandle(hProcess);
	return;
}


void CMFCPEDlg::OnEIP_Inject()
{
	// TODO:  在此添加命令处理程序代码
	TCHAR* szFileName = OnGetFileName();
	if (szFileName == nullptr)
	{
		return;
	}
	HWND hwnd = ::FindWindow(NULL, L"");
	/*
	思路：
	1.查找窗口，获得窗口句柄
	2.获得线程ID进程PID
	3.获得线程句柄,同时也要获得进程的句柄
	4.挂起线程
	5.获得寄存器的值
	6.修改EIP的值
	7.申请远程内存
	8.写入远程内存，把EIP也要写进去，这样远程执行完毕之后会切换回来继续执行
	9.恢复线程
	10.关闭线程句柄
	*/
}




char* TCHAR2char(TCHAR* STR)
{

	//返回字符串的长度

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);

	//申请一个多字节的字符串变量

	char* pszMultiByte = (char*)malloc(size*sizeof(char));;

	//将STR转成str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, pszMultiByte, size, NULL, NULL);

	return pszMultiByte;

}




void CMFCPEDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR* strFile = OnGetFileName();
	char* strChar = TCHAR2char(strFile);
	FILE * file = fopen(strChar, "rb+");
	DWORD dwFile_Size = 0;

	TCHAR* pFilePe;
	if (file)
	{
		fseek(file, 0L, SEEK_END); /* 定位到文件末尾 */
		dwFile_Size = ftell(file);
		pFilePe = (TCHAR*)malloc(dwFile_Size); /* 根据文件大小动态分配内存空间 */
		ZeroMemory(pFilePe, dwFile_Size);
		if (pFilePe == NULL)
		{
			fclose(file);
			return ;
		}
		fseek(file, 0L, SEEK_SET); /* 定位到文件开头 */
		fread(pFilePe, dwFile_Size, 1, file); /* 一次性读取全部文件内容 */
		fclose(file);
		PeLoad* peload = new PeLoad(pFilePe, dwFile_Size);
		peload->Create(IDD_DIALOG1, this);
		peload->ShowWindow(SW_SHOW);
	}
	return;
}
