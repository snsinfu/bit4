import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    id: root

    title: 'Hello!'

    width: 600
    height: 400
    minimumWidth: 320
    minimumHeight: 100

    Text {
        text: 'Hello, Qt and QML!'

        width: root.width
        height: root.height

        font.pixelSize: 32
        font.bold: true

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
