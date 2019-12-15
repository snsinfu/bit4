#include <iostream>
#include <wx/wx.h>

#include "MainWindow.h"


MainWindow::MainWindow(wxString const& title)
{
    Create(
        nullptr,
        wxID_ANY,
        title,
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)
    );
    SetClientSize(600, 400);

    // The documentation [1] says that wxPanel allows TAB traversal among child
    // controls. However, TAB does not work on macOS. Also a panel is colored
    // in white on macOS in both light and dark mode.
    //
    // [1] https://docs.wxwidgets.org/3.0/classwx_panel.html

    auto panel = new wxPanel(this, wxID_ANY);
    auto text = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 10), wxSize(350, 25));
    auto button = new wxButton(panel, wxID_ANY, "Push", wxPoint(10, 40), wxSize(150, 25));

    (void) text;
    (void) button;
}
