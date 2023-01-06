//  ChildFrame.h: interface of CChildFrame (child frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once
#ifndef _KHUCV_SDI
class CKcImage 
{
public:
	cv::Mat cvImage;
	wxPoint pos;

	CKcImage() {};
	CKcImage(cv::Mat mat, wxPoint p) : cvImage(mat), pos(p) {};
};

class CChildFrame : public wxMDIChildFrame
{
public:
	CChildFrame(wxMDIParentFrame* parent, wxWindowID id, const wxString& title);

	void DispalyLastImage();
	void ClearAllImages();

	CClientView* m_pClientView;
	std::vector<CKcImage> m_ImageList;

	DECLARE_EVENT_TABLE();
};
#endif

