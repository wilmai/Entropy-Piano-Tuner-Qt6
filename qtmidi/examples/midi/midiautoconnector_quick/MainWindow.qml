import QtQuick 2.4
import QtQuick.Controls 2.2

MainWindowForm {
    midiInput.onCurrentDeviceChanged: {
        console.log("Current input:" + deviceName);
        connectionStatus.checked = true
    }

    midiInput.onMidiMessageReceived: {
        midiInputPower.value = message.volume()
        if (connectionStatus.checked) {
            midiOutputPower.value = message.volume()
            midiOutput.sendMessage(message)
        }
        console.log(message.toJson())
    }

    midiOutput.onCurrentDeviceChanged: {
        console.log("Current output: " + deviceName);
        connectionStatus.checked = true
    }

    midiOutput.onCurrentDeviceDetached: {
        console.warn("Output device datached");
    }

    midiOutput.onCurrentDeviceToBeDeleted: {
        console.info("Output device to be deleted");
    }
}
