#include "../../include/ui/logisim_app.h"
#include "../../include/ui/logisim_main_frame.h"

bool LogisimApp::OnInit() {
    LogisimMainFrame* frame = new LogisimMainFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(LogisimApp);