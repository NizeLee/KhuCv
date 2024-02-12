//  Project.h: interface of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.5.0

#pragma once

std::vector<cv::Mat> ReadMnistImage(std::string filename);
std::vector<int> ReadMnistLabel(std::string filename);

void ReadCifar(std::string filename, std::vector<cv::Mat> *pImages, std::vector<int> *pLabel1, std::vector<int> *pLabels2 = nullptr, bool nCifar100 = false);

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

