#include <wx/wx.h>

#include "App.h"
#include "MainWindow.h"


wxIMPLEMENT_APP(App);


bool App::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }

    auto mainWindow = new MainWindow("Hello, world!");
    mainWindow->Show(true);

    return true;
}
