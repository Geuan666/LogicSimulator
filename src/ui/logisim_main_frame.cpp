#include "../../include/ui/logisim_main_frame.h"
#include "../../include/ui/icon_factory.h"

wxBEGIN_EVENT_TABLE(LogisimMainFrame, wxFrame)
    EVT_TOOL(wxID_ANY, LogisimMainFrame::OnToolSelected)
    EVT_MENU(wxID_FORWARD, LogisimMainFrame::OnSimulate)
    EVT_MENU(wxID_ABOUT, LogisimMainFrame::OnAbout)
    EVT_MENU(wxID_EXIT, LogisimMainFrame::OnExit)

    // File menu events
    EVT_MENU(wxID_NEW, LogisimMainFrame::OnNewFile)
    EVT_MENU(wxID_OPEN, LogisimMainFrame::OnOpenFile)
    EVT_MENU(wxID_SAVE, LogisimMainFrame::OnSaveFile)
    EVT_MENU(wxID_SAVEAS, LogisimMainFrame::OnSaveAsFile)

    // Edit menu events
    EVT_MENU(wxID_UNDO, LogisimMainFrame::OnUndo)
    EVT_MENU(wxID_REDO, LogisimMainFrame::OnRedo)
    EVT_MENU(wxID_COPY, LogisimMainFrame::OnCopy)
    EVT_MENU(wxID_PASTE, LogisimMainFrame::OnPaste)
    EVT_MENU(wxID_DELETE, LogisimMainFrame::OnDelete)

    // View menu events
    EVT_MENU(ID_SHOW_GRID, LogisimMainFrame::OnShowGrid)
    EVT_MENU(ID_SNAP_TO_GRID, LogisimMainFrame::OnSnapToGrid)
    EVT_MENU(ID_SHOW_PROPERTIES, LogisimMainFrame::OnShowProperties)
    EVT_MENU(ID_SHOW_LIBRARY, LogisimMainFrame::OnShowLibrary)
    EVT_MENU(ID_ZOOM_IN, LogisimMainFrame::OnZoomIn)
    EVT_MENU(ID_ZOOM_OUT, LogisimMainFrame::OnZoomOut)
    EVT_MENU(ID_ZOOM_RESET, LogisimMainFrame::OnZoomReset)
    EVT_MENU(ID_ZOOM_FIT, LogisimMainFrame::OnZoomFit)

    // Theme menu events
    EVT_MENU(ID_LIGHT_THEME, LogisimMainFrame::OnLightTheme)
    EVT_MENU(ID_DARK_THEME, LogisimMainFrame::OnDarkTheme)

    EVT_COMMAND(wxID_ANY, ComponentLibraryPanel::wxEVT_COMPONENT_SELECTED, LogisimMainFrame::OnComponentSelected)
    EVT_COMMAND(wxID_ANY, CircuitCanvas::wxEVT_COMPONENT_SELECTED, LogisimMainFrame::OnCanvasComponentSelected)
wxEND_EVENT_TABLE()

LogisimMainFrame::LogisimMainFrame()
    : wxFrame(nullptr, wxID_ANY, "Enhanced Logic Circuit Simulator",
              wxDefaultPosition, wxSize(1200, 800)) {

    // Initialize UI
    CreateUI();

    // Apply default theme
    ApplyTheme(false);
}

LogisimMainFrame::~LogisimMainFrame() {
    auiManager.UnInit();
}

void LogisimMainFrame::CreateUI() {
    // Create status bar
    CreateStatusBar(2);
    SetStatusText("Ready - Enhanced Logic Circuit Simulator", 0);
    SetStatusText("Zoom: 100%", 1);

    // Create menu bar
    CreateMenuBar();

    // Create toolbar
    CreateToolbar();

    // Create main panels
    canvas = new CircuitCanvas(this);
    propertiesPanel = new PropertiesPanel(this);
    libraryPanel = new ComponentLibraryPanel(this);

    // Setup AUI layout
    SetupAUI();
}

void LogisimMainFrame::CreateToolbar() {
    wxToolBar* toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT | wxTB_NODIVIDER);
    toolbar->SetToolBitmapSize(wxSize(24, 24));

    // Basic tools
    toolbar->AddTool(static_cast<int>(ComponentType::SELECT), "Select",
                      IconFactory::CreateSelectIcon(), "Select Tool");
    toolbar->AddSeparator();

    // Logic gates
    toolbar->AddTool(static_cast<int>(ComponentType::AND_GATE), "AND Gate",
                      IconFactory::CreateAndGateIcon(), "AND Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::OR_GATE), "OR Gate",
                      IconFactory::CreateOrGateIcon(), "OR Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::NOT_GATE), "NOT Gate",
                      IconFactory::CreateNotGateIcon(), "NOT Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::NAND_GATE), "NAND Gate",
                      IconFactory::CreateNandGateIcon(), "NAND Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::NOR_GATE), "NOR Gate",
                      IconFactory::CreateNorGateIcon(), "NOR Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::XOR_GATE), "XOR Gate",
                      IconFactory::CreateXorGateIcon(), "XOR Gate");
    toolbar->AddTool(static_cast<int>(ComponentType::XNOR_GATE), "XNOR Gate",
                      IconFactory::CreateXnorGateIcon(), "XNOR Gate");
    toolbar->AddSeparator();

    // Arithmetic components
    toolbar->AddTool(static_cast<int>(ComponentType::HALF_ADDER), "Half Adder",
                      IconFactory::CreateHalfAdderIcon(), "Half Adder");
    toolbar->AddTool(static_cast<int>(ComponentType::FULL_ADDER), "Full Adder",
                      IconFactory::CreateFullAdderIcon(), "Full Adder");
    toolbar->AddTool(static_cast<int>(ComponentType::ADDER_4BIT), "4-bit Adder",
                      IconFactory::CreateAdder4BitIcon(), "4-bit Adder");
    toolbar->AddSeparator();

    // Multiplexers and demultiplexers
    toolbar->AddTool(static_cast<int>(ComponentType::MULTIPLEXER_2TO1), "2:1 MUX",
                      IconFactory::CreateMultiplexerIcon(), "2-to-1 Multiplexer");
    toolbar->AddTool(static_cast<int>(ComponentType::MULTIPLEXER_4TO1), "4:1 MUX",
                      IconFactory::CreateMultiplexerIcon(), "4-to-1 Multiplexer");
    toolbar->AddTool(static_cast<int>(ComponentType::DEMULTIPLEXER_1TO2), "1:2 DEMUX",
                      IconFactory::CreateDemultiplexerIcon(), "1-to-2 Demultiplexer");
    toolbar->AddTool(static_cast<int>(ComponentType::DEMULTIPLEXER_1TO4), "1:4 DEMUX",
                      IconFactory::CreateDemultiplexerIcon(), "1-to-4 Demultiplexer");
    toolbar->AddSeparator();

    // I/O components
    toolbar->AddTool(static_cast<int>(ComponentType::INPUT_PIN), "Input",
                      IconFactory::CreateInputIcon(), "Input Switch");
    toolbar->AddTool(static_cast<int>(ComponentType::OUTPUT_PIN), "Output",
                      IconFactory::CreateOutputIcon(), "Output LED");
    toolbar->AddSeparator();

    // Wire tool
    toolbar->AddTool(static_cast<int>(ComponentType::WIRE), "Wire",
                      IconFactory::CreateWireIcon(), "Wire Tool");

    toolbar->Realize();
}

void LogisimMainFrame::CreateMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar;

    // File menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_NEW, "&New Circuit\tCtrl+N", "Create a new circuit");
    fileMenu->Append(wxID_SAVE, "&Save Circuit\tCtrl+S", "Save the current circuit");
    fileMenu->Append(wxID_OPEN, "&Open Circuit\tCtrl+O", "Open a circuit file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt+F4", "Exit the application");

    // Edit menu
    wxMenu* editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z", "Undo last action");
    editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y", "Redo last undone action");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selection");
    editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste from clipboard");
    editMenu->Append(wxID_DELETE, "&Delete\tDel", "Delete selection");

    // View menu
    wxMenu* viewMenu = new wxMenu;
    showGridMenuItem = viewMenu->AppendCheckItem(ID_SHOW_GRID, "Show &Grid\tCtrl+G", "Show/hide grid");
    snapToGridMenuItem = viewMenu->AppendCheckItem(ID_SNAP_TO_GRID, "&Snap to Grid\tCtrl+S", "Enable/disable grid snapping");
    showGridMenuItem->Check(true);
    snapToGridMenuItem->Check(true);
    viewMenu->AppendSeparator();

    viewMenu->Append(ID_ZOOM_IN, "Zoom &In\t+", "Zoom in");
    viewMenu->Append(ID_ZOOM_OUT, "Zoom &Out\t-", "Zoom out");
    viewMenu->Append(ID_ZOOM_RESET, "&Reset Zoom\t0", "Reset zoom to 100%");
    viewMenu->Append(ID_ZOOM_FIT, "&Fit to Window\tF", "Fit circuit to window");
    viewMenu->AppendSeparator();

    showPropertiesMenuItem = viewMenu->AppendCheckItem(ID_SHOW_PROPERTIES, "&Properties Panel", "Show/hide properties panel");
    showLibraryMenuItem = viewMenu->AppendCheckItem(ID_SHOW_LIBRARY, "&Component Library", "Show/hide component library");
    showPropertiesMenuItem->Check(true);
    showLibraryMenuItem->Check(true);

    // Theme menu
    wxMenu* themeMenu = new wxMenu;
    themeMenu->AppendRadioItem(ID_LIGHT_THEME, "&Light Theme", "Apply light theme");
    themeMenu->AppendRadioItem(ID_DARK_THEME, "&Dark Theme", "Apply dark theme");
    themeMenu->FindItem(ID_LIGHT_THEME)->Check(true);

    // Simulation menu
    wxMenu* simulationMenu = new wxMenu;
    simulationMenu->Append(wxID_FORWARD, "&Simulate\tF5", "Simulate the circuit");

    // Help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About", "About this application");

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(editMenu, "&Edit");
    menuBar->Append(viewMenu, "&View");
    menuBar->Append(themeMenu, "&Theme");
    menuBar->Append(simulationMenu, "&Simulation");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);
}

void LogisimMainFrame::SetupAUI() {
    auiManager.SetManagedWindow(this);

    // Add canvas as center pane
    auiManager.AddPane(canvas, wxAuiPaneInfo()
                       .CenterPane()
                       .Name("canvas")
                       .Caption("Circuit Design"));

    // Add properties panel on the right
    auiManager.AddPane(propertiesPanel, wxAuiPaneInfo()
                       .Right()
                       .Name("properties")
                       .Caption("Properties")
                       .MinSize(200, -1)
                       .BestSize(250, -1));

    // Add component library on the left
    auiManager.AddPane(libraryPanel, wxAuiPaneInfo()
                       .Left()
                       .Name("library")
                       .Caption("Component Library")
                       .MinSize(200, -1)
                       .BestSize(220, -1));

    auiManager.Update();
}

void LogisimMainFrame::ApplyTheme(bool isDark) {
    if (isDark) {
        // Dark theme colors
        SetBackgroundColour(wxColour(45, 45, 48));
        canvas->SetBackgroundColor(wxColour(37, 37, 38));
        canvas->SetGridColor(wxColour(60, 60, 60));
    } else {
        // Light theme colors
        SetBackgroundColour(wxColour(240, 240, 240));
        canvas->SetBackgroundColor(wxColour(248, 248, 248));
        canvas->SetGridColor(wxColour(220, 220, 220));
    }

    Refresh();
}

// Event handlers
void LogisimMainFrame::OnToolSelected(wxCommandEvent& event) {
    int toolId = event.GetId();
    ComponentType tool = static_cast<ComponentType>(toolId);

    canvas->SetTool(tool);

    // Update status bar
    switch (tool) {
        case ComponentType::SELECT:
            SetStatusText("Select and move components");
            break;
        case ComponentType::AND_GATE:
            SetStatusText("Add AND gate");
            break;
        case ComponentType::OR_GATE:
            SetStatusText("Add OR gate");
            break;
        case ComponentType::NOT_GATE:
            SetStatusText("Add NOT gate");
            break;
        case ComponentType::NAND_GATE:
            SetStatusText("Add NAND gate");
            break;
        case ComponentType::NOR_GATE:
            SetStatusText("Add NOR gate");
            break;
        case ComponentType::XOR_GATE:
            SetStatusText("Add XOR gate");
            break;
        case ComponentType::XNOR_GATE:
            SetStatusText("Add XNOR gate");
            break;
        case ComponentType::HALF_ADDER:
            SetStatusText("Add Half Adder");
            break;
        case ComponentType::FULL_ADDER:
            SetStatusText("Add Full Adder");
            break;
        case ComponentType::ADDER_4BIT:
            SetStatusText("Add 4-bit Adder");
            break;
        case ComponentType::MULTIPLEXER_2TO1:
            SetStatusText("Add 2-to-1 Multiplexer");
            break;
        case ComponentType::MULTIPLEXER_4TO1:
            SetStatusText("Add 4-to-1 Multiplexer");
            break;
        case ComponentType::DEMULTIPLEXER_1TO2:
            SetStatusText("Add 1-to-2 Demultiplexer");
            break;
        case ComponentType::DEMULTIPLEXER_1TO4:
            SetStatusText("Add 1-to-4 Demultiplexer");
            break;
        case ComponentType::INPUT_PIN:
            SetStatusText("Add input switch (right-click to toggle)");
            break;
        case ComponentType::OUTPUT_PIN:
            SetStatusText("Add output LED");
            break;
        case ComponentType::WIRE:
            SetStatusText("Connect components with wires");
            break;
        default:
            SetStatusText("Ready");
            break;
    }
}

void LogisimMainFrame::OnComponentSelected(wxCommandEvent& event) {
    int componentType = event.GetInt();
    canvas->SetTool(static_cast<ComponentType>(componentType));
}

void LogisimMainFrame::OnCanvasComponentSelected(wxCommandEvent& event) {
    CircuitComponent* component = static_cast<CircuitComponent*>(event.GetClientData());
    propertiesPanel->SetComponent(component);
}

void LogisimMainFrame::OnSimulate(wxCommandEvent& event) {
    canvas->SimulateCircuit();
    canvas->Refresh();
    SetStatusText("Circuit simulated");
}

void LogisimMainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox(wxT("Enhanced Logic Circuit Simulator v2.0\n")
                 wxT("A professional Logisim-compatible application\n\n")
                 wxT("✨ NEW FEATURES:\n")
                 wxT("• Dockable panels with modern AUI interface\n")
                 wxT("• Properties panel for component editing\n")
                 wxT("• Organized component library with search\n")
                 wxT("• Advanced zoom and pan capabilities\n")
                 wxT("• Grid snapping and alignment tools\n")
                 wxT("• Professional light/dark themes\n")
                 wxT("• Enhanced keyboard shortcuts\n\n")
                 wxT("🔧 COMPONENTS:\n")
                 wxT("• Logic gates (AND, OR, NOT, NAND, NOR, XOR, XNOR)\n")
                 wxT("• Arithmetic components (Half/Full Adders, 4-bit Adder)\n")
                 wxT("• Data selection (Multiplexers, Demultiplexers)\n")
                 wxT("• Input switches and LED outputs\n")
                 wxT("• Real-time circuit simulation\n\n")
                 wxT("⌨️ SHORTCUTS:\n")
                 wxT("• Mouse wheel: Zoom in/out\n")
                 wxT("• Middle mouse: Pan view\n")
                 wxT("• Del: Delete selected component\n")
                 wxT("• Esc: Cancel current operation\n")
                 wxT("• +/-: Zoom in/out\n")
                 wxT("• 0: Reset zoom\n")
                 wxT("• Ctrl+G: Toggle grid\n")
                 wxT("• Ctrl+S: Toggle snap to grid"),
                 wxT("About Enhanced Logic Circuit Simulator"),
                 wxOK | wxICON_INFORMATION);
}

void LogisimMainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

// View menu handlers
void LogisimMainFrame::OnShowGrid(wxCommandEvent& event) {
    bool show = event.IsChecked();
    canvas->SetShowGrid(show);
}

void LogisimMainFrame::OnSnapToGrid(wxCommandEvent& event) {
    bool snap = event.IsChecked();
    canvas->SetSnapToGrid(snap);
}

void LogisimMainFrame::OnShowProperties(wxCommandEvent& event) {
    wxAuiPaneInfo& pane = auiManager.GetPane("properties");
    pane.Show(event.IsChecked());
    auiManager.Update();
}

void LogisimMainFrame::OnShowLibrary(wxCommandEvent& event) {
    wxAuiPaneInfo& pane = auiManager.GetPane("library");
    pane.Show(event.IsChecked());
    auiManager.Update();
}

void LogisimMainFrame::OnZoomIn(wxCommandEvent& event) {
    canvas->ZoomIn();
    SetStatusText(wxString::Format("Zoom: %.0f%%", canvas->GetZoom() * 100), 1);
}

void LogisimMainFrame::OnZoomOut(wxCommandEvent& event) {
    canvas->ZoomOut();
    SetStatusText(wxString::Format("Zoom: %.0f%%", canvas->GetZoom() * 100), 1);
}

void LogisimMainFrame::OnZoomReset(wxCommandEvent& event) {
    canvas->ResetZoom();
    SetStatusText("Zoom: 100%", 1);
}

void LogisimMainFrame::OnZoomFit(wxCommandEvent& event) {
    canvas->FitToWindow();
    SetStatusText(wxString::Format("Zoom: %.0f%%", canvas->GetZoom() * 100), 1);
}

// Theme handlers
void LogisimMainFrame::OnLightTheme(wxCommandEvent& event) {
    ApplyTheme(false);
    SetStatusText("Light theme applied");
}

void LogisimMainFrame::OnDarkTheme(wxCommandEvent& event) {
    ApplyTheme(true);
    SetStatusText("Dark theme applied");
}

// File menu handlers
void LogisimMainFrame::OnNewFile(wxCommandEvent& event) {
    // Ask to save current document if modified
    if (document && document->IsModified()) {
        int result = wxMessageBox("The current circuit has been modified. Do you want to save it?",
                                 "Save Changes", wxYES_NO | wxCANCEL | wxICON_QUESTION, this);
        if (result == wxCANCEL) return;
        if (result == wxYES) {
            OnSaveFile(event);
        }
    }

    // Create new document
    if (!document) {
        document = std::make_unique<CircuitDocument>();
    } else {
        document->NewDocument();
    }

    // Clear canvas
    canvas->ClearComponents();
    canvas->Refresh();

    // Update UI
    UpdateTitle();
    SetStatusText("New circuit created", 0);
}

void LogisimMainFrame::OnOpenFile(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open Circuit File", "", "",
                               "Circuit files (*.lcf)|*.lcf|All files (*.*)|*.*",
                               wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    wxString filename = openFileDialog.GetPath();

    if (!document) {
        document = std::make_unique<CircuitDocument>();
    }

    if (document->LoadFromFile(filename)) {
        document->LoadToCanvas(canvas);
        UpdateTitle();
        SetStatusText("Circuit loaded: " + filename, 0);
    } else {
        wxMessageBox("Failed to load circuit file.", "Error", wxOK | wxICON_ERROR, this);
    }
}

void LogisimMainFrame::OnSaveFile(wxCommandEvent& event) {
    if (!document) {
        document = std::make_unique<CircuitDocument>();
    }

    if (document->GetFilename().IsEmpty()) {
        OnSaveAsFile(event);
        return;
    }

    // Save from canvas
    document->SaveFromCanvas(canvas);

    if (document->SaveToFile(document->GetFilename())) {
        UpdateTitle();
        SetStatusText("Circuit saved: " + document->GetFilename(), 0);
    } else {
        wxMessageBox("Failed to save circuit file.", "Error", wxOK | wxICON_ERROR, this);
    }
}

void LogisimMainFrame::OnSaveAsFile(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save Circuit File", "", "",
                               "Circuit files (*.lcf)|*.lcf|All files (*.*)|*.*",
                               wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

    wxString filename = saveFileDialog.GetPath();

    if (!document) {
        document = std::make_unique<CircuitDocument>();
    }

    // Save from canvas
    document->SaveFromCanvas(canvas);

    if (document->SaveAs(filename)) {
        UpdateTitle();
        SetStatusText("Circuit saved as: " + filename, 0);
    } else {
        wxMessageBox("Failed to save circuit file.", "Error", wxOK | wxICON_ERROR, this);
    }
}

// Edit menu handlers
void LogisimMainFrame::OnUndo(wxCommandEvent& event) {
    if (canvas) {
        canvas->GetCommandManager().Undo();
        canvas->Refresh();
        SetStatusText("Undo completed", 0);
    }
}

void LogisimMainFrame::OnRedo(wxCommandEvent& event) {
    if (canvas) {
        canvas->GetCommandManager().Redo();
        canvas->Refresh();
        SetStatusText("Redo completed", 0);
    }
}

void LogisimMainFrame::OnCopy(wxCommandEvent& event) {
    // TODO: Implement clipboard copy functionality
    SetStatusText("Copy functionality not yet implemented", 0);
}

void LogisimMainFrame::OnPaste(wxCommandEvent& event) {
    // TODO: Implement clipboard paste functionality
    SetStatusText("Paste functionality not yet implemented", 0);
}

void LogisimMainFrame::OnDelete(wxCommandEvent& event) {
    if (canvas) {
        // Send delete key event to canvas
        wxKeyEvent keyEvent(wxEVT_KEY_DOWN);
        keyEvent.m_keyCode = WXK_DELETE;
        canvas->GetEventHandler()->ProcessEvent(keyEvent);
        SetStatusText("Selected component deleted", 0);
    }
}

// Private helper methods
void LogisimMainFrame::UpdateTitle() {
    wxString title = "Enhanced Logic Circuit Simulator";

    if (document) {
        if (!document->GetTitle().IsEmpty()) {
            title = document->GetTitle() + " - " + title;
        }
        if (document->IsModified()) {
            title = "*" + title;
        }
    }

    SetTitle(title);
}