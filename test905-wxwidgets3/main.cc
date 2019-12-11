#include <wx/wx.h>


class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    auto frame = new wxFrame(nullptr, wxID_ANY, "Hello, world!");
    frame->SetSize(600, 400);
    frame->CreateStatusBar();
    frame->SetStatusText("Lorem ipsum dolor sit amet");
    frame->Show(true);

    return true;
}
