#pragma once
#include <wx/wx.h>
#include <vector>
#include <memory>
#include "../components/circuit_component.h"
#include "../components/wire.h"
#include "../core/command_system.h"

// Enhanced canvas with zoom and pan capabilities
class CircuitCanvas : public wxWindow {
private:
    std::vector<std::unique_ptr<CircuitComponent>> components;
    std::unique_ptr<Wire> currentWire;
    CircuitComponent* selectedComponent;
    ComponentType currentTool;
    wxPoint dragStart;
    wxPoint dragStartPos; // Original position of component when drag started
    bool isDragging;
    bool isPanning;

    // Command system for undo/redo
    CommandManager commandManager;

    // View transformation
    double zoomFactor;
    wxPoint panOffset;
    wxPoint lastPanPoint;

    // Grid settings
    bool showGrid;
    bool snapToGrid;
    int gridSize;

    // Visual enhancements
    wxColour backgroundColor;
    wxColour gridColor;
    wxColour selectionColor;

public:
    CircuitCanvas(wxWindow* parent);

    void OnPaint(wxPaintEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnMiddleDown(wxMouseEvent& event);
    void OnMiddleUp(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);

    // Menu command handlers for accelerator table
    void OnUndoCommand(wxCommandEvent& event);
    void OnRedoCommand(wxCommandEvent& event);
    void OnDeleteCommand(wxCommandEvent& event);

    void SetTool(ComponentType tool);
    void SimulateCircuit();

    // View control methods
    void ZoomIn();
    void ZoomOut();
    void ResetZoom();
    void FitToWindow();
    void SetZoom(double factor);
    double GetZoom() const { return zoomFactor; }

    // Grid control
    void SetShowGrid(bool show) { showGrid = show; Refresh(); }
    bool GetShowGrid() const { return showGrid; }
    void SetSnapToGrid(bool snap) { snapToGrid = snap; }
    bool GetSnapToGrid() const { return snapToGrid; }
    void SetGridSize(int size) { gridSize = size; Refresh(); }
    int GetGridSize() const { return gridSize; }

    // Coordinate transformation
    wxPoint ScreenToWorld(const wxPoint& screenPoint) const;
    wxPoint WorldToScreen(const wxPoint& worldPoint) const;
    wxPoint SnapToGrid(const wxPoint& point) const;

    // Selection and editing
    void SelectComponent(CircuitComponent* component);
    void ClearSelection();
    CircuitComponent* GetSelectedComponent() const { return selectedComponent; }

    // Theme customization
    void SetBackgroundColor(const wxColour& color) { backgroundColor = color; Refresh(); }
    void SetGridColor(const wxColour& color) { gridColor = color; Refresh(); }

    // Document integration methods
    void ClearComponents();
    void AddComponentCopy(const CircuitComponent* component);
    std::unique_ptr<CircuitComponent> CloneComponent(const CircuitComponent* component) const;
    const std::vector<std::unique_ptr<CircuitComponent>>& GetComponents() const { return components; }

    // Command system support methods
    void RemoveComponentDirectly(CircuitComponent* component);
    void InsertComponentAt(size_t index, std::unique_ptr<CircuitComponent> component);
    size_t GetComponentIndex(CircuitComponent* component) const;
    void AddComponentDirectly(std::unique_ptr<CircuitComponent> component);
    std::unique_ptr<CircuitComponent> ExtractComponent(CircuitComponent* component);

    // Access to command manager
    CommandManager& GetCommandManager() { return commandManager; }

public:
    CircuitComponent* CreateComponent(ComponentType type, const wxPoint& pos);

private:
    void DrawGrid(wxDC& dc);
    void DrawComponents(wxDC& dc);
    void DrawSelection(wxDC& dc);

    wxDECLARE_EVENT_TABLE();

public:
    // Custom events
    static const wxEventType wxEVT_COMPONENT_SELECTED;
};