//  Project.cpp: implementation of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.7.0

#include "KhuCvApp.h"
#include "Project.h"

CProject::CProject() {
	GetExecutionPath();

	m_RunFns.push_back(std::bind(&CProject::Run0, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run1, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run3, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run4, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run5, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run6, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run7, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run8, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	m_RunFns.push_back(std::bind(&CProject::Run9, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

CProject::~CProject() {
}

void CProject::GetExecutionPath() {
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());

	m_ExePathUnicode = appPath;
}

void CProject::Run(int nProjNum, cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
	try {
		m_RunFns.at(nProjNum)(Input, Output, bFirstRun, bVerbose);
	}
	catch (std::exception& e) {
		DlgPrintf("%s", e.what());
	}
}

void CProject::Run0(cv::Mat Input, cv::Mat & Output, bool bFirstRun, bool bVerbose) {
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

void CProject::Run1(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run2(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run3(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run4(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run5(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run6(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run7(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run8(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

void CProject::Run9(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
}

