import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import QtWebEngine 1.1


ApplicationWindow {
    id: root
    title: `Browser - ${webView.title}`

    width: 1200
    height: 700
    minimumWidth: 300
    minimumHeight: 100

    menuBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 0

            ToolButton {
                text: qsTr("Back")
                onClicked: {
                    webView.goBack()
                }
            }

            Item {
                Layout.preferredWidth: 5
            }

            TextField {
                id: urlBox
                text: webView.url
                Layout.fillWidth: true
                selectByMouse: true

                onAccepted: {
                    webView.url = text
                }
            }

            Item {
                Layout.preferredWidth: 5
            }

            ToolButton {
                text: qsTr("Go")
                onClicked: {
                    webView.url = urlBox.text
                }
            }
        }
    }

    WebEngineView {
        id: webView
        anchors.fill: parent
        url: "https://www.google.com/"
    }
}
