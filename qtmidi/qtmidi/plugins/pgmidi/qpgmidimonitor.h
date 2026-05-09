#ifndef QPGMIDIMONITOR_H
#define QPGMIDIMONITOR_H

#include <QtCore/qglobal.h>
#import "../../3rdparty/pgmidi/PGMidi.h"

#include "qpgmidiwrapper.h"

QT_BEGIN_NAMESPACE

class QPGMidiInput;

@interface QPGMidiInputInterface : NSObject

@property QPGMidiInput *mInput;

-(id)initInput:(QPGMidiInput *)input;
@end

@interface QPGMidiMonitor : NSObject

@property QPGMidiWrapper *mWrapper;
@property (assign) PGMidi *mPgMidiInterface;

-(id)initMonitor:(QPGMidiWrapper *)wrapper;
-(void)start;
-(QList<QByteArray>)availableInputDevices;
-(QList<QByteArray>)availableOutputDevices;
-(void)connectSource:(const QByteArray&)handle input:(QPGMidiInputInterface *)interface;
-(void)disconnectSource:(const QByteArray&)handle input:(QPGMidiInputInterface *)interface;
-(PGMidiDestination*)destination:(const QByteArray&)handle;
@end



QT_END_NAMESPACE

#endif // QPGMIDIMONITOR_H
