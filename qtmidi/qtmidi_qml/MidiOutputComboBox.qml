import QtQuick 2.0
import QtMidi 5.0
import QtQuick.Controls 2.2

ComboBox {
    id: midiComboBox
    model: midiDevice.availableDevices
    currentIndex: midiDevice.currentIndex
    MidiOutput {
        id: midiDevice
        onCurrentIndexChanged: {
            midiComboBox.currentDeviceChanged(midiComboBox.textAt(midiDevice.currentIndex));
        }
        onCurrentDeviceToBeDeleted: {
            midiComboBox.currentDeviceToBeDeleted()
        }
        onCurrentDeviceDetached: {
            midiComboBox.currentDeviceDetached()
        }
    }

    signal currentDeviceChanged(string deviceName)
    signal currentDeviceToBeDeleted()
    signal currentDeviceDetached()

    function sendMessage(message) {
        midiDevice.sendMessage(message);
    }

    onCurrentIndexChanged: {
        midiDevice.setCurrentIndex(currentIndex)
    }
}
