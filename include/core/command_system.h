#pragma once
#include <wx/wx.h>
#include <memory>
#include <vector>
#include <stack>
#include "../components/circuit_component.h"

// Forward declaration
class CircuitCanvas;

// Base command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual bool CanMerge(const Command* other) const { return false; }
    virtual void MergeWith(Command* other) {}
    virtual wxString GetDescription() const = 0;
};

// Command for adding a component
class AddComponentCommand : public Command {
private:
    CircuitCanvas* canvas;
    std::unique_ptr<CircuitComponent> component;
    bool executed;

public:
    AddComponentCommand(CircuitCanvas* canvas, std::unique_ptr<CircuitComponent> comp);
    ~AddComponentCommand() override = default;

    void Execute() override;
    void Undo() override;
    wxString GetDescription() const override;
};

// Command for removing a component
class RemoveComponentCommand : public Command {
private:
    CircuitCanvas* canvas;
    std::unique_ptr<CircuitComponent> component;
    size_t originalIndex;
    bool executed;

public:
    RemoveComponentCommand(CircuitCanvas* canvas, CircuitComponent* comp);
    ~RemoveComponentCommand() override = default;

    void Execute() override;
    void Undo() override;
    wxString GetDescription() const override;
};

// Command for moving a component
class MoveComponentCommand : public Command {
private:
    CircuitComponent* component;
    wxPoint oldPosition;
    wxPoint newPosition;

public:
    MoveComponentCommand(CircuitComponent* comp, const wxPoint& oldPos, const wxPoint& newPos);
    ~MoveComponentCommand() override = default;

    void Execute() override;
    void Undo() override;
    bool CanMerge(const Command* other) const override;
    void MergeWith(Command* other) override;
    wxString GetDescription() const override;
};

// Command for selecting/deselecting components
class SelectionCommand : public Command {
private:
    std::vector<CircuitComponent*> components;
    std::vector<bool> oldStates;
    std::vector<bool> newStates;

public:
    SelectionCommand(const std::vector<CircuitComponent*>& comps,
                    const std::vector<bool>& oldSel,
                    const std::vector<bool>& newSel);
    ~SelectionCommand() override = default;

    void Execute() override;
    void Undo() override;
    wxString GetDescription() const override;
};

// Composite command for multiple operations
class CompositeCommand : public Command {
private:
    std::vector<std::unique_ptr<Command>> commands;
    wxString description;

public:
    CompositeCommand(const wxString& desc);
    ~CompositeCommand() override = default;

    void AddCommand(std::unique_ptr<Command> command);
    void Execute() override;
    void Undo() override;
    wxString GetDescription() const override;
};

// Command manager for undo/redo functionality
class CommandManager {
private:
    std::vector<std::unique_ptr<Command>> undoStack;
    std::vector<std::unique_ptr<Command>> redoStack;
    size_t maxUndoLevels;
    bool isExecuting;

public:
    CommandManager(size_t maxLevels = 50);
    ~CommandManager() = default;

    // Command execution
    void ExecuteCommand(std::unique_ptr<Command> command);
    bool CanUndo() const;
    bool CanRedo() const;
    void Undo();
    void Redo();

    // Stack management
    void Clear();
    size_t GetUndoCount() const { return undoStack.size(); }
    size_t GetRedoCount() const { return redoStack.size(); }

    // Command descriptions for UI
    wxString GetUndoDescription() const;
    wxString GetRedoDescription() const;

    // Transaction support
    void BeginTransaction(const wxString& description);
    void EndTransaction();
    void AbortTransaction();

private:
    void LimitUndoStack();
    std::unique_ptr<CompositeCommand> currentTransaction;
};

// Macro command for recording multiple operations
class MacroCommand {
private:
    std::unique_ptr<CompositeCommand> composite;
    CommandManager* manager;

public:
    MacroCommand(CommandManager* mgr, const wxString& description);
    ~MacroCommand();

    void AddCommand(std::unique_ptr<Command> command);
    void Execute();
    void Cancel();
};