// Copyright (c) 2014-2015, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import QtQuick 2.2
import QtQml 2.2
import QtQuick.Layouts 1.1
import "../components"

ColumnLayout {
    id: page
    signal createWalletClicked()
    signal recoveryWalletClicked()
    signal openWalletClicked()
    opacity: 0
    visible: false
    property int buttonSize: (isMobile) ? 80 : 190
    property int buttonImageSize: (isMobile) ? buttonSize - 10 : buttonSize - 30

    function onPageClosed() {
        // Save settings used in open from file.
        // other wizard settings are saved on last page in applySettings()
        appWindow.persistentSettings.language = wizard.settings.language
        appWindow.persistentSettings.locale   = wizard.settings.locale

        return true;
    }

    Behavior on opacity {
        NumberAnimation { duration: 100; easing.type: Easing.InQuad }
    }

    onOpacityChanged: visible = opacity !== 0

    ColumnLayout {
        id: headerColumn
        Layout.leftMargin: wizardLeftMargin
        Layout.rightMargin: wizardRightMargin
        Layout.bottomMargin: (!isMobile) ? 100 : 20
        spacing: 10

        Text {
            Layout.fillWidth: true
            font.family: "微软雅黑"
            font.pixelSize: 28
            Layout.bottomMargin: 25
            //renderType: Text.NativeRendering
            color: "#ffffff"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Welcome to CSSP!") + translationManager.emptyString
        }

        Text {
            Layout.fillWidth: true
            font.family: "微软雅黑"
            font.pixelSize: 18
            //renderType: Text.NativeRendering
            color: "#ffffff"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Please select one of the following options:") + translationManager.emptyString
        }
    }

    GridLayout {
        Layout.leftMargin: wizardLeftMargin
        Layout.rightMargin: wizardRightMargin
        Layout.alignment: Qt.AlignCenter
        id: actionButtons
        columnSpacing: 40
        rowSpacing: 10
        Layout.fillWidth: true
        Layout.fillHeight: true
        flow: isMobile ? GridLayout.TopToBottom : GridLayout.LeftToRight

        GridLayout {
            // Layout.fillHeight: true
            // Layout.fillWidth: true
            flow: !isMobile ? GridLayout.TopToBottom : GridLayout.LeftToRight
            rowSpacing: 20
            columnSpacing: 10

            ColumnLayout {
                Layout.preferredHeight: page.buttonSize
                Layout.preferredWidth: page.buttonSize
               // radius: page.buttonSize
               // color: "transparent"
                spacing: 30

                Image {
                    width: page.buttonImageSize
                    height: page.buttonImageSize
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignRight
                    verticalAlignment: Image.AlignTop
                    // anchors.centerIn: parent
                    source: createWalletArea.containsMouse ? "qrc:///images/createWalletpr.png" : "qrc:///images/createWallet.png"
                }

                Text {
                    Layout.preferredWidth: 190
                    font.family: "微软雅黑"
                    font.pixelSize: 16
                    color: createWalletArea.containsMouse ? "#4ed9d9" : "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text: qsTr("Create a new wallet") + translationManager.emptyString
                }
                MouseArea {
                    id: createWalletArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        page.createWalletClicked()
                    }
                }
            }

        }

        GridLayout {
            // Layout.fillWidth: true
            // Layout.fillHeight: true
            flow: !isMobile ? GridLayout.TopToBottom : GridLayout.LeftToRight
            rowSpacing: 20
            columnSpacing: 10

            ColumnLayout {
                Layout.preferredHeight: page.buttonSize
                Layout.preferredWidth:  page.buttonSize
                // radius: page.buttonSize
                // color: recoverWalletArea.containsMouse ? "#4ed9d9" : "#FFFFFF"
                // color: "transparent"
                spacing: 30

                Image {
                    width: page.buttonImageSize
                    height: page.buttonImageSize
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignRight
                    verticalAlignment: Image.AlignTop
                    // anchors.centerIn: parent
                    source: recoverWalletArea.containsMouse ? "qrc:///images/recoverWalletpr.png" : "qrc:///images/recoverWallet.png"
                }
                Text {
                    Layout.preferredWidth: 190
                    font.family: "微软雅黑"
                    font.pixelSize: 16
                    color: recoverWalletArea.containsMouse ? "#4ed9d9" : "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Restore wallet from keys or mnemonic seed") + translationManager.emptyString
                    width:page.buttonSize
                    wrapMode: Text.WordWrap
                }

                MouseArea {
                    id: recoverWalletArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        page.recoveryWalletClicked()
                    }
                }
            }
        }

        GridLayout {
            // Layout.fillHeight: true
            // Layout.fillWidth: true
            flow: !isMobile ? GridLayout.TopToBottom : GridLayout.LeftToRight
            rowSpacing: 20
            columnSpacing: 10

            ColumnLayout {
                Layout.preferredHeight: page.buttonSize
                Layout.preferredWidth:  page.buttonSize
                // radius: page.buttonSize
                // color: openWalletArea.containsMouse ? "#4ed9d9" : "#FFFFFF"
                // color: "transparent"
                spacing: 30

                Image {

                    width: page.buttonImageSize
                    height: page.buttonImageSize
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignRight
                    verticalAlignment: Image.AlignTop
                    // anchors.centerIn: parent
                    source: openWalletArea.containsMouse ? "qrc:///images/openAccountpr.png" : "qrc:///images/openAccount.png"
                }

                Text {
                    Layout.preferredWidth: 190
                    font.family: "微软雅黑"
                    font.pixelSize: 16
                    color: openWalletArea.containsMouse ? "#4ed9d9" : "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Open a wallet from file") + translationManager.emptyString
                    width:page.buttonSize
                    wrapMode: Text.WordWrap
                }

                MouseArea {
                    id: openWalletArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        page.openWalletClicked()
                    }
                }
            }
        }
    }

    /*
    RowLayout {
        Layout.leftMargin: wizardLeftMargin
        Layout.rightMargin: wizardRightMargin
        Layout.topMargin: 30
        Layout.alignment: Qt.AlignCenter
        Layout.fillWidth: true

        Rectangle {
            width: 100
            CheckBox {
                id: testNet
                text: qsTr("Testnet") + translationManager.emptyString
                background: "#FFFFFF"
                fontColor: "#ffffff"
                fontSize: 16
                checkedIcon: "../images/checkedVioletIcon.png"
                uncheckedIcon: "../images/uncheckedIcon.png"
                checked: appWindow.persistentSettings.testnet;
                onClicked: {
                    persistentSettings.testnet = testNet.checked
                    console.log("testnet set to ", persistentSettings.testnet)
                }
            }
        }
    }
    */
}

