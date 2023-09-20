#pragma once
#include <list>
#include "afxwin.h"
#include "PeHex.h"
#include <iostream>

// PeLoad 对话框



class PeLoad : public CDialogEx
{
	DECLARE_DYNAMIC(PeLoad)

public:
	PeLoad( TCHAR* pFilePe, DWORD dwFile_Size, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PeLoad();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL OpenFileData(TCHAR* FileData);
	VOID ShowOpenPe();
	TCHAR*  m_pFilePe;
	DWORD m_dwFile_Size;
	IMAGE_DOS_HEADER File_Dos;		//DOS头数据
	IMAGE_FILE_HEADER File_Pe;		//标准PE头数据

	IMAGE_DATA_DIRECTORY DataDirectry[IMAGE_NUMBEROF_DIRECTORY_ENTRIES] = {};
	//节表记录
	std::list<IMAGE_SECTION_HEADER> list_Section;

	union
	{
		IMAGE_OPTIONAL_HEADER32 image_Opeional32;
		IMAGE_OPTIONAL_HEADER64 image_Opeional64;
	}File_Optional;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Magic
	CEdit m_Magic;
	CEdit m_MajorLinkVersion;
	CEdit m_MinorLinkerVersion;
	CEdit m_SizeOfCode;
	CEdit m_SizeOfIntializedData;
	CEdit m_SizeOfUninitializedData;
	CEdit m_AddressOfEntryPoint;
	CEdit m_BaseOfCode;
	CEdit m_BaseOfData;
	CEdit m_ImageBase;
	CEdit m_SectionAlignment;
	CEdit m_FileAlignment;
	CEdit m_MajorOperatingSystemVersion;
	CEdit m_MinorOperatingSystemVersion;
	CEdit m_MajorImageVersion;
	CEdit m_MinorImageVersion;
	CEdit m_MajorSubsystemVersion;
	CEdit m_MinorSubsystemVersion;
	CEdit m_Win32VersionValue;
	CEdit m_SizeOfImage;
	CEdit m_SizeofHeaders;
	CEdit m_CheckStem;
	CEdit m_Subsystem;
	CEdit m_DllCharacteristics;
	CEdit m_SizeOfStackReserve;
	CEdit m_SizeofStackCommit;
	CEdit m_SizeofHeapReserve;
	CEdit m_SizeOfHeapCommit;
	CEdit m_LoaderFlags;
	CEdit m_NumberOfRvaAndSize;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
