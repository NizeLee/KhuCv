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
EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, CMainFrame::OnRecentFiles)
EVT_MENU(IDM_ABOUT, CMainFrame::OnAbout)
EVT_CLOSE(CMainFrame::OnClose)
END_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title)
    : wxMDIParentFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(1920, 1080))
{
    m_pConfig = new wxConfig(title);
   
    wxMenu* menuFile = new wxMenu;
    wxMenu* menuRecent = new wxMenu;
    menuFile->Append(IDM_OPEN, "&Open File...\tCtrl+O");
    menuFile->AppendSubMenu(menuRecent, "Open Recent");
    menuFile->AppendSeparator();
    menuFile->Append(ID_QUIT, "&Quit");

    wxMenu* menuFile2 = new wxMenu;
    menuFile2->Append(IDM_ABOUT, "&KhuCv App...");
  
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuFile2, "&Help");

    SetMenuBar(menuBar);

    m_pFileHistory = new wxFileHistory(10);
    
    m_pFileHistory->UseMenu(menuRecent);
    m_pFileHistory->AddFilesToMenu(menuRecent);
    m_pFileHistory->Load(*m_pConfig);
    
    m_AuiMgr.SetManagedWindow(this);

    m_pPrintListBox = new wxListBox(this, IDC_PRINT_LIST, wxDefaultPosition, wxSize(1920, 150));
    m_AuiMgr.AddPane(m_pPrintListBox, wxBOTTOM, wxT("Output"));

    m_AuiMgr.Update();

    CChildFrame *pChildFrame = new CChildFrame(this, wxID_ANY, "KhuCv Image");

    pChildFrame->Show();
}

CMainFrame::~CMainFrame() {
    m_AuiMgr.UnInit();
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
    wxMessageBox(wxT("KhuCv App ver. 0.0.0.3\n(Open development SW for computer vision)\nCopyright(c) 2022, \nDaeho Lee, Kyung Hee University"), wxT("KhuCv App"), wxICON_INFORMATION);
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

void CMainFrame::DlgPrintf(const char* ptr, ...)
{
    unsigned int Num;

    char ach[1024];
    va_list args;
    va_start(args, ptr);
    vsnprintf(ach, 1024, ptr, args);

    wxString msg = ach;

    Num = m_pPrintListBox->GetCount();
    m_pPrintListBox->InsertItems(1, &msg, Num);
    m_pPrintListBox->SetSelection(Num);
}

