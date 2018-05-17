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

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import QtQuick.Controls 1.2

import "../components"
import moneroComponents.Wallet 1.0
import moneroComponents.RpcManager 1.0

Rectangle {
    id: root
    color: "#25313c"

    property var currentHashRate: 0
    property var currentPool:0
    property var rpcStartSuccess: false
    property var rpcJsonConfig : "";

    /* main layout */
    ColumnLayout {
        id: mainLayout
        anchors.margins: 40
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 20

        QtObject{
                    id:attr
                    property int counter;
                    Component.onCompleted: {
                        counter=100
                    }
                }


        // solo
        ColumnLayout {

            id: soloBox
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            spacing: 20

            Label {
                id: soloTitleLabel
                fontSize: 18
                color:"#ffffff"
                text: qsTr("Solo mining") + translationManager.emptyString
            }


            RowLayout {
                id: soloMinersel
                //anchors.margins: 25
                //anchors.left: parent.left
                //anchors.top: parent.top
                //anchors.right: parent.right
                //anchors.bottom: parent.bottom
                spacing: 3
                Label {
                    id: labelminingtype
                    color: "#ffffff"
                    text: qsTr("mingtype") + translationManager.emptyString
                    fontSize: 14
                    Layout.preferredWidth: 120
                }
                ComboBox {
                    id: choiceminingtype

                    // currentIndex: 1
                    model: ListModel {
                        id: cbItems
                          ListElement { text: ""; index: "" }
                         // ListElement { text: "Apple"; index: "Green" }
                         // ListElement { text: "Coconut"; index: "Brown" }
                    }
                    Layout.preferredWidth:  250
                    onCurrentIndexChanged:{
                    currentPool = currentIndex
                   
                    // currentInfo.setSelectMinInfo(currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)

                    if (currentIndex == 0) {
                        currentInfo.setCurrentNodeInfo(cbItems.get(choiceminingtype.currentIndex).index,appWindow.currentWallet.address,soloMinerThreadsLine.text);
                    } else  if (currentIndex > 0) {
                        var pool_address = cbItems.get(currentIndex).index.split(":")[0];
                        var pool_port = cbItems.get(currentIndex).index.split(":")[1];
                        currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                    }

                    console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).index)
                    //walletManager.stopMining()
                    //update()
                   }
                }
            }

            Label {
                id: soloLocalDaemonsLabel
                fontSize: 18
                color: "#D02020"
                text: qsTr("(only available for local daemons)")
                visible: false//!isDaemonLocal()
            }

            Text {
                id: soloMainLabel
                color:"#ffffff"
                text: qsTr("Mining with your computer helps strengthen the RRNC network. The more that people mine, the harder it is for the network to be attacked, and every little bit helps.Mining also gives you a small chance to earn some RRCoin. Your computer will create hashes looking for block solutions. If you find a block, you will get the associated reward. Good luck!") + translationManager.emptyString
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }

            RowLayout {
                id: soloMinerThreadsRow
                Label {
                    id: soloMinerThreadsLabel
                    color: "#ffffff"
                    text: qsTr("CPU threads") + translationManager.emptyString
                    fontSize: 14
                    Layout.preferredWidth: 120
                }
                LineEdit {
                    id: soloMinerThreadsLine
                    Layout.preferredWidth:  200
                    text: "1"
                    placeholderText: '<font color="#747b82" fontFamily="微软雅黑/Microsoft YaHei">' + qsTr("(optional)") + translationManager.emptyString + '</font>'
                    validator: IntValidator { bottom: 1 }
                    onTextUpdated: {
                        if(!is32){
                            if (choiceminingtype.currentIndex == 0) {
                                currentInfo.setCurrentNodeInfo(cbItems.get(choiceminingtype.currentIndex).index,appWindow.currentWallet.address,soloMinerThreadsLine.text);
                            } else if (choiceminingtype.currentIndex > 0) {
                            var pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                            var pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];
                            currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                            currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)

                            }
                        } else {
                            var pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                            var pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];
                            currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                            currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                        }

                    }
                }
            }

            RowLayout {
                Layout.leftMargin: 125
                CheckBox {
                    background:{
                        color:"#25313c"
                    }
                    id: backgroundMining
                    enabled: startSoloMinerButton.enabled
                    checked: {
                        //console.log("edit the updat1e####################################")
                        persistentSettings.allow_background_mining
                    }
                    onClicked: {
                        //console.log("edit the updat2e####################################")
                        persistentSettings.allow_background_mining = checked
                        currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                    }
                    text: qsTr("Background mining (experimental)") + translationManager.emptyString
                    checkedIcon: "../images/checkedVioletIcon.png"
                    uncheckedIcon: "../images/uncheckedIcon.png"
                }
            }

            RowLayout {
                // Disable this option until stable
                visible: false
                Layout.leftMargin: 125
                CheckBox {
                    background:{
                        color:"#25313c"
                    }
                    id: ignoreBattery
                    enabled: startSoloMinerButton.enabled
                    checked: !persistentSettings.miningIgnoreBattery
                    onClicked: {persistentSettings.miningIgnoreBattery = !checked}
                    text: qsTr("Enable mining when running on battery") + translationManager.emptyString
                    checkedIcon: "../images/checkedVioletIcon.png"
                    uncheckedIcon: "../images/uncheckedIcon.png"
                }
            }

            RowLayout {
                Label {
                    id: manageSoloMinerLabel
                    color: "#ffffff"
                    text: qsTr("Manage miner") + translationManager.emptyString
                    fontSize: 14
                }

                StandardButton {
                    visible: true
                    //enabled: !walletManager.isMining()
                    id: startSoloMinerButton
                    width: 110
                    text: qsTr("Start mining") + translationManager.emptyString
                    shadowReleasedColor: "#3b848c"
                    shadowPressedColor: "#ff0000"
                    releasedColor: "#4ed9d9"
                    pressedColor: "#0000ff"
                    onClicked: {

                    console.debug("begin the starting mining")
                    mentionPopup.okText = qsTr("ok") + translationManager.emptyString
                    mentionPopup.title = qsTr("mention starting mining") + translationManager.emptyString
                    mentionPopup.text = qsTr("When the system performs mining operations, some anti-virus software will prompt the risk.") + translationManager.emptyString
                    mentionPopup.text += qsTr("If you download the installation package for the official channel, you can add to the trust list. Please feel free to use it.") + translationManager.emptyString
                    mentionPopup.icon = StandardIcon.Critical
                    mentionPopup.open()

                    console.debug("after the starting mining")

                    /*
                        console.debug(cbItems.get(choiceminingtype.currentIndex).text + ", " + cbItems.get(choiceminingtype.currentIndex).index)
                        var success = false;
                        //set xmrig rpc port 7777
                        var rpc_xmrig_port = 7777;
                        var pool_address = ""; 
                        var pool_port = "";
                        var json_config = ""; 

                        if(!is32){

                         if (choiceminingtype.currentIndex > 0) {

                           pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                           pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];

                           json_config ="{\"algorithm\":\"cryptonight\","
                           + "\"pool\":" + "\"" + pool_address + "\"," + "\"port\":" + pool_port + ","
                           + "\"user\":" + "\""+ appWindow.currentWallet.address +"\"," + "\"password\":\"x\"}";

                           currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                            console.debug( "###########setcurrentInurrentIndex2" +choiceminingtype.currentIndex)
                           currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                           console.debug(json_config);

                           if (!rpcManager.isMining()) {

                                //todo: startxmrig process
                                if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                     //start json rpc send /start request to mining
                                     success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                     if (success == false) {
                                        //connect fail to restart xmrig
                                        console.debug("start json rpc mining failed\n");
                                        rpcManager.stopXmrig();
                                        if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                            success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                         }

                                     }

                                } else {
                                        console.debug("startXmrig failed\n");
                                }

                           } else {
                                  console.debug("rpcManager.isrun ==true")
                                  success = true;
                           }

                          } else {
                               currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                               currentInfo.setCurrentNodeInfo(cbItems.get(choiceminingtype.currentIndex).index,appWindow.currentWallet.address,soloMinerThreadsLine.text);
                               success = walletManager.startMining(appWindow.currentWallet.address, soloMinerThreadsLine.text, persistentSettings.allow_background_mining, persistentSettings.miningIgnoreBattery)
                          }
                        }
                        else { //is 32 bit 
                           
                            pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                            pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];
                            json_config ="{\"algorithm\":\"cryptonight\","
                           + "\"pool\":" + "\"" + pool_address + "\"," + "\"port\":" + pool_port + ","
                           + "\"user\":" + "\""+ appWindow.currentWallet.address +"\"," + "\"password\":\"x\"}";

                           currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                           console.debug( "###########setcurrentInurrentIndex2" +choiceminingtype.currentIndex)
                           currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                           console.debug(json_config);

                           if (!rpcManager.isMining()) {
                               
                                //todo: startxmrig process
                                if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                     //start json rpc send /start request to mining
                                     success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                     if (success == false) {
                                        //connect fail to restart xmrig
                                        console.debug("start json rpc mining failed\n");
                                        rpcManager.stopXmrig();
                                        if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                            success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                         }

                                     }

                                } else {
                                        console.debug("startXmrig failed\n");
                                        success = false;
                                }

                           } else {
                                  console.debug("rpcManager.isrun ==true")
                                  success = true;
                           }

                        }

                 

                        if (success) {

                            updateStatusText()
                            if(!is32){
                                if (choiceminingtype.currentIndex > 0) {
                                   startSoloMinerButton.enabled = !rpcManager.isMining()
                                } else  {
                                    startSoloMinerButton.enabled = !walletManager.isMining()
                                }
                            } else {
                                startSoloMinerButton.enabled = !rpcManager.isMining()
                            }

                            stopSoloMinerButton.enabled = !startSoloMinerButton.enabled
                        } else {
                            errorPopup.title  = qsTr("Error starting mining") + translationManager.emptyString;
                            errorPopup.text = qsTr("Couldn't start mining.<br>")
                            if (!isDaemonLocal())
                                errorPopup.text += qsTr("Mining is only available on local daemons. Run a local daemon to be able to mine.<br>")
                            errorPopup.icon = StandardIcon.Critical
                            errorPopup.open()
                        }*/
                    }
                }

                StandardButton {
                    visible: true
                    //enabled:  walletManager.isMining()
                    id: stopSoloMinerButton
                    width: 110
                    text: qsTr("Stop mining") + translationManager.emptyString
                    shadowReleasedColor: "#3b848c"
                    shadowPressedColor: "#ff0000"
                    releasedColor: "#4ed9d9"
                    pressedColor: "#0000ff"
                    onClicked: {
                    if(!is32){
                        if (choiceminingtype.currentIndex > 0) {
                             rpcManager.stopMining()
                        } else {
                             walletManager.stopMining()
                        }
                    } else {
                        rpcManager.stopMining()
                    }
                        
                    update()
                    }
                }
            }
        }

        Text {
            id: statusText
            color:"#ffffff"
            text: qsTr("Status: not mining")
            textFormat: Text.RichText
            wrapMode: Text.Wrap
        }
    }

    // update the pool info
    function updteMinPoolInfo(){
        cbItems.clear()

        // 32bit cannot local-mining
        if(!is32){
            cbItems.append({"text": qsTr("localmin"), "index":"127.0.0.1:22338"})
            choiceminingtype.currentIndex = 0;
        }

        var data = JSON.parse(dohttp.get_pools_info())
        if(data.code !== 0){ // okdata
            return
        }else{
            for (var i= 0;i<data.data.length;i++){
                cbItems.append({"text":  qsTr("remotepool")+i.toString(), "index":data.data[i].ip+":"+data.data[i].port.toString()})
            }
        }

        choiceminingtype.currentIndex = 1;

        var selNodeInfo = currentInfo.getSelectMinInfo()
        if(selNodeInfo.split(":")[0].length > 0)
        {
            //console.log("edit the update####################################")
            choiceminingtype.currentIndex = selNodeInfo.split(":")[0]
            soloMinerThreadsLine.text =  selNodeInfo.split(":")[2]
            if(selNodeInfo.split(":")[1] === "true"){
                backgroundMining.checked = true
            }
            else{
                backgroundMining.checked = false
            }
        }
    }

    function updateStatusText() {
        var text = ""
         if(!is32){
            if (choiceminingtype.currentIndex > 0) {
                if (rpcManager.isMining()) {
                    if(errorPopup.active){
                        errorPopup.close();
                    }
                    if(connectRpcPopup.active){
                        connectRpcPopup.close();
                    }

                    if (text !== "")
                        text += "<br>";
                    text += qsTr("Mining at %1 H/s").arg(rpcManager.miningHashRate())
                }
            } else {
                if (walletManager.isMining()) {
                    if (text !== "")
                        text += "<br>";
                    text += qsTr("Mining at %1 H/s").arg(walletManager.miningHashRate())
                }
            }
         }
         else 
         {
                if (rpcManager.isMining()) {
                    if(errorPopup.active){
                        errorPopup.close();
                    }
                    if(connectRpcPopup.active){
                        connectRpcPopup.close();
                    }

                    if (text !== "")
                        text += "<br>";
                    text += qsTr("Mining at %1 H/s").arg(rpcManager.miningHashRate())
                }
         }

        if (text === "") {
            text += qsTr("Not mining") + translationManager.emptyString;
        }
        statusText.text = qsTr("Status: ") + text
    }

    function update() {

        updateStatusText()
        if(!is32){
            if (choiceminingtype.currentIndex > 0) {
                startSoloMinerButton.enabled = !rpcManager.isMining()
            } else  {
                startSoloMinerButton.enabled = !walletManager.isMining()
            }
        } else {
             startSoloMinerButton.enabled = !rpcManager.isMining()
        }

        stopSoloMinerButton.enabled = !startSoloMinerButton.enabled
        choiceminingtype.enabled = !stopSoloMinerButton.enabled
    }

    StandardDialog {
        id: errorPopup
        cancelVisible: false
    }
    StandardDialog {
        id: connectRpcPopup
        cancelVisible: false
        okVisible: false
//        opacity: 0
        height: 1
        width: 1
        modality: Qt.ApplicationModal
    }
    Timer{
        id:countdown
        interval: 100
        repeat: true
        triggeredOnStart: true//这一设置保证了立即触发，如果没有，你会发现有延迟
        onTriggered: {
            attr.counter--;
            rpcStartSuccess = rpcManager.startMining(rpcJsonConfig, soloMinerThreadsLine.text);
            if(attr.counter<0||rpcStartSuccess)
            {
                if(connectRpcPopup.active){
                    connectRpcPopup.close();
                }

                countdown.stop();
            }
        }
    }
    StandardDialog {
        id: mentionPopup
        cancelVisible: false
        modality:Qt.ApplicationModal
        okText: qsTr("ok")
        onAccepted: {
                console.debug(cbItems.get(choiceminingtype.currentIndex).text + ", " + cbItems.get(choiceminingtype.currentIndex).index)
                var success = false;
                //set xmrig rpc port 7777
                var rpc_xmrig_port = 7777;
                var pool_address = "";
                var pool_port = "";
                var json_config = "";

                if(!is32){

                 if (choiceminingtype.currentIndex > 0) {

                   pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                   pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];

                   json_config ="{\"algorithm\":\"cryptonight\","
                   + "\"pool\":" + "\"" + pool_address + "\"," + "\"port\":" + pool_port + ","
                   + "\"user\":" + "\""+ appWindow.currentWallet.address +"\"," + "\"password\":\"x\"}";

                   currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                    console.debug( "###########setcurrentInurrentIndex2" +choiceminingtype.currentIndex)
                   currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                   console.debug(json_config);

                   if (!rpcManager.isMining()) {

                        //todo: startxmrig process
                       if (rpcManager.startXmrig(rpc_xmrig_port)) {
                             //start json rpc send /start request to mining
                             //success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                             attr.counter = 100;
                             rpcStartSuccess = false;
                             rpcJsonConfig = json_config;
                             countdown.start();

                             connectRpcPopup.alert();

                             success = rpcStartSuccess;

//                             while(attr.counter>=0&&!rpcStartSuccess){

//                             }

                             if (success == false) {
                                //connect fail to restart xmrig
                                console.debug("start json rpc mining failed\n");
                                rpcManager.stopXmrig();
                                if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                    success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                 }

                             }

                        } else {
                                console.debug("startXmrig failed\n");
                        }

                   } else {
                          console.debug("rpcManager.isrun ==true")
                          success = true;
                   }

                  } else {
                       currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                       currentInfo.setCurrentNodeInfo(cbItems.get(choiceminingtype.currentIndex).index,appWindow.currentWallet.address,soloMinerThreadsLine.text);
                       success = walletManager.startMining(appWindow.currentWallet.address, soloMinerThreadsLine.text, persistentSettings.allow_background_mining, persistentSettings.miningIgnoreBattery)
                  }
                }
                else { //is 32 bit

                    pool_address = cbItems.get(choiceminingtype.currentIndex).index.split(":")[0];
                    pool_port = cbItems.get(choiceminingtype.currentIndex).index.split(":")[1];
                    json_config ="{\"algorithm\":\"cryptonight\","
                   + "\"pool\":" + "\"" + pool_address + "\"," + "\"port\":" + pool_port + ","
                   + "\"user\":" + "\""+ appWindow.currentWallet.address +"\"," + "\"password\":\"x\"}";

                   currentInfo.setCurrentPoolInfo( pool_address,pool_port, appWindow.currentWallet.address,soloMinerThreadsLine.text)
                   console.debug( "###########setcurrentInurrentIndex2" +choiceminingtype.currentIndex)
                   currentInfo.setSelectMinInfo(choiceminingtype.currentIndex,backgroundMining.checked.toString(),soloMinerThreadsLine.text)
                   console.debug(json_config);

                   if (!rpcManager.isMining()) {

                        //todo: startxmrig process
                        if (rpcManager.startXmrig(rpc_xmrig_port)) {
                             //start json rpc send /start request to mining
                             success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                             if (success == false) {
                                //connect fail to restart xmrig
                                console.debug("start json rpc mining failed\n");
                                rpcManager.stopXmrig();
                                if (rpcManager.startXmrig(rpc_xmrig_port)) {
                                    success = rpcManager.startMining(json_config, soloMinerThreadsLine.text);
                                 }

                             }

                        } else {
                                console.debug("startXmrig failed\n");
                                success = false;
                        }

                   } else {
                          console.debug("rpcManager.isrun ==true")
                          success = true;
                   }

                }


                if (success) {

                    updateStatusText()
                    if(!is32){
                        if (choiceminingtype.currentIndex > 0) {
                           startSoloMinerButton.enabled = !rpcManager.isMining()
                        } else  {
                            startSoloMinerButton.enabled = !walletManager.isMining()
                        }
                    } else {
                        startSoloMinerButton.enabled = !rpcManager.isMining()
                    }

                    stopSoloMinerButton.enabled = !startSoloMinerButton.enabled
                } else {
                    errorPopup.title  = qsTr("Error starting mining") + translationManager.emptyString;
                    errorPopup.text = qsTr("Couldn't start mining.<br>")
                    if (!isDaemonLocal())
                        errorPopup.text += qsTr("Mining is only available on local daemons. Run a local daemon to be able to mine.<br>")
                    errorPopup.icon = StandardIcon.Critical
                    errorPopup.open()
                }
        }
    }
    Timer {
        id: timer
        interval: 2000; running: false; repeat: true
        onTriggered: update()
    }

    function onPageCompleted() {
        console.log("Mining page loaded");

        updteMinPoolInfo()

        update()
        timer.running = isDaemonLocal()

    }
    function onPageClosed() {
        timer.running = false
    }

    Connections {
        target: currentInfo

    }
}
