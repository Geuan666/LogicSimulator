#include "../../include/ui/properties_panel.h"

wxBEGIN_EVENT_TABLE(PropertiesPanel, wxPanel)
    EVT_PG_CHANGED(wxID_ANY, PropertiesPanel::OnPropertyChanged)
wxEND_EVENT_TABLE()

PropertiesPanel::PropertiesPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), currentComponent(nullptr) {

    // Set background color
    SetBackgroundColour(wxColour(245, 245, 245));

    // Create sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Title
    titleLabel = new wxStaticText(this, wxID_ANY, "Properties");
    titleLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(titleLabel, 0, wxALL | wxEXPAND, 5);

    // Separator (simplified)
    sizer->AddSpacer(5);

    // Property grid
    propGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                  wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
    propGrid->SetBackgroundColour(wxColour(255, 255, 255));
    sizer->Add(propGrid, 1, wxALL | wxEXPAND, 5);

    SetSizer(sizer);
    ClearProperties();
}

void PropertiesPanel::SetComponent(CircuitComponent* component) {
    currentComponent = component;
    if (component) {
        PopulateProperties();
        titleLabel->SetLabel(wxString::Format("Properties - %s",
                            component->GetType() == ComponentType::AND_GATE ? "AND Gate" :
                            component->GetType() == ComponentType::OR_GATE ? "OR Gate" :
                            component->GetType() == ComponentType::NOT_GATE ? "NOT Gate" :
                            component->GetType() == ComponentType::NAND_GATE ? "NAND Gate" :
                            component->GetType() == ComponentType::NOR_GATE ? "NOR Gate" :
                            component->GetType() == ComponentType::XOR_GATE ? "XOR Gate" :
                            component->GetType() == ComponentType::XNOR_GATE ? "XNOR Gate" :
                            component->GetType() == ComponentType::HALF_ADDER ? "Half Adder" :
                            component->GetType() == ComponentType::FULL_ADDER ? "Full Adder" :
                            component->GetType() == ComponentType::ADDER_4BIT ? "4-bit Adder" :
                            component->GetType() == ComponentType::MULTIPLEXER_2TO1 ? "2:1 MUX" :
                            component->GetType() == ComponentType::MULTIPLEXER_4TO1 ? "4:1 MUX" :
                            component->GetType() == ComponentType::DEMULTIPLEXER_1TO2 ? "1:2 DEMUX" :
                            component->GetType() == ComponentType::DEMULTIPLEXER_1TO4 ? "1:4 DEMUX" :
                            component->GetType() == ComponentType::INPUT_PIN ? "Input Switch" :
                            component->GetType() == ComponentType::OUTPUT_PIN ? "Output LED" :
                            "Component"));
    } else {
        ClearProperties();
    }
}

void PropertiesPanel::ClearProperties() {
    propGrid->Clear();
    titleLabel->SetLabel("Properties");

    // Add welcome message
    propGrid->Append(new wxStringProperty("Selection", "selection", "No component selected"));
    propGrid->SetPropertyReadOnly("selection");
    propGrid->SetPropertyTextColour("selection", wxColour(128, 128, 128));
}

void PropertiesPanel::PopulateProperties() {
    propGrid->Clear();

    if (!currentComponent) {
        ClearProperties();
        return;
    }

    // Basic properties for all components
    PopulateBasicProperties();

    // Type-specific properties
    PopulateTypeSpecificProperties();

    // Pin information
    PopulatePinProperties();
}

void PropertiesPanel::PopulateBasicProperties() {
    wxPGProperty* basicCat = propGrid->Append(new wxPropertyCategory("Basic Properties"));

    // Position (editable)
    wxPoint pos = currentComponent->GetPosition();
    propGrid->Append(new wxIntProperty("X Position", "pos_x", pos.x));
    propGrid->Append(new wxIntProperty("Y Position", "pos_y", pos.y));

    // Size (read-only for now)
    wxSize size = currentComponent->GetSize();
    propGrid->Append(new wxIntProperty("Width", "width", size.x));
    propGrid->Append(new wxIntProperty("Height", "height", size.y));
    propGrid->SetPropertyReadOnly("width");
    propGrid->SetPropertyReadOnly("height");

    // Component name (editable)
    propGrid->Append(new wxStringProperty("Component Name", "name", GetComponentName()));

    // Rotation (now editable)
    propGrid->Append(new wxIntProperty("Rotation (degrees)", "rotation",
                                       static_cast<int>(currentComponent->GetRotation())));

    // Scale factors
    double scaleX, scaleY;
    currentComponent->GetScale(scaleX, scaleY);
    propGrid->Append(new wxFloatProperty("Scale X", "scale_x", scaleX));
    propGrid->Append(new wxFloatProperty("Scale Y", "scale_y", scaleY));
}

void PropertiesPanel::PopulateTypeSpecificProperties() {
    ComponentType type = currentComponent->GetType();

    // Create type-specific properties category
    wxPGProperty* typeCat = propGrid->Append(new wxPropertyCategory("Component Properties"));

    switch (type) {
        case ComponentType::INPUT_PIN:
            PopulateInputProperties();
            break;
        case ComponentType::OUTPUT_PIN:
            PopulateOutputProperties();
            break;
        case ComponentType::AND_GATE:
        case ComponentType::OR_GATE:
        case ComponentType::NAND_GATE:
        case ComponentType::NOR_GATE:
        case ComponentType::XOR_GATE:
        case ComponentType::XNOR_GATE:
            PopulateLogicGateProperties();
            break;
        case ComponentType::CLOCK_GENERATOR:
            PopulateClockProperties();
            break;
        case ComponentType::SEVEN_SEGMENT_DISPLAY:
            PopulateDisplayProperties();
            break;
        default:
            PopulateDefaultProperties();
            break;
    }
}

void PropertiesPanel::PopulateInputProperties() {
    // Input switch specific properties
    propGrid->Append(new wxBoolProperty("Current State", "input_state", false));
    propGrid->Append(new wxStringProperty("Input Type", "input_type", "Toggle Switch"));
    propGrid->SetPropertyReadOnly("input_type");
}

void PropertiesPanel::PopulateOutputProperties() {
    // Output LED specific properties
    propGrid->Append(new wxBoolProperty("Current State", "output_state", false));
    propGrid->SetPropertyReadOnly("output_state");
    propGrid->Append(new wxColourProperty("On Color", "on_color", *wxRED));
    propGrid->Append(new wxColourProperty("Off Color", "off_color", *wxBLACK));
}

void PropertiesPanel::PopulateLogicGateProperties() {
    // Logic gate properties
    propGrid->Append(new wxIntProperty("Input Count", "input_count", 2));
    propGrid->Append(new wxIntProperty("Propagation Delay (ns)", "delay", 10));
    propGrid->Append(new wxBoolProperty("Enable Delay Simulation", "enable_delay", false));
}

void PropertiesPanel::PopulateClockProperties() {
    // Clock generator properties
    propGrid->Append(new wxIntProperty("Frequency (Hz)", "frequency", 1));
    propGrid->Append(new wxBoolProperty("Running", "clock_running", false));
    propGrid->Append(new wxIntProperty("Duty Cycle (%)", "duty_cycle", 50));
}

void PropertiesPanel::PopulateDisplayProperties() {
    // 7-segment display properties
    propGrid->Append(new wxBoolProperty("Common Cathode", "common_cathode", true));
    propGrid->Append(new wxColourProperty("On Color", "segment_on_color", *wxRED));
    propGrid->Append(new wxColourProperty("Off Color", "segment_off_color", wxColour(64, 64, 64)));
}

void PropertiesPanel::PopulateDefaultProperties() {
    // Default properties for other components
    propGrid->Append(new wxStringProperty("Description", "description", "Standard component"));
}

void PropertiesPanel::PopulatePinProperties() {
    wxPGProperty* pinsCat = propGrid->Append(new wxPropertyCategory("Pins Information"));
    const auto& pins = currentComponent->GetPins();

    propGrid->Append(new wxIntProperty("Total Pins", "pin_count", pins.size()));
    propGrid->SetPropertyReadOnly("pin_count");

    int inputCount = 0, outputCount = 0;
    for (const auto& pin : pins) {
        if (pin.isInput) inputCount++;
        else outputCount++;
    }

    propGrid->Append(new wxIntProperty("Input Pins", "input_pins", inputCount));
    propGrid->Append(new wxIntProperty("Output Pins", "output_pins", outputCount));
    propGrid->SetPropertyReadOnly("input_pins");
    propGrid->SetPropertyReadOnly("output_pins");

    // Component-specific properties
    SetupComponentProperties();
}

void PropertiesPanel::SetupComponentProperties() {
    if (!currentComponent) return;

    wxPGProperty* specificCat = propGrid->Append(new wxPropertyCategory("Component Properties"));

    ComponentType type = currentComponent->GetType();

    // Add properties based on component type
    switch (type) {
        case ComponentType::INPUT_PIN: {
            propGrid->Append(new wxBoolProperty("State", "input_state", false));
            propGrid->Append(new wxStringProperty("Label", "input_label", "Input"));
            break;
        }
        case ComponentType::OUTPUT_PIN: {
            const auto& pins = currentComponent->GetPins();
            if (!pins.empty()) {
                wxString stateStr = pins[0].value == LogicValue::HIGH ? "HIGH" :
                                  pins[0].value == LogicValue::LOW ? "LOW" : "UNDEFINED";
                propGrid->Append(new wxStringProperty("Current State", "output_state", stateStr));
                propGrid->SetPropertyReadOnly("output_state");
            }
            propGrid->Append(new wxStringProperty("Label", "output_label", "Output"));
            break;
        }
        case ComponentType::ADDER_4BIT: {
            propGrid->Append(new wxStringProperty("Data Width", "data_width", "4 bits"));
            propGrid->SetPropertyReadOnly("data_width");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        case ComponentType::MULTIPLEXER_2TO1:
        case ComponentType::MULTIPLEXER_4TO1: {
            int inputs = type == ComponentType::MULTIPLEXER_2TO1 ? 2 : 4;
            propGrid->Append(new wxIntProperty("Input Count", "mux_inputs", inputs));
            propGrid->SetPropertyReadOnly("mux_inputs");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        case ComponentType::DEMULTIPLEXER_1TO2:
        case ComponentType::DEMULTIPLEXER_1TO4: {
            int outputs = type == ComponentType::DEMULTIPLEXER_1TO2 ? 2 : 4;
            propGrid->Append(new wxIntProperty("Output Count", "demux_outputs", outputs));
            propGrid->SetPropertyReadOnly("demux_outputs");
            propGrid->Append(new wxBoolProperty("Show Pin Labels", "show_labels", true));
            break;
        }
        default: {
            // Generic logic gate properties
            propGrid->Append(new wxStringProperty("Logic Function", "logic_function",
                type == ComponentType::AND_GATE ? "AND" :
                type == ComponentType::OR_GATE ? "OR" :
                type == ComponentType::NOT_GATE ? "NOT" :
                type == ComponentType::NAND_GATE ? "NAND" :
                type == ComponentType::NOR_GATE ? "NOR" :
                type == ComponentType::XOR_GATE ? "XOR" :
                type == ComponentType::XNOR_GATE ? "XNOR" : "Unknown"));
            propGrid->SetPropertyReadOnly("logic_function");

            if (type != ComponentType::NOT_GATE) {
                propGrid->Append(new wxIntProperty("Input Count", "input_count", 2));
                propGrid->SetPropertyReadOnly("input_count");
            }
            break;
        }
    }

    // Appearance properties
    wxPGProperty* appearanceCat = propGrid->Append(new wxPropertyCategory("Appearance"));
    propGrid->Append(new wxBoolProperty("Selected", "is_selected", currentComponent->IsSelected()));
    propGrid->SetPropertyReadOnly("is_selected");

    propGrid->Append(new wxColourProperty("Border Color", "border_color",
                      currentComponent->IsSelected() ? *wxRED : *wxBLACK));
    propGrid->Append(new wxColourProperty("Fill Color", "fill_color", wxColour(240, 240, 240)));
}

void PropertiesPanel::OnPropertyChanged(wxPropertyGridEvent& event) {
    if (!currentComponent) return;

    wxPGProperty* prop = event.GetProperty();
    if (!prop) return;

    wxString name = prop->GetName();

    // Handle position changes
    if (name == "pos_x" || name == "pos_y") {
        wxPoint currentPos = currentComponent->GetPosition();
        int newX = propGrid->GetPropertyValue("pos_x").GetLong();
        int newY = propGrid->GetPropertyValue("pos_y").GetLong();

        wxPoint offset(newX - currentPos.x, newY - currentPos.y);
        if (offset.x != 0 || offset.y != 0) {
            currentComponent->Move(offset);
            GetParent()->Refresh(); // Refresh the canvas
        }
    }

    // Handle rotation changes
    if (name == "rotation") {
        double newRotation = propGrid->GetPropertyValue("rotation").GetLong();
        currentComponent->SetRotation(newRotation);
        GetParent()->Refresh();
    }

    // Handle scale changes
    if (name == "scale_x" || name == "scale_y") {
        double scaleX = propGrid->GetPropertyValue("scale_x").GetDouble();
        double scaleY = propGrid->GetPropertyValue("scale_y").GetDouble();
        currentComponent->SetScale(scaleX, scaleY);
        GetParent()->Refresh();
    }

    // Handle other property changes as needed
    // This can be extended for more interactive properties
}

void PropertiesPanel::UpdateProperties() {
    if (currentComponent) {
        PopulateProperties();
    }
}

wxString PropertiesPanel::GetComponentName() const {
    if (!currentComponent) return "Unknown";

    ComponentType type = currentComponent->GetType();
    switch (type) {
        case ComponentType::AND_GATE: return "AND Gate";
        case ComponentType::OR_GATE: return "OR Gate";
        case ComponentType::NOT_GATE: return "NOT Gate";
        case ComponentType::NAND_GATE: return "NAND Gate";
        case ComponentType::NOR_GATE: return "NOR Gate";
        case ComponentType::XOR_GATE: return "XOR Gate";
        case ComponentType::XNOR_GATE: return "XNOR Gate";
        case ComponentType::INPUT_PIN: return "Input Switch";
        case ComponentType::OUTPUT_PIN: return "Output LED";
        case ComponentType::HALF_ADDER: return "Half Adder";
        case ComponentType::FULL_ADDER: return "Full Adder";
        case ComponentType::ADDER_4BIT: return "4-bit Adder";
        case ComponentType::MULTIPLEXER_2TO1: return "2:1 MUX";
        case ComponentType::MULTIPLEXER_4TO1: return "4:1 MUX";
        case ComponentType::DEMULTIPLEXER_1TO2: return "1:2 DEMUX";
        case ComponentType::DEMULTIPLEXER_1TO4: return "1:4 DEMUX";
        case ComponentType::D_FLIPFLOP: return "D Flip-Flop";
        case ComponentType::JK_FLIPFLOP: return "JK Flip-Flop";
        case ComponentType::SR_LATCH: return "SR Latch";
        case ComponentType::CLOCK_GENERATOR: return "Clock Generator";
        case ComponentType::SEVEN_SEGMENT_DISPLAY: return "7-Segment Display";
        case ComponentType::LED_MATRIX_8X8: return "LED Matrix 8x8";
        case ComponentType::DECODER_3TO8: return "3:8 Decoder";
        case ComponentType::BCD_TO_7SEGMENT: return "BCD to 7-Segment";
        case ComponentType::PRIORITY_ENCODER: return "Priority Encoder";
        default: return "Component";
    }
}