import QtQuick 2.0
import moneroComponents.Clipboard 1.0

Column {

    property alias memoText : memoTextInput.text
    property alias tipText: wordsTipText.text
    property alias tipTextVisible: tipRect.visible
    property alias memoTextReadOnly : memoTextInput.readOnly
    property alias clipboardButtonVisible: clipboardButton.visible


    Rectangle {
        id: memoTextRect
        width: parent.width
        height:  {
            memoTextInput.height
                    // to have less gap between button and text input we reduce overall height by button height
                    //+ (clipboardButton.visible ? clipboardButton.height : 0)
                    + (tipRect.visible ? tipRect.height : 0)
        }
        border.width: 1
        border.color: "#DBDBDB"
        color:"#25313c" // 0d0d0d

        TextEdit {
            id: memoTextInput
            property alias placeholderText: memoTextPlaceholder.text
            textMargin: 8
            text: ""
            font.family: "Arial"
            font.pixelSize: 16
            color:"#ffffff"
            anchors.bottom:parent.bottom - 10
            wrapMode: TextInput.Wrap
            width: parent.width
            selectByMouse: true
            property int minimumHeight: 100
            height: contentHeight > minimumHeight ? contentHeight : minimumHeight

            Text {
                id: memoTextPlaceholder
                anchors.fill:parent
                font.pixelSize: 16
                anchors.margins: 8
                font.bold:true
                text: qsTr("Enter your 25 word mnemonic seed") + translationManager.emptyString
                color: "#25313c" // 0d0d0d
                visible: !memoTextInput.text/* && !parent.focus*/
            }
        }
        Image {
            id : clipboardButton
            anchors.right: parent.right
            anchors.bottom: tipRect.top
            // source: "qrc:///images/greyTriangle.png"
            source: memoTextArea.containsMouse ? "qrc:///images/greyTrianglepr.png" : "qrc:///images/greyTriangle.png"

            Image {
                anchors.centerIn: parent
                source: "qrc:///images/greyTriangle.png"
            }
            Clipboard { id: clipboard }
            MouseArea {
                 id: memoTextArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: clipboard.setText(memoTextInput.text)
            }
        }
        Rectangle {
            id: tipRect
            visible: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: memoTextRect.bottom
            height: wordsTipText.contentHeight + wordsTipText.anchors.topMargin
            color: "transparent" // 0d0d0d #25313c
            property alias text: wordsTipText.text
            border.width: 1
            border.color: "#DBDBDB"
            //color:"#25313c" // 0d0d0d

            Text {
                id: wordsTipText
                anchors.fill: parent
                anchors.topMargin : 26
                anchors.bottomMargin: 16
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Arial"
                font.pixelSize: 15
                color: "#ffffff"
                wrapMode: Text.Wrap
                //text: qsTr("This seed is <b><font color=\"#4ed9d9\">very</font></b> important to write down and keep secret. It is all you need to backup and restore your wallet.")
                text: qsTr("This seed is <b>very</b> important to write down and keep secret. It is all you need to backup and restore your wallet.")
                      + translationManager.emptyString
            }
        }
    }

    Image {
        id: image
        width: 64
        height: 100
        source: "qrc:/qtquickplugin/images/template_image.png"
    }
}
