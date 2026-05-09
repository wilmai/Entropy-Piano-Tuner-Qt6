#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QListWidget>

#include <qtmidi/qmidideviceinfo.h>
#include <qtmidi/qmidiinput.h>
#include <qtmidi/qmidioutput.h>

class MainWindow
        : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:

    // monitor slots
    void inputDeviceAttached(const QMidiDeviceInfo info);
    void outputDeviceAttached(const QMidiDeviceInfo info);
    void inputDeviceDetached(const QMidiDeviceInfo info);
    void outputDeviceDetached(const QMidiDeviceInfo info);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);

    void receiveMessage(const QMidiMessage &m);

    // auto connector slots
    void acDeviceCreated(const QMidiDevice* dev, QMidi::Mode mode);

    // List widget slots
    void onItemDoubleClicked(QListWidgetItem *item);

signals:
    void connectWith(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void addLine(QString line);

private:
    // helper functions
    QListWidgetItem *itemByInfo(const QMidiDeviceInfo &info, QMidi::Mode mode);

private:
    QPlainTextEdit* mConsoleOutput;
    QListWidget* mInputDevices;
    QListWidget* mOutputDevices;
};

#endif // MAINWINDOW_H
