#pragma once
#include <wx/wx.h>
#include <memory>
#include <vector>
#include <map>
#include "../components/circuit_component.h"

// Forward declarations
class CircuitCanvas;
class Wire;

// Circuit document class for managing circuit data and file operations
class CircuitDocument {
private:
    wxString filename;
    wxString title;
    bool modified;
    std::vector<std::unique_ptr<CircuitComponent>> components;
    std::map<wxString, wxString> properties;

public:
    CircuitDocument();
    ~CircuitDocument();

    // File operations
    bool LoadFromFile(const wxString& filepath);
    bool SaveToFile(const wxString& filepath);
    bool SaveAs(const wxString& filepath);

    // Document properties
    const wxString& GetFilename() const { return filename; }
    const wxString& GetTitle() const { return title; }
    bool IsModified() const { return modified; }
    void SetModified(bool mod = true) { modified = mod; }

    // Component management
    void AddComponent(std::unique_ptr<CircuitComponent> component);
    void RemoveComponent(CircuitComponent* component);
    void ClearComponents();
    const std::vector<std::unique_ptr<CircuitComponent>>& GetComponents() const { return components; }

    // Document operations
    void NewDocument();
    void SetTitle(const wxString& newTitle) { title = newTitle; }

    // Properties
    void SetProperty(const wxString& key, const wxString& value);
    wxString GetProperty(const wxString& key, const wxString& defaultValue = "") const;

    // Serialization
    wxString SerializeToJSON() const;
    bool DeserializeFromJSON(const wxString& jsonData);

    // Canvas integration
    void LoadToCanvas(CircuitCanvas* canvas);
    void SaveFromCanvas(CircuitCanvas* canvas);

private:
    // Helper functions for JSON serialization
    std::unique_ptr<CircuitComponent> CreateComponentFromType(ComponentType type, const wxPoint& pos) const;
    wxString LogicValueToString(LogicValue value) const;
    LogicValue LogicValueFromString(const wxString& str) const;
};

// Document manager for handling multiple documents and recent files
class DocumentManager {
private:
    std::vector<wxString> recentFiles;
    static const size_t MAX_RECENT_FILES = 10;

public:
    DocumentManager();
    ~DocumentManager();

    // Recent files management
    void AddRecentFile(const wxString& filepath);
    const std::vector<wxString>& GetRecentFiles() const { return recentFiles; }
    void ClearRecentFiles();

    // Configuration persistence
    void LoadConfig();
    void SaveConfig();

    // Static instance
    static DocumentManager& GetInstance();
};