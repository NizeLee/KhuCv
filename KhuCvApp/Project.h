//  Project.h: interface of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.6.0

#pragma once

class CProject
{
	cv::Mat m_PreviousImage;
public:
	std::wstring m_ExePathUnicode;

	CProject();
	~CProject();
	void GetExecutionPath();
	void Run(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose);
};

