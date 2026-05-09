#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QTableWidget>

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

signals:
    void addLine(QString line);

protected slots:
    void onAddConnection();
    void onDeleteConnection();
    void deleteRow(int index);

    void inputDeviceAttached(const QMidiDeviceInfo info);
    void outputDeviceAttached(const QMidiDeviceInfo info);
    void inputDeviceDetached(const QMidiDeviceInfo info);
    void outputDeviceDetached(const QMidiDeviceInfo info);

    void inputDeviceCreated(const QMidiInput* device);
    void outputDeviceCreated(const QMidiOutput* device);
    void inputDeviceDeleted(QMidiDeviceInfo info);
    void outputDeviceDeleted(QMidiDeviceInfo info);

    void receiveMessage(const QMidiMessage &m);

    void inputIndexChanged(int index);
    void outputIndexChanged(int index);
    void forcedChannelChanged(int index);

private:
    QMidiInput* accessInput(const QMidiDeviceInfo &info);
    QMidiOutput* accessOutput(const QMidiDeviceInfo &info);
    QMidiDevice* access(const QMidiDeviceInfo &info, QMidi::Mode mode);
    void release(const QMidiDeviceInfo &info, QMidi::Mode mode);

    QMidiInput* getInput(const QMidiDeviceInfo &info);
    QMidiOutput* getOutput(const QMidiDeviceInfo &info);
    QMidiDevice* get(const QMidiDeviceInfo &info, QMidi::Mode mode);


private:
    QPlainTextEdit* mConsoleOutput;
    QComboBox* mInputDevices;
    QComboBox* mOutputDevices;
    QComboBox* mForcedOutputChannel;
    QTableWidget* mConnections;

    QList<QPair<int, QMidiDevice*> > mCurrentDevices[2];
};

#endif // MAINWINDOW_H
