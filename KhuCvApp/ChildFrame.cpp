//  ChildFrame.cpp: implementation of CChildFrame (child frame of multiple document interface)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"

#ifndef _KHUCV_SDI
#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

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