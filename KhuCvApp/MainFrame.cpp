//  MainFrame.cpp: implementation of CMainFrame (parent frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"
#include "Logo.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

#ifndef _KHUCV_SDI
BEGIN_EVENT_TABLE(CMainFrame, wxMDIParentFrame)
#else
BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
#endif
EVT_DROP_FILES(CMainFrame::OnDropFiles)
EVT_MENU(ID_QUIT, CMainFrame::OnQuit)
EVT_MENU(IDM_OPEN, CMainFrame::OnFileOpen)
EVT_MENU(IDM_SAVE, CMainFrame::OnFileSave)
EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, CMainFrame::OnRecentFiles)
EVT_MENU(IDM_ABOUT, CMainFrame::OnAbout)
EVT_CLOSE(CMainFrame::OnClose)
EVT_UPDATE_UI(IDM_VIWE_MAIN_DLG, CMainFrame::OnUpdateMenuCheckViewMainDlg)
EVT_MENU(IDM_VIWE_MAIN_DLG, CMainFrame::OnMenuCheckViewMainDlg)
#ifndef _KHUCV_SDI
EVT_UPDATE_UI(IDM_VIWE_OUTPUT_WND, CMainFrame::OnUpdateMenuCheckViewOutputWnd)
EVT_MENU(IDM_VIWE_OUTPUT_WND, CMainFrame::OnMenuCheckViewOutputWnd)
#endif

EVT_MENU(IDM_CONTEXT_COPY, CMainFrame::OnCopy)
EVT_MENU(IDM_CONTEXT_PASTE, CMainFrame::OnPaste)
EVT_MENU(IDM_CONTEXT_DUPLICATE, CMainFrame::OnDuplicate)
EVT_MENU(IDM_CONTEXT_ZOOM_IN, CMainFrame::OnZoomIn)
EVT_MENU(IDM_CONTEXT_ZOOM_OUT, CMainFrame::OnZoomOut)
END_EVENT_TABLE()

CMainFrame::CMainFrame(const wxString& title)
#ifndef _KHUCV_SDI
#ifdef _MSC_VER
    : wxMDIParentFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(1920, 1080))
#endif
#ifdef __linux__
    : wxMDIParentFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(1920, 1080))
#endif
#ifdef  __APPLE__ 
    : wxMDIParentFrame(nullptr, wxID_ANY, title, wxPoint(0, 100), /*wxDefaultPosition,*/ wxSize(350, 480))
#endif
#else
#ifndef  __APPLE__ 
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(1920, 1080))
#else
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(100, 20), wxSize(1280, 960))
#endif
#endif
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
#ifndef _KHUCV_SDI
    menuView->AppendCheckItem(IDM_VIWE_OUTPUT_WND, "&Output");
#endif

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
#ifndef _KHUCV_SDI
    m_AuiMgrOutputWnd.SetManagedWindow(this);
 
    m_pPrintListBox = new wxListBox(this, IDC_PRINT_LIST, wxDefaultPosition, wxSize(1920, 150));
    m_AuiMgrOutputWnd.AddPane(m_pPrintListBox, wxBOTTOM, wxT("Output"));
    m_AuiMgrOutputWnd.Update();
#endif

    wxAcceleratorEntry entries[5];
    entries[0].Set(wxACCEL_CTRL, (int)'C', IDM_CONTEXT_COPY);
    entries[1].Set(wxACCEL_CTRL, (int)'V', IDM_CONTEXT_PASTE);
    entries[2].Set(wxACCEL_CTRL, (int)'D', IDM_CONTEXT_DUPLICATE);
    entries[3].Set(wxACCEL_CTRL, WXK_UP, IDM_CONTEXT_ZOOM_IN);
    entries[4].Set(wxACCEL_CTRL, WXK_DOWN, IDM_CONTEXT_ZOOM_OUT);

    wxAcceleratorTable accel(5, entries);
    SetAcceleratorTable(accel);
#ifndef _KHUCV_SDI
    CChildFrame *pChildFrame = new CChildFrame(this, wxID_ANY, "KhuCv Image");

    pChildFrame->Show();
#else
    m_pClientView = new CClientView(this);
#endif
}

CMainFrame::~CMainFrame() {
#ifndef _KHUCV_SDI
    m_AuiMgrOutputWnd.UnInit();
#endif
}

#ifndef _KHUCV_SDI
wxListBox* CMainFrame::GetPrintListBox() const {
    return m_pPrintListBox;
}
#else
void CMainFrame::DispalyLastImage() {

}

void CMainFrame::ClearAllImages() {
    m_ImageList.clear();
}
#endif

void CMainFrame::OnDropFiles(wxDropFilesEvent& event) {
    const wxString* files = event.GetFiles();

    for(int i = 0 ; i < event.GetNumberOfFiles() ; ++i) {
        wxFileName wxFileNameIst(files[i]);
    
        char filePath[256], fileName[256];
        strcpy(filePath, files[i]);
        strcpy(fileName, wxFileNameIst.GetFullName());
        cv::Mat cvImage = cv::imread(filePath, cv::IMREAD_COLOR);

        if (!cvImage.empty()) {
#ifndef _KHUCV_SDI
            NewFileOpen(fileName, cvImage);
#else
            DisplayImage(cvImage, 0, 0, false, false);
#endif

            m_pFileHistory->AddFileToHistory(filePath);
        }
    }
}

void CMainFrame::OnQuit(wxCommandEvent& event) {
    Close();
}

void CMainFrame::OnClose(wxCloseEvent& event) {
    GetMainDialog()->Destroy();
    delete GetMainDialog();

    m_pFileHistory->Save(*m_pConfig);

    delete m_pConfig;
    delete m_pFileHistory;

    Destroy();
}

class CAboutDialog : public wxDialog
{
public:
    CAboutDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style): wxDialog(parent, id, title, pos, size, style) {
        
        m_pImage = new wxImage(kcLogo::nW, kcLogo::nH, kcLogo::logo, true);
        Bind(wxEVT_PAINT, &CAboutDialog::OnPaint, this);

        m_pOkButton = new wxButton(this, wxID_EXIT, wxT("Ok"), wxPoint(175, 200), wxSize(70, 30));

        Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CAboutDialog::OnOk));

        m_pVbox = new wxBoxSizer(wxVERTICAL);

        for(int i = 0 ;i < 2; ++i)
            m_pHbox[i] = new wxBoxSizer(wxHORIZONTAL);
        
        wxPanel *panel = new wxPanel(this, -1);
        m_pHbox[0]->Add(new wxPanel(panel, -1));
        m_pVbox->Add(m_pHbox[0], 1, wxEXPAND);

        m_pHbox[1]->Add(m_pOkButton, 0);
        m_pVbox->Add(m_pHbox[1], 1, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);
        
        //panel->SetSizer(m_pVbox);
    }
    virtual ~CAboutDialog() {
        delete m_pImage;
    };

    void OnPaint(wxPaintEvent &event) {
        wxPaintDC dc(this);
       
        wxBitmap bmp = *m_pImage;
        dc.DrawBitmap(bmp, wxPoint(50, 10), true);

        wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        dc.SetFont(font);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.SetTextForeground(*wxBLACK);
        dc.SetTextBackground(*wxWHITE);
        dc.DrawText(wxString("KhuCv App ver. 1.0.1.0\n(Open development SW for computer vision)\nCopyright(c) 2022, \nDaeho Lee, Kyung Hee University"), 
            30, 100);
    }

    void OnOk(wxCommandEvent& event) {
        Close(true);
    }

    wxImage *m_pImage;
    wxButton *m_pOkButton;

    wxBoxSizer* m_pVbox;
    wxBoxSizer* m_pHbox[2];
};

void CMainFrame::OnAbout(wxCommandEvent& event) {
    wxImage::AddHandler(new wxPNGHandler); 

    wxSize MainFrameSize = GetSize();
    wxPoint Pos;
    Pos.x = (MainFrameSize.x - 425)/2;
    Pos.y = (MainFrameSize.y - 300)/2;

    CAboutDialog dlg(this, wxID_ANY, wxString("KhuCv App"), Pos, wxSize(425, 300), wxDEFAULT_DIALOG_STYLE);
    dlg.ShowModal();

    //wxMessageBox(wxT("KhuCv App ver. 1.0.1.0\n(Open development SW for computer vision)\nCopyright(c) 2022, \nDaeho Lee, Kyung Hee University"), wxT("KhuCv App"), wxICON_INFORMATION);
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
#ifndef _KHUCV_SDI
        NewFileOpen(fileName, cvImage);
#else
        DisplayImage(cvImage, 0, 0, false, false);
#endif

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

        if (!cvImage.empty()) {
#ifndef _KHUCV_SDI
            NewFileOpen(fileName, cvImage);
#else
            DisplayImage(cvImage, 0, 0, false, false);
#endif
        }
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

#ifndef _KHUCV_SDI
void CMainFrame::OnUpdateMenuCheckViewOutputWnd(wxUpdateUIEvent& event) {
    wxAuiPaneInfo& Pane = m_AuiMgrOutputWnd.GetPane(m_pPrintListBox);
    
    event.Check(Pane.IsShown());
}

void CMainFrame::OnMenuCheckViewOutputWnd(wxCommandEvent& event) {
    wxAuiPaneInfo &Pane = m_AuiMgrOutputWnd.GetPane(m_pPrintListBox);

    Pane.Show(!Pane.IsShown());
    m_AuiMgrOutputWnd.Update();
}
#endif

void CMainFrame::OnCopy(wxCommandEvent& event) {
#ifndef _KHUCV_SDI
    CChildFrame* pChild = (CChildFrame *)GetActiveChild();
    if(pChild) {
         pChild->m_pClientView->ProcessEvent(event);
    }
#else
    m_pClientView->ProcessEvent(event);
#endif
}

void CMainFrame::OnPaste(wxCommandEvent& event) {
#ifndef _KHUCV_SDI
    CChildFrame* pChild = (CChildFrame *)GetActiveChild();
    if(pChild) {
        pChild->m_pClientView->ProcessEvent(event);
    }
#else
    m_pClientView->ProcessEvent(event);
#endif
}

void CMainFrame::OnDuplicate(wxCommandEvent& event) {
#ifndef _KHUCV_SDI
    CChildFrame* pChild = (CChildFrame *)GetActiveChild();
    if(pChild) {
        pChild->m_pClientView->ProcessEvent(event);
    }
#else
    m_pClientView->ProcessEvent(event);
#endif
}

void CMainFrame::OnZoomIn(wxCommandEvent& event) {
#ifndef _KHUCV_SDI
    CChildFrame* pChild = (CChildFrame *)GetActiveChild();
    if(pChild) {
        pChild->m_pClientView->ProcessEvent(event);
    }
#else
    m_pClientView->ProcessEvent(event);
#endif
}

void CMainFrame::OnZoomOut(wxCommandEvent& event) {
#ifndef _KHUCV_SDI
    CChildFrame* pChild = (CChildFrame *)GetActiveChild();
    if(pChild) {
        pChild->m_pClientView->ProcessEvent(event);
    }
#else
    m_pClientView->ProcessEvent(event);
#endif
}

