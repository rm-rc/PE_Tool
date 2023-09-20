// PeLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC-PE.h"
#include "PeLoad.h"
#include "afxdialogex.h"


// PeLoad 对话框

IMPLEMENT_DYNAMIC(PeLoad, CDialogEx)

PeLoad::PeLoad(TCHAR* pFilePe,DWORD dwFile_Size,CWnd* pParent /*=NULL*/)
	: CDialogEx(PeLoad::IDD, pParent)
{
	m_pFilePe = pFilePe;
	m_dwFile_Size = dwFile_Size;
}

PeLoad::~PeLoad()
{
	
}

void PeLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Magic);
	DDX_Control(pDX, IDC_EDIT2, m_MajorLinkVersion);
	DDX_Control(pDX, IDC_EDIT3, m_MinorLinkerVersion);
	DDX_Control(pDX, IDC_EDIT4, m_SizeOfCode);
	DDX_Control(pDX, IDC_EDIT5, m_SizeOfIntializedData);
	DDX_Control(pDX, IDC_EDIT6, m_SizeOfUninitializedData);
	DDX_Control(pDX, IDC_EDIT7, m_AddressOfEntryPoint);
	DDX_Control(pDX, IDC_EDIT8, m_BaseOfCode);
	DDX_Control(pDX, IDC_EDIT9, m_BaseOfData);
	DDX_Control(pDX, IDC_EDIT10, m_ImageBase);
	DDX_Control(pDX, IDC_EDIT11, m_SectionAlignment);
	DDX_Control(pDX, IDC_EDIT12, m_FileAlignment);
	DDX_Control(pDX, IDC_EDIT13, m_MajorOperatingSystemVersion);
	DDX_Control(pDX, IDC_EDIT14, m_MinorOperatingSystemVersion);
	DDX_Control(pDX, IDC_EDIT15, m_MajorImageVersion);
	DDX_Control(pDX, IDC_EDIT16, m_MinorImageVersion);
	DDX_Control(pDX, IDC_EDIT17, m_MajorSubsystemVersion);
	DDX_Control(pDX, IDC_EDIT18, m_MinorSubsystemVersion);
	DDX_Control(pDX, IDC_EDIT19, m_Win32VersionValue);
	DDX_Control(pDX, IDC_EDIT20, m_SizeOfImage);
	DDX_Control(pDX, IDC_EDIT21, m_SizeofHeaders);
	DDX_Control(pDX, IDC_EDIT22, m_CheckStem);
	DDX_Control(pDX, IDC_EDIT23, m_Subsystem);
	DDX_Control(pDX, IDC_EDIT24, m_DllCharacteristics);
	DDX_Control(pDX, IDC_EDIT25, m_SizeOfStackReserve);
	DDX_Control(pDX, IDC_EDIT26, m_SizeofStackCommit);
	DDX_Control(pDX, IDC_EDIT27, m_SizeofHeapReserve);
	DDX_Control(pDX, IDC_EDIT28, m_SizeOfHeapCommit);
	DDX_Control(pDX, IDC_EDIT29, m_LoaderFlags);
	DDX_Control(pDX, IDC_EDIT30, m_NumberOfRvaAndSize);
}


BEGIN_MESSAGE_MAP(PeLoad, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PeLoad::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// PeLoad 消息处理程序


BOOL PeLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OpenFileData(m_pFilePe);
	ShowOpenPe();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


BOOL PeLoad::OpenFileData(TCHAR* FileData)
{
	WORD MZ = (WORD)*FileData;
	if (MZ != 0x5A4D)
	{
		//printf("OpenFileData Not MZ");
		MessageBox(L"错误", L"Not MZ", 1);
		return FALSE;
	}

	//获取Dos头
	memcpy(&File_Dos, FileData, sizeof(IMAGE_DOS_HEADER));
	//对DOS头 标准PE头  可选PE头 进行拷贝
	char* address = (char*)FileData + 4 + File_Dos.e_lfanew;
	memcpy(&File_Pe, address, sizeof(IMAGE_FILE_HEADER));
	//printf("32位%d   64位%d", sizeof(IMAGE_OPTIONAL_HEADER32), sizeof(IMAGE_OPTIONAL_HEADER64));


	if (File_Pe.SizeOfOptionalHeader == 0xE0)
	{
		address = (char*)FileData + 4 + File_Dos.e_lfanew + sizeof(IMAGE_FILE_HEADER);
		memcpy(&File_Optional.image_Opeional32, address, File_Pe.SizeOfOptionalHeader);
		memcpy(DataDirectry, File_Optional.image_Opeional32.DataDirectory, sizeof(DataDirectry));
	}
	else
	{
		address = (char*)FileData + 4 + File_Dos.e_lfanew + sizeof(IMAGE_FILE_HEADER);
		memcpy(&File_Optional.image_Opeional64, address, File_Pe.SizeOfOptionalHeader);
		memcpy(DataDirectry, File_Optional.image_Opeional64.DataDirectory, sizeof(DataDirectry));
	}
	//获取一下从DOS头到可选PE头的指针  头指针 + DOS头偏移 + 标准PE大小 + 可选PE大小
	TCHAR* opFile = FileData + 4 + File_Dos.e_lfanew + sizeof(IMAGE_FILE_HEADER) + File_Pe.SizeOfOptionalHeader;


	//节表保存
	for (size_t i = 0; i < File_Pe.NumberOfSections; i++)
	{
		IMAGE_SECTION_HEADER section = { 0 };
		memcpy(&section, opFile, sizeof(IMAGE_SECTION_HEADER));
		list_Section.push_back(section);
		opFile += sizeof(IMAGE_SECTION_HEADER);
	}



}

VOID PeLoad::ShowOpenPe()
{
	TCHAR str[MAX_PATH] = { 0 };
	//CString str;
	//str.Format(L"%05d", File_Optional.image_Opeional32.Magic);
	_itow(File_Optional.image_Opeional32.Magic, str, 16);
	m_Magic.SetWindowTextW(_wcsupr(str));
	_itow(File_Optional.image_Opeional32.MajorLinkerVersion, str, 16);
	m_MajorLinkVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MinorLinkerVersion, str, 16);
	m_MinorLinkerVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfCode, str, 16);
	m_SizeOfCode.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfInitializedData, str, 16);
	m_SizeOfIntializedData.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfUninitializedData, str, 16);
	m_SizeOfUninitializedData.SetWindowTextW(_wcsupr(str));

	///////////////////
	_itow(File_Optional.image_Opeional32.AddressOfEntryPoint, str, 16);
	m_AddressOfEntryPoint.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.BaseOfCode, str, 16);
	m_BaseOfCode.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.BaseOfData, str, 16);
	m_BaseOfData.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.ImageBase, str, 16);
	m_ImageBase.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SectionAlignment, str, 16);
	m_SectionAlignment.SetWindowTextW(_wcsupr(str));

	////////
	_itow(File_Optional.image_Opeional32.FileAlignment, str, 16);
	m_FileAlignment.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MajorOperatingSystemVersion, str, 16);
	m_MajorOperatingSystemVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MinorOperatingSystemVersion, str, 16);
	m_MinorOperatingSystemVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MajorImageVersion, str, 16);
	m_MajorImageVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MinorImageVersion, str, 16);
	m_MinorImageVersion.SetWindowTextW(_wcsupr(str));


	//
	_itow(File_Optional.image_Opeional32.MajorSubsystemVersion, str, 16);
	m_MajorSubsystemVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.MinorSubsystemVersion, str, 16);
	m_MinorSubsystemVersion.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.Win32VersionValue, str, 16);
	m_Win32VersionValue.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfImage, str, 16);
	m_SizeOfImage.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfHeaders, str, 16);
	m_SizeofHeaders.SetWindowTextW(_wcsupr(str));

	//
	_itow(File_Optional.image_Opeional32.CheckSum, str, 16);
	m_CheckStem.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.Subsystem, str, 16);
	m_Subsystem.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.DllCharacteristics, str, 16);
	m_DllCharacteristics.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfStackReserve, str, 16);
	m_SizeOfStackReserve.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfStackCommit, str, 16);
	m_SizeofStackCommit.SetWindowTextW(_wcsupr(str));

	//
	_itow(File_Optional.image_Opeional32.SizeOfHeapReserve, str, 16);
	m_SizeofHeapReserve.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.SizeOfHeapCommit, str, 16);
	m_SizeOfHeapCommit.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.LoaderFlags, str, 16);
	m_LoaderFlags.SetWindowTextW(_wcsupr(str));

	_itow(File_Optional.image_Opeional32.NumberOfRvaAndSizes, str, 16);
	m_NumberOfRvaAndSize.SetWindowTextW(_wcsupr(str));

}


BOOL PeLoad::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


void PeLoad::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	PeHex* peHex = new PeHex(m_pFilePe,m_dwFile_Size);
	peHex->Create(IDD_DIALOG2, this);
	peHex->ShowWindow(SW_SHOW);
}


void PeLoad::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	free(m_pFilePe);
	CDialogEx::OnClose();
}
