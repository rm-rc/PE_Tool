
// MFC-PEDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC-PE.h"
#include "MFC-PEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCPEDlg �Ի���



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


// CMFCPEDlg ��Ϣ�������

BOOL CMFCPEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	EnableDebugPrivilege();
	CRect rect;
	GetClientRect(&rect);
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;



	CRect UpRectList;
	m_UpList.GetClientRect(&UpRectList);
	//��ȡ��������б���ͼ��λ�úʹ�С
	m_UpList.SetExtendedStyle(m_UpList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_UpList.InsertColumn(0, L"ͼ��", LVCFMT_CENTER, UpRectList.Width() / 6, 0);
	m_UpList.InsertColumn(1, L"����", LVCFMT_CENTER, UpRectList.Width() / 6, 0);
	m_UpList.InsertColumn(2, L"architecture", LVCFMT_CENTER, UpRectList.Width() / 6, 1);
	m_UpList.InsertColumn(3, L"����PID", LVCFMT_CENTER, UpRectList.Width() / 6, 2);
	m_UpList.InsertColumn(4, L"�ڴ��ַ", LVCFMT_CENTER, UpRectList.Width() / 6, 3);
	m_UpList.InsertColumn(5, L"�ڴ��С", LVCFMT_CENTER, UpRectList.Width() / 6, 4);

	CRect DwonRectList;
	m_DwonList.GetClientRect(&DwonRectList);
	//��ȡ��������б���ͼ��λ�úʹ�С
	m_DwonList.SetExtendedStyle(m_DwonList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DwonList.InsertColumn(0, L"ģ����", LVCFMT_CENTER, UpRectList.Width() / 3, 0);
	m_DwonList.InsertColumn(1, L"�ڴ��ַ", LVCFMT_CENTER, UpRectList.Width() / 3, 1);
	m_DwonList.InsertColumn(2, L"�ڴ��С", LVCFMT_CENTER, UpRectList.Width() / 3, 2);

	

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

	
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCPEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCPEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCPEDlg::OnLvnItemchangedListTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMFCPEDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType != SIZE_MINIMIZED)
	{
		Resize();
	}
}
void CMFCPEDlg::Resize()
{
	float fsp[2];
	POINT Newp;	//��ȡ���ڶԻ���Ĵ�С
	CRect recta;
	GetClientRect(&recta);	//ȡ�ͻ��˴�С
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint;	//���Ͻ�
	CPoint OldBRPoint, BRPoint;	//���Ͻ�
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
			return L"32Bit"; // 32λ����
		}
		else
		{
			return L"64bit"; // 64λ����
		}
	}
	else
	{
		return L"N/A"; // ��ȡʧ��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����߳�
		menu.LoadMenu(IDR_MENU1);	//����������1�ж����MENU���ļ�����
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		//��������д�����Ҫ��Ϊ�˺���Ĳ�����׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��
		//POSITION m_pstion = GetFirstSelectedItemPosition();
		int nIndex = m_UpList.GetSelectionMark();
		CString s = m_UpList.GetItemText(nIndex, 3);
		m_dwPid = _tcstoul(s, NULL, 16);
	}
}


/*void CAboutDlg::OnOpenPE()
{
	// TODO:  �ڴ���������������
	MessageBox(0, 0, 0);
}*/
TCHAR* CMFCPEDlg::OnGetFileName()
{
	//���ļ��Ի���
	TCHAR* szFileName = (TCHAR*)malloc(MAX_PATH);
	OPENFILENAMEW openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEW);
	openFileName.nMaxFile = MAX_PATH;//����������ã������õĻ�������ִ��ļ��Ի���  
	openFileName.lpstrFilter = L"�ı��ļ���*.dll*��\0*.dll\0�κ��ļ���*.*��\0*.*\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&openFileName))// �����������GetSaveFileName()
	{
		return szFileName;

	}
	return nullptr;
}



void CMFCPEDlg::OnRemoteThreadInjection()
{
	// TODO:  �ڴ���������������
	TCHAR* szFileName = OnGetFileName();
	if (szFileName == nullptr)
	{
		return;
	}
	//TCHAR szFileName[MAX_PATH] = L"C:\\Users\\DELL\\Desktop\\��\\Dll\\Debug\\Dll.dll";
	//��PID�򿪽��� ����1��Ȩ�� ����2���Ƿ�̳� ����3��PID
	HANDLE  Handle = OpenProcess(PROCESS_ALL_ACCESS, false, m_dwPid);
	int a = GetLastError();
	//Զ�̽���������ռ�
	//����1�����̾�� ����2����ʼ��ַ��nill �����������ģ�
	//����3��Ҫ����Ĵ�С������һҳ��һҳ������4���ڴ�������� ����5���ڴ汣��
	LPVOID Address = VirtualAllocEx(Handle, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	int b = GetLastError();
	//3Զ�̽���д������
	//Ҳ����LoadLibrary�Ĳ��� �� DLL�ĵ�ַ
	SIZE_T RealWrite = 0; //ʵ��д��ĵ�ַ
	//����1 �����̾�� ����2��д�����ݵ���ʼ��ַ ����3��д������� ����4��д���ֽ��� ����5��ʵ��д���ֽ���
	bool ret = WriteProcessMemory(Handle, Address, szFileName, MAX_PATH, &RealWrite);
	if (!ret)
	{
		VirtualFreeEx(Handle, Address, 0, MEM_RELEASE);
		CloseHandle(Handle);
		return;
	}
	int c = GetLastError();
	
	//4��ȡLoadLibrary������ַ
	//HINSTANCE LibHandle = LoadLibrary(L"kernel32");
	//DWORD ProcAdd = (DWORD)GetProcAddress(LibHandle, "LoadLibraryW");
	FARPROC ProcAdd = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryW");

	//5.��Ŀ������ڴ���Զ���߳�
	//�̵߳���ʼλ����LoadLibrary ��Windows������windows API �ڲ�ͬ�Ľ����еĺ�����ַ������ͬ��
	//����1 ���̾�� ����2����ȫ��������Ϊ0Ĭ�� �� ����3����ջ��С��Ĭ��Ϊ0��
	//����4 ָ�����߳�ִ�е� ����ΪLPTHREAD_START_POUYINE ��Ӧ�ó�����ĺ���ָ��
	//����5 �����Ĳ���ָ�� ����6 �����̴߳����ı�־��Ϊ0����ִ�У� ����7 ָ������̱߳�ʶ���ı���ָ�루Ϊ0�����գ�
	DWORD threadid = 0;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	hThread = CreateRemoteThread(Handle, NULL, NULL, (LPTHREAD_START_ROUTINE)ProcAdd, Address, NULL, &threadid);

	//5.�ȴ�Զ���߳�ִ�����
	WaitForSingleObject(hThread, 42294967295);
	//6.�ͷſռ�
	//����ѡ���Ƿ��ͷ�ע���ģ��
	//VirtualFreeEx(Handle, Address, NULL, MEM_RELEASE);

	CloseHandle(Handle);
	CloseHandle(hThread);
	return ;
	
}

// ��Ȩ����������ΪDEBUGȨ��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMFCPEDlg::OnAPC_Inject()
{
	// TODO:  �ڴ���������������
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
	// TODO:  �ڴ���������������
	TCHAR* szFileName = OnGetFileName();
	if (szFileName == nullptr)
	{
		return;
	}
	HWND hwnd = ::FindWindow(NULL, L"");
	/*
	˼·��
	1.���Ҵ��ڣ���ô��ھ��
	2.����߳�ID����PID
	3.����߳̾��,ͬʱҲҪ��ý��̵ľ��
	4.�����߳�
	5.��üĴ�����ֵ
	6.�޸�EIP��ֵ
	7.����Զ���ڴ�
	8.д��Զ���ڴ棬��EIPҲҪд��ȥ������Զ��ִ�����֮����л���������ִ��
	9.�ָ��߳�
	10.�ر��߳̾��
	*/
}




char* TCHAR2char(TCHAR* STR)
{

	//�����ַ����ĳ���

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);

	//����һ�����ֽڵ��ַ�������

	char* pszMultiByte = (char*)malloc(size*sizeof(char));;

	//��STRת��str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, pszMultiByte, size, NULL, NULL);

	return pszMultiByte;

}




void CMFCPEDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR* strFile = OnGetFileName();
	char* strChar = TCHAR2char(strFile);
	FILE * file = fopen(strChar, "rb+");
	DWORD dwFile_Size = 0;

	TCHAR* pFilePe;
	if (file)
	{
		fseek(file, 0L, SEEK_END); /* ��λ���ļ�ĩβ */
		dwFile_Size = ftell(file);
		pFilePe = (TCHAR*)malloc(dwFile_Size); /* �����ļ���С��̬�����ڴ�ռ� */
		ZeroMemory(pFilePe, dwFile_Size);
		if (pFilePe == NULL)
		{
			fclose(file);
			return ;
		}
		fseek(file, 0L, SEEK_SET); /* ��λ���ļ���ͷ */
		fread(pFilePe, dwFile_Size, 1, file); /* һ���Զ�ȡȫ���ļ����� */
		fclose(file);
		PeLoad* peload = new PeLoad(pFilePe, dwFile_Size);
		peload->Create(IDD_DIALOG1, this);
		peload->ShowWindow(SW_SHOW);
	}
	return;
}
