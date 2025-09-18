#pragma once
#include <wx/wx.h>

// Helper class for creating tool icons
class IconFactory {
public:
    static wxBitmap CreateSelectIcon();
    static wxBitmap CreateAndGateIcon();
    static wxBitmap CreateOrGateIcon();
    static wxBitmap CreateNotGateIcon();
    static wxBitmap CreateNandGateIcon();
    static wxBitmap CreateNorGateIcon();
    static wxBitmap CreateXorGateIcon();
    static wxBitmap CreateXnorGateIcon();
    static wxBitmap CreateInputIcon();
    static wxBitmap CreateOutputIcon();
    static wxBitmap CreateWireIcon();

    // New icons for extended components
    static wxBitmap CreateHalfAdderIcon();
    static wxBitmap CreateFullAdderIcon();
    static wxBitmap CreateAdder4BitIcon();
    static wxBitmap CreateMultiplexerIcon();
    static wxBitmap CreateDemultiplexerIcon();
};