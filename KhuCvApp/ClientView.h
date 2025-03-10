//  ClientView.h: interface of CClientView (client view of child frame)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.1.0.0

#pragma once

class CClientView : public wxScrolled<wxWindow>
{
public:
#ifndef _KHUCV_SDI
	CClientView(wxMDIChildFrame* parent);
#else
	CClientView(wxWindow* parent);
#endif
	friend class CMainFrame;

private:
	void OnPaint(wxPaintEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseLeftDblClk(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);

	int GetPosImage(const wxPoint &pos);

	void OnCut(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);
	void OnDuplicate(wxCommandEvent& event);
	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);
	void OnZoomNn(wxCommandEvent& event);
	void OnZoomBl(wxCommandEvent& event);
	void OnZoomBc(wxCommandEvent& event);
	void OnUpdateZoomNn(wxUpdateUIEvent& event);
	void OnUpdateZoomBl(wxUpdateUIEvent& event);
	void OnUpdateZoomBc(wxUpdateUIEvent& event);
	void OnAddImages(wxCommandEvent& event);
	void OnSubtractImages(wxCommandEvent& event);
	void OnSelRegion(wxCommandEvent& event);
	void OnImageInformation(wxCommandEvent& event);

	int m_nCurrentGrabImageNum;

	wxPoint m_SaveGrabPoint, m_ContextPoint, m_SaveMousePos;
	wxRect m_SavePrevRect;

	int m_nSelRegionIndex;
    bool m_bLeftIsDown;
public:
	int m_nLastSelImageNum;
	int m_nZoomInterpolation;

	DECLARE_EVENT_TABLE();
};

