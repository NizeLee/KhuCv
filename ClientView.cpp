//  ClientView.cpp: implementation of CClientView (client view of child frame)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuCvApp.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

BEGIN_EVENT_TABLE(CClientView, wxScrolled<wxWindow>)
EVT_PAINT(CClientView::OnPaint)
EVT_LEFT_DOWN(CClientView::OnMouseLeftDown)
EVT_LEFT_UP(CClientView::OnMouseLeftUp)
EVT_LEFT_DCLICK(CClientView::OnMouseLeftDblClk)
EVT_MOTION(CClientView::OnMouseMotion)
EVT_CONTEXT_MENU(CClientView::OnContextMenu)

EVT_MENU(IDM_CONTEXT_COPY, CClientView::OnCopy)
EVT_MENU(IDM_CONTEXT_PASTE, CClientView::OnPaste)
EVT_MENU(IDM_CONTEXT_DUPLICATE, CClientView::OnDuplicate)
END_EVENT_TABLE()

CClientView::CClientView(wxMDIChildFrame* parent)  {
    m_nCurrentGrabImageNum = -1;

    Create(parent, wxID_ANY);
    SetScrollRate(10, 10);
    SetVirtualSize(1024 * 20, 768 * 20);

    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int)'C', IDM_CONTEXT_COPY);
    entries[1].Set(wxACCEL_CTRL, (int)'V', IDM_CONTEXT_PASTE);
    entries[2].Set(wxACCEL_CTRL, (int)'D', IDM_CONTEXT_DUPLICATE);
    
    wxAcceleratorTable accel(3, entries);
    SetAcceleratorTable(accel);

    Bind(wxEVT_PAINT, &CClientView::OnPaint, this);
};

void CClientView::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    wxRect rectUpdate = GetUpdateRegion().GetBox();
    CalcUnscrolledPosition(rectUpdate.x, rectUpdate.y,
        &rectUpdate.x, &rectUpdate.y);

    for (CKcImage& kcImage : ((CChildFrame*)(GetParent()))->m_ImageList) {

        cv::Mat rgbImage;

        cv::cvtColor(kcImage.cvImage, rgbImage, cv::COLOR_BGR2RGB);
    
        wxImage wx_Image = wxImage(kcImage.cvImage.cols, kcImage.cvImage.rows, (unsigned char*)rgbImage.data, true);

        wxBitmap bmp = wx_Image;
        dc.DrawBitmap(bmp, -rectUpdate.x + kcImage.pos.x, -rectUpdate.y + kcImage.pos.y);
    }
}

int CClientView::GetPosImage(const wxPoint &pos) {
    for (int i = ((CChildFrame*)(GetParent()))->m_ImageList.size() - 1; i >= 0; --i) {
        CKcImage& kcImage = ((CChildFrame*)(GetParent()))->m_ImageList[i];
        wxRect rt(kcImage.pos, wxSize(kcImage.cvImage.cols, kcImage.cvImage.rows));
        if (rt.Contains(pos)) return i;
    }

    return -1;
}

void CClientView::OnMouseLeftDown(wxMouseEvent& event) {
    m_SaveGrapPoint = event.GetPosition();
    m_nCurrentGrabImageNum = GetPosImage(m_SaveGrapPoint);
    m_SavePrevRect = wxRect(0, 0, 0, 0);

    m_nLastSelImageNum = m_nCurrentGrabImageNum;
}

void CClientView::OnMouseMotion(wxMouseEvent& event) {
    if (m_nCurrentGrabImageNum < 0) return;

    wxClientDC dc(this);
    dc.SetLogicalFunction(wxINVERT);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    CKcImage& kcImge = ((CChildFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum];

    wxRect rt(kcImge.pos, wxSize(kcImge.cvImage.cols, kcImge.cvImage.rows));

    rt.Offset(event.GetPosition() - m_SaveGrapPoint);

    dc.DrawRectangle(m_SavePrevRect);
    dc.DrawRectangle(rt);  

    m_SavePrevRect = rt;
}

void CClientView::OnMouseLeftUp(wxMouseEvent& event) {
    if (m_nCurrentGrabImageNum < 0) return;

    ((CChildFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].pos += event.GetPosition() - m_SaveGrapPoint;
 
    wxSize size = GetClientSize();
    wxRect rectUpdate(wxPoint(0, 0), size);

    RefreshRect(rectUpdate);
    Update();

    m_nCurrentGrabImageNum = -1;
}

void CClientView::OnMouseLeftDblClk(wxMouseEvent& event) {
    m_nCurrentGrabImageNum = GetPosImage(m_SaveGrapPoint);

    if (m_nCurrentGrabImageNum >= 0) {
        CKcImage kcImage;
        kcImage.cvImage = ((CChildFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].cvImage.clone();
        kcImage.pos = ((CChildFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].pos;
        
        ((CChildFrame*)(GetParent()))->ClearAllImages();
        ((CChildFrame*)(GetParent()))->m_ImageList.push_back(kcImage);
        
        Refresh();
        Update();

        m_nCurrentGrabImageNum = -1;
    }
}

void CClientView::OnContextMenu(wxContextMenuEvent& event) {
    m_ContextPoint = event.GetPosition();
   
    if ((m_ContextPoint.x == -1) && (m_ContextPoint.y == -1))
    {
        wxSize size = GetSize();
        m_ContextPoint.x = size.x / 2;
        m_ContextPoint.y = size.y / 2;
    }
    else
    {
        m_ContextPoint = ScreenToClient(m_ContextPoint);
    }
    
    wxMenu* popupMenu = new wxMenu;

    popupMenu->Append(IDM_CONTEXT_COPY, "&Copy\tCtlr+C");
    popupMenu->Append(IDM_CONTEXT_PASTE, "&Paste\tCtlr+V");
    popupMenu->Append(IDM_CONTEXT_DUPLICATE, "&Duplicate\tCtlr+D");

    PopupMenu(popupMenu, m_ContextPoint);
}

void CClientView::OnCopy(wxCommandEvent& event) {
    int nCurrentGrabImageNum = GetPosImage(m_ContextPoint);

    if (nCurrentGrabImageNum < 0) return;
    
    cv::Mat cvImage;
    cvImage = ((CChildFrame*)GetParent())->m_ImageList[nCurrentGrabImageNum].cvImage.clone();

    cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB);
    wxImage image(cvImage.cols, cvImage.rows, cvImage.data, true);
    wxBitmap bitmap = image;

    if (wxTheClipboard->Open()) {     
        wxTheClipboard->SetData(new wxBitmapDataObject(bitmap));  
        wxTheClipboard->Close();
    } 
}

void CClientView::OnPaste(wxCommandEvent& event) {
    if (wxTheClipboard->Open()) {
        if (wxTheClipboard->IsSupported(wxDF_BITMAP)) {
            wxBitmapDataObject data;
            wxTheClipboard->GetData(data);
            wxBitmap bitmap = data.GetBitmap();

            wxImage image = bitmap.ConvertToImage();

            cv::Mat cvImage(image.GetHeight(), image.GetWidth(), CV_8UC3);

            memcpy(cvImage.data, image.GetData(), image.GetWidth()* image.GetHeight() * 3);
            cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB);
            //DisplayImage(cvImage, m_ContextPoint.x, m_ContextPoint.y, true, false);

            wxRect rectUpdate = GetUpdateRegion().GetBox();
            CalcUnscrolledPosition(rectUpdate.x, rectUpdate.y,
                &rectUpdate.x, &rectUpdate.y);
            DisplayImage(cvImage, rectUpdate.x, rectUpdate.y, true, false);

        }
        wxTheClipboard->Close();
    }
}

void CClientView::OnDuplicate(wxCommandEvent& event) {
    int nCurrentGrabImageNum = GetPosImage(m_ContextPoint);

    if (nCurrentGrabImageNum < 0) return;

    CKcImage kcImage = ((CChildFrame*)GetParent())->m_ImageList[nCurrentGrabImageNum];
    cv::Mat cvImage = ((CChildFrame*)GetParent())->m_ImageList[nCurrentGrabImageNum].cvImage.clone();

    DisplayImage(cvImage, kcImage.pos.x+kcImage.cvImage.cols, kcImage.pos.y, true, false);
}