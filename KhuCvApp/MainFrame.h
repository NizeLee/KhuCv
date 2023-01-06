//  MainFrame.h: interface of CMainFrame (parent frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once

enum
{
    ID_QUIT = 1,
    IDM_OPEN,
    IDM_SAVE,
    IDM_FILE_OPEN_RECENT,
    IDM_VIWE_MAIN_DLG,
    IDM_VIWE_OUTPUT_WND,
    IDM_ABOUT,

    IDM_CONTEXT_COPY = 51,
    IDM_CONTEXT_PASTE,
    IDM_CONTEXT_DUPLICATE,
    IDM_CONTEXT_ZOOM_IN,
    IDM_CONTEXT_ZOOM_OUT,
    IDM_CONTEXT_SEL_REGION,

    IDC_SEL_FILE_FOLDER = 101,
    IDC_DISPLAY_SRC_PATH,
    IDC_VIDEO_FILE_CHECK,
    IDC_SEL_DES_FOLDER,
    IDC_DISPLAY_DES_PATH,
    IDC_SAVE_FRAME_CHECK,
    IDC_FILE_LIST_CTRL,
    IDC_START_NUM,
    IDC_END_NUM,
    IDC_SEL_CAM,
    IDC_RUN,
    IDC_PAUSE,
    IDC_SEL_STEP_CHECK,
    IDC_SEL_VERBOSE_CHECK, 
    IDC_EXAMPLE,

    IDC_PRINT_LIST = 201,

    ID_TIMER_SEQUENCE_RUN = 1001,
    ID_TIMER_VIDEO_RUN,
    ID_TIMER_CAM_RUN,
};

#ifndef _KHUCV_SDI
#else
class CKcImage
{
public:
    cv::Mat cvImage;
    wxPoint pos;

    CKcImage() {};
    CKcImage(cv::Mat mat, wxPoint p) : cvImage(mat), pos(p) {};
};
#endif

#ifndef _KHUCV_SDI
class CMainFrame : public wxMDIParentFrame {
#else
class CMainFrame : public wxFrame {
#endif
public:
    CMainFrame(const wxString& title);
    ~CMainFrame();
#ifndef _KHUCV_SDI
    wxListBox* GetPrintListBox() const;
#else
    void DispalyLastImage();
    void ClearAllImages();

    CClientView* m_pClientView;
    std::vector<CKcImage> m_ImageList;
#endif
 
private:
    void OnDropFiles(wxDropFilesEvent&);
    void OnQuit(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);
    void OnFileSave(wxCommandEvent& event);
    void OnRecentFiles(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void OnUpdateMenuCheckViewMainDlg(wxUpdateUIEvent& event);
    void OnMenuCheckViewMainDlg(wxCommandEvent& event);

#ifndef _KHUCV_SDI
    void OnUpdateMenuCheckViewOutputWnd(wxUpdateUIEvent& event);
    void OnMenuCheckViewOutputWnd(wxCommandEvent& event);
#endif

    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnDuplicate(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);

#ifndef _KHUCV_SDI
    wxAuiManager m_AuiMgrOutputWnd; 
    wxListBox* m_pPrintListBox;
#endif

    wxFileHistory* m_pFileHistory;
    wxConfig* m_pConfig;

    DECLARE_EVENT_TABLE();
};






