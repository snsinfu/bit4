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

    auto text = new wxTextCtrl(
        this,
        wxID_ANY,
        wxEmptyString,
        wxPoint(10, 12),
        wxSize(580, 26),
        wxTE_PROCESS_ENTER
    );

    auto list = new wxListBox(
        this,
        wxID_ANY,
        wxPoint(10, 50),
        wxSize(580, 340)
    );

    Bind(wxEVT_TEXT_ENTER, [=](wxCommandEvent&) {
        if (auto const message = text->GetValue(); !message.empty()) {
            text->Clear();
            list->InsertItems(1, &message, 0);
        }
    });
}
