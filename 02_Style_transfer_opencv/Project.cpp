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
}

CProject::~CProject() {
}

void CProject::GetExecutionPath() {
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());

	wcscpy(m_ExePathUnicode, appPath);
	strcpy(m_ExePath, appPath.c_str());
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bFirstRun, bool bVerbose) {
	if(bFirstRun) {
		char ModelPath[256];
		strcpy(ModelPath, m_ExePath);
		strcat(ModelPath, "/mosaic-9.onnx");
		//Model files: https://github.com/onnx/models/tree/main/vision/style_transfer/fast_neural_style 

		m_DnnModel = cv::dnn::readNetFromONNX(ModelPath);
	}

	cv::Mat DnnInput;
	int nW = Input.cols;
	int nH = Input.rows;
	cv::Mat inputBlob = cv::dnn::blobFromImage(Input, 1., cv::Size(nW, nH), cv::Scalar(128,128,128), false);
	m_DnnModel.setInput(inputBlob);

	cv::Mat ResultMat = m_DnnModel.forward();

	cv::Mat OutImageBGR[3];

	for(int i = 0 ; i < 3 ; ++i) {
		OutImageBGR[i] = cv::Mat(nH, nW, CV_32FC1);

		int nNewW = (nW+3)/4*4;

		for(int y = 0 ; y < nH ; ++y) {
			memcpy((float *)(OutImageBGR[i].data)+nW*y, (float *)(ResultMat.data) + nNewW*nH*i + nNewW*y, nW*sizeof(float));

			for(int x = 0 ; x < nW ; ++x) {
				float *data = ((float *)(OutImageBGR[i].data)+nW*y + x);
				if(*data < 0) *data = 0;
				else if(*data > 255) *data = 255;
			}
		}
	}

	cv::Mat OutImage;

	cv::merge(OutImageBGR, 3, OutImage);

	if(bVerbose) 
		DisplayImage(OutImage, 0, nH, false, true);
}

