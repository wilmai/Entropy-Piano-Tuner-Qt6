#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "midicommunicator.h"

QTextStream qout(stdout);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MidiCommunicator c;

    return a.exec();
}
