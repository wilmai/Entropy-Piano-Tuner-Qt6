#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

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
    void printInputDevices();
    void printOutputDevices();

    void inputDeviceAttached(const QMidiDeviceInfo info);
    void outputDeviceAttached(const QMidiDeviceInfo info);
    void inputDeviceDetached(const QMidiDeviceInfo info);
    void outputDeviceDetached(const QMidiDeviceInfo info);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);

    void receiveMessage(const QMidiMessage &m);

signals:
    void addLine(QString line);

private:
    QPlainTextEdit* mConsoleOutput;
};

#endif // MAINWINDOW_H
