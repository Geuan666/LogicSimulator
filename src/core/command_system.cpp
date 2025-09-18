#include "../../include/core/command_system.h"
#include "../../include/ui/circuit_canvas.h"

// AddComponentCommand implementation
AddComponentCommand::AddComponentCommand(CircuitCanvas* canvas, std::unique_ptr<CircuitComponent> comp)
    : canvas(canvas), component(std::move(comp)), executed(false) {}

void AddComponentCommand::Execute() {
    if (!executed && canvas && component) {
        canvas->AddComponentDirectly(std::move(component));
        executed = true;
        canvas->Refresh();
    }
}

void AddComponentCommand::Undo() {
    if (executed && canvas && component) {
        canvas->RemoveComponentDirectly(component.get());
        executed = false;
        canvas->Refresh();
    }
}

wxString AddComponentCommand::GetDescription() const {
    return "Add Component";
}

// RemoveComponentCommand implementation
RemoveComponentCommand::RemoveComponentCommand(CircuitCanvas* canvas, CircuitComponent* comp)
    : canvas(canvas), executed(false), originalIndex(0) {

    if (canvas && comp) {
        const auto& components = canvas->GetComponents();
        for (size_t i = 0; i < components.size(); ++i) {
            if (components[i].get() == comp) {
                originalIndex = i;
                component = canvas->ExtractComponent(comp);
                break;
            }
        }
    }
}

void RemoveComponentCommand::Execute() {
    if (!executed && canvas && component) {
        canvas->RemoveComponentDirectly(component.get());
        executed = true;
        canvas->Refresh();
    }
}

void RemoveComponentCommand::Undo() {
    if (executed && canvas && component) {
        canvas->InsertComponentAt(originalIndex, std::move(component));
        executed = false;
        canvas->Refresh();
    }
}

wxString RemoveComponentCommand::GetDescription() const {
    return "Remove Component";
}

// MoveComponentCommand implementation
MoveComponentCommand::MoveComponentCommand(CircuitComponent* comp, const wxPoint& oldPos, const wxPoint& newPos)
    : component(comp), oldPosition(oldPos), newPosition(newPos) {}

void MoveComponentCommand::Execute() {
    if (component) {
        wxPoint offset = newPosition - component->GetPosition();
        component->Move(offset);
    }
}

void MoveComponentCommand::Undo() {
    if (component) {
        wxPoint offset = oldPosition - component->GetPosition();
        component->Move(offset);
    }
}

bool MoveComponentCommand::CanMerge(const Command* other) const {
    const MoveComponentCommand* moveCmd = dynamic_cast<const MoveComponentCommand*>(other);
    return moveCmd && moveCmd->component == component;
}

void MoveComponentCommand::MergeWith(Command* other) {
    MoveComponentCommand* moveCmd = dynamic_cast<MoveComponentCommand*>(other);
    if (moveCmd && moveCmd->component == component) {
        newPosition = moveCmd->newPosition;
    }
}

wxString MoveComponentCommand::GetDescription() const {
    return "Move Component";
}

// SelectionCommand implementation
SelectionCommand::SelectionCommand(const std::vector<CircuitComponent*>& comps,
                                 const std::vector<bool>& oldSel,
                                 const std::vector<bool>& newSel)
    : components(comps), oldStates(oldSel), newStates(newSel) {}

void SelectionCommand::Execute() {
    for (size_t i = 0; i < components.size() && i < newStates.size(); ++i) {
        components[i]->Select(newStates[i]);
    }
}

void SelectionCommand::Undo() {
    for (size_t i = 0; i < components.size() && i < oldStates.size(); ++i) {
        components[i]->Select(oldStates[i]);
    }
}

wxString SelectionCommand::GetDescription() const {
    return "Change Selection";
}

// CompositeCommand implementation
CompositeCommand::CompositeCommand(const wxString& desc) : description(desc) {}

void CompositeCommand::AddCommand(std::unique_ptr<Command> command) {
    commands.push_back(std::move(command));
}

void CompositeCommand::Execute() {
    for (auto& cmd : commands) {
        cmd->Execute();
    }
}

void CompositeCommand::Undo() {
    // Undo in reverse order
    for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
        (*it)->Undo();
    }
}

wxString CompositeCommand::GetDescription() const {
    return description;
}

// CommandManager implementation
CommandManager::CommandManager(size_t maxLevels)
    : maxUndoLevels(maxLevels), isExecuting(false) {}

void CommandManager::ExecuteCommand(std::unique_ptr<Command> command) {
    if (!command || isExecuting) return;

    isExecuting = true;

    // If we're in a transaction, add to the composite command
    if (currentTransaction) {
        currentTransaction->AddCommand(std::move(command));
        isExecuting = false;
        return;
    }

    // Try to merge with the last command if possible
    if (!undoStack.empty() && undoStack.back()->CanMerge(command.get())) {
        undoStack.back()->MergeWith(command.get());
        isExecuting = false;
        return;
    }

    // Execute the command
    command->Execute();

    // Add to undo stack
    undoStack.push_back(std::move(command));

    // Clear redo stack
    redoStack.clear();

    // Limit undo stack size
    LimitUndoStack();

    isExecuting = false;
}

bool CommandManager::CanUndo() const {
    return !undoStack.empty() && !isExecuting;
}

bool CommandManager::CanRedo() const {
    return !redoStack.empty() && !isExecuting;
}

void CommandManager::Undo() {
    if (!CanUndo()) return;

    isExecuting = true;

    auto command = std::move(undoStack.back());
    undoStack.pop_back();

    command->Undo();
    redoStack.push_back(std::move(command));

    isExecuting = false;
}

void CommandManager::Redo() {
    if (!CanRedo()) return;

    isExecuting = true;

    auto command = std::move(redoStack.back());
    redoStack.pop_back();

    command->Execute();
    undoStack.push_back(std::move(command));

    isExecuting = false;
}

void CommandManager::Clear() {
    undoStack.clear();
    redoStack.clear();
    currentTransaction.reset();
}

wxString CommandManager::GetUndoDescription() const {
    if (undoStack.empty()) return "";
    return "Undo " + undoStack.back()->GetDescription();
}

wxString CommandManager::GetRedoDescription() const {
    if (redoStack.empty()) return "";
    return "Redo " + redoStack.back()->GetDescription();
}

void CommandManager::BeginTransaction(const wxString& description) {
    if (currentTransaction) {
        // Nested transactions not supported - end current one
        EndTransaction();
    }
    currentTransaction = std::make_unique<CompositeCommand>(description);
}

void CommandManager::EndTransaction() {
    if (currentTransaction) {
        if (currentTransaction->GetDescription() != "") {
            ExecuteCommand(std::move(currentTransaction));
        }
        currentTransaction.reset();
    }
}

void CommandManager::AbortTransaction() {
    currentTransaction.reset();
}

void CommandManager::LimitUndoStack() {
    while (undoStack.size() > maxUndoLevels) {
        undoStack.erase(undoStack.begin());
    }
}

// MacroCommand implementation
MacroCommand::MacroCommand(CommandManager* mgr, const wxString& description)
    : manager(mgr), composite(std::make_unique<CompositeCommand>(description)) {}

MacroCommand::~MacroCommand() {
    if (composite) {
        // Auto-execute if not explicitly executed or cancelled
        Execute();
    }
}

void MacroCommand::AddCommand(std::unique_ptr<Command> command) {
    if (composite) {
        composite->AddCommand(std::move(command));
    }
}

void MacroCommand::Execute() {
    if (composite && manager) {
        manager->ExecuteCommand(std::move(composite));
    }
}

void MacroCommand::Cancel() {
    composite.reset();
}