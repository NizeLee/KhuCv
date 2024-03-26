//  KhuCvApp.h: interface of CKhuCvApp
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.7.0

#pragma once
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/clipbrd.h>
#include <wx/listctrl.h>
#include <wx/dir.h>
#include <wx/filehistory.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <functional>

#include "ClientView.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "MainDialog.h"

class CKhuCvApp : public wxApp
{
public:
    CKhuCvApp() : wxApp() {
        m_pMainFrame = nullptr;
        m_pMainDialog = nullptr;
    }

    CMainFrame* m_pMainFrame;
    CMainDialog* m_pMainDialog;
    wxLocale m_locale;

    std::vector<wxString> m_PrintVector;

    ~CKhuCvApp();
    virtual bool OnInit();
};

DECLARE_APP(CKhuCvApp)

#ifndef _KHUCV_SDI
void NewFileOpen(const wchar_t* fileName, cv::Mat cvImage, int nPosX=0, int nPosY=0);
#endif
void DisplayImage(cv::Mat cvImage, int nPosX, int nPosY, bool bErase, bool bClearPos, bool bNormalize = false);
CKcImage GetLastSelImage(int nLastIndex = 0);
CMainDialog* GetMainDialog();
void DlgPrintf(const wchar_t* ptr, ...);
void DlgPrintf(const char* ptr, ...);
void DrawTextOnImage(cv::Mat& cvImage, const std::wstring& str, int x, int y, unsigned char R = 255, unsigned char G = 0, unsigned char B = 0, int pointSize = 15);
std::string UnicodeToUTF8(const std::wstring& ws);