#include "qpgmidimonitor.h"
#include "qpgmidiinput.h"
#include "qpgmidiplugin.h"

#include <QtCore/qvector.h>
#include <QtCore/qdebug.h>
#include <QDateTime>

QT_BEGIN_NAMESPACE

@interface QPGMidiMonitor()<PGMidiDelegate>
@end

@implementation QPGMidiMonitor

-(id) initMonitor:(QPGMidiWrapper *)wrapper
{
    self = [super init];
    if (self) {
        self.mWrapper = wrapper;
    }

    self.mPgMidiInterface = [[PGMidi alloc] init];
    self.mPgMidiInterface.networkEnabled = YES;
    self.mPgMidiInterface.virtualDestinationEnabled = YES;
    self.mPgMidiInterface.virtualSourceEnabled = YES;

    self.mPgMidiInterface.delegate = (id<PGMidiDelegate>)self;

    return self;
}

-(void)start
{
    // Pg midi does not notify on already connected devices
    for (quint16 i = 0; i < [self.mPgMidiInterface.sources count]; ++i) {
        PGMidiSource * source = self.mPgMidiInterface.sources[i];
        auto handle = QString::fromUtf8([source.name UTF8String]).toUtf8();
        self.mWrapper->plugin().notifyAttached(handle, QMidi::MidiInput);
    }

    for (quint16 i = 0; i < [self.mPgMidiInterface.destinations count]; ++i) {
        PGMidiDestination *destination = self.mPgMidiInterface.destinations[i];
        auto handle = QString::fromUtf8([destination.name UTF8String]).toUtf8();
        self.mWrapper->plugin().notifyAttached(handle, QMidi::MidiOutput);
    }

}
-(QList<QByteArray>)availableInputDevices
{
    QList<QByteArray> out;
    for (quint16 i = 0; i < [self.mPgMidiInterface.sources count]; ++i) {
        PGMidiSource * source = self.mPgMidiInterface.sources[i];
        out.append(QString::fromUtf8([source.name UTF8String]).toUtf8());
    }

    return out;
}

-(QList<QByteArray>)availableOutputDevices
{
    QList<QByteArray> out;
    for (quint16 i = 0; i < [self.mPgMidiInterface.destinations count]; ++i) {
        PGMidiDestination * dest = self.mPgMidiInterface.destinations[i];
        out.append(QString::fromUtf8([dest.name UTF8String]).toUtf8());
    }

    return out;
}

-(void)connectSource:(const QByteArray&)handle input:(QPGMidiInputInterface *)interface {
    QString name = QString::fromUtf8(handle);
    for (quint16 i = 0; i < [self.mPgMidiInterface.sources count]; ++i) {
        PGMidiSource *source = self.mPgMidiInterface.sources[i];
        if (name == QString::fromUtf8([source.name UTF8String])) {
            [source addDelegate:(id<PGMidiSourceDelegate>)interface];
            break;
        }
    }
}

-(void)disconnectSource:(const QByteArray&)handle input:(QPGMidiInputInterface *)interface {
    QString name = QString::fromUtf8(handle);
    for (quint16 i = 0; i < [self.mPgMidiInterface.sources count]; ++i) {
        PGMidiSource *source = self.mPgMidiInterface.sources[i];
        if (name == QString::fromUtf8([source.name UTF8String])) {
            [source removeDelegate:(id<PGMidiSourceDelegate>)interface];
            break;
        }
    }
}

-(PGMidiDestination*)destination:(const QByteArray&)handle {
    QString name = QString::fromUtf8(handle);
    for (quint16 i = 0; i < [self.mPgMidiInterface.destinations count]; ++i) {
        PGMidiDestination *dest = self.mPgMidiInterface.destinations[i];
        if (name == QString::fromUtf8([dest.name UTF8String])) {
            return dest;
        }
    }

    return nil;
}

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source
{
    Q_UNUSED(midi)

    auto handle = QString::fromUtf8([source.name UTF8String]).toUtf8();
    self.mWrapper->plugin().notifyAttached(handle, QMidi::MidiInput);
}

- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source
{
    Q_UNUSED(midi)

    auto handle = QString::fromUtf8([source.name UTF8String]).toUtf8();
    self.mWrapper->plugin().notifyDetached(handle, QMidi::MidiInput);
}

- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination
{
    Q_UNUSED(midi)

    auto handle = QString::fromUtf8([destination.name UTF8String]).toUtf8();
    self.mWrapper->plugin().notifyAttached(handle, QMidi::MidiOutput);
}

- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination
{
    Q_UNUSED(midi)

    auto handle = QString::fromUtf8([destination.name UTF8String]).toUtf8();
    self.mWrapper->plugin().notifyDetached(handle, QMidi::MidiOutput);
}

@end

@interface QPGMidiInputInterface()<PGMidiSourceDelegate>
@end

@implementation QPGMidiInputInterface

-(id)initInput:(QPGMidiInput *)input {
    self.mInput = input;
    return self;
}

- (void) midiSource:(PGMidiSource*)midi midiReceived:(const MIDIPacketList *)packetList
{
    Q_UNUSED(midi)
    const MIDIPacket *packet = &packetList->packet[0];
    for (quint32 i = 0; i < packetList->numPackets; ++i)
    {
        QVector<quint8> bytes(packet->length);

        for (int i = 0; i < packet->length; ++i) {
            bytes[i] = packet->data[i];
        }

        // convert timestamp in ns relative to the uptime in s to ms since epoch
        NSTimeInterval uptime = [[NSProcessInfo processInfo] systemUptime];
        self.mInput->handleMidiMessage(QMidiMessage(bytes, packet->timeStamp / 1000000 - uptime * 1000 + QDateTime::currentMSecsSinceEpoch()));

        packet = MIDIPacketNext(packet);
    }
}

@end

QT_END_NAMESPACE
