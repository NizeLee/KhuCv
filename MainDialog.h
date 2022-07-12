//  MainDialog.h: interface of CMainDialog (main dialog of KhuCvApp)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once

#include "Project.h"

#define H_BOX_SIZER_NUM	6

class CMainDialog : public wxDialog
{
public:
	CMainDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
	virtual ~CMainDialog();

	wxBoxSizer* m_pVbox = new wxBoxSizer(wxVERTICAL);
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
	wxCheckBox* m_pClearImagesCheck;

	wxCheckBox* m_pSelCam;
	wxButton* m_pRunButton;
	wxButton* m_pPauseButton;
	wxCheckBox* m_pStepCheck;
	wxCheckBox* m_pVerboseCheck;
	
	wxButton* m_pExampleButton;

	wxTimer m_SequenceRunTimer;
	int m_nProcessingNum;
	bool m_bRunTimer, m_bRunPause;

	wxTimer m_VideoRunTimer;
	cv::VideoCapture m_VideoProcessingVc;
	cv::VideoCapture m_CamProcessingVc;
	char m_VideoFileName[256];

	wxTimer m_CamRunTimer;

	CProject m_Project;

	void OnSelSrcFileFolder(wxCommandEvent& event);
	void OnSelDesFileFolder(wxCommandEvent& event);
	void OnActivatedFileListCtrl(wxListEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnPause(wxCommandEvent& event);
	void OnExample(wxCommandEvent& event);


	DECLARE_EVENT_TABLE();
};

