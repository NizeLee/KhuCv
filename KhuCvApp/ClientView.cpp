//  ClientView.cpp: implementation of CClientView (client view of child frame)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//	KhuCv App ver. 1.0.6.0

#include "KhuCvApp.h"

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
EVT_MENU(IDM_CONTEXT_ZOOM_IN, CClientView::OnZoomIn)
EVT_MENU(IDM_CONTEXT_ZOOM_OUT, CClientView::OnZoomOut)
EVT_MENU(IDM_CONTEXT_SEL_REGION, CClientView::OnSelRegion)
END_EVENT_TABLE()

#ifndef _KHUCV_SDI
typedef CChildFrame CParentFrame;
#else
typedef CMainFrame CParentFrame;
#endif

#ifndef _KHUCV_SDI
CClientView::CClientView(wxMDIChildFrame* parent)  {
#else
CClientView::CClientView(wxWindow * parent) {
#endif
    m_nCurrentGrabImageNum = -1;

    Create(parent, wxID_ANY);
    SetScrollRate(10, 10);
    SetVirtualSize(1024 * 20, 768 * 20);

    m_nSelRegionIndex = -1;
    m_bLeftIsDown = false;
    
    Bind(wxEVT_PAINT, &CClientView::OnPaint, this);
};


void CClientView::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    wxPoint ScrolledPosition;
    CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));

    for (CKcImage& kcImage : ((CParentFrame*)(GetParent()))->m_ImageList) {

        cv::Mat rgbImage;

        cv::cvtColor(kcImage.cvImage, rgbImage, cv::COLOR_BGR2RGB);

        wxImage wx_Image = wxImage(kcImage.cvImage.cols, kcImage.cvImage.rows, (unsigned char*)rgbImage.data, true);

        wxBitmap bmp = wx_Image;
        dc.DrawBitmap(bmp, ScrolledPosition.x + kcImage.pos.x, ScrolledPosition.y + kcImage.pos.y);
    }
    
#ifndef _MSC_VER
    if(m_nSelRegionIndex < 0) {
        if (m_nCurrentGrabImageNum < 0) return;

        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        dc.DrawRectangle(m_SavePrevRect);
    }
    else {
        if(m_bLeftIsDown) {
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            
            dc.DrawRectangle(m_SavePrevRect);
            
        }
        else {
            wxSize sizeView = GetClientSize();
            
            dc.DrawLine(m_SaveMousePos.x, 0, m_SaveMousePos.x, sizeView.y);
            dc.DrawLine(0, m_SaveMousePos.y, sizeView.x, m_SaveMousePos.y);
        }
    }
#endif
}

int CClientView::GetPosImage(const wxPoint &pos) {
    for (int i = (int)((CParentFrame*)(GetParent()))->m_ImageList.size() - 1; i >= 0; --i) {
        CKcImage& kcImage = ((CParentFrame*)(GetParent()))->m_ImageList[i];
        wxRect rt(kcImage.pos, wxSize(kcImage.cvImage.cols, kcImage.cvImage.rows));
        if (rt.Contains(pos)) return i;
    }

    return -1;
}

void CClientView::OnMouseLeftDown(wxMouseEvent& event) {
    m_bLeftIsDown = true;
    m_SaveGrabPoint = event.GetPosition();

    if(m_nSelRegionIndex < 0) {
        wxPoint ScrolledPosition;
        CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));

        m_nCurrentGrabImageNum = (int)GetPosImage(m_SaveGrabPoint - ScrolledPosition);
        m_SavePrevRect = wxRect(0, 0, 0, 0);

        if(m_nCurrentGrabImageNum >= 0){
            int nLastViewImageNum = (int)(((CParentFrame*)(GetParent()))->m_ImageList.size())-1;

            std::swap(((CParentFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum],
                ((CParentFrame*)(GetParent()))->m_ImageList[nLastViewImageNum]);

            m_nCurrentGrabImageNum = nLastViewImageNum;
            m_nLastSelImageNum = nLastViewImageNum;
        }
    }
    else {
        m_SavePrevRect = wxRect(event.GetPosition(), wxSize(0,0));
    }
}

void CClientView::OnMouseMotion(wxMouseEvent& event) {
    if(m_nSelRegionIndex < 0) {
        if (m_nCurrentGrabImageNum < 0) return;

#ifdef _MSC_VER
        wxClientDC dc(this);
        dc.SetLogicalFunction(wxINVERT);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
#endif

        CKcImage& kcImge = ((CParentFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum];

        wxRect rt(kcImge.pos, wxSize(kcImge.cvImage.cols, kcImge.cvImage.rows));

        wxPoint ScrolledPosition;
        CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));
        rt.Offset(event.GetPosition() - m_SaveGrabPoint + ScrolledPosition);

#ifdef _MSC_VER
        dc.DrawRectangle(m_SavePrevRect);
        dc.DrawRectangle(rt);  
#endif

        m_SavePrevRect = rt;

#ifndef _MSC_VER
        Refresh(false);
#endif
    }
    else {
#ifdef _MSC_VER
        wxClientDC dc(this);
        dc.SetLogicalFunction(wxINVERT);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
#endif

        if(event.LeftIsDown()) {
            wxRect rt;
            rt.x = m_SavePrevRect.x;
            rt.y = m_SavePrevRect.y;
            rt.width = event.GetPosition().x - m_SavePrevRect.x;
            rt.height = event.GetPosition().y - m_SavePrevRect.y;

#ifdef _MSC_VER
            dc.DrawRectangle(m_SavePrevRect);
            dc.DrawRectangle(rt);  
#endif

            m_SavePrevRect = rt;

            if(m_nSelRegionIndex == 1) {
                wxSize sizeView = GetClientSize();

#ifdef _MSC_VER
                dc.DrawLine(m_SaveMousePos.x, 0, m_SaveMousePos.x, sizeView.y);
                dc.DrawLine(0, m_SaveMousePos.y, sizeView.x, m_SaveMousePos.y);
#endif
                m_nSelRegionIndex++;
            }
        }
        else {
            wxSize sizeView = GetClientSize();

#ifdef _MSC_VER
            dc.DrawLine(m_SaveMousePos.x, 0, m_SaveMousePos.x, sizeView.y);
            dc.DrawLine(0, m_SaveMousePos.y, sizeView.x, m_SaveMousePos.y);

            dc.DrawLine(event.GetPosition().x, 0, event.GetPosition().x, sizeView.y);
            dc.DrawLine(0, event.GetPosition().y, sizeView.x, event.GetPosition().y);
#endif

            m_SaveMousePos = event.GetPosition();
        }
#ifndef _MSC_VER
        Refresh(false);
#endif
    }
}

void CClientView::OnMouseLeftUp(wxMouseEvent& event) {
    m_bLeftIsDown = false;
    if(m_nSelRegionIndex < 0) {
        if (m_nCurrentGrabImageNum < 0) return;

        ((CParentFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].pos += event.GetPosition() - m_SaveGrabPoint;

        wxSize size = GetClientSize();
        wxRect rectUpdate(wxPoint(0, 0), size);

        RefreshRect(rectUpdate);
        Update();

        m_nCurrentGrabImageNum = -1;
    }
    else {
        wxRect rt;
        rt.x = m_SavePrevRect.x;
        rt.y = m_SavePrevRect.y;
        rt.width = event.GetPosition().x - m_SavePrevRect.x;
        rt.height = event.GetPosition().y - m_SavePrevRect.y;

        if(rt.width < 0) {
            rt.x += rt.width;
            rt.width *= -1;
        }

        if(rt.height < 0) {
            rt.y += rt.height;
            rt.height *= -1;
        }

        if(rt.height > 0 && rt.width > 0) {
            cv::Mat cvImage(rt.height, rt.width, CV_8UC3);

            wxPoint ScrolledPosition;
            CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));

            for(int y = 0 ; y < rt.height ; ++y)
                for(int x = 0 ; x < rt.width ; ++x) {

                    cvImage.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);

                    for (CKcImage& kcImage : ((CParentFrame*)(GetParent()))->m_ImageList) {
                        wxPoint pos = wxPoint(rt.x, rt.y)+wxPoint(x, y)-ScrolledPosition;
                        wxRect rtImage(kcImage.pos, wxSize(kcImage.cvImage.cols, kcImage.cvImage.rows));

                        if(rtImage.Contains(pos)) 
                            cvImage.at<cv::Vec3b>(y,x) = kcImage.cvImage.at<cv::Vec3b>(
                                y+rt.y-(kcImage.pos.y+ScrolledPosition.y), x+rt.x-(kcImage.pos.x+ScrolledPosition.x));
                    }
                }

            DisplayImage(cvImage, rt.x+rt.width-ScrolledPosition.x, rt.y-ScrolledPosition.y, true, false);
        }

        m_nSelRegionIndex = -1;
    }
}

void CClientView::OnMouseLeftDblClk(wxMouseEvent& event) {
    wxPoint ScrolledPosition;
    CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));

    m_nCurrentGrabImageNum = (int)GetPosImage(m_SaveGrabPoint-ScrolledPosition);

    if (m_nCurrentGrabImageNum >= 0) {
        CKcImage kcImage;
        kcImage.cvImage = ((CParentFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].cvImage.clone();
        kcImage.pos = ((CParentFrame*)(GetParent()))->m_ImageList[m_nCurrentGrabImageNum].pos;

        ((CParentFrame*)(GetParent()))->ClearAllImages();
        ((CParentFrame*)(GetParent()))->m_ImageList.push_back(kcImage);

        Refresh();
        Update();

        m_nCurrentGrabImageNum = -1;
        m_nLastSelImageNum = 0;
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

    wxPoint ScrolledPosition;
    CalcScrolledPosition(0, 0, &(ScrolledPosition.x), &(ScrolledPosition.y));

    m_nLastSelImageNum = (int)GetPosImage(m_ContextPoint-ScrolledPosition);

    wxMenu* popupMenu = new wxMenu;

    popupMenu->Append(IDM_CONTEXT_COPY, "&Copy\tCtrl+C");
    popupMenu->Append(IDM_CONTEXT_PASTE, "&Paste\tCtrl+V");
    popupMenu->Append(IDM_CONTEXT_DUPLICATE, "&Duplicate\tCtrl+D");
    popupMenu->Append(IDM_CONTEXT_ZOOM_IN, "Zoom &In\tCtrl+UP");
    popupMenu->Append(IDM_CONTEXT_ZOOM_OUT, "Zoom &Out\tCtrl+DOWN");
    popupMenu->Append(wxID_SEPARATOR);
    popupMenu->Append(IDM_CONTEXT_SEL_REGION, "Sel Region");

    PopupMenu(popupMenu, m_ContextPoint);
}

void CClientView::OnCopy(wxCommandEvent& event) {
    if (m_nLastSelImageNum < 0 || ((CParentFrame*)GetParent())->m_ImageList.size() <= m_nLastSelImageNum) return;

    cv::Mat cvImage;
    cvImage = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage.clone();

    wxImage::AddHandler(new wxPNGHandler); 
    cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB);
    wxImage image(cvImage.cols, cvImage.rows, cvImage.data, true);
    wxBitmap bitmap = image;

    if (wxTheClipboard->Open()) {  
        wxBitmapDataObject bitmapDataObject(bitmap);
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
    if (m_nLastSelImageNum < 0 || ((CParentFrame*)GetParent())->m_ImageList.size() <= m_nLastSelImageNum) return;

    CKcImage kcImage = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum];
    cv::Mat cvImage = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage.clone();

    DisplayImage(cvImage, kcImage.pos.x+kcImage.cvImage.cols, kcImage.pos.y, true, false);
}

void CClientView::OnZoomIn(wxCommandEvent& event) {
    if (m_nLastSelImageNum < 0 || ((CParentFrame*)GetParent())->m_ImageList.size() <= m_nLastSelImageNum) return;

    CKcImage kcImage = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum];
    cv::Mat cvImage;
    cv::Size cvSize = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage.size();

    cv::resize(((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage, cvImage,
        cvSize*2, 0, 0, cv::INTER_CUBIC);

    DisplayImage(cvImage, kcImage.pos.x+kcImage.cvImage.cols, kcImage.pos.y, true, false);
}

void CClientView::OnZoomOut(wxCommandEvent& event) {
    if (m_nLastSelImageNum < 0 || ((CParentFrame*)GetParent())->m_ImageList.size() <= m_nLastSelImageNum) return;

    CKcImage kcImage = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum];
    cv::Mat cvImage;
    cv::Size cvSize = ((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage.size();

    cv::resize(((CParentFrame*)GetParent())->m_ImageList[m_nLastSelImageNum].cvImage, cvImage,
        cvSize/2, 0, 0, cv::INTER_CUBIC);

    DisplayImage(cvImage, kcImage.pos.x+kcImage.cvImage.cols, kcImage.pos.y, true, false);
}

void CClientView::OnSelRegion(wxCommandEvent& event) {
    m_nSelRegionIndex = 1;

    m_SaveMousePos = wxPoint(-10, -10);
}
