//  MainFrame.h: interface of CMainFrame (parent frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.1.0.0

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

    IDM_CONTEXT_CUT = 51,
    IDM_CONTEXT_COPY,
    IDM_CONTEXT_PASTE,
    IDM_CONTEXT_DUPLICATE,
    IDM_CONTEXT_ZOOM_IN,
    IDM_CONTEXT_ZOOM_OUT,
    IDM_CONTEXT_ZOOM_NN,
    IDM_CONTEXT_ZOOM_BL,
    IDM_CONTEXT_ZOOM_BC,
    IDM_CONTEXT_ADD_IMAGES,
    IDM_CONTEXT_SUBTRACT_IMAGES,
    IDM_CONTEXT_SEL_REGION,
    IDM_CONTEXT_IMAGE_INFOMATION,

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
    IDC_SEL_PROJ_NUM,
    IDC_SEL_PROJ_NUM_LABEL,
    IDC_RUN,
    IDC_PAUSE,
    IDC_SEL_STEP_CHECK,
    IDC_SEL_VERBOSE_CHECK, 
    IDC_REVERSE,
    IDC_THRESHOLDING,
    IDC_HISTOGRAM,
    IDC_LABDELING,
    IDC_EXAMPLE0,
    IDC_EXAMPLE1,
    IDC_EXAMPLE2,
    IDC_EXAMPLE3,
    IDC_EXAMPLE4,
    IDC_EXAMPLE5,
    IDC_EXAMPLE6,
    IDC_EXAMPLE7,
    IDC_EXAMPLE8,
    IDC_EXAMPLE9,
    IDC_SAVE_LIST,

    IDC_PRINT_LIST = 201,

    ID_TIMER_SEQUENCE_RUN = 1001,
    ID_TIMER_VIDEO_RUN,
    ID_TIMER_CAM_RUN,
    ID_TIMER_IMAGE_LIST_RUN,

    IDC_SLIDER_THRESHOLD = 2001,
    IDC_DISPLAY_THRESHOLD,
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

    void OnCut(wxCommandEvent& event);
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






