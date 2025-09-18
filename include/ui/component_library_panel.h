#pragma once
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <map>
#include "../components/circuit_component.h"

// Component library panel with categorized components
class ComponentLibraryPanel : public wxPanel {
private:
    wxTreeCtrl* treeCtrl;
    wxImageList* imageList;
    wxStaticText* titleLabel;
    wxTextCtrl* searchBox;

    // Tree item IDs
    wxTreeItemId rootId;
    wxTreeItemId logicGatesId;
    wxTreeItemId arithmeticId;
    wxTreeItemId dataSelectionId;
    wxTreeItemId sequentialId;
    wxTreeItemId encodersId;
    wxTreeItemId displayId;
    wxTreeItemId inputOutputId;
    wxTreeItemId wiringId;

    // Map to store component types for tree items
    std::map<void*, ComponentType> componentTypeMap;

public:
    ComponentLibraryPanel(wxWindow* parent);
    ~ComponentLibraryPanel();

    ComponentType GetSelectedComponentType() const;

private:
    void CreateTree();
    void PopulateTree();
    void CreateImageList();
    void OnTreeSelChanged(wxTreeEvent& event);
    void OnTreeItemActivated(wxTreeEvent& event);
    void OnSearchText(wxCommandEvent& event);
    void FilterComponents(const wxString& searchText);

    ComponentType GetComponentTypeFromTreeItem(const wxTreeItemId& item) const;

    wxDECLARE_EVENT_TABLE();

public:
    // Custom event for component selection
    static const wxEventType wxEVT_COMPONENT_SELECTED;
};