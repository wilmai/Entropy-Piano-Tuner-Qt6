#include "qmidimessage.h"
#include <QtCore/qdebug.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>

QT_BEGIN_NAMESPACE

/*!
    \class QMidiMessage
    \brief The QMidiMessage class provides a container for a single midi message.

    \inmodule QtMidi
    \ingroup midi
    \ingroup midi_audio

    A QMidiMessage stores all bytes of a single midi messages.
    Refer to a midi documentation for further information.

    Basically the first byte defines an identifier of the message. E.g. System message,
    key pressed, or key released. On some message identifiers the last 4 bits define the
    \l channel of the message whilst the first 4 bytes represent the \l command.
    E.g. a press, release, pitch event.

    All following bytes are optional and depend on the message identifier.

    Moreover, the QMidiMessage can store a timestamp in milliseconds that is the duration
    since the last message of the QInputDevice was sent.

    There are several ways to construct a QMidiMessage. Either by a vector of bytes
    or by specifying the bytes directly.

    It is also possible to create a QMidiMessage \l fromCommand or \l fromCommandChannel.
 */

static void qRegisterAutoMetaTypes()
{
    qRegisterMetaType<QMidiMessage>();
}

Q_CONSTRUCTOR_FUNCTION(qRegisterAutoMetaTypes)

/*!
    Constructor for an empty QMidiMessage.
 */
QMidiMessage::QMidiMessage()
{
}

/*!
    Construct a midi message from given bytes \a m and an optional \a timestamp.
 */
QMidiMessage::QMidiMessage(const QVector<quint8> &m, quint64 timestamp)
    : mMessage(m)
    , mTimestamp(timestamp)
{
}

/*!
    Construct a midi message from two bytes \a byte0 and \a byte1 and an optional \a timestamp.
 */
QMidiMessage::QMidiMessage(quint8 byte0, quint8 byte1, quint64 timestamp)
    : mMessage(2)
    , mTimestamp(timestamp)
{
    mMessage[0] = byte0;
    mMessage[1] = byte1;
}

/*!
    Construct a midi message from three bytes \a byte0, \a byte1, and \a byte2 and an optional \a timestamp.
 */
QMidiMessage::QMidiMessage(quint8 byte0, quint8 byte1, quint8 byte2, quint64 timestamp)
    : mMessage(3)
    , mTimestamp(timestamp)
{
    mMessage[0] = byte0;
    mMessage[1] = byte1;
    mMessage[2] = byte2;
}

/*!
    Construct a midi message from four bytes \a byte0, \a byte1, \a byte2, and \a byte3.
    A optional \a timestamp can be provided.
 */
QMidiMessage::QMidiMessage(quint8 byte0, quint8 byte1, quint8 byte2, quint8 byte3, quint64 timestamp)
    : mMessage(4)
    , mTimestamp(timestamp)
{
    mMessage[0] = byte0;
    mMessage[1] = byte1;
    mMessage[2] = byte2;
    mMessage[3] = byte3;
}

/*!
    Construct a midi message from a \a command (4 bit representation) and two bytes \a byte1 and \a byte2.
    An optional \a timestamp my be provided.
    The \a command will be bitshifted by 4 to the left to get the corresponding byte0. The channel will be zero.
 */
QMidiMessage QMidiMessage::fromCommand(int command, int byte1, int byte2, quint64 timestamp) {
    QVector<quint8> m;
    m.append(static_cast<quint8>(command));
    if (byte1 >= 0) {
        m.append(static_cast<quint8>(byte1));
        if (byte2 >= 0) {
            m.append(static_cast<quint8>(byte2));
        }
    }

    return QMidiMessage(m, timestamp);
}

/*!
    Construct a midi message from a \a command (4 bit representation) and a \a channel (4 bit representation) and two bytes \a byte1 and \a byte2.
    An optional \a timestamp my be provided.
    The \a command will be bitshifted by 4 to the left and added to the \a channel to get the corresponding byte0.
 */

QMidiMessage QMidiMessage::fromCommandChannel(quint8 command, quint8 channel, quint8 byte1, quint64 timestamp) {
    return QMidiMessage((command << 4) + channel, byte1, timestamp);
}

QMidiMessage QMidiMessage::fromCommandChannel(quint8 command, quint8 channel, quint8 byte1, quint8 byte2, quint64 timestamp) {
    return QMidiMessage((command << 4) + channel, byte1, byte2, timestamp);
}

/*!
    Parse the command of the messsage. This will return the first four bits of \l byte0(). The range will therefore be in [0-127].

    \sa setCommand, channel
 */
quint8 QMidiMessage::command() const {
    return (byte0() & 0xF0) >> 4;
}

/*!
    Set the command of the message. The command \a c is expected to be a 4 bit number in [0-127]. The first four bits of \l byte0() will
    be set to \a c.

    \sa command, channel, setChannel
 */
void QMidiMessage::setCommand(quint8 c) {
    // add old channel with new command
    setByte0((byte0() & 0x0F) | ((c << 4) & 0xF0));
}

/*!
    Parse the channel of the message. This will return the last four biys of \l byte0(). The range will therefore be in [0-127].

    \sa command, setChannel
 */
quint8 QMidiMessage::channel() const {
    return byte0() & 0x0F;
}

/*!
    Set the channel of the message. The channel \a c is expected to be a four bit number in [0-127]. The last four bits of \l byte0() will
    be set to \a c.
 */
void QMidiMessage::setChannel(quint8 c) {
    // add old command with new channel
    setByte0((byte0() & 0xF0) | (c & 0x0F));
}

/*!
    Get the first byte of the messsage. This function will return 0 if the \l size() is zero.
 */
quint8 QMidiMessage::byte0() const {
    if (mMessage.size() > 0) {return mMessage[0];}
    return 0;
}

/*!
    Set the first byte of the messsage to \a b0.
 */
void QMidiMessage::setByte0(quint8 b0) {
    if (mMessage.size() > 0) {mMessage[0] = b0;}
    else {appendByte(b0);}
}

/*!
    Get the second byte of the messsage. This function will return 0 if the \l size() is smaller than two.
 */
quint8 QMidiMessage::byte1() const {
    if (mMessage.size() > 1) {return mMessage[1];}
    return 0;
}

/*!
    Get the thrid byte of the messsage. This function will return 0 if the \l size() is smaller than three.
 */
quint8 QMidiMessage::byte2() const {
    if (mMessage.size() > 2) {return mMessage[2];}
    return 0;
}

/*!
    Transforms a QMidiMessage maximal first four bytes into a 32 bit unsigned integer.

    \sa byte0, byte1, byte2
 */
quint32 QMidiMessage::toUInt32() const {
    quint32 packet;
    quint8 *ptr = (quint8 *) &packet;
    // write a maximum of 4 bytes
    for ( int i=0; i < std::min<int>(4, mMessage.size()); ++i ) {
        *ptr = mMessage[i];
        ++ptr;
    }

    return packet;
}

/*!
    Appends the \a byte to the message.
 */
void QMidiMessage::appendByte(quint8 byte) {
    mMessage.append(byte);
}

/*!
    Sets the timestamp of the message to \a d.

    \sa timestamp
 */
void QMidiMessage::setTimestamp(const quint64 d) {
    mTimestamp = d;
}

/*!
    Returns the timestamp of the message.
 */
quint64 QMidiMessage::timestamp() const {
    return mTimestamp;
}

/*!
    Returns all available bytes in the message.
 */
const QVector<quint8> QMidiMessage::message() const {
    return mMessage;
}

/*!
    Returns the number of bytes stored in the message.
 */
int QMidiMessage::size() const {
    return mMessage.size();
}

/*!
    Returns the \a{i}th byte with read access only.
 */
const quint8 &QMidiMessage::operator[](int i) const {
    return mMessage[i];
}

/*!
    Returns the \a{i}th byte with read and write access.
 */
quint8 &QMidiMessage::operator[](int i) {
    return mMessage[i];
}

bool QMidiMessage::equalBytes(const QMidiMessage &other) const {
    return mMessage == other.mMessage;
}

bool QMidiMessage::operator==(const QMidiMessage &other) const {
    return mMessage == other.mMessage && mTimestamp == other.mTimestamp;
}

bool QMidiMessage::operator!=(const QMidiMessage &other) const {
    return mMessage != other.mMessage || mTimestamp != other.mTimestamp;
}

QString QMidiMessage::toJson() const {
    QJsonObject json;
    QJsonArray array;
    for (int i = 0; i < size(); ++i) {
        array.append(QString::fromLatin1("0x%1").arg(mMessage[i], 2, 16, QChar::fromLatin1('0')));
    }
    json.insert(QString::fromLatin1("bytes"), array);
    json.insert(QString::fromLatin1("timestamp"), QString::number(timestamp()));

    QJsonDocument doc(json);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

#ifndef QT_NO_DEBUG_STREAM

/*!
    Writes the midi message \a m to the debug stream \a dbg.

    Usage e.g. qDebug() << QMidiMessage();
 */
QDebug operator<<(QDebug dbg, const QMidiMessage& m) {
    QDebugStateSaver saver(dbg);
    dbg.nospace();
    dbg << "{QMidiMessage {size: " << m.size();
    if (m.size() > 0) {
        dbg << ", command: 0x" << QString::number(m[0], 16);
    }
    dbg << ", timestamp: " << m.timestamp();
    dbg << ", bytes: [ ";
    for (int i = 1; i < m.size(); ++i) {
        dbg << "0x" << QString::number(m[i], 16) << " ";
    }
    dbg << "]}}";

    return dbg;
}
#endif

QT_END_NAMESPACE
