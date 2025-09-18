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
#include <algorithm>
#include <memory>

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
}

void CircuitDocument::SetProperty(const wxString& key, const wxString& value) {
    properties[key] = value;
}

wxString CircuitDocument::GetProperty(const wxString& key, const wxString& defaultValue) const {
    auto it = properties.find(key);
    return (it != properties.end()) ? it->second : defaultValue;
}

wxString CircuitDocument::SerializeToJSON() const {
    // Simplified JSON serialization
    wxString json = "{\n";

    // Metadata
    json += "  \"metadata\": {\n";
    json += "    \"version\": \"" + GetProperty("version", "2.0") + "\",\n";
    json += "    \"title\": \"" + title + "\",\n";
    json += "    \"description\": \"Logic Circuit Design\",\n";
    json += "    \"created\": \"" + wxDateTime::Now().Format() + "\"\n";
    json += "  },\n";

    // Simple components array (empty for now)
    json += "  \"components\": []\n";
    json += "}";

    return json;
}

bool CircuitDocument::DeserializeFromJSON(const wxString& jsonData) {
    // Simplified JSON parsing - for now, just return true
    // In a full implementation, you would use a proper JSON parser

    ClearComponents();

    // For now, just parse basic metadata
    if (jsonData.Contains("\"title\"")) {
        // Very basic title extraction
        size_t titleStart = jsonData.Find("\"title\": \"");
        if (titleStart != wxNOT_FOUND) {
            titleStart += 10; // Length of "\"title\": \""
            size_t titleEnd = jsonData.find('\"', titleStart);
            if (titleEnd != wxNOT_FOUND) {
                title = jsonData.Mid(titleStart, titleEnd - titleStart);
            }
        }
    }

    return true;
}

void CircuitDocument::LoadToCanvas(CircuitCanvas* canvas) {
    if (!canvas) return;

    // Clear canvas components
    canvas->ClearComponents();

    // For now, just refresh canvas
    // In a full implementation, we would recreate components from saved data
    canvas->Refresh();
}

void CircuitDocument::SaveFromCanvas(CircuitCanvas* canvas) {
    if (!canvas) return;

    // For now, just mark document as modified
    // The actual serialization will happen in SerializeToJSON based on canvas state
    SetModified(true);
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

    // Add to front
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
    wxConfig* config = new wxConfig("LogicSimulator");

    wxString recentFile;
    for (size_t i = 0; i < MAX_RECENT_FILES; ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        if (config->Read(key, &recentFile)) {
            if (wxFileExists(recentFile)) {
                recentFiles.push_back(recentFile);
            }
        }
    }

    delete config;
}

void DocumentManager::SaveConfig() {
    wxConfig* config = new wxConfig("LogicSimulator");

    // Clear old entries
    for (size_t i = 0; i < MAX_RECENT_FILES; ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        config->DeleteEntry(key);
    }

    // Save current entries
    for (size_t i = 0; i < recentFiles.size(); ++i) {
        wxString key = wxString::Format("RecentFile%zu", i);
        config->Write(key, recentFiles[i]);
    }

    delete config;
}

DocumentManager& DocumentManager::GetInstance() {
    static DocumentManager instance;
    return instance;
}