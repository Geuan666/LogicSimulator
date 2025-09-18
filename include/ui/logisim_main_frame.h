#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include "circuit_canvas.h"
#include "properties_panel.h"
#include "component_library_panel.h"
#include "../core/circuit_document.h"
#include "../core/command_system.h"

// Enhanced main application frame with dockable panels
class LogisimMainFrame : public wxFrame {
private:
    // AUI Manager for dockable panels
    wxAuiManager auiManager;

    // Main panels
    CircuitCanvas* canvas;
    PropertiesPanel* propertiesPanel;
    ComponentLibraryPanel* libraryPanel;

    // View menu items
    wxMenuItem* showGridMenuItem;
    wxMenuItem* snapToGridMenuItem;
    wxMenuItem* showPropertiesMenuItem;
    wxMenuItem* showLibraryMenuItem;

    // Document and command system
    std::unique_ptr<CircuitDocument> document;
    std::unique_ptr<CommandManager> commandManager;

public:
    LogisimMainFrame();
    ~LogisimMainFrame();

    void OnToolSelected(wxCommandEvent& event);
    void OnComponentSelected(wxCommandEvent& event);
    void OnCanvasComponentSelected(wxCommandEvent& event);
    void OnSimulate(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    // View menu handlers
    void OnShowGrid(wxCommandEvent& event);
    void OnSnapToGrid(wxCommandEvent& event);
    void OnShowProperties(wxCommandEvent& event);
    void OnShowLibrary(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnZoomReset(wxCommandEvent& event);
    void OnZoomFit(wxCommandEvent& event);

    // Theme menu handlers
    void OnLightTheme(wxCommandEvent& event);
    void OnDarkTheme(wxCommandEvent& event);

    // File menu handlers
    void OnNewFile(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    void OnSaveAsFile(wxCommandEvent& event);

    // Edit menu handlers
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);

private:
    void CreateUI();
    void CreateToolbar();
    void CreateMenuBar();
    void SetupAUI();
    void ApplyTheme(bool isDark = false);
    void UpdateTitle();
    void UpdateMenus();

    // Menu IDs
    enum {
        ID_SHOW_GRID = wxID_HIGHEST + 1,
        ID_SNAP_TO_GRID,
        ID_SHOW_PROPERTIES,
        ID_SHOW_LIBRARY,
        ID_ZOOM_IN,
        ID_ZOOM_OUT,
        ID_ZOOM_RESET,
        ID_ZOOM_FIT,
        ID_LIGHT_THEME,
        ID_DARK_THEME
    };

    wxDECLARE_EVENT_TABLE();
};