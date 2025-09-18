#include "../../include/ui/component_library_panel.h"
#include "../../include/ui/icon_factory.h"

// Define custom event
const wxEventType ComponentLibraryPanel::wxEVT_COMPONENT_SELECTED = wxNewEventType();

wxBEGIN_EVENT_TABLE(ComponentLibraryPanel, wxPanel)
    EVT_TREE_SEL_CHANGED(wxID_ANY, ComponentLibraryPanel::OnTreeSelChanged)
    EVT_TREE_ITEM_ACTIVATED(wxID_ANY, ComponentLibraryPanel::OnTreeItemActivated)
    EVT_TEXT(wxID_ANY, ComponentLibraryPanel::OnSearchText)
wxEND_EVENT_TABLE()

ComponentLibraryPanel::ComponentLibraryPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), imageList(nullptr) {

    // Set background color
    SetBackgroundColour(wxColour(250, 250, 250));

    // Create sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Title
    titleLabel = new wxStaticText(this, wxID_ANY, "Component Library");
    titleLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(titleLabel, 0, wxALL | wxEXPAND, 5);

    // Search box
    searchBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    searchBox->SetHint("Search components...");
    sizer->Add(searchBox, 0, wxALL | wxEXPAND, 5);

    // Separator (simplified)
    sizer->AddSpacer(5);

    // Create tree control
    CreateTree();
    sizer->Add(treeCtrl, 1, wxALL | wxEXPAND, 5);

    SetSizer(sizer);
}

ComponentLibraryPanel::~ComponentLibraryPanel() {
    if (imageList) {
        delete imageList;
    }
}

void ComponentLibraryPanel::CreateTree() {
    treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                              wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_SINGLE);

    // Create image list for icons
    CreateImageList();

    // Populate the tree
    PopulateTree();

    // Expand all categories by default
    treeCtrl->ExpandAll();
}

void ComponentLibraryPanel::CreateImageList() {
    imageList = new wxImageList(16, 16);

    // Add icons for different component types
    imageList->Add(IconFactory::CreateSelectIcon().ConvertToImage().Scale(16, 16));        // 0 - Folder
    imageList->Add(IconFactory::CreateAndGateIcon().ConvertToImage().Scale(16, 16));       // 1 - AND
    imageList->Add(IconFactory::CreateOrGateIcon().ConvertToImage().Scale(16, 16));        // 2 - OR
    imageList->Add(IconFactory::CreateNotGateIcon().ConvertToImage().Scale(16, 16));       // 3 - NOT
    imageList->Add(IconFactory::CreateNandGateIcon().ConvertToImage().Scale(16, 16));      // 4 - NAND
    imageList->Add(IconFactory::CreateNorGateIcon().ConvertToImage().Scale(16, 16));       // 5 - NOR
    imageList->Add(IconFactory::CreateXorGateIcon().ConvertToImage().Scale(16, 16));       // 6 - XOR
    imageList->Add(IconFactory::CreateXnorGateIcon().ConvertToImage().Scale(16, 16));      // 7 - XNOR
    imageList->Add(IconFactory::CreateHalfAdderIcon().ConvertToImage().Scale(16, 16));     // 8 - Half Adder
    imageList->Add(IconFactory::CreateFullAdderIcon().ConvertToImage().Scale(16, 16));     // 9 - Full Adder
    imageList->Add(IconFactory::CreateAdder4BitIcon().ConvertToImage().Scale(16, 16));     // 10 - 4-bit Adder
    imageList->Add(IconFactory::CreateMultiplexerIcon().ConvertToImage().Scale(16, 16));   // 11 - MUX
    imageList->Add(IconFactory::CreateDemultiplexerIcon().ConvertToImage().Scale(16, 16)); // 12 - DEMUX
    imageList->Add(IconFactory::CreateInputIcon().ConvertToImage().Scale(16, 16));         // 13 - Input
    imageList->Add(IconFactory::CreateOutputIcon().ConvertToImage().Scale(16, 16));        // 14 - Output
    imageList->Add(IconFactory::CreateWireIcon().ConvertToImage().Scale(16, 16));          // 15 - Wire

    treeCtrl->SetImageList(imageList);
}

void ComponentLibraryPanel::PopulateTree() {
    // Root item
    rootId = treeCtrl->AddRoot("Components", 0);

    // Logic Gates category
    logicGatesId = treeCtrl->AppendItem(rootId, "Logic Gates", 0);
    treeCtrl->AppendItem(logicGatesId, "AND Gate", 1);
    treeCtrl->AppendItem(logicGatesId, "OR Gate", 2);
    treeCtrl->AppendItem(logicGatesId, "NOT Gate", 3);
    treeCtrl->AppendItem(logicGatesId, "NAND Gate", 4);
    treeCtrl->AppendItem(logicGatesId, "NOR Gate", 5);
    treeCtrl->AppendItem(logicGatesId, "XOR Gate", 6);
    treeCtrl->AppendItem(logicGatesId, "XNOR Gate", 7);

    // Arithmetic category
    arithmeticId = treeCtrl->AppendItem(rootId, "Arithmetic", 0);
    treeCtrl->AppendItem(arithmeticId, "Half Adder", 8);
    treeCtrl->AppendItem(arithmeticId, "Full Adder", 9);
    treeCtrl->AppendItem(arithmeticId, "4-bit Adder", 10);

    // Data Selection category
    dataSelectionId = treeCtrl->AppendItem(rootId, "Data Selection", 0);
    treeCtrl->AppendItem(dataSelectionId, "2:1 Multiplexer", 11);
    treeCtrl->AppendItem(dataSelectionId, "4:1 Multiplexer", 11);
    treeCtrl->AppendItem(dataSelectionId, "1:2 Demultiplexer", 12);
    treeCtrl->AppendItem(dataSelectionId, "1:4 Demultiplexer", 12);

    // Sequential Logic category
    sequentialId = treeCtrl->AppendItem(rootId, "Sequential Logic", 0);
    treeCtrl->AppendItem(sequentialId, "D Flip-Flop", 1);
    treeCtrl->AppendItem(sequentialId, "JK Flip-Flop", 1);
    treeCtrl->AppendItem(sequentialId, "SR Latch", 1);
    treeCtrl->AppendItem(sequentialId, "T Flip-Flop", 1);
    treeCtrl->AppendItem(sequentialId, "Clock Generator", 1);
    treeCtrl->AppendItem(sequentialId, "4-bit Register", 1);
    treeCtrl->AppendItem(sequentialId, "4-bit Shift Register", 1);
    treeCtrl->AppendItem(sequentialId, "4-bit Counter", 1);
    treeCtrl->AppendItem(sequentialId, "BCD Counter", 1);

    // Encoders/Decoders category
    encodersId = treeCtrl->AppendItem(rootId, "Encoders/Decoders", 0);
    treeCtrl->AppendItem(encodersId, "2:4 Decoder", 1);
    treeCtrl->AppendItem(encodersId, "3:8 Decoder", 1);
    treeCtrl->AppendItem(encodersId, "BCD to 7-Segment", 1);
    treeCtrl->AppendItem(encodersId, "4:2 Encoder", 1);
    treeCtrl->AppendItem(encodersId, "8:3 Encoder", 1);
    treeCtrl->AppendItem(encodersId, "Priority Encoder", 1);

    // Display Components category
    displayId = treeCtrl->AppendItem(rootId, "Display Components", 0);
    treeCtrl->AppendItem(displayId, "7-Segment Display", 14);
    treeCtrl->AppendItem(displayId, "LED Matrix 8x8", 14);
    treeCtrl->AppendItem(displayId, "Hex Display", 14);
    treeCtrl->AppendItem(displayId, "Binary Display", 14);

    // Input/Output category
    inputOutputId = treeCtrl->AppendItem(rootId, "Input/Output", 0);
    treeCtrl->AppendItem(inputOutputId, "Input Switch", 13);
    treeCtrl->AppendItem(inputOutputId, "Output LED", 14);

    // Wiring category
    wiringId = treeCtrl->AppendItem(rootId, "Wiring", 0);
    treeCtrl->AppendItem(wiringId, "Wire", 15);

    // Set data for each item to identify component types
    // Component type data will be set in the loop below

    // Set component type data for each component item
    wxTreeItemIdValue cookie;
    wxTreeItemId categoryId = treeCtrl->GetFirstChild(rootId, cookie);

    while (categoryId.IsOk()) {
        wxTreeItemIdValue childCookie;
        wxTreeItemId componentId = treeCtrl->GetFirstChild(categoryId, childCookie);

        while (componentId.IsOk()) {
            wxString itemText = treeCtrl->GetItemText(componentId);
            ComponentType type = ComponentType::SELECT;

            // Map item text to component type
            if (itemText == "AND Gate") type = ComponentType::AND_GATE;
            else if (itemText == "OR Gate") type = ComponentType::OR_GATE;
            else if (itemText == "NOT Gate") type = ComponentType::NOT_GATE;
            else if (itemText == "NAND Gate") type = ComponentType::NAND_GATE;
            else if (itemText == "NOR Gate") type = ComponentType::NOR_GATE;
            else if (itemText == "XOR Gate") type = ComponentType::XOR_GATE;
            else if (itemText == "XNOR Gate") type = ComponentType::XNOR_GATE;
            else if (itemText == "Half Adder") type = ComponentType::HALF_ADDER;
            else if (itemText == "Full Adder") type = ComponentType::FULL_ADDER;
            else if (itemText == "4-bit Adder") type = ComponentType::ADDER_4BIT;
            else if (itemText == "2:1 Multiplexer") type = ComponentType::MULTIPLEXER_2TO1;
            else if (itemText == "4:1 Multiplexer") type = ComponentType::MULTIPLEXER_4TO1;
            else if (itemText == "1:2 Demultiplexer") type = ComponentType::DEMULTIPLEXER_1TO2;
            else if (itemText == "1:4 Demultiplexer") type = ComponentType::DEMULTIPLEXER_1TO4;
            // Sequential Logic Components
            else if (itemText == "D Flip-Flop") type = ComponentType::D_FLIPFLOP;
            else if (itemText == "JK Flip-Flop") type = ComponentType::JK_FLIPFLOP;
            else if (itemText == "SR Latch") type = ComponentType::SR_LATCH;
            else if (itemText == "T Flip-Flop") type = ComponentType::T_FLIPFLOP;
            else if (itemText == "Clock Generator") type = ComponentType::CLOCK_GENERATOR;
            else if (itemText == "4-bit Register") type = ComponentType::REGISTER_4BIT;
            else if (itemText == "4-bit Shift Register") type = ComponentType::SHIFT_REGISTER_4BIT;
            else if (itemText == "4-bit Counter") type = ComponentType::COUNTER_4BIT;
            else if (itemText == "BCD Counter") type = ComponentType::BCD_COUNTER;
            // Encoder/Decoder Components
            else if (itemText == "2:4 Decoder") type = ComponentType::DECODER_2TO4;
            else if (itemText == "3:8 Decoder") type = ComponentType::DECODER_3TO8;
            else if (itemText == "BCD to 7-Segment") type = ComponentType::BCD_TO_7SEGMENT;
            else if (itemText == "4:2 Encoder") type = ComponentType::ENCODER_4TO2;
            else if (itemText == "8:3 Encoder") type = ComponentType::ENCODER_8TO3;
            else if (itemText == "Priority Encoder") type = ComponentType::PRIORITY_ENCODER;
            // Display Components
            else if (itemText == "7-Segment Display") type = ComponentType::SEVEN_SEGMENT_DISPLAY;
            else if (itemText == "LED Matrix 8x8") type = ComponentType::LED_MATRIX_8X8;
            else if (itemText == "Hex Display") type = ComponentType::HEX_DISPLAY;
            else if (itemText == "Binary Display") type = ComponentType::BINARY_DISPLAY;
            // I/O Components
            else if (itemText == "Input Switch") type = ComponentType::INPUT_PIN;
            else if (itemText == "Output LED") type = ComponentType::OUTPUT_PIN;
            else if (itemText == "Wire") type = ComponentType::WIRE;

            // Store component type in map instead of tree data
            componentTypeMap[componentId.GetID()] = type;

            componentId = treeCtrl->GetNextChild(categoryId, childCookie);
        }

        categoryId = treeCtrl->GetNextChild(rootId, cookie);
    }
}

ComponentType ComponentLibraryPanel::GetSelectedComponentType() const {
    wxTreeItemId selection = treeCtrl->GetSelection();
    if (!selection.IsOk()) {
        return ComponentType::SELECT;
    }

    // Look up component type in map
    auto it = componentTypeMap.find(selection.GetID());
    if (it != componentTypeMap.end()) {
        return it->second;
    }

    return ComponentType::SELECT;
}

ComponentType ComponentLibraryPanel::GetComponentTypeFromTreeItem(const wxTreeItemId& item) const {
    if (!item.IsOk()) {
        return ComponentType::SELECT;
    }

    // Look up component type in map
    auto it = componentTypeMap.find(item.GetID());
    if (it != componentTypeMap.end()) {
        return it->second;
    }

    return ComponentType::SELECT;
}

void ComponentLibraryPanel::OnTreeSelChanged(wxTreeEvent& event) {
    ComponentType type = GetComponentTypeFromTreeItem(event.GetItem());

    // Send custom event to parent
    wxCommandEvent componentEvent(wxEVT_COMPONENT_SELECTED, GetId());
    componentEvent.SetEventObject(this);
    componentEvent.SetInt(static_cast<int>(type));
    ProcessEvent(componentEvent);
}

void ComponentLibraryPanel::OnTreeItemActivated(wxTreeEvent& event) {
    // Double-click behavior - same as selection for now
    OnTreeSelChanged(event);
}

void ComponentLibraryPanel::OnSearchText(wxCommandEvent& event) {
    wxString searchText = searchBox->GetValue().Lower();
    FilterComponents(searchText);
}

void ComponentLibraryPanel::FilterComponents(const wxString& searchText) {
    if (searchText.IsEmpty()) {
        // Show all items (simplified - just expand all)
        treeCtrl->ExpandAll();
        return;
    }

    // Hide items that don't match search
    wxTreeItemIdValue cookie;
    wxTreeItemId categoryId = treeCtrl->GetFirstChild(rootId, cookie);

    while (categoryId.IsOk()) {
        bool categoryHasMatches = false;

        wxTreeItemIdValue childCookie;
        wxTreeItemId componentId = treeCtrl->GetFirstChild(categoryId, childCookie);

        while (componentId.IsOk()) {
            wxString itemText = treeCtrl->GetItemText(componentId).Lower();

            if (itemText.Contains(searchText)) {
                // Show matching item
                categoryHasMatches = true;
            }

            componentId = treeCtrl->GetNextChild(categoryId, childCookie);
        }

        // Show/hide category based on whether it has matches
        if (categoryHasMatches) {
            treeCtrl->Expand(categoryId);
        }

        categoryId = treeCtrl->GetNextChild(rootId, cookie);
    }
}