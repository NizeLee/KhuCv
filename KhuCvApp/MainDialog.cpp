//  MainDialog.cpp: implementation of CMainDialog (main dialog of KhuCvApp)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.1.0.0

#include "KhuCvApp.h"

#include <chrono>

void ReadCifar(std::string filename, std::vector<cv::Mat>* pImages, std::vector<int>* pLabels1, std::vector<int>* pLabels2, bool bCifar100) {
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs) {
		const int w = 32, h = 32;
		unsigned char label1, label2;
		unsigned char bufferR[w * h], bufferG[w * h], bufferB[w * h];
		while (ifs.read((char*)&label1, 1)) {
			cv::Mat R, G, B;
			if (bCifar100) ifs.read((char*)&label2, 1);
			ifs.read((char*)bufferR, h * w);	R = cv::Mat(h, w, CV_8UC1, bufferR);
			ifs.read((char*)bufferG, h * w);	G = cv::Mat(h, w, CV_8UC1, bufferG);
			ifs.read((char*)bufferB, h * w);	B = cv::Mat(h, w, CV_8UC1, bufferB);

			std::vector<cv::Mat> channels = { B, G, R };
			cv::Mat image;
			cv::merge(channels, image);
			DisplayImage(image, 0, 0, false, true);
			pLabels1->push_back(label1);
			if (bCifar100) pLabels2->push_back(label2);
			pImages->push_back(image.clone());
		}
		ifs.close();
	}
	else {
		DlgPrintf("%s was not opened.", filename.c_str());
	}
}

std::vector<int> ReadMnistLabel(std::string filename) {
	std::ifstream ifs(filename, std::ios::binary);
	std::vector<int> lables;
	if (ifs) {
		unsigned char buffer[4];
		int header, nums;
		auto big2little = [](const unsigned char* buffer) {
			unsigned char reverse[4] = { buffer[3], buffer[2], buffer[1], buffer[0] };
			return *(int*)reverse;	};

		ifs.read((char*)buffer, sizeof(int));	header = big2little(buffer);
		ifs.read((char*)buffer, sizeof(int));	nums = big2little(buffer);

		DlgPrintf(L"%d, %d", header, nums);
		while (ifs.read((char*)buffer, 1)) {
			lables.push_back(buffer[0]);
		}
		ifs.close();
	}
	else {
		DlgPrintf("%s was not opened.", filename.c_str());
	}

	return lables;
}

std::vector<cv::Mat> ReadMnistImage(std::string filename) {
	std::ifstream ifs(filename, std::ios::binary);
	std::vector<cv::Mat> images;
	if (ifs) {
		unsigned char buffer[28 * 28];
		int header, nums, w, h;
		auto big2little = [](const unsigned char* buffer) {
			unsigned char reverse[4] = { buffer[3], buffer[2], buffer[1], buffer[0] };
			return *(int*)reverse; };

		ifs.read((char*)buffer, sizeof(int));	header = big2little(buffer);
		ifs.read((char*)buffer, sizeof(int));	nums = big2little(buffer);
		ifs.read((char*)buffer, sizeof(int));	h = big2little(buffer);
		ifs.read((char*)buffer, sizeof(int));	w = big2little(buffer);

		DlgPrintf(L"%d, %d, %d, %d", header, nums, h, w);

		while (ifs.read((char*)buffer, h * w)) {
			cv::Mat image(h, w, CV_8UC1, buffer);
			DisplayImage(image, 0, 0, false, true);
			images.push_back(image.clone());
		}
		ifs.close();
	}
	else {
		DlgPrintf("%s was not opened.", filename.c_str());
	}

	return images;
}

BEGIN_EVENT_TABLE(CMainDialog, wxDialog)
EVT_BUTTON(IDC_SEL_FILE_FOLDER, CMainDialog::OnSelSrcFileFolder)
EVT_BUTTON(IDC_SEL_DES_FOLDER, CMainDialog::OnSelDesFileFolder)
EVT_LIST_ITEM_ACTIVATED(IDC_FILE_LIST_CTRL, CMainDialog::OnActivatedFileListCtrl)
EVT_BUTTON(IDC_RUN, CMainDialog::OnRun)
EVT_BUTTON(IDC_PAUSE, CMainDialog::OnPause)
EVT_BUTTON(IDC_REVERSE, CMainDialog::OnReverse)
EVT_BUTTON(IDC_THRESHOLDING, CMainDialog::OnThresholding)
EVT_BUTTON(IDC_HISTOGRAM, CMainDialog::OnHistogram)
EVT_BUTTON(IDC_LABDELING, CMainDialog::OnLabeling)
EVT_BUTTON(IDC_SAVE_LIST, CMainDialog::OnSaveList)
EVT_BUTTON(IDC_EXAMPLE0, CMainDialog::OnExample0)
EVT_BUTTON(IDC_EXAMPLE1, CMainDialog::OnExample1)
EVT_BUTTON(IDC_EXAMPLE2, CMainDialog::OnExample2)
EVT_BUTTON(IDC_EXAMPLE3, CMainDialog::OnExample3)
EVT_BUTTON(IDC_EXAMPLE4, CMainDialog::OnExample4)
EVT_BUTTON(IDC_EXAMPLE5, CMainDialog::OnExample5)
EVT_BUTTON(IDC_EXAMPLE6, CMainDialog::OnExample6)
EVT_BUTTON(IDC_EXAMPLE7, CMainDialog::OnExample7)
EVT_BUTTON(IDC_EXAMPLE8, CMainDialog::OnExample8)
EVT_BUTTON(IDC_EXAMPLE9, CMainDialog::OnExample9)
EVT_TIMER(-1, CMainDialog::OnTimer)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CThresholdingDlg, wxDialog)
EVT_COMMAND_SCROLL(IDC_SLIDER_THRESHOLD, CThresholdingDlg::OnSliderScroll)
END_EVENT_TABLE()

CThresholdingDlg::CThresholdingDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style), m_Hist(256) {
	m_pVbox = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 2; ++i)
		m_pHbox[i] = new wxBoxSizer(wxHORIZONTAL);

	m_pSlider = new wxSlider(this, IDC_SLIDER_THRESHOLD, 0, 0, 255, wxDefaultPosition, wxSize(250, 20));
	m_pHbox[0]->Add(m_pSlider, 0);

	m_pDisplayThreshold = new wxStaticText(this, IDC_DISPLAY_THRESHOLD, wxT("0"), wxDefaultPosition, wxSize(75, 20), wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);
	m_pHbox[1]->Add(m_pDisplayThreshold, 0);

	m_pVbox->Add(m_pHbox[0], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[1], 1, wxALIGN_CENTER | wxTOP, 5);

	SetSizer(m_pVbox);
}

CThresholdingDlg::~CThresholdingDlg() {

}

void CThresholdingDlg::SetImage(cv::Mat cvImage, wxPoint pos) {
	cv::cvtColor(cvImage, m_ImageGray, cv::COLOR_BGR2GRAY);
	m_ImageOut = cv::Mat(m_ImageGray.size(), CV_8UC1);
	m_Pos = pos;

	for (int y = 0; y < m_ImageGray.rows; ++y)
		for (int x = 0; x < m_ImageGray.cols; ++x) {
			unsigned char gray = *(m_ImageGray.data + y * m_ImageGray.step + x);
			m_Hist[gray]++;
		}

	m_pSlider->SetValue(128);
	SetThreshold(128);
}

void CThresholdingDlg::SetThreshold(int nTh) {
	std::stringstream ss;
	ss << nTh;
	m_pDisplayThreshold->SetLabelText(ss.str());

	int max_hist = *std::max_element(m_Hist.begin(), m_Hist.end());
	int hist_w = 256, hist_h = 100;

	cv::Mat histImg(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	for (int i = 0; i < 256; i++) {
		if (max_hist > 0 && m_Hist[i] > 0)
			cv::line(histImg, { i, hist_h - 1 }, { i, hist_h - m_Hist[i] * hist_h / max_hist }, cv::Scalar(255, 255, 255));
	}

	cv::line(histImg, { nTh, 0 }, { nTh, hist_h - 1 }, cv::Scalar(0, 0, 255));
	DisplayImage(histImg, m_Pos.x, m_Pos.y + m_ImageGray.rows, false, true);

	for (int r = 0; r < m_ImageGray.rows; r++)
		for (int c = 0; c < m_ImageGray.cols; c++) {
			unsigned char* p = m_ImageGray.data + r * m_ImageGray.step + c;
			*(m_ImageOut.data + r * m_ImageOut.step + c) = *p > nTh ? 255 : 0;
		}

	DisplayImage(m_ImageOut, m_Pos.x + m_ImageGray.cols, m_Pos.y, false, true);
}

void CThresholdingDlg::OnSliderScroll(wxScrollEvent& event) {
	int threshold = event.GetPosition();
	
	SetThreshold(threshold);
}

CMainDialog::CMainDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxDialog(parent, id, title, pos, size, style),
	m_SequenceRunTimer(this, ID_TIMER_SEQUENCE_RUN), m_VideoRunTimer(this, ID_TIMER_VIDEO_RUN), m_CamRunTimer(this, ID_TIMER_CAM_RUN), m_ImageListRunTimer(this, ID_TIMER_IMAGE_LIST_RUN), m_bFirstRun(false), m_nProcessingNum(0),
	m_nVideoFileFrameCnt(0)
{
	m_pVbox = new wxBoxSizer(wxVERTICAL);
	
	for(int i = 0 ;i < H_BOX_SIZER_NUM; ++i)
		m_pHbox[i] = new wxBoxSizer(wxHORIZONTAL);

	m_pSrcFolderSelButton = new wxButton(this, IDC_SEL_FILE_FOLDER, wxT("..."), wxDefaultPosition, wxSize(30, 20));
	m_pDisplaySrcPathText = new wxStaticText(this, IDC_DISPLAY_SRC_PATH, wxT(""), wxDefaultPosition, wxSize(380, 20), wxST_NO_AUTORESIZE);
	m_pVideoFileCheck = new wxCheckBox(this, IDC_VIDEO_FILE_CHECK, wxT("Video file"), wxDefaultPosition, wxSize(90, 20));

	m_pHbox[0]->Add(m_pSrcFolderSelButton, 0);
	m_pHbox[0]->Add(m_pDisplaySrcPathText, 1, wxLEFT, 5);
	m_pHbox[0]->Add(m_pVideoFileCheck, 0, wxLEFT, 5);

	m_pDesFolderSelButton = new wxButton(this, IDC_SEL_DES_FOLDER, wxT("..."), wxDefaultPosition, wxSize(30, 20));
	m_pDisplayDesPathText = new wxStaticText(this, IDC_DISPLAY_DES_PATH, wxT(""), wxDefaultPosition, wxSize(380, 20), wxST_NO_AUTORESIZE);
	m_pSaveFrameCheck = new wxCheckBox(this, IDC_SAVE_FRAME_CHECK, wxT("Frame save"), wxDefaultPosition, wxSize(90, 20));

	m_pHbox[1]->Add(m_pDesFolderSelButton, 0);
	m_pHbox[1]->Add(m_pDisplayDesPathText, 1, wxLEFT, 5);
	m_pHbox[1]->Add(m_pSaveFrameCheck, 0, wxLEFT, 5);


	m_pListCtrl = new wxListCtrl(this, IDC_FILE_LIST_CTRL, wxDefaultPosition, wxSize(520, 150),
		wxLC_REPORT | wxLC_SINGLE_SEL | wxSUNKEN_BORDER);
	long index1 = m_pListCtrl->InsertColumn(0, "Num", wxLIST_FORMAT_LEFT, 95);
	long index2 = m_pListCtrl->InsertColumn(1, "File name", wxLIST_FORMAT_LEFT, 270);

	m_pHbox[2]->Add(m_pListCtrl, 1);

	m_pStartNum = new wxTextCtrl(this, IDC_START_NUM, wxT(" "), wxDefaultPosition, wxSize(70, 20), wxTE_RIGHT);
	m_pEndNum = new wxTextCtrl(this, IDC_END_NUM, wxT(" "), wxDefaultPosition, wxSize(70, 20), wxTE_RIGHT);

	m_pHbox[3]->Add(m_pStartNum, 1);
	m_pHbox[3]->Add(m_pEndNum, 0, wxLEFT, 5);

	cv::VideoCapture camera;
	int device_counts = 0;
	while (true) {
#ifdef _MSC_VER
		if (!camera.open(device_counts, cv::CAP_DSHOW)) {
#else
		if (!camera.open(device_counts)) {
#endif
			camera.release();
			break;
		}
		std::string camBackend = camera.getBackendName().c_str();
		//std::wstring camBackendw;
		//camBackendw.assign(camBackend.begin(), camBackend.end());

		//DlgPrintf(L"WebCam(%d): %ls", device_counts, camBackend.c_str());
		DlgPrintf("WebCam(%d): %s", device_counts, camBackend.c_str());
		device_counts++;
		camera.release();
	}

	wxArrayString CamString;
	CamString.Add(L"No-Cam");
	for (int i = 0; i < device_counts; ++i) {
		std::wstringstream ListName;
		ListName << i << L"-Cam";
		CamString.Add(ListName.str());
	}

	wxArrayString ProjectString;
	for (int i = 0; i < KHUCV_MAX_PROJECT_NUMS; ++i) {
		std::wstringstream ListName;
		ListName << L"Project #" << i;
		ProjectString.Add(ListName.str());
	}

	m_pSelCam = new wxComboBox(this, IDC_SEL_CAM, wxEmptyString, wxDefaultPosition, wxSize(80, 20), CamString, wxCB_DROPDOWN | wxCB_READONLY);
	m_pSelCam->SetSelection(0);
	m_pSelPorjNumLabel = new wxStaticText(this, IDC_SEL_PROJ_NUM_LABEL, wxT("   Project: "), wxDefaultPosition, wxSize(60, 20));
	m_pSelProjNum = new wxComboBox(this, IDC_SEL_PROJ_NUM, wxEmptyString, wxDefaultPosition, wxSize(80, 20), ProjectString, wxCB_DROPDOWN | wxCB_READONLY);
	m_pSelProjNum->SetSelection(0);
	m_pRunButton = new wxButton(this, IDC_RUN, wxT("Run"), wxDefaultPosition, wxSize(50, 20));
	m_pPauseButton = new wxButton(this, IDC_PAUSE, wxT("Pause"), wxDefaultPosition, wxSize(50, 20));
	m_pStepCheck = new wxCheckBox(this, IDC_SEL_STEP_CHECK, wxT("Step"), wxDefaultPosition, wxSize(50, 20));
	m_pVerboseCheck = new wxCheckBox(this, IDC_SEL_VERBOSE_CHECK, wxT("Verbose"), wxDefaultPosition, wxSize(70, 20));
	m_pVerboseCheck->SetValue(true);

	m_pHbox[4]->Add(m_pSelCam, 0);
	m_pHbox[4]->Add(m_pSelPorjNumLabel, 0, wxLeft | wxALIGN_CENTER_VERTICAL, 5);
	m_pHbox[4]->Add(m_pSelProjNum, 0, wxLeft, 5);
	m_pHbox[4]->Add(m_pRunButton, 0, wxLEFT, 5);
	m_pHbox[4]->Add(m_pPauseButton, 0, wxLEFT, 5);
	m_pHbox[4]->Add(m_pStepCheck, 0, wxLEFT, 5);
	m_pHbox[4]->Add(m_pVerboseCheck, 0, wxLEFT, 5);

	m_pReverseButton = new wxButton(this, IDC_REVERSE, wxT("Reverse"), wxDefaultPosition, wxSize(50, 20));
	m_pThresholdingButton = new wxButton(this, IDC_THRESHOLDING, wxT("Thresholding"), wxDefaultPosition, wxSize(75, 20));
	m_pHistogramButton = new wxButton(this, IDC_HISTOGRAM, wxT("Histogram"), wxDefaultPosition, wxSize(70, 20));
	m_pThreLabelingButton = new wxButton(this, IDC_LABDELING, wxT("Labeling"), wxDefaultPosition, wxSize(70, 20));
	
	m_pHbox[5]->Add(m_pReverseButton, 0);
	m_pHbox[5]->Add(m_pThresholdingButton, 0, wxLEFT, 5);
	m_pHbox[5]->Add(m_pHistogramButton, 0, wxLEFT, 5);
	m_pHbox[5]->Add(m_pThreLabelingButton, 0, wxLEFT, 5);

	m_pSaveListButton = new wxButton(this, IDC_SAVE_LIST, wxT("Save list"), wxDefaultPosition, wxSize(70, 20));

	m_pExample0Button = new wxButton(this, IDC_EXAMPLE0, wxT("Ex0"), wxDefaultPosition, wxSize(40, 20));
	m_pExample1Button = new wxButton(this, IDC_EXAMPLE1, wxT("Ex1"), wxDefaultPosition, wxSize(40, 20));
	m_pExample2Button = new wxButton(this, IDC_EXAMPLE2, wxT("Ex2"), wxDefaultPosition, wxSize(40, 20));
	m_pExample3Button = new wxButton(this, IDC_EXAMPLE3, wxT("Ex3"), wxDefaultPosition, wxSize(40, 20));
	m_pExample4Button = new wxButton(this, IDC_EXAMPLE4, wxT("Ex4"), wxDefaultPosition, wxSize(40, 20));
	m_pExample5Button = new wxButton(this, IDC_EXAMPLE5, wxT("Ex5"), wxDefaultPosition, wxSize(40, 20));
	m_pExample6Button = new wxButton(this, IDC_EXAMPLE6, wxT("Ex6"), wxDefaultPosition, wxSize(40, 20));
	m_pExample7Button = new wxButton(this, IDC_EXAMPLE7, wxT("Ex7"), wxDefaultPosition, wxSize(40, 20));
	m_pExample8Button = new wxButton(this, IDC_EXAMPLE8, wxT("Ex8"), wxDefaultPosition, wxSize(40, 20));
	m_pExample9Button = new wxButton(this, IDC_EXAMPLE9, wxT("Ex9"), wxDefaultPosition, wxSize(40, 20));	

	m_pHbox[6]->Add(m_pExample0Button, 0);
	m_pHbox[6]->Add(m_pExample1Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample2Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample3Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample4Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample5Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample6Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample7Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample8Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pExample9Button, 0, wxLEFT, 5);
	m_pHbox[6]->Add(m_pSaveListButton, 0, wxLEFT, 5);

	m_pVbox->Add(m_pHbox[0], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[1], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[2], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[3], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[4], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[5], 1, wxALIGN_CENTER | wxTOP, 5);
	m_pVbox->Add(m_pHbox[6], 1, wxALIGN_CENTER | wxTOP, 5);
#ifndef _KHUCV_SDI
#else
	m_pPrintListBox = new wxListBox(this, IDC_PRINT_LIST, wxDefaultPosition, wxSize(520, 150));
	m_pHbox[7]->Add(m_pPrintListBox, 1);
	m_pVbox->Add(m_pHbox[7], 1, wxALIGN_CENTER | wxTOP, 5);
#endif

	SetSizer(m_pVbox);
	
	m_bRunTimer = false;
	m_bRunPause = false;
}

CMainDialog::~CMainDialog() {
	m_SequenceRunTimer.Stop();
	m_VideoRunTimer.Stop();
	m_CamRunTimer.Stop();
	m_ImageListRunTimer.Stop();

	m_VideoProcessingVc.release();
	m_CamProcessingVc.release();
}

void CMainDialog::OnSelSrcFileFolder(wxCommandEvent& event) {
	if (m_bRunTimer) {
		wxMessageBox("Timer is running!!");
		return;
	}

	if (!m_pVideoFileCheck->IsEnabled()) {
		m_pVideoFileCheck->Enable(true);

		m_pListCtrl->DeleteAllItems();
		return;
	}

	m_pStartNum->Clear();
	*m_pStartNum << 0;

	bool bFileMode = m_pVideoFileCheck->GetValue();
	
	if (!bFileMode) {
		wxDirDialog dirDialog(this, "Folder Selection", "", wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize,
			wxDirDialogNameStr);

		if (dirDialog.ShowModal() == wxID_CANCEL) return;

		m_pListCtrl->DeleteAllItems();

		//m_pDisplaySrcPathText->SetLabelText(dirDialog.GetPath());
		std::wstring wstrPath(dirDialog.GetPath());
		if (dirDialog.GetPath().Length() > 30) {
			auto fsPath = std::filesystem::path(wstrPath);
			wstrPath = fsPath.root_name().wstring() + std::wstring(L"/.../") + fsPath.filename().wstring();
		}
		m_pDisplaySrcPathText->SetLabelText(wstrPath);

		wxDir dir(dirDialog.GetPath());

		if (!dir.IsOpened()) return;
		
		wxString filename;
		bool cont = dir.GetFirst(&filename);
		int nNum = 0;
		std::wstringstream NumString;

		wxArrayString FilePathArray;

		while (cont) {
			wxString filePath = dir.GetNameWithSep() + filename;
			FilePathArray.Add(filePath);
			cont = dir.GetNext(&filename);
		}

		FilePathArray.Sort();
		for (int i = 0; i < FilePathArray.GetCount(); ++i) {
			std::wstring wstrFilePath;
			wstrFilePath = FilePathArray[i];
			auto fsPath = std::filesystem::path(wstrFilePath);
			if (std::filesystem::is_directory(fsPath)) continue;

			NumString << std::setw(10) << std::setfill(L'0') << nNum++;
			int len = m_pListCtrl->GetItemCount();
			long index = m_pListCtrl->InsertItem(len, NumString.str().c_str());
			m_pListCtrl->SetItem(index, 1, FilePathArray[i]);

			NumString.str(L"");
		}

		m_pEndNum->Clear();
		*m_pEndNum << nNum - 1;
	}
	else {
		 wxFileDialog openFileDialog(this, L"Open Video file", L"", L"",
			 L"Mp4 files(*.mp4)|*.mp4|All files(*.*)|*.*", wxFD_OPEN);

		 if (openFileDialog.ShowModal() == wxID_CANCEL) return;

		 //m_pDisplaySrcPathText->SetLabelText(openFileDialog.GetPath());
		 std::wstring wstrPath(openFileDialog.GetPath());
		 if (openFileDialog.GetPath().Length() > 30) {
			 auto fsPath = std::filesystem::path(wstrPath);
			 wstrPath = fsPath.root_name().wstring() + std::wstring(L"/.../") + fsPath.filename().wstring();
		 }
		 m_pDisplaySrcPathText->SetLabelText(wstrPath);

		 m_VideoFilePath = openFileDialog.GetPath();
		 m_VideoFileName = openFileDialog.GetFilename();

		 m_VideoFilePathUtf8 = UnicodeToUTF8(m_VideoFilePath);
		 m_VideoFileNameUtf8 = UnicodeToUTF8(m_VideoFileName);

		 m_VideoProcessingVc.open(m_VideoFilePathUtf8);

		 if (m_VideoProcessingVc.isOpened()) {
			 cv::Mat videoFrame;

			 float videoFPS = m_VideoProcessingVc.get(cv::CAP_PROP_FPS);
			 int videoWidth = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_WIDTH);
			 int videoHeight = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_HEIGHT);

			 m_pListCtrl->DeleteAllItems();

			 int nNum = 0;
			 std::wstringstream NumString;

			 m_nVideoFileFrameCnt = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_COUNT);
			 for (int i = 0; i < m_nVideoFileFrameCnt; ++i) {
				 NumString << std::setw(10) << std::setfill(L'0') << nNum++;
				 int len = m_pListCtrl->GetItemCount();
				 long index = m_pListCtrl->InsertItem(len, NumString.str().c_str());
				 m_pListCtrl->SetItem(index, 1, m_VideoFilePath);

				 NumString.str(L"");
			 }

			 m_pEndNum->Clear();
			 *m_pEndNum << m_nVideoFileFrameCnt - 1;
		 }
	}
}

void CMainDialog::OnSelDesFileFolder(wxCommandEvent& event) {
	if (m_bRunTimer) {
		wxMessageBox("Timer is running!!");
		return;
	}

	wxDirDialog dirDialog(this, "Folder Selection", "", wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize,
		wxDirDialogNameStr);

	if (dirDialog.ShowModal() == wxID_CANCEL) return;

	m_pDisplayDesPathText->SetLabelText(dirDialog.GetPath());
}

void CMainDialog::OnActivatedFileListCtrl(wxListEvent& event) {
	long nSel = m_pListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	wxString pathNameLc = m_pListCtrl->GetItemText(nSel, 1);
	m_pListCtrl->SetItemState(nSel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	m_pListCtrl->EnsureVisible(nSel);

	bool bFileMode = m_pVideoFileCheck->GetValue();

	m_pStartNum->Clear();
	*m_pStartNum << nSel;

	if (!m_pVideoFileCheck->IsEnabled()) {
		cv::Mat image;
		if(nSel >= 0 && nSel < m_SequenceListImage.size())
			image = m_SequenceListImage[nSel];
#ifndef _KHUCV_SDI
		CMainFrame* pMainFrame = (CMainFrame*)GetParent();
		CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());
#endif

		if (!image.empty()) {
#ifndef _KHUCV_SDI
			if (pChildFrame) {
				DisplayImage(image, 0, 0, false, false);
			}
			else {
				NewFileOpen(L"New Image", image);
			}
#else
			DisplayImage(image, 0, 0, false, false);
#endif
		}
	}
	else {
		if (!bFileMode) {
			std::wstring filename;
			filename = pathNameLc;

			auto start = std::chrono::steady_clock::now();

			cv::Mat image;
			std::ifstream f(std::filesystem::path(filename), std::iostream::binary);

			if (f.good()) {
				try {
					std::filebuf* pbuf = f.rdbuf();
					size_t size = pbuf->pubseekoff(0, f.end, f.in);
					pbuf->pubseekpos(0, f.in);

					std::vector<uchar> buffer(size);
					pbuf->sgetn((char*)buffer.data(), size);

					image = cv::imdecode(buffer, cv::IMREAD_COLOR);
				}
				catch (std::exception& e) {
					DlgPrintf("*** Exception: %s", e.what());
				}

				f.close();
			}

#ifndef _KHUCV_SDI
			CMainFrame* pMainFrame = (CMainFrame*)GetParent();
			CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());
#endif

			if (!image.empty()) {
#ifndef _KHUCV_SDI
				if (pChildFrame) {
					DisplayImage(image, 0, 0, false, false);
				}
				else {
					NewFileOpen(L"New Image", image);
				}
#else
				DisplayImage(image, 0, 0, false, false);
#endif
			}
		}
		else {
			if (m_VideoProcessingVc.isOpened()) {
				cv::Mat videoFrame;

				m_VideoProcessingVc.set(cv::CAP_PROP_POS_FRAMES, nSel);
				m_VideoProcessingVc.grab();
				m_VideoProcessingVc.retrieve(videoFrame);
				m_VideoProcessingVc.set(cv::CAP_PROP_POS_FRAMES, m_nProcessingNum);
				m_VideoProcessingVc.grab();
#ifndef _KHUCV_SDI
				CMainFrame* pMainFrame = (CMainFrame*)GetParent();
				CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());
#endif

				if (!videoFrame.empty()) {
#ifndef _KHUCV_SDI
					if (pChildFrame) {
						DisplayImage(videoFrame, 0, 0, false, false);
					}
					else {
						NewFileOpen(L"New Image", videoFrame);
					}
#else
					DisplayImage(videoFrame, 0, 0, false, false);
#endif
				}
			}
		}
	}
}

void CMainDialog::OnTimer(wxTimerEvent& event) {
	if (m_bRunPause) return;

	std::wstring filename;
	std::wstringstream frameInfo;
	cv::Mat CurrentImage;
	bool bLoaded = false;
	std::string strFrame;

	if (event.GetId() == ID_TIMER_CAM_RUN) {
		m_CamProcessingVc >> CurrentImage;

		frameInfo << L"Cam frame: " << std::setw(9) << std::setprecision(3) << std::fixed << m_nProcessingNum / 30.;

		if (CurrentImage.empty()) {
			m_nProcessingNum++;

			return;
		}

		if (m_pSaveFrameCheck->GetValue()) {
			if (m_nProcessingNum == 0) {
				time_t curTime = time(nullptr);
				struct tm* pLocal = localtime(&curTime);

				std::wstringstream ss;
				ss
					<< (1900 + pLocal->tm_year)
					<< std::setfill(L'0') << std::setw(2) << (pLocal->tm_mon + 1)
					<< std::setfill(L'0') << std::setw(2) << pLocal->tm_mday << L'_'
					<< std::setfill(L'0') << std::setw(2) << pLocal->tm_hour
					<< std::setfill(L'0') << std::setw(2) << pLocal->tm_min
					<< std::setfill(L'0') << std::setw(2) << pLocal->tm_sec << L'_';

				m_strSaveFileNameHeader = ss.str();
			}

			std::wstringstream SaveFileName;

			std::wstring FolderName;
			FolderName = m_pDisplayDesPathText->GetLabelText();

			SaveFileName << FolderName << L"/" << m_strSaveFileNameHeader.c_str() << L"frame" << std::setw(10) << std::setfill(L'0') << m_nProcessingNum << L".jpg";

			std::vector<uchar> buffer;
			cv::imencode(".jpg", CurrentImage, buffer);

			std::ofstream f(std::filesystem::path(SaveFileName.str()), std::iostream::binary);
			if (f.good()) {
				f.write((char*)buffer.data(), buffer.size());
				f.close();
			}
		}

		bLoaded = true;
	}
	else if (event.GetId() == ID_TIMER_SEQUENCE_RUN) {
		int nEnd = 0;
		m_pEndNum->GetLineText(0).ToInt(&nEnd, 10);
		if (nEnd < 0) nEnd = 0;

		if (m_nProcessingNum >= m_pListCtrl->GetItemCount() || m_nProcessingNum > nEnd) {
			m_bRunTimer = false;
			m_pRunButton->SetLabelText(L"Run");
			m_SequenceRunTimer.Stop();
			return;
		}

		wxString pathNameLc = m_pListCtrl->GetItemText(m_nProcessingNum, 1);
		m_pListCtrl->SetItemState(m_nProcessingNum, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		m_pListCtrl->EnsureVisible(m_nProcessingNum);

		filename = pathNameLc;

		std::ifstream f(std::filesystem::path(filename), std::iostream::binary);

		if (f.good()) {
			try {
				std::filebuf* pbuf = f.rdbuf();
				size_t size = pbuf->pubseekoff(0, f.end, f.in);
				pbuf->pubseekpos(0, f.in);

				std::vector<uchar> buffer(size);
				pbuf->sgetn((char*)buffer.data(), size);

				CurrentImage = cv::imdecode(buffer, cv::IMREAD_COLOR);
			}
			catch (std::exception& e) {
				DlgPrintf("*** Exception: %s", e.what());
			}

			f.close();
		}

		if (CurrentImage.empty()) {
			m_nProcessingNum++;

			return;
		}

		bLoaded = true;
	}
	else if (event.GetId() == ID_TIMER_IMAGE_LIST_RUN) {
		int nEnd = 0;
		m_pEndNum->GetLineText(0).ToInt(&nEnd, 10);
		if (nEnd < 0) nEnd = 0;

		if (m_nProcessingNum >= m_pListCtrl->GetItemCount() || m_nProcessingNum > nEnd) {
			m_bRunTimer = false;
			m_pRunButton->SetLabelText(L"Run");
			m_ImageListRunTimer.Stop();
			return;
		}

		wxString pathNameLc = m_pListCtrl->GetItemText(m_nProcessingNum, 1);
		m_pListCtrl->SetItemState(m_nProcessingNum, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		m_pListCtrl->EnsureVisible(m_nProcessingNum);

		if(m_nProcessingNum < m_SequenceListImage.size())
			CurrentImage = m_SequenceListImage[m_nProcessingNum];

		strFrame = m_SequenceListText[m_nProcessingNum];
		if (CurrentImage.empty()) {
			m_nProcessingNum++;

			return;
		}

		bLoaded = true;
	}
	else if (event.GetId() == ID_TIMER_VIDEO_RUN) {
		int nEnd = 0;
		m_pEndNum->GetLineText(0).ToInt(&nEnd, 10);
		if (nEnd < 0) nEnd = 0;

		if (m_nProcessingNum >= m_pListCtrl->GetItemCount() || m_nProcessingNum > nEnd) {
			m_bRunTimer = false;
			m_pRunButton->SetLabelText(L"Run");
			m_VideoRunTimer.Stop();
			return;
		}

		if (m_VideoProcessingVc.isOpened()) {
			float videoFPS = m_VideoProcessingVc.get(cv::CAP_PROP_FPS);
			int videoWidth = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_WIDTH);
			int videoHeight = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_HEIGHT);

			int nFrameCnt = m_VideoProcessingVc.get(cv::CAP_PROP_FRAME_COUNT);

			m_pListCtrl->SetItemState(m_nProcessingNum, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			m_pListCtrl->EnsureVisible(m_nProcessingNum);

			m_VideoProcessingVc >> CurrentImage;

			frameInfo << L"Video frame: " << std::setw(9) << std::setprecision(3) << std::fixed << m_nProcessingNum / videoFPS;

			if (CurrentImage.empty()) {
				m_nProcessingNum++;

				return;
			}

			if (m_pSaveFrameCheck->GetValue()) {
				if (m_nProcessingNum == 0) {
					time_t curTime = time(nullptr);
					struct tm* pLocal = localtime(&curTime);

					wchar_t VideoFileStem[256] = { 0 };
					wcsncpy(VideoFileStem, m_VideoFileName.c_str(), wcslen(m_VideoFileName.c_str()) - 4);

					std::wstringstream ss;
					ss	<< VideoFileStem <<L'_'
						<< (1900 + pLocal->tm_year)
						<< std::setfill(L'0') << std::setw(2) << (pLocal->tm_mon + 1)
						<< std::setfill(L'0') << std::setw(2) << pLocal->tm_mday << L'_'
						<< std::setfill(L'0') << std::setw(2) << pLocal->tm_hour
						<< std::setfill(L'0') << std::setw(2) << pLocal->tm_min
						<< std::setfill(L'0') << std::setw(2) << pLocal->tm_sec << L'_';

					m_strSaveFileNameHeader = ss.str();
				}

				std::wstringstream SaveFileName;

				std::wstring FolderName;
				FolderName = m_pDisplayDesPathText->GetLabelText();

				SaveFileName << FolderName << L"/" << m_strSaveFileNameHeader.c_str() << L"frame" << std::setw(10) << std::setfill(L'0') << m_nProcessingNum << L".jpg";

				std::vector<uchar> buffer;
				cv::Mat BufImage = CurrentImage.clone();
				cv::resize(BufImage, BufImage, cv::Size(CurrentImage.cols/2, CurrentImage.rows/2));
				cv::imencode(".jpg", BufImage, buffer);

				std::ofstream f(std::filesystem::path(SaveFileName.str()), std::iostream::binary);
				if (f.good()) {
					f.write((char*)buffer.data(), buffer.size());
					f.close();
				}
			}

			bLoaded = true;
		}
	}

	if(bLoaded) {
#ifndef _KHUCV_SDI
		CMainFrame* pMainFrame = (CMainFrame*)GetParent();
		CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->GetActiveChild());
#endif

		if (!CurrentImage.empty()) {
#ifndef _KHUCV_SDI
			if (pChildFrame) {
				DisplayImage(CurrentImage, 0, 0, false, true);
			}
			else {
				NewFileOpen(L"New Image", CurrentImage);
			}
#else
			DisplayImage(CurrentImage, 0, 0, false, true);
#endif
		}

		auto start = std::chrono::steady_clock::now();
		cv::Mat Output;
		
		// Project Run;

		int nProjNum = m_pSelProjNum->GetSelection();

		if (event.GetId() == ID_TIMER_VIDEO_RUN) {
			m_Project.m_ProcessingFileName = m_VideoFilePath;
			m_Project.m_nFrameNum = m_nProcessingNum;
		}
		else
			m_Project.m_ProcessingFileName = filename;
		
		m_Project.Run(nProjNum, CurrentImage, Output, strFrame, m_bFirstRun, m_pVerboseCheck->GetValue());
		m_bFirstRun = false;

		auto end = std::chrono::steady_clock::now();
		double processingTime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000;

		if (!CurrentImage.empty()) {

			if (m_pVerboseCheck->GetValue()) {
				if (!Output.empty()) DisplayImage(Output, CurrentImage.cols, 0, false, true);
			}
			
			if (event.GetId() == ID_TIMER_CAM_RUN) {
				DlgPrintf(L"%05d: %ls, %10.5lfms, %7dx%4d", m_nProcessingNum, frameInfo.str().c_str(), processingTime, CurrentImage.cols, CurrentImage.rows);
			}
			else if (event.GetId() == ID_TIMER_SEQUENCE_RUN) {
				DlgPrintf(L"%05d: %ls, %10.5lfms, %7dx%4d", m_nProcessingNum, filename.c_str(), processingTime, CurrentImage.cols, CurrentImage.rows);
			}
			else if (event.GetId() == ID_TIMER_VIDEO_RUN) {
				DlgPrintf(L"%05d: %ls, %10.5lfms, %7dx%4d", m_nProcessingNum, frameInfo.str().c_str(), processingTime, CurrentImage.cols, CurrentImage.rows);
			}
		}
		else
		{
			if (event.GetId() == ID_TIMER_CAM_RUN) {
				DlgPrintf(L"%05d: %ls - load error", m_nProcessingNum, frameInfo.str().c_str());
			}
			else if (event.GetId() == ID_TIMER_SEQUENCE_RUN) {
				DlgPrintf(L"%05d: %ls - load error", m_nProcessingNum, filename.c_str());
			}
			else if (event.GetId() == ID_TIMER_VIDEO_RUN) {
				DlgPrintf(L"%05d: %ls - load error", m_nProcessingNum, frameInfo.str().c_str());
			}
		}

		m_nProcessingNum++;
	}
	
	if (m_pStepCheck->GetValue()) m_bRunPause = true;
}

void CMainDialog::OnPause(wxCommandEvent& event) {
	m_bRunPause = !m_bRunPause;
}

void CMainDialog::OnRun(wxCommandEvent& event) {
	bool bVidieFileMode = m_pVideoFileCheck->GetValue();
	bool bCamMode;

	int nCamNum = m_pSelCam->GetSelection()-1;
	bCamMode = nCamNum>=0?true:false;

	if (bCamMode) {
		if (!m_bRunTimer) {
			int nStart = 0;
#ifdef _MSC_VER			
			m_CamProcessingVc.open(nCamNum, cv::CAP_DSHOW);
#else
			m_CamProcessingVc.open(nCamNum);
#endif
			
			if (m_CamProcessingVc.isOpened()) {
				m_nProcessingNum = nStart;

				m_bRunTimer = true;
				m_bRunPause = false;

				m_bFirstRun = true;
				m_pRunButton->SetLabelText(L"Stop");
				m_CamRunTimer.Start(10);
			}
		}
		else {
			m_bRunTimer = false;
			m_pRunButton->SetLabelText(L"Run");
			m_CamRunTimer.Stop();

			m_CamProcessingVc.release();
		}
	}
	else if (!m_pVideoFileCheck->IsEnabled()) {
		if (!m_bRunTimer && m_pListCtrl->GetItemCount() > 0) {
			m_bRunTimer = true;
			m_bRunPause = false;
			int nStart = 0;
			m_pStartNum->GetLineText(0).ToInt(&nStart, 10);
			if (nStart < 0) nStart = 0;
			if (nStart >= m_pListCtrl->GetItemCount()) nStart = m_pListCtrl->GetItemCount() - 1;
			m_nProcessingNum = nStart;

			m_bFirstRun = true;
			m_pRunButton->SetLabelText(L"Stop");
			m_ImageListRunTimer.Start(10);
		}
		else {
			m_bRunTimer = false;
			m_pRunButton->SetLabelText(L"Run");
			m_ImageListRunTimer.Stop();
		}
	}
	else {
		if (!bVidieFileMode) {
			if (!m_bRunTimer && m_pListCtrl->GetItemCount() > 0) {
				m_bRunTimer = true;
				m_bRunPause = false;
				int nStart = 0;
				m_pStartNum->GetLineText(0).ToInt(&nStart, 10);
				if (nStart < 0) nStart = 0;
				if (nStart >= m_pListCtrl->GetItemCount()) nStart = m_pListCtrl->GetItemCount() - 1;
				m_nProcessingNum = nStart;

				m_bFirstRun = true;
				m_pRunButton->SetLabelText(L"Stop");
				m_SequenceRunTimer.Start(10);
			}
			else {
				m_bRunTimer = false;
				m_pRunButton->SetLabelText(L"Run");
				m_SequenceRunTimer.Stop();
			}
		}
		else {
			if (!m_bRunTimer) {
				m_VideoProcessingVc.open(m_VideoFilePathUtf8);

				int nStart = 0;
				m_pStartNum->GetLineText(0).ToInt(&nStart, 10);
				if (nStart < 0) nStart = 0;
				if (nStart >= m_pListCtrl->GetItemCount()) nStart = m_pListCtrl->GetItemCount() - 1;

				if (m_VideoProcessingVc.isOpened()) {
					m_VideoProcessingVc.set(cv::CAP_PROP_POS_FRAMES, nStart);
					//m_VideoProcessingVc.grab();

					m_nProcessingNum = nStart;

					m_bRunTimer = true;
					m_bRunPause = false;
					m_bFirstRun = true;

					m_pRunButton->SetLabelText(L"Stop");
					m_VideoRunTimer.Start(10);
				}
			}
			else {
				m_bRunTimer = false;
				m_pRunButton->SetLabelText(L"Run");
				m_VideoRunTimer.Stop();
			}
		}
	}
}

#ifndef _KHUCV_SDI
#else
wxListBox* CMainDialog::GetPrintListBox() const {
	return m_pPrintListBox;
}
#endif

void CMainDialog::OnReverse(wxCommandEvent& event) {
	CKcImage kcImage = GetLastSelImage();
	if (kcImage.cvImage.empty()) return;

	cv::Mat cvImage = ~kcImage.cvImage;
	
	/*
	cv::Mat cvImage = kcImage.cvImage.clone();
	for (int y = 0; y < cvImage.rows; y++)
		for (int x = 0; x < cvImage.cols; x++) {
			cvImage.data[y * cvImage.step + x * 3] = ~cvImage.data[y * cvImage.step + x * 3];
			cvImage.data[y * cvImage.step + x * 3 + 1] = ~cvImage.data[y * cvImage.step + x * 3 + 1];
			cvImage.data[y * cvImage.step + x * 3 + 2] = ~cvImage.data[y * cvImage.step + x * 3 + 2];
		}
	*/

	DisplayImage(cvImage, kcImage.pos.x + kcImage.cvImage.cols, kcImage.pos.y, true, false);
}

void CMainDialog::OnThresholding(wxCommandEvent& event) {
	CKcImage kcImage = GetLastSelImage();
	if (kcImage.cvImage.empty()) return;

	CThresholdingDlg dlg(wxGetApp().m_pMainFrame, wxID_ANY, wxT("Thresholding"), wxPoint(wxGetApp().m_pMainDialog->GetPosition() + wxSize(wxGetApp().m_pMainDialog->GetSize().x, 0)));

	dlg.SetImage(kcImage.cvImage, kcImage.pos);
	dlg.ShowModal();
}

void CMainDialog::OnHistogram(wxCommandEvent& event) {
	CKcImage kcImage = GetLastSelImage();
	if (kcImage.cvImage.empty()) return;

	int hSize = 256;
	std::vector<int> hist_red(hSize), hist_green(hSize), hist_blue(hSize);

	for (int y = 0; y < kcImage.cvImage.rows; ++y)
		for (int x = 0; x < kcImage.cvImage.cols; ++x) {
			unsigned char red = *(kcImage.cvImage.data + y * kcImage.cvImage.step + x * 3 + 2);
			hist_red[red]++;
			unsigned char green = *(kcImage.cvImage.data + y * kcImage.cvImage.step + x * 3 + 1);
			hist_green[green]++;
			unsigned char blue = *(kcImage.cvImage.data + y * kcImage.cvImage.step + x * 3 + 0);
			hist_blue[blue]++;
		}

	int max_hist_red = *std::max_element(hist_red.begin(), hist_red.end());
	int max_hist_green = *std::max_element(hist_green.begin(), hist_green.end());
	int max_hist_blue = *std::max_element(hist_blue.begin(), hist_blue.end());
	int hist_w = 256, hist_h = 100;

	cv::Mat histImgR(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0)), histImgG(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0)), histImgB(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	for (int i = 0; i < hSize; i++) {
		if (max_hist_red > 0 && hist_red[i] > 0)
			cv::line(histImgR, { i, hist_h - 1 }, { i, hist_h - hist_red[i] * hist_h / max_hist_red }, cv::Scalar(0, 0, 255));
		if (max_hist_green > 0 && hist_green[i] > 0)
			cv::line(histImgG, { i, hist_h - 1 }, { i, hist_h - hist_green[i] * hist_h / max_hist_green }, cv::Scalar(0, 255, 0));
		if (max_hist_blue > 0 && hist_blue[i] > 0)
			cv::line(histImgB, { i, hist_h - 1 }, { i, hist_h - hist_blue[i] * hist_h / max_hist_blue }, cv::Scalar(255, 0, 0));
	}

	DisplayImage(histImgR, kcImage.pos.x + kcImage.cvImage.cols, kcImage.pos.y, true, false);
	DisplayImage(histImgG, kcImage.pos.x + kcImage.cvImage.cols + hist_w, kcImage.pos.y, true, false);
	DisplayImage(histImgB, kcImage.pos.x + kcImage.cvImage.cols + hist_w * 2, kcImage.pos.y, true, false);
}

void CMainDialog::OnLabeling(wxCommandEvent& event) {
	CKcImage kcImage = GetLastSelImage();
	if (kcImage.cvImage.empty()) return;

	cv::Mat cvImage = kcImage.cvImage.clone();

	cv::Mat img_gray;
	cv::cvtColor(cvImage, img_gray, cv::COLOR_BGR2GRAY);

	cv::Mat img_threshold;
	threshold(img_gray, img_threshold, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	cv::Mat img_labels, stats, centroids;
	int numOfLabels = cv::connectedComponentsWithStats(img_threshold, img_labels, stats, centroids, 8, CV_32S);

	DisplayImage(img_threshold, kcImage.pos.x + kcImage.cvImage.cols, kcImage.pos.y, true, false);
	DisplayImage(img_labels, kcImage.pos.x + kcImage.cvImage.cols * 2, kcImage.pos.y, true, false);
}

void CMainDialog::SetProcessingList(std::vector<cv::Mat> images, std::vector<int> labels1, std::vector<int> labels2, bool bFineLabel, std::wstring name) {
	m_pVideoFileCheck->Enable(false);

	m_pStartNum->Clear();
	*m_pStartNum << 0;

	m_pListCtrl->DeleteAllItems();

	int nNum = 0;
	std::wstringstream NumString;

	m_nVideoFileFrameCnt = images.size();
	m_SequenceListImage.clear();
	for (int i = 0; i < m_nVideoFileFrameCnt; ++i) {
		std::stringstream ss;
		ss << labels1[i] << " ";
		if(bFineLabel)
			ss << labels2[i] << " ";

		m_SequenceListImage.push_back(images[i].clone());
		m_SequenceListText.push_back(ss.str());

		NumString << std::setw(10) << std::setfill(L'0') << nNum++;
		int len = m_pListCtrl->GetItemCount();
		long index = m_pListCtrl->InsertItem(len, NumString.str().c_str());
		m_pListCtrl->SetItem(index, 1, L"[Image list] - " + name);

		NumString.str(L"");
	}

	m_pEndNum->Clear();
	*m_pEndNum << m_nVideoFileFrameCnt - 1;
}

void CMainDialog::OnSaveList(wxCommandEvent& event) {
	wxFileDialog saveFileDialog(this, L"Save listbox", L"", L"",
		L"Txt files (*.txt)|*.txt|All files(*.*)|*.*", wxFD_SAVE);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

	std::wstring filePath;
	filePath = saveFileDialog.GetPath();

	std::ofstream f(std::filesystem::path(filePath), std::iostream::out);
	if (f.good()) {
#ifdef _KHUCV_SDI
		for (int i = 0; i < m_pPrintListBox->GetCount(); ++i)
			f << m_pPrintListBox->GetString(i) << std::endl;
#else
		CMainFrame* pMainFrame = wxGetApp().m_pMainFrame;
		for (int i = 0; i < pMainFrame->GetPrintListBox()->GetCount(); ++i)
			f << pMainFrame->GetPrintListBox()->GetString(i) << std::endl;

#endif
		f.close();
	}
}

void CMainDialog::OnExample0(wxCommandEvent& event) {
	std::vector<cv::Mat> images;
	std::vector<int> labels;

	ReadCifar("data_batch_1.bin", &images, &labels, nullptr, false);

	SetProcessingList(images, labels, std::vector<int>(), false, L"Cifar10");
}

void CMainDialog::OnExample1(wxCommandEvent& event) {
}

void CMainDialog::OnExample2(wxCommandEvent& event) {
}

void CMainDialog::OnExample3(wxCommandEvent& event) {
}

void CMainDialog::OnExample4(wxCommandEvent& event) {
}

void CMainDialog::OnExample5(wxCommandEvent& event) {
}

void CMainDialog::OnExample6(wxCommandEvent& event) {
}

void CMainDialog::OnExample7(wxCommandEvent& event) {
}

void CMainDialog::OnExample8(wxCommandEvent& event) {
}

void CMainDialog::OnExample9(wxCommandEvent& event) {
}


