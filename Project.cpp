//  Project.cpp: implementation of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"
#include "Project.h"

CProject::CProject() {
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bVerbose) {
	cv::Sobel(Input, Output, -1, 1, 1);

	cv::Mat Difference;

	if (!m_PreviousImage.empty()) {
		cv::subtract(Input, m_PreviousImage, Difference);

		if(bVerbose) DisplayImage(Difference, 0, Input.rows, false, false);
	}

	m_PreviousImage = Input.clone();
}

