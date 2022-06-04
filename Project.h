//  Project.h: interface of CProject (main project class you will write)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once


class CProject
{
	cv::Mat m_PreviousImage;
public:
	CProject();
	void Run(cv::Mat Input, cv::Mat& Output, bool bVerbose);
};

