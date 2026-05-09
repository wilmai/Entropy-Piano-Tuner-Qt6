import QtQuick 2.0
import QtQuick.Controls 2.2

ProgressBar {
    id: progressBar
    from: 0
    to: 127
    value: 100
    Timer {
        interval: 20; running: true; repeat: true
        onTriggered: progressBar.value = progressBar.value - 8
    }
}
