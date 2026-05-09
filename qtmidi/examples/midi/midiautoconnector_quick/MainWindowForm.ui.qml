import QtQuick 2.4
import QtMidi 5.0
import QtQuick.Controls 2.3

Item {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    property alias connectionStatus: connectionStatus
    property alias midiInput: midiInput
    property alias midiOutput: midiOutput
    property alias midiInputPower: midiInputPower
    property alias midiOutputPower: midiOutputPower

    Switch {
        id: connectionStatus
        text: checked ? qsTr("Connected") : qsTr("Disconnected")
        anchors.top: midiInputPower.bottom
    }
    MidiInputComboBox {
        id: midiInput
        anchors.top: parent.top
    }

    MidiOutputComboBox {
        id: midiOutput
        anchors.top: connectionStatus.bottom
    }

    MidiPowerBar {
        id: midiInputPower
        anchors.top: midiInput.bottom
    }

    MidiPowerBar {
        id: midiOutputPower
        anchors.top: midiOutput.bottom
    }

}
