# KhuCv
* KhuCv: Open development source for computer vision  (Kyung Hee University, Data Analysis & Vision Intelligence)
* C++ (wxWidgets + OpenCV)

## Introduction
* Open program sources
* wxWidgets 3.x + OpenCV 4.x
* File open by OpenCV
* File save by OpenCV (individual image)
* Images can be displayed any position and can be moved by mouse
* Strings can be written to Output window (list box)
* Sequence processing (webcam, image sequence, video file)

## Sequence processing
* Image sequences and video files
* **Example**
  + Open mp4 file and Run
<br><img src="KhuCv.gif"></img>  

## Source code
* Call a method (Run) for image processing
``` C++
// MainDialog.cpp
void CMainDialog::OnTimer(wxTimerEvent& event) {
	// Timer for sequence processing

	...

	if(bLoaded) {
		auto start = std::chrono::steady_clock::now();
		
		cv::Mat Output;
		
		// Project Run;
		m_Project.Run(CurrentImage, Output, m_pVerboseCheck->GetValue());
    		// m_Porject is a CProject class instance for sequence processing
   		// Since you only modify the CProject class, you can easily show and debug the processing and results
    
   		// Display codes
		...
	}

	...
}
```

* CProject class
``` C++
// CProcessing.cpp / h
class CProject
{
	cv::Mat m_PreviousImage;
public:
	CProject();
	void Run(cv::Mat Input, cv::Mat& Output, bool bVerbose);
};

CProject::CProject() {
}

void CProject::Run(cv::Mat Input, cv::Mat& Output, bool bVerbose) {
	cv::Sobel(Input, Output, -1, 1, 1);

	cv::Mat Difference;

	if (!m_PreviousImage.empty()) {

		if (Input.type() == m_PreviousImage.type() &&
			Input.cols == m_PreviousImage.cols &&
			Input.rows == m_PreviousImage.rows) {
			cv::subtract(Input, m_PreviousImage, Difference);

			if (bVerbose) DisplayImage(Difference, 0, Input.rows, false, false);
		}
	}

	m_PreviousImage = Input.clone();
}
```

* Showing string information (DlgPrintf function) - lower output window
``` C++
DlgPrintf("%05d: %s, %10.5lfms", m_nProcessingNum, fileName, processingTime);
```
