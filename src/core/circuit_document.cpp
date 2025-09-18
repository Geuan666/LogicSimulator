#include "../../include/core/circuit_document.h"
#include "../../include/ui/circuit_canvas.h"
#include "../../include/components/logic_gates.h"
#include "../../include/components/arithmetic_components.h"
#include "../../include/components/io_components.h"
#include "../../include/components/wire.h"
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>

CircuitDocument::CircuitDocument() : modified(false), title("Untitled Circuit") {
    SetProperty("version", "2.0");
    SetProperty("description", "Logic Circuit Design");
}

CircuitDocument::~CircuitDocument() {
    ClearComponents();
}

bool CircuitDocument::LoadFromFile(const wxString& filepath) {
    if (!wxFileExists(filepath)) {
        return false;
    }

    wxFileInputStream fileStream(filepath);
    if (!fileStream.IsOk()) {
        return false;
    }

    wxTextInputStream textStream(fileStream);
    wxString jsonData;
    wxString line;

    while (!fileStream.Eof()) {
        line = textStream.ReadLine();
        jsonData += line + "\n";
    }

    if (DeserializeFromJSON(jsonData)) {
        filename = filepath;
        title = wxFileName(filepath).GetName();
        modified = false;
        return true;
    }

    return false;
}

bool CircuitDocument::SaveToFile(const wxString& filepath) {
    wxString jsonData = SerializeToJSON();

    wxFileOutputStream fileStream(filepath);
    if (!fileStream.IsOk()) {
        return false;
    }

    wxTextOutputStream textStream(fileStream);
    textStream.WriteString(jsonData);

    if (fileStream.IsOk()) {
        filename = filepath;
        title = wxFileName(filepath).GetName();
        modified = false;
        return true;
    }

    return false;
}

bool CircuitDocument::SaveAs(const wxString& filepath) {
    return SaveToFile(filepath);
}

void CircuitDocument::AddComponent(std::unique_ptr<CircuitComponent> component) {
    components.push_back(std::move(component));
    SetModified(true);
}

void CircuitDocument::RemoveComponent(CircuitComponent* component) {
    auto it = std::find_if(components.begin(), components.end(),
        [component](const std::unique_ptr<CircuitComponent>& ptr) {
            return ptr.get() == component;
        });

    if (it != components.end()) {
        components.erase(it);
        SetModified(true);
    }
}

void CircuitDocument::ClearComponents() {
    components.clear();
    SetModified(true);
}

void CircuitDocument::NewDocument() {
    ClearComponents();
    filename.Clear();
    title = "Untitled Circuit";
    modified = false;

    // Reset properties
    properties.clear();
    SetProperty("version", "2.0");
    SetProperty("description", "Logic Circuit Design");
}

void CircuitDocument::SetProperty(const wxString& key, const wxString& value) {
    properties[key] = value;
    SetModified(true);
}

wxString CircuitDocument::GetProperty(const wxString& key, const wxString& defaultValue) const {
    auto it = properties.find(key);
    return (it != properties.end()) ? it->second : defaultValue;
}

wxString CircuitDocument::SerializeToJSON() const {
    // Simplified JSON serialization without external library
    wxString json = "{\n";

    // Metadata
    json += "  \"metadata\": {\n";
    json += "    \"version\": \"" + GetProperty("version", "2.0") + "\",\n";
    json += "    \"title\": \"" + title + "\",\n";
    json += "    \"description\": \"" + GetProperty("description", "") + "\",\n";
    json += "    \"created\": \"" + wxDateTime::Now().Format() + "\"\n";
    json += "  },\n";

    // Properties
    json += "  \"properties\": {\n";
    bool first = true;
    for (const auto& prop : properties) {
        if (!first) json += ",\n";
        json += "    \"" + prop.first + "\": \"" + prop.second + "\"";
        first = false;
    }
    json += "\n  },\n";

    // Components
    json += "  \"components\": [\n";
    first = true;
    for (const auto& component : components) {
        if (!first) json += ",\n";

        wxPoint pos = component->GetPosition();
        wxSize size = component->GetSize();

        json += "    {\n";
        json += "      \"type\": " + wxString::Format("%d", static_cast<int>(component->GetType())) + ",\n";
        json += "      \"x\": " + wxString::Format("%d", pos.x) + ",\n";
        json += "      \"y\": " + wxString::Format("%d", pos.y) + ",\n";
        json += "      \"width\": " + wxString::Format("%d", size.x) + ",\n";
        json += "      \"height\": " + wxString::Format("%d", size.y) + ",\n";
        json += wxString::Format("      \"selected\": %s,\n", component->IsSelected() ? "true" : "false");

        // Pins
        json += "      \"pins\": [\n";
        const auto& pins = component->GetPins();
        for (size_t i = 0; i < pins.size(); ++i) {
            if (i > 0) json += ",\n";
            json += "        {\n";
            json += "          \"x\": " + wxString::Format("%d", pins[i].position.x) + ",\n";
            json += "          \"y\": " + wxString::Format("%d", pins[i].position.y) + ",\n";
            json += wxString::Format("          \"isInput\": %s,\n", pins[i].isInput ? "true" : "false");
            json += wxString::Format("          \"isConnected\": %s,\n", pins[i].isConnected ? "true" : "false");
            json += "          \"value\": \"" + LogicValueToString(pins[i].value) + "\"\n";
            json += "        }";
        }
        json += "\n      ]\n";
        json += "    }";
        first = false;
    }
    json += "\n  ]\n";
    json += "}";

    return json;
}

bool CircuitDocument::DeserializeFromJSON(const wxString& jsonData) {
    // Simplified JSON parsing - for now, just parse component type and position
    // In a full implementation, you would use a proper JSON parser

    ClearComponents();

    // Basic parsing - look for component entries
    wxArrayString lines = wxSplit(jsonData, '\n');
    bool inComponents = false;
    bool inComponent = false;
    ComponentType currentType = ComponentType::SELECT;
    wxPoint currentPos(0, 0);

    for (const wxString& line : lines) {
        wxString trimmed = line;
        trimmed.Trim().Trim(false);

        if (trimmed.Contains("\"components\"")) {
            inComponents = true;
            continue;
        }

        if (inComponents && trimmed.StartsWith("{")) {
            inComponent = true;
            continue;
        }

        if (inComponent) {
            if (trimmed.Contains("\"type\":")) {
                long typeValue;
                wxString typeStr = trimmed.AfterFirst(':').BeforeFirst(',').Trim().Trim(false);
                if (typeStr.ToLong(&typeValue)) {
                    currentType = static_cast<ComponentType>(typeValue);
                }
            }
            else if (trimmed.Contains("\"x\":")) {
                long x;
                wxString xStr = trimmed.AfterFirst(':').BeforeFirst(',').Trim().Trim(false);
                if (xStr.ToLong(&x)) {
                    currentPos.x = x;
                }
            }
            else if (trimmed.Contains("\"y\":")) {
                long y;
                wxString yStr = trimmed.AfterFirst(':').BeforeFirst(',').Trim().Trim(false);
                if (yStr.ToLong(&y)) {
                    currentPos.y = y;
                }
            }
            else if (trimmed.StartsWith("}")) {
                // End of component - create it
                std::unique_ptr<CircuitComponent> component = CreateComponentFromType(currentType, currentPos);
                if (component) {
                    AddComponent(std::move(component));
                }
                inComponent = false;
            }
        }

        if (trimmed.Contains("]") && inComponents) {
            inComponents = false;
            break;
        }
    }

    modified = false;
    return true;
}

std::unique_ptr<CircuitComponent> CircuitDocument::CreateComponentFromType(ComponentType type, const wxPoint& pos) const {
    switch (type) {
        case ComponentType::AND_GATE:
            return std::make_unique<AndGate>(pos);
        case ComponentType::OR_GATE:
            return std::make_unique<OrGate>(pos);
        case ComponentType::NOT_GATE:
            return std::make_unique<NotGate>(pos);
        case ComponentType::NAND_GATE:
            return std::make_unique<NandGate>(pos);
        case ComponentType::NOR_GATE:
            return std::make_unique<NorGate>(pos);
        case ComponentType::XOR_GATE:
            return std::make_unique<XorGate>(pos);
        case ComponentType::XNOR_GATE:
            return std::make_unique<XnorGate>(pos);
        case ComponentType::INPUT_PIN:
            return std::make_unique<InputSwitch>(pos);
        case ComponentType::OUTPUT_PIN:
            return std::make_unique<OutputLED>(pos);
        case ComponentType::HALF_ADDER:
            return std::make_unique<HalfAdder>(pos);
        case ComponentType::FULL_ADDER:
            return std::make_unique<FullAdder>(pos);
        case ComponentType::ADDER_4BIT:
            return std::make_unique<Adder4Bit>(pos);
        case ComponentType::MULTIPLEXER_2TO1:
            return std::make_unique<Multiplexer2to1>(pos);
        case ComponentType::MULTIPLEXER_4TO1:
            return std::make_unique<Multiplexer4to1>(pos);
        case ComponentType::DEMULTIPLEXER_1TO2:
            return std::make_unique<Demultiplexer1to2>(pos);
        case ComponentType::DEMULTIPLEXER_1TO4:
            return std::make_unique<Demultiplexer1to4>(pos);
        default:
            return nullptr;
    }
}

void CircuitDocument::LoadToCanvas(CircuitCanvas* canvas) {
    if (!canvas) return;

    // Clear canvas components
    canvas->ClearComponents();

    // Add components to canvas
    for (const auto& component : components) {
        canvas->AddComponentCopy(component.get());
    }

    canvas->Refresh();
}

void CircuitDocument::SaveFromCanvas(CircuitCanvas* canvas) {
    if (!canvas) return;

    // Clear current components
    ClearComponents();

    // Get components from canvas
    const auto& canvasComponents = canvas->GetComponents();
    for (const auto& component : canvasComponents) {
        AddComponent(canvas->CloneComponent(component.get()));
    }
}

wxString CircuitDocument::LogicValueToString(LogicValue value) const {
    switch (value) {
        case LogicValue::LOW: return "LOW";
        case LogicValue::HIGH: return "HIGH";
        case LogicValue::UNDEFINED: return "UNDEFINED";
        default: return "UNDEFINED";
    }
}

LogicValue CircuitDocument::LogicValueFromString(const wxString& str) const {
    if (str == "LOW") return LogicValue::LOW;
    if (str == "HIGH") return LogicValue::HIGH;
    return LogicValue::UNDEFINED;
}

// DocumentManager implementation
DocumentManager::DocumentManager() {
    LoadConfig();
}

DocumentManager::~DocumentManager() {
    SaveConfig();
}

void DocumentManager::AddRecentFile(const wxString& filepath) {
    // Remove if already exists
    auto it = std::find(recentFiles.begin(), recentFiles.end(), filepath);
    if (it != recentFiles.end()) {
        recentFiles.erase(it);
    }

    // Add to beginning
    recentFiles.insert(recentFiles.begin(), filepath);

    // Limit size
    if (recentFiles.size() > MAX_RECENT_FILES) {
        recentFiles.resize(MAX_RECENT_FILES);
    }

    SaveConfig();
}

void DocumentManager::ClearRecentFiles() {
    recentFiles.clear();
    SaveConfig();
}

void DocumentManager::LoadConfig() {
    wxConfig config("LogicCircuitSimulator");

    recentFiles.clear();
    for (size_t i = 0; i < MAX_RECENT_FILES; ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        wxString filepath = config.Read(key, "");
        if (!filepath.IsEmpty() && wxFileExists(filepath)) {
            recentFiles.push_back(filepath);
        }
    }
}

void DocumentManager::SaveConfig() {
    wxConfig config("LogicCircuitSimulator");

    // Clear old entries
    for (size_t i = 0; i < MAX_RECENT_FILES; ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        config.DeleteEntry(key);
    }

    // Save current recent files
    for (size_t i = 0; i < recentFiles.size(); ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        config.Write(key, recentFiles[i]);
    }
}

DocumentManager& DocumentManager::GetInstance() {
    static DocumentManager instance;
    return instance;
}