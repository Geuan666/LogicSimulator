#pragma once
#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include "../components/circuit_component.h"

// Properties panel for editing component properties
class PropertiesPanel : public wxPanel {
private:
    wxPropertyGrid* propGrid;
    CircuitComponent* currentComponent;
    wxStaticText* titleLabel;

public:
    PropertiesPanel(wxWindow* parent);

    void SetComponent(CircuitComponent* component);
    void ClearProperties();
    void UpdateProperties();

private:
    void OnPropertyChanged(wxPropertyGridEvent& event);
    void PopulateProperties();
    void SetupComponentProperties();

    // New methods for type-specific properties
    void PopulateBasicProperties();
    void PopulateTypeSpecificProperties();
    void PopulatePinProperties();
    void PopulateInputProperties();
    void PopulateOutputProperties();
    void PopulateLogicGateProperties();
    void PopulateClockProperties();
    void PopulateDisplayProperties();
    void PopulateDefaultProperties();

    // Helper methods
    wxString GetComponentName() const;

    wxDECLARE_EVENT_TABLE();
};