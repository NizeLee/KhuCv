//  Project.cpp: implementation of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.2.0

#include "KhuCvApp.h"
#include "Project.h"

CProject::CProject() {
	GetExecutionPath();
}

CProject::~CProject() {
}

void CProject::GetExecutionPath() {
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());

	m_ExePathUnicode = appPath;
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
	///// Example code - Begin
	cv::Sobel(Input, Output, -1, 1, 1);

	cv::Mat Difference;

	if (!m_PreviousImage.empty()) {

		if (Input.type() == m_PreviousImage.type() &&
			Input.cols == m_PreviousImage.cols &&
			Input.rows == m_PreviousImage.rows) {
			cv::subtract(Input, m_PreviousImage, Difference);

			if (bVerbose) DisplayImage(Difference, 0, Input.rows, false, true);
		}
	}

	m_PreviousImage = Input.clone();
	///// Example code - End
}

