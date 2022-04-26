//  KhuCvApp.cpp: implementation of CKhuCvApp
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

bool CKhuCvApp::OnInit()
{
    m_pMainFrame = new CMainFrame(wxT("KhuCV App"));
    m_pMainFrame->Maximize(true);
    m_pMainFrame->Show();

    m_pMainFrame->DragAcceptFiles(true);
    
    wxSize MainFrameSize = m_pMainFrame->GetSize();
    m_pMainDialog = new CMainDialog(m_pMainFrame, wxID_ANY, wxT("Main Dialog"), wxPoint(5, MainFrameSize.y-200-350), wxSize(350, 350), wxDEFAULT_DIALOG_STYLE);
    m_pMainDialog->Show();

    return true;
}

CKhuCvApp::~CKhuCvApp() {
    
}

IMPLEMENT_APP(CKhuCvApp)

void NewFileOpen(const char* fileName, cv::Mat cvImage, int nPosX, int nPosY) {
    CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;
    CChildFrame* pChildFrame = new CChildFrame(pMainFrame, wxID_ANY, fileName);

    CKcImage kcImage = CKcImage(cvImage, wxPoint(nPosX, nPosY));
    pChildFrame->m_ImageList.push_back(kcImage);
    pChildFrame->Show();
}

void DisplayImage(cv::Mat cvImage, int nPosX, int nPosY, bool bErase, bool bClearAll) {
    CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;
    CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());

    CKcImage kcImage;
    if (cvImage.type() != CV_8UC3) {
        if (cvImage.channels() == 1) {
            cv::Mat cvNewImage(cvImage.rows, cvImage.cols, CV_8UC3);
            cv::normalize(cvImage, cvImage, 0, 255, cv::NORM_MINMAX);
            cvImage.convertTo(cvImage, CV_8UC1);
            cv::cvtColor(cvImage, cvNewImage, cv::COLOR_GRAY2BGR);
            kcImage = CKcImage(cvNewImage, wxPoint(nPosX, nPosY));
        }
    }
    else
        kcImage = CKcImage(cvImage, wxPoint(nPosX, nPosY));

    if (bClearAll) pChildFrame->ClearAllImages();

    pChildFrame->m_ImageList.push_back(kcImage);
    pChildFrame->m_pClientView->Refresh(bErase);
    pChildFrame->m_pClientView->Update();
}

CKcImage GetLastSelImage() {
    CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;
    CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());

    int nCurrentGrabImageNum = pChildFrame->m_pClientView->m_nLastSelImageNum;

    if (nCurrentGrabImageNum < 0) {

        if(pChildFrame->m_ImageList.size() > 0)
            return pChildFrame->m_ImageList[pChildFrame->m_ImageList.size()-1];

        return CKcImage();
    }
    else if (nCurrentGrabImageNum >= pChildFrame->m_ImageList.size()) {

        if (pChildFrame->m_ImageList.size() > 0)
            return pChildFrame->m_ImageList[pChildFrame->m_ImageList.size() - 1];

        return CKcImage();
    }

    return pChildFrame->m_ImageList[nCurrentGrabImageNum];
}