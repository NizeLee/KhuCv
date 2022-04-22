//  ClientView.h: interface of CClientView (client view of child frame)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#pragma once

class CClientView : public wxScrolled<wxWindow>
{
public:
	CClientView(wxMDIChildFrame* parent);

private:
	void OnPaint(wxPaintEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseLeftDblClk(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);

	int GetPosImage(const wxPoint &pos);

	void OnCopy(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);
	void OnDuplicate(wxCommandEvent& event);

	int m_nCurrentGrabImageNum;

	wxPoint m_SaveGrapPoint, m_ContextPoint;
	wxRect m_SavePrevRect;

public:
	int m_nLastSelImageNum;

	DECLARE_EVENT_TABLE();
};

