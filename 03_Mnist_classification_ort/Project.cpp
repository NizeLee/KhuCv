//  Project.cpp: implementation of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"
#include "Project.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

CProject::CProject() {
	GetExecutionPath();
    
#ifdef _MSC_VER
    wchar_t MNistPath[256];
    wcscpy(MNistPath, m_ExePathUnicode);
    wcscat(MNistPath, "/mnist-7.onnx");
#else
    char MNistPath[256];
    strcpy(MNistPath, m_ExePath);
    strcat(MNistPath, "/mnist-7.onnx");
#endif
    m_pMnist = new MNIST(MNistPath);
}

CProject::~CProject() {
    delete m_pMnist;
}

void CProject::GetExecutionPath() {
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());

	wcscpy(m_ExePathUnicode, appPath);
	strcpy(m_ExePath, appPath.c_str());
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
    cv::Mat ResizeInput, DnnInput;
    cv::resize(Input, ResizeInput, cv::Size(28, 28), 0, 0, cv::INTER_AREA);
    cv::cvtColor(ResizeInput, ResizeInput, cv::COLOR_BGR2GRAY);
    ResizeInput.convertTo(DnnInput, CV_32FC1, 1./255, 0);
    
    if (bVerbose) DisplayImage(DnnInput, 0, Input.rows, false, true);
    
    float* input = m_pMnist->input_image_.data();

    memcpy(input, DnnInput.data, sizeof(float)*28*28);

    m_pMnist->Run();
    
    DlgPrintf("%d", m_pMnist->result_);
}

