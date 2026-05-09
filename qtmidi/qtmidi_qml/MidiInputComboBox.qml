import QtQuick 2.0
import QtMidi 5.0
import QtQuick.Controls 2.2

ComboBox {
    id: midiComboBox
    model: midiDevice.availableDevices
    currentIndex: midiDevice.currentIndex
    MidiInput {
        id: midiDevice
        onCurrentIndexChanged: {
            midiComboBox.currentDeviceChanged(midiComboBox.textAt(midiDevice.currentIndex));
        }
        onMidiMessageReceived: {
            midiComboBox.midiMessageReceived(message)
        }
        onCurrentDeviceToBeDeleted: {
            midiComboBox.currentDeviceToBeDeleted()
        }
        onCurrentDeviceDetached: {
            midiComboBox.currentDeviceDetached()
        }
    }

    signal currentDeviceChanged(string deviceName)
    signal midiMessageReceived(variant message)
    signal currentDeviceToBeDeleted()
    signal currentDeviceDetached()

    onCurrentIndexChanged: {
        midiDevice.setCurrentIndex(currentIndex)
    }
}
