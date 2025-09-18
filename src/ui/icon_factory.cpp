#include "../../include/ui/icon_factory.h"

wxBitmap IconFactory::CreateSelectIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    // Draw cursor arrow
    wxPoint arrow[] = {
        wxPoint(2, 2), wxPoint(2, 14), wxPoint(6, 10),
        wxPoint(9, 13), wxPoint(11, 11), wxPoint(8, 8), wxPoint(2, 2)
    };
    dc.DrawPolygon(7, arrow);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateAndGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw AND gate shape
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(10, 4), wxPoint(16, 10),
        wxPoint(10, 16), wxPoint(2, 16), wxPoint(2, 4)
    };
    dc.DrawPolygon(6, points);
    dc.DrawText("&", 6, 7);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateOrGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw OR gate shape
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(10, 4), wxPoint(16, 10),
        wxPoint(10, 16), wxPoint(2, 16), wxPoint(6, 10), wxPoint(2, 4)
    };
    dc.DrawPolygon(7, points);
    dc.DrawText(">=1", 4, 7);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateNotGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw NOT gate triangle
    wxPoint points[] = {wxPoint(2, 4), wxPoint(14, 10), wxPoint(2, 16)};
    dc.DrawPolygon(3, points);
    // Draw bubble
    dc.DrawCircle(16, 10, 2);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateNandGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw AND gate with bubble
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(9, 4), wxPoint(14, 10),
        wxPoint(9, 16), wxPoint(2, 16), wxPoint(2, 4)
    };
    dc.DrawPolygon(6, points);
    dc.DrawCircle(16, 10, 2);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateNorGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw OR gate with bubble
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(9, 4), wxPoint(14, 10),
        wxPoint(9, 16), wxPoint(2, 16), wxPoint(5, 10), wxPoint(2, 4)
    };
    dc.DrawPolygon(7, points);
    dc.DrawCircle(16, 10, 2);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateXorGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw XOR gate
    wxPoint points[] = {
        wxPoint(4, 4), wxPoint(10, 4), wxPoint(16, 10),
        wxPoint(10, 16), wxPoint(4, 16), wxPoint(7, 10), wxPoint(4, 4)
    };
    dc.DrawPolygon(7, points);
    // Extra curve for XOR
    dc.DrawArc(4, 16, 4, 4, 1, 10);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateXnorGateIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw XOR gate with bubble
    wxPoint points[] = {
        wxPoint(4, 4), wxPoint(9, 4), wxPoint(14, 10),
        wxPoint(9, 16), wxPoint(4, 16), wxPoint(6, 10), wxPoint(4, 4)
    };
    dc.DrawPolygon(7, points);
    dc.DrawArc(4, 16, 4, 4, 1, 10);
    dc.DrawCircle(16, 10, 2);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateInputIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxGREEN_BRUSH);
    dc.DrawRectangle(2, 6, 16, 8);
    dc.SetTextForeground(*wxBLACK);
    dc.DrawText("IN", 6, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateOutputIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxRED_BRUSH);
    dc.DrawCircle(10, 10, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateWireIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(wxPen(*wxBLUE, 2));
    dc.DrawLine(2, 10, 18, 10);
    dc.DrawLine(10, 2, 10, 18);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateHalfAdderIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(2, 2, 16, 16);
    dc.DrawText("HA", 6, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateFullAdderIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(2, 2, 16, 16);
    dc.DrawText("FA", 6, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateAdder4BitIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxCYAN_BRUSH);
    dc.DrawRectangle(2, 2, 16, 16);
    dc.DrawText("4+", 5, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateMultiplexerIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw trapezoid shape for MUX
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(12, 4), wxPoint(16, 16), wxPoint(2, 16)
    };
    dc.DrawPolygon(4, points);
    dc.DrawText("MX", 5, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}

wxBitmap IconFactory::CreateDemultiplexerIcon() {
    wxBitmap bitmap(20, 20);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    // Draw trapezoid shape for DEMUX
    wxPoint points[] = {
        wxPoint(2, 4), wxPoint(16, 4), wxPoint(12, 16), wxPoint(2, 16)
    };
    dc.DrawPolygon(4, points);
    dc.DrawText("DX", 5, 8);
    dc.SelectObject(wxNullBitmap);
    return bitmap;
}