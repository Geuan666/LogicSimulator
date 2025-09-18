#include "../../include/ui/circuit_canvas.h"
#include "../../include/components/logic_gates.h"
#include "../../include/components/arithmetic_components.h"
#include "../../include/components/io_components.h"
#include "../../include/components/wire.h"
#include "../../include/components/sequential_components.h"
#include "../../include/components/decoder_encoder_components.h"
#include "../../include/components/display_components.h"
#include "../../include/core/command_system.h"
#include <wx/dcbuffer.h>
#include <cmath>

// Define custom event
const wxEventType CircuitCanvas::wxEVT_COMPONENT_SELECTED = wxNewEventType();

wxBEGIN_EVENT_TABLE(CircuitCanvas, wxWindow)
    EVT_PAINT(CircuitCanvas::OnPaint)
    EVT_LEFT_DOWN(CircuitCanvas::OnLeftDown)
    EVT_LEFT_UP(CircuitCanvas::OnLeftUp)
    EVT_MOTION(CircuitCanvas::OnMouseMove)
    EVT_RIGHT_DOWN(CircuitCanvas::OnRightDown)
    EVT_MOUSEWHEEL(CircuitCanvas::OnMouseWheel)
    EVT_MIDDLE_DOWN(CircuitCanvas::OnMiddleDown)
    EVT_MIDDLE_UP(CircuitCanvas::OnMiddleUp)
    EVT_KEY_DOWN(CircuitCanvas::OnKeyDown)
wxEND_EVENT_TABLE()

CircuitCanvas::CircuitCanvas(wxWindow* parent)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS),
      selectedComponent(nullptr),
      currentTool(ComponentType::SELECT),
      isDragging(false),
      isPanning(false),
      zoomFactor(1.0),
      panOffset(0, 0),
      showGrid(true),
      snapToGrid(true),
      gridSize(20),
      backgroundColor(wxColour(248, 248, 248)),
      gridColor(wxColour(220, 220, 220)),
      selectionColor(wxColour(0, 120, 215)) {

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetCanFocus(true);
}

void CircuitCanvas::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);

    // Clear background
    dc.SetBackground(wxBrush(backgroundColor));
    dc.Clear();

    // Apply transformations
    dc.SetUserScale(zoomFactor, zoomFactor);
    dc.SetDeviceOrigin(panOffset.x, panOffset.y);

    // Draw grid
    if (showGrid) {
        DrawGrid(dc);
    }

    // Draw components
    DrawComponents(dc);

    // Draw current wire being created
    if (currentWire) {
        currentWire->Draw(dc);
    }

    // Draw selection highlight
    DrawSelection(dc);

    // Reset transformations for UI elements
    dc.SetUserScale(1.0, 1.0);
    dc.SetDeviceOrigin(0, 0);

    // Draw zoom indicator
    dc.SetTextForeground(wxColour(100, 100, 100));
    dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    dc.DrawText(wxString::Format("Zoom: %.0f%%", zoomFactor * 100), 10, 10);
}

void CircuitCanvas::OnLeftDown(wxMouseEvent& event) {
    SetFocus(); // Ensure canvas can receive keyboard events
    wxPoint screenPos = event.GetPosition();
    wxPoint worldPos = ScreenToWorld(screenPos);

    // Check if we're clicking on a component
    selectedComponent = nullptr;
    for (auto it = components.rbegin(); it != components.rend(); ++it) {
        if ((*it)->Contains(worldPos)) {
            selectedComponent = it->get();
            break;
        }
    }

    // Deselect all components
    for (auto& component : components) {
        component->Select(false);
    }

    if (selectedComponent) {
        selectedComponent->Select(true);

        // Check if clicked on a pin
        Pin* pin = selectedComponent->GetPinAt(worldPos);
        if (pin && currentTool == ComponentType::WIRE) {
            // Start creating a wire
            currentWire = std::make_unique<Wire>(pin);
            pin->isConnected = true;
        } else {
            // Start dragging the component
            dragStart = screenPos;
            isDragging = true;
        }
    } else {
        // Create a new component based on the current tool
        wxPoint gridPos = snapToGrid ? SnapToGrid(worldPos) : worldPos;
        CircuitComponent* newComponent = CreateComponent(currentTool, gridPos);
        if (newComponent) {
            components.push_back(std::unique_ptr<CircuitComponent>(newComponent));
            selectedComponent = components.back().get();
            selectedComponent->Select(true);
        }
    }

    Refresh();
}

void CircuitCanvas::OnLeftUp(wxMouseEvent& event) {
    wxPoint screenPos = event.GetPosition();
    wxPoint worldPos = ScreenToWorld(screenPos);

    if (isDragging) {
        isDragging = false;
    }

    if (currentWire) {
        // Check if we're connecting to another pin
        bool connected = false;
        for (auto& component : components) {
            Pin* pin = component->GetPinAt(worldPos);
            if (pin && pin != currentWire->GetStartPin()) {
                // Connect the wire to this pin
                currentWire->SetEndPin(pin);
                pin->isConnected = true;
                connected = true;
                components.push_back(std::move(currentWire));
                break;
            }
        }

        if (!connected) {
            // Wire not connected to an end pin, discard it
            currentWire->GetStartPin()->isConnected = false;
            currentWire.reset();
        }
    }

    SimulateCircuit();
    Refresh();
}

void CircuitCanvas::OnMouseMove(wxMouseEvent& event) {
    wxPoint screenPos = event.GetPosition();
    wxPoint worldPos = ScreenToWorld(screenPos);

    if (isPanning) {
        // Pan the view
        wxPoint delta = screenPos - lastPanPoint;
        panOffset += delta;
        lastPanPoint = screenPos;
        Refresh();
        return;
    }

    if (isDragging && selectedComponent) {
        // Move the selected component
        wxPoint screenDelta = screenPos - dragStart;
        wxPoint worldDelta(screenDelta.x / zoomFactor, screenDelta.y / zoomFactor);

        if (snapToGrid) {
            wxPoint currentPos = selectedComponent->GetPosition();
            wxPoint newPos = SnapToGrid(currentPos + worldDelta);
            worldDelta = newPos - currentPos;
        }

        selectedComponent->Move(worldDelta);
        dragStart = screenPos;
        Refresh();
    }

    if (currentWire) {
        // Update the end point of the wire being created
        currentWire->AddPoint(worldPos);
        Refresh();
    }
}

void CircuitCanvas::OnRightDown(wxMouseEvent& event) {
    wxPoint pos = event.GetPosition();

    // Check if we clicked on a switch
    for (auto& component : components) {
        if (component->GetType() == ComponentType::INPUT_PIN && component->Contains(pos)) {
            // Toggle the switch
            static_cast<InputSwitch*>(component.get())->Toggle();
            SimulateCircuit();
            Refresh();
            break;
        }
    }
}

void CircuitCanvas::SetTool(ComponentType tool) {
    currentTool = tool;
}

CircuitComponent* CircuitCanvas::CreateComponent(ComponentType type, const wxPoint& pos) {
    switch (type) {
        case ComponentType::AND_GATE:
            return new AndGate(pos);
        case ComponentType::OR_GATE:
            return new OrGate(pos);
        case ComponentType::NOT_GATE:
            return new NotGate(pos);
        case ComponentType::NAND_GATE:
            return new NandGate(pos);
        case ComponentType::NOR_GATE:
            return new NorGate(pos);
        case ComponentType::XOR_GATE:
            return new XorGate(pos);
        case ComponentType::XNOR_GATE:
            return new XnorGate(pos);
        case ComponentType::INPUT_PIN:
            return new InputSwitch(pos);
        case ComponentType::OUTPUT_PIN:
            return new OutputLED(pos);
        case ComponentType::HALF_ADDER:
            return new HalfAdder(pos);
        case ComponentType::FULL_ADDER:
            return new FullAdder(pos);
        case ComponentType::ADDER_4BIT:
            return new Adder4Bit(pos);
        case ComponentType::MULTIPLEXER_2TO1:
            return new Multiplexer2to1(pos);
        case ComponentType::MULTIPLEXER_4TO1:
            return new Multiplexer4to1(pos);
        case ComponentType::DEMULTIPLEXER_1TO2:
            return new Demultiplexer1to2(pos);
        case ComponentType::DEMULTIPLEXER_1TO4:
            return new Demultiplexer1to4(pos);
        // Sequential Logic Components
        case ComponentType::D_FLIPFLOP:
            return new DFlipFlop(pos);
        case ComponentType::JK_FLIPFLOP:
            return new JKFlipFlop(pos);
        case ComponentType::SR_LATCH:
            return new SRLatch(pos);
        case ComponentType::CLOCK_GENERATOR:
            return new ClockGenerator(pos);
        // Only include implemented components for now
        case ComponentType::DECODER_3TO8:
            return new Decoder3to8(pos);
        case ComponentType::BCD_TO_7SEGMENT:
            return new BCDTo7SegmentDecoder(pos);
        case ComponentType::PRIORITY_ENCODER:
            return new PriorityEncoder8to3(pos);
        // Display Components
        case ComponentType::SEVEN_SEGMENT_DISPLAY:
            return new SevenSegmentDisplay(pos);
        case ComponentType::LED_MATRIX_8X8:
            return new LEDMatrix8x8(pos);
        case ComponentType::HEX_DISPLAY:
            return new HexDisplay(pos);
        case ComponentType::BINARY_DISPLAY:
            return new BinaryDisplay8Bit(pos);
        default:
            return nullptr;
    }
}

void CircuitCanvas::SimulateCircuit() {
    // Reset all non-input values
    for (auto& component : components) {
        if (component->GetType() != ComponentType::INPUT_PIN) {
            for (auto& pin : component->GetPins()) {
                if (pin.isInput) {
                    pin.value = LogicValue::UNDEFINED;
                }
            }
        }
    }

    // Propagate values through the circuit
    bool changed = true;
    while (changed) {
        changed = false;

        // Process all wires to propagate values
        for (auto& component : components) {
            if (component->GetType() == ComponentType::WIRE) {
                Wire* wire = static_cast<Wire*>(component.get());
                Pin* startPin = wire->GetStartPin();
                Pin* endPin = wire->GetEndPin();

                if (startPin && endPin) {
                    // Propagate from output to input
                    if (!startPin->isInput && endPin->isInput) {
                        if (endPin->value != startPin->value) {
                            endPin->value = startPin->value;
                            changed = true;
                        }
                    }
                    // Propagate from input to output
                    else if (startPin->isInput && !endPin->isInput) {
                        if (startPin->value != endPin->value) {
                            startPin->value = endPin->value;
                            changed = true;
                        }
                    }
                }
            }
        }

        // Evaluate logic gates
        for (auto& component : components) {
            if (auto gate = dynamic_cast<LogicGate*>(component.get())) {
                LogicValue result = gate->Evaluate();

                // Find the output pin and update its value
                for (auto& pin : gate->GetPins()) {
                    if (!pin.isInput && pin.value != result) {
                        pin.value = result;
                        changed = true;
                    }
                }
            }
            // Evaluate arithmetic components
            else if (auto arithComp = dynamic_cast<ArithmeticComponent*>(component.get())) {
                // Store old output values to detect changes
                std::vector<LogicValue> oldOutputs;
                for (const auto& pin : arithComp->GetPins()) {
                    if (!pin.isInput) {
                        oldOutputs.push_back(pin.value);
                    }
                }

                arithComp->ComputeOutputs();

                // Check if any output changed
                int outputIndex = 0;
                for (const auto& pin : arithComp->GetPins()) {
                    if (!pin.isInput) {
                        if (outputIndex < oldOutputs.size() && pin.value != oldOutputs[outputIndex]) {
                            changed = true;
                        }
                        outputIndex++;
                    }
                }
            }
        }
    }
}

// New event handlers for enhanced functionality
void CircuitCanvas::OnMouseWheel(wxMouseEvent& event) {
    int rotation = event.GetWheelRotation();
    wxPoint mousePos = event.GetPosition();

    if (rotation > 0) {
        ZoomIn();
    } else {
        ZoomOut();
    }

    // TODO: Zoom towards mouse position
    Refresh();
}

void CircuitCanvas::OnMiddleDown(wxMouseEvent& event) {
    isPanning = true;
    lastPanPoint = event.GetPosition();
    SetCursor(wxCursor(wxCURSOR_SIZING));
}

void CircuitCanvas::OnMiddleUp(wxMouseEvent& event) {
    isPanning = false;
    SetCursor(wxCursor(wxCURSOR_DEFAULT));
}

void CircuitCanvas::OnKeyDown(wxKeyEvent& event) {
    int keyCode = event.GetKeyCode();

    switch (keyCode) {
        case WXK_DELETE:
        case WXK_BACK:
            // Delete selected component directly for now
            if (selectedComponent) {
                for (auto it = components.begin(); it != components.end(); ++it) {
                    if (it->get() == selectedComponent) {
                        components.erase(it);
                        selectedComponent = nullptr;
                        Refresh();
                        break;
                    }
                }
            }
            break;

        case WXK_ESCAPE:
            // Cancel current operation
            if (currentWire) {
                currentWire->GetStartPin()->isConnected = false;
                currentWire.reset();
                Refresh();
            }
            ClearSelection();
            break;

        case 'R':
        case 'r':
            // Rotate selected component
            if (selectedComponent) {
                selectedComponent->Rotate(90.0);
                Refresh();
            }
            break;

        case WXK_PAGEUP:
            // Scale up selected component
            if (selectedComponent) {
                selectedComponent->Scale(1.25, 1.25);
                Refresh();
            }
            break;

        case WXK_PAGEDOWN:
            // Scale down selected component
            if (selectedComponent) {
                selectedComponent->Scale(0.8, 0.8);
                Refresh();
            }
            break;

        case '1':
            // Reset scale and rotation to default
            if (selectedComponent) {
                selectedComponent->SetScale(1.0, 1.0);
                selectedComponent->SetRotation(0.0);
                Refresh();
            }
            break;

        case '+':
        case WXK_NUMPAD_ADD:
            ZoomIn();
            break;

        case '-':
        case WXK_NUMPAD_SUBTRACT:
            ZoomOut();
            break;

        case '0':
        case WXK_NUMPAD0:
            ResetZoom();
            break;

        case 'G':
        case 'g':
            if (event.ControlDown()) {
                SetShowGrid(!GetShowGrid());
            }
            break;

        case 'S':
        case 's':
            if (event.ControlDown()) {
                SetSnapToGrid(!GetSnapToGrid());
            }
            break;

        default:
            event.Skip();
            break;
    }
}

// View control methods
void CircuitCanvas::ZoomIn() {
    SetZoom(zoomFactor * 1.2);
}

void CircuitCanvas::ZoomOut() {
    SetZoom(zoomFactor / 1.2);
}

void CircuitCanvas::ResetZoom() {
    zoomFactor = 1.0;
    panOffset = wxPoint(0, 0);
    Refresh();
}

void CircuitCanvas::FitToWindow() {
    if (components.empty()) {
        ResetZoom();
        return;
    }

    // Calculate bounding box of all components
    wxRect bounds;
    bool first = true;

    for (const auto& component : components) {
        wxPoint pos = component->GetPosition();
        wxSize size = component->GetSize();
        wxRect compRect(pos, size);

        if (first) {
            bounds = compRect;
            first = false;
        } else {
            bounds = bounds.Union(compRect);
        }
    }

    // Add some padding
    bounds.Inflate(50);

    // Calculate zoom to fit
    wxSize canvasSize = GetClientSize();
    double zoomX = (double)canvasSize.x / bounds.width;
    double zoomY = (double)canvasSize.y / bounds.height;
    double newZoom = std::min(zoomX, zoomY) * 0.9; // 90% to leave some margin

    // Set zoom and center view
    SetZoom(newZoom);
    panOffset.x = (canvasSize.x - bounds.width * zoomFactor) / 2 - bounds.x * zoomFactor;
    panOffset.y = (canvasSize.y - bounds.height * zoomFactor) / 2 - bounds.y * zoomFactor;

    Refresh();
}

void CircuitCanvas::SetZoom(double factor) {
    zoomFactor = std::max(0.1, std::min(5.0, factor));
    Refresh();
}

// Coordinate transformation methods
wxPoint CircuitCanvas::ScreenToWorld(const wxPoint& screenPoint) const {
    return wxPoint((screenPoint.x - panOffset.x) / zoomFactor,
                   (screenPoint.y - panOffset.y) / zoomFactor);
}

wxPoint CircuitCanvas::WorldToScreen(const wxPoint& worldPoint) const {
    return wxPoint(worldPoint.x * zoomFactor + panOffset.x,
                   worldPoint.y * zoomFactor + panOffset.y);
}

wxPoint CircuitCanvas::SnapToGrid(const wxPoint& point) const {
    int gridX = ((point.x + gridSize / 2) / gridSize) * gridSize;
    int gridY = ((point.y + gridSize / 2) / gridSize) * gridSize;
    return wxPoint(gridX, gridY);
}

// Selection methods
void CircuitCanvas::SelectComponent(CircuitComponent* component) {
    // Deselect all components
    for (auto& comp : components) {
        comp->Select(false);
    }

    // Select the specified component
    selectedComponent = component;
    if (component) {
        component->Select(true);
    }

    // Send selection event
    wxCommandEvent selectionEvent(wxEVT_COMPONENT_SELECTED, GetId());
    selectionEvent.SetEventObject(this);
    selectionEvent.SetClientData(component);
    ProcessEvent(selectionEvent);

    Refresh();
}

void CircuitCanvas::ClearSelection() {
    SelectComponent(nullptr);
}

// Drawing helper methods
void CircuitCanvas::DrawGrid(wxDC& dc) {
    dc.SetPen(wxPen(gridColor, 1));

    wxSize size = GetClientSize();

    // Calculate visible world area
    wxPoint topLeft = ScreenToWorld(wxPoint(0, 0));
    wxPoint bottomRight = ScreenToWorld(wxPoint(size.x, size.y));

    // Adjust to grid boundaries
    int startX = (topLeft.x / gridSize) * gridSize;
    int startY = (topLeft.y / gridSize) * gridSize;
    int endX = ((bottomRight.x / gridSize) + 1) * gridSize;
    int endY = ((bottomRight.y / gridSize) + 1) * gridSize;

    // Draw vertical lines
    for (int x = startX; x <= endX; x += gridSize) {
        dc.DrawLine(x, startY, x, endY);
    }

    // Draw horizontal lines
    for (int y = startY; y <= endY; y += gridSize) {
        dc.DrawLine(startX, y, endX, y);
    }
}

void CircuitCanvas::DrawComponents(wxDC& dc) {
    for (const auto& component : components) {
        component->Draw(dc);
    }
}

void CircuitCanvas::DrawSelection(wxDC& dc) {
    if (selectedComponent) {
        wxPoint pos = selectedComponent->GetPosition();
        wxSize size = selectedComponent->GetSize();

        dc.SetPen(wxPen(selectionColor, 2, wxPENSTYLE_DOT));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(pos.x - 5, pos.y - 5, size.x + 10, size.y + 10);

        // Draw selection handles
        dc.SetBrush(wxBrush(selectionColor));
        dc.SetPen(wxPen(selectionColor, 1));

        int handleSize = 6;
        // Top-left
        dc.DrawRectangle(pos.x - handleSize/2, pos.y - handleSize/2, handleSize, handleSize);
        // Top-right
        dc.DrawRectangle(pos.x + size.x - handleSize/2, pos.y - handleSize/2, handleSize, handleSize);
        // Bottom-left
        dc.DrawRectangle(pos.x - handleSize/2, pos.y + size.y - handleSize/2, handleSize, handleSize);
        // Bottom-right
        dc.DrawRectangle(pos.x + size.x - handleSize/2, pos.y + size.y - handleSize/2, handleSize, handleSize);
    }
}

// Document integration methods
void CircuitCanvas::ClearComponents() {
    components.clear();
    selectedComponent = nullptr;
    currentWire.reset();
    Refresh();
}

void CircuitCanvas::AddComponentCopy(const CircuitComponent* component) {
    if (!component) return;

    std::unique_ptr<CircuitComponent> newComponent = CloneComponent(component);
    if (newComponent) {
        components.push_back(std::move(newComponent));
    }
}

std::unique_ptr<CircuitComponent> CircuitCanvas::CloneComponent(const CircuitComponent* component) const {
    if (!component) return nullptr;

    ComponentType type = component->GetType();
    wxPoint pos = component->GetPosition();

    return std::unique_ptr<CircuitComponent>(const_cast<CircuitCanvas*>(this)->CreateComponent(type, pos));
}

// Command system methods
void CircuitCanvas::AddComponentDirectly(std::unique_ptr<CircuitComponent> component) {
    if (component) {
        components.push_back(std::move(component));
        Refresh();
    }
}

void CircuitCanvas::RemoveComponentDirectly(CircuitComponent* component) {
    auto it = std::find_if(components.begin(), components.end(),
        [component](const std::unique_ptr<CircuitComponent>& ptr) {
            return ptr.get() == component;
        });

    if (it != components.end()) {
        if (selectedComponent == component) {
            selectedComponent = nullptr;
        }
        components.erase(it);
        Refresh();
    }
}

std::unique_ptr<CircuitComponent> CircuitCanvas::ExtractComponent(CircuitComponent* component) {
    auto it = std::find_if(components.begin(), components.end(),
        [component](const std::unique_ptr<CircuitComponent>& ptr) {
            return ptr.get() == component;
        });

    if (it != components.end()) {
        if (selectedComponent == component) {
            selectedComponent = nullptr;
        }
        std::unique_ptr<CircuitComponent> extracted = std::move(*it);
        components.erase(it);
        return extracted;
    }

    return nullptr;
}

void CircuitCanvas::InsertComponentAt(size_t index, std::unique_ptr<CircuitComponent> component) {
    if (component && index <= components.size()) {
        components.insert(components.begin() + index, std::move(component));
        Refresh();
    }
}