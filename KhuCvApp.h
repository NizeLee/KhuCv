//  KhuCvApp.h: interface of CKhuCvApp
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/clipbrd.h>
#include <wx/listctrl.h>
#include <wx/dir.h>
#include <wx/filehistory.h>
#include <wx/config.h>
#include <wx/filename.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "MainFrame.h"
#include "ClientView.h"
#include "ChildFrame.h"
#include "MainDialog.h"

class CKhuCvApp : public wxApp
{
public:
    CMainFrame* m_pMainFrame;
    CMainDialog* m_pMainDialog;

    ~CKhuCvApp();
    virtual bool OnInit();
};

void NewFileOpen(const char* fileName, cv::Mat cvImage, int nPosX=0, int nPosY=0);
void DisplayImage(cv::Mat cvImage, int nPosX, int nPosY, bool bErase, bool bClearAll);
CKcImage GetLastSelImage();
