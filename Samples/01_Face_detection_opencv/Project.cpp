//  Project.cpp: implementation of CProject (main project class you will write)
//    Dept. Software Convergence, Kyung Hee University
//    Prof. Daeho Lee, nize@khu.ac.kr
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
    
    m_pUltraface = new UltraFace(m_ExePath, 320, 240);
}

CProject::~CProject() {
    delete m_pUltraface;
}

void CProject::GetExecutionPath() {
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());

    wcscpy(m_ExePathUnicode, appPath);
    strcpy(m_ExePath, appPath.c_str());
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
    std::vector<FaceInfo> faceList;
    m_pUltraface->detect(Input, faceList);
 
    cv::Mat OutImage = Input.clone();

    for(auto list : faceList) {
		cv::rectangle(OutImage, cv::Point(list.x1, list.y1), cv::Point(list.x2, list.y2), cv::Scalar(0, 0, 255), 2);
	}

	if(bVerbose)
		DisplayImage(OutImage, Input.cols, 0, false, true);
}
