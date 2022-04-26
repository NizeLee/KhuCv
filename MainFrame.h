//  MainFrame.h: interface of CMainFrame (parent frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once

enum
{
    ID_QUIT = 1,
    IDM_OPEN,
    IDM_FILE_OPEN_RECENT,
    IDM_ABOUT,

    IDM_CONTEXT_COPY = 51,
    IDM_CONTEXT_PASTE,
    IDM_CONTEXT_DUPLICATE,

    IDC_SEL_FILE_FOLDER = 101,
    IDC_DISPLAY_PATH,
    IDC_SEL_FILE_FOLDER_CHECK,
    IDC_FILE_LIST_CTRL,
    IDC_START_NUM,
    IDC_END_NUM,
    IDC_CLEAR_IMAGES_CHECK,
    IDC_RUN,
    IDC_PAUSE,
    IDC_SEL_STEP_CHECK,
    IDC_EXAMPLE,

    IDC_PRINT_LIST = 201,

    ID_TIMER_SEQUENCE_RUN = 1001,
    ID_TIMER_VIDEO_RUN,
};

class CMainFrame : public wxMDIParentFrame {
public:
    CMainFrame(const wxString& title);
    ~CMainFrame();

    void DlgPrintf(const char* ptr, ...);
 
private:
    void OnDropFiles(wxDropFilesEvent&);
    void OnQuit(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);
    void OnRecentFiles(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    wxAuiManager m_AuiMgr; 
    wxListBox* m_pPrintListBox;

    wxFileHistory* m_pFileHistory;
    wxConfig* m_pConfig;

    DECLARE_EVENT_TABLE();
};






