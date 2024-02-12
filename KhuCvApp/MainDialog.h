//  MainDialog.h: interface of CMainDialog (main dialog of KhuCvApp)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.5.0

#pragma once

#include "Project.h"

#ifndef _KHUCV_SDI
#define H_BOX_SIZER_NUM	6
#else
#define H_BOX_SIZER_NUM	7
#endif

class CMainDialog : public wxDialog
{
public:
	CMainDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
	virtual ~CMainDialog();
	
	wxBoxSizer* m_pVbox;
	wxBoxSizer* m_pHbox[H_BOX_SIZER_NUM];

	wxButton* m_pSrcFolderSelButton;
	wxCheckBox* m_pVideoFileCheck;
	wxStaticText* m_pDisplaySrcPathText;

	wxButton* m_pDesFolderSelButton;
	wxCheckBox* m_pSaveFrameCheck;
	wxStaticText* m_pDisplayDesPathText;

	wxListCtrl* m_pListCtrl;

	wxTextCtrl* m_pStartNum;
	wxTextCtrl* m_pEndNum;

	wxComboBox* m_pSelCam;
	wxButton* m_pRunButton;
	wxButton* m_pPauseButton;
	wxCheckBox* m_pStepCheck;
	wxCheckBox* m_pVerboseCheck;
	
	wxButton* m_pReverseButton;
	wxButton* m_pAddButton;
	wxButton* m_pHistogramButton;
	wxButton* m_pThreLabelingButton;
	wxButton* m_pExample1Button;
	wxButton* m_pExample2Button;
	wxButton* m_pExample3Button;
	wxButton* m_pExample4Button;
	wxButton* m_pExample5Button;
#ifndef _KHUCV_SDI
#else
	wxListBox* m_pPrintListBox;
#endif

	wxTimer m_SequenceRunTimer;
	int m_nProcessingNum;
	std::wstring m_strSaveFileNameHeader;
	bool m_bRunTimer, m_bRunPause;

	wxTimer m_VideoRunTimer;
	cv::VideoCapture m_VideoProcessingVc;
	cv::VideoCapture m_CamProcessingVc;
	std::wstring m_VideoFileName;
	std::wstring m_VideoFilePath;
	std::string m_VideoFileNameUtf8;
	std::string m_VideoFilePathUtf8;
	int m_nVideoFileFrameCnt;

	wxTimer m_CamRunTimer;

	bool m_bFirstRun;

	CProject m_Project;
#ifndef _KHUCV_SDI
#else
	wxListBox* GetPrintListBox() const;
#endif
	
	void OnSelSrcFileFolder(wxCommandEvent& event);
	void OnSelDesFileFolder(wxCommandEvent& event);
	void OnActivatedFileListCtrl(wxListEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnPause(wxCommandEvent& event);
	void OnReverse(wxCommandEvent& event);
	void OnAddImages(wxCommandEvent& event);
	void OnHistogram(wxCommandEvent& event);
	void OnLabeling(wxCommandEvent& event);
	void OnExample1(wxCommandEvent& event);
	void OnExample2(wxCommandEvent& event);
	void OnExample3(wxCommandEvent& event);
	void OnExample4(wxCommandEvent& event);
	void OnExample5(wxCommandEvent& event);

	DECLARE_EVENT_TABLE();
};

