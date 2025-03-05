//  Project.h: interface of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.1.0.0

#pragma once

#define KHUCV_MAX_PROJECT_NUMS	(10)

class CProject
{
	cv::Mat m_PreviousImage;
	std::vector < std::function<void(cv::Mat, cv::Mat&, std::string, bool, bool)>> m_RunFns;
public:
	std::wstring m_ExePathUnicode;

	std::wstring m_ProcessingFileName;
	int m_nFrameNum;
	int m_nTP, m_nFN, m_nFP;

	CProject();
	~CProject();
	void GetExecutionPath();
	void Run(int nProjNum, cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);

	void Run0(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run1(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run2(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run3(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run4(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run5(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run6(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run7(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run8(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
	void Run9(cv::Mat Input, cv::Mat& Output, std::string strFrame, bool bFirstRun, bool bVerbose);
};

