//  MainFrame.cpp: implementation of CMainFrame (parent frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

BEGIN_EVENT_TABLE(CMainFrame, wxMDIParentFrame)
EVT_DROP_FILES(CMainFrame::OnDropFiles)
EVT_MENU(ID_QUIT, CMainFrame::OnQuit)
EVT_MENU(IDM_OPEN, CMainFrame::OnFileOpen)
EVT_MENU(IDM_SAVE, CMainFrame::OnFileSave)
EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, CMainFrame::OnRecentFiles)
EVT_MENU(IDM_ABOUT, CMainFrame::OnAbout)
EVT_CLOSE(CMainFrame::OnClose)
EVT_UPDATE_UI(IDM_VIWE_MAIN_DLG, CMainFrame::OnUpdateMenuCheckViewMainDlg)
EVT_MENU(IDM_VIWE_MAIN_DLG, CMainFrame::OnMenuCheckViewMainDlg)
EVT_UPDATE_UI(IDM_VIWE_OUTPUT_WND, CMainFrame::OnUpdateMenuCheckViewOutputWnd)
EVT_MENU(IDM_VIWE_OUTPUT_WND, CMainFrame::OnMenuCheckViewOutputWnd)
END_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title)
    : wxMDIParentFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(1920, 1080))
{
    m_pConfig = new wxConfig(title);
   
    wxMenu* menuFile = new wxMenu;
    wxMenu* menuRecent = new wxMenu;
    menuFile->Append(IDM_OPEN, "&Open File...\tCtrl+O");
    menuFile->Append(IDM_SAVE, "&Open Save...\tCtrl+S");
    menuFile->AppendSubMenu(menuRecent, "Open Recent");
    menuFile->AppendSeparator();
    menuFile->Append(ID_QUIT, "&Quit");

    wxMenu* menuView = new wxMenu;
    menuView->AppendCheckItem(IDM_VIWE_MAIN_DLG, "&Main Dlg");
    menuView->AppendCheckItem(IDM_VIWE_OUTPUT_WND, "&Output");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(IDM_ABOUT, "&KhuCv App...");
  
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    m_pFileHistory = new wxFileHistory(10);
    
    m_pFileHistory->UseMenu(menuRecent);
    m_pFileHistory->AddFilesToMenu(menuRecent);
    m_pFileHistory->Load(*m_pConfig);
    
    m_AuiMgrOutputWnd.SetManagedWindow(this);
 
    m_pPrintListBox = new wxListBox(this, IDC_PRINT_LIST, wxDefaultPosition, wxSize(1920, 150));
    m_AuiMgrOutputWnd.AddPane(m_pPrintListBox, wxBOTTOM, wxT("Output"));
    m_AuiMgrOutputWnd.Update();

    CChildFrame *pChildFrame = new CChildFrame(this, wxID_ANY, "KhuCv Image");

    pChildFrame->Show();
}

CMainFrame::~CMainFrame() {
    m_AuiMgrOutputWnd.UnInit();
}

wxListBox* CMainFrame::GetPrintListBox() const {
    return m_pPrintListBox;
}

void CMainFrame::OnDropFiles(wxDropFilesEvent& event) {
    const wxString* files = event.GetFiles();

    for(int i = 0 ; i < event.GetNumberOfFiles() ; ++i) {
        wxFileName wxFileNameIst(files[i]);
    
        char filePath[256], fileName[256];
        strcpy(filePath, files[i]);
        strcpy(fileName, wxFileNameIst.GetFullName());
        cv::Mat cvImage = cv::imread(filePath, cv::IMREAD_COLOR);

        if (!cvImage.empty()) {
            NewFileOpen(fileName, cvImage);

            m_pFileHistory->AddFileToHistory(filePath);
        }
    }
}

void CMainFrame::OnQuit(wxCommandEvent& event) {
    Close();
}

void CMainFrame::OnClose(wxCloseEvent& event) {
    m_pFileHistory->Save(*m_pConfig);

    delete m_pConfig;
    delete m_pFileHistory;

    Destroy();
}

void CMainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox(wxT("KhuCv App ver. 1.0.2.0\n(Open development SW for computer vision)\nCopyright(c) 2022, \nDaeho Lee, Kyung Hee University"), wxT("KhuCv App"), wxICON_INFORMATION);
}

void CMainFrame::OnFileOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Image file", "", "",
            "Jpeg files (*.jpg)|*.jpg|Bmp files(*.bmp)|*.bmp|Gif files(*.gif)|*.gif|All files(*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return; 

    char filePath[256], fileName[256];
    strcpy(filePath, openFileDialog.GetPath());
    strcpy(fileName, openFileDialog.GetFilename());
    cv::Mat cvImage = cv::imread(filePath, cv::IMREAD_COLOR);

    if (!cvImage.empty()) {
        NewFileOpen(fileName, cvImage);

        m_pFileHistory->AddFileToHistory(filePath);
    }
}

void CMainFrame::OnFileSave(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Image file", "", "",
        "Jpeg files (*.jpg)|*.jpg|Bmp files(*.bmp)|*.bmp|Gif files(*.gif)|*.gif|All files(*.*)|*.*", wxFD_SAVE | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    char filePath[256];
    strcpy(filePath, openFileDialog.GetPath());
    
    CKcImage kcImage = GetLastSelImage();
    if (kcImage.cvImage.empty()) return;
    
    cv::imwrite(filePath, kcImage.cvImage);
}

void CMainFrame::OnRecentFiles(wxCommandEvent & event) {
    wxString FileNameString(m_pFileHistory->GetHistoryFile(event.GetId() - wxID_FILE1));

    if (!FileNameString.empty())
    {
        char filePath[256], fileName[256];
        wxFileName wxFileNameIst(FileNameString);
        strcpy(filePath, FileNameString);
        strcpy(fileName, wxFileNameIst.GetFullName());
        cv::Mat cvImage = cv::imread(filePath, cv::IMREAD_COLOR);

        if (!cvImage.empty())
            NewFileOpen(fileName, cvImage);
    }
}

void CMainFrame::OnUpdateMenuCheckViewMainDlg(wxUpdateUIEvent& event) {
    CMainDialog* pMainDialog = GetMainDialog();
    
    event.Check(pMainDialog->IsShown());
}

void CMainFrame::OnMenuCheckViewMainDlg(wxCommandEvent& event) {
    CMainDialog* pMainDialog = GetMainDialog();

    pMainDialog->Show(!pMainDialog->IsShown());
}

void CMainFrame::OnUpdateMenuCheckViewOutputWnd(wxUpdateUIEvent& event) {
    wxAuiPaneInfo& Pane = m_AuiMgrOutputWnd.GetPane(m_pPrintListBox);
    
    event.Check(Pane.IsShown());
}

void CMainFrame::OnMenuCheckViewOutputWnd(wxCommandEvent& event) {
    wxAuiPaneInfo &Pane = m_AuiMgrOutputWnd.GetPane(m_pPrintListBox);

    Pane.Show(!Pane.IsShown());
    m_AuiMgrOutputWnd.Update();
}

