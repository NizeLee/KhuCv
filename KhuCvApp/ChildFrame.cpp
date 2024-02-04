//  ChildFrame.cpp: implementation of CChildFrame (child frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.2.0

#include "KhuCvApp.h"

#ifndef _KHUCV_SDI

BEGIN_EVENT_TABLE(CChildFrame, wxMDIChildFrame)
END_EVENT_TABLE()

CChildFrame::CChildFrame(wxMDIParentFrame* parent, wxWindowID id, const wxString& title) {
	
#ifdef __APPLE__
    Create(parent, id, title, wxPoint(350, 100), wxSize(1024, 480+350));
	Maximize(false);
#else
    Create(parent, id, title, wxDefaultPosition, wxSize(1024, 768));
    Maximize(true);
#endif

	m_pClientView = new CClientView(this);
	Show();
}

void CChildFrame::DispalyLastImage() {
	
}

void CChildFrame::ClearAllImages() {
	m_ImageList.clear();
}

#endif