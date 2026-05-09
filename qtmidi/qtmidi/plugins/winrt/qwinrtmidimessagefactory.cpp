#include "qwinrtmidimessagefactory.h"
#include "qwinrtnativehelper.h"

QT_BEGIN_NAMESPACE

IMidiMessage^ QWinRTMidiMessageFactory::create(const QMidiMessage &m) {
    switch (m.command())
    {
    case MidiMessageType::NoteOff:

        return ref new MidiNoteOffMessage(m.channel(), m.byte1(), m.byte2());
        break;

    case MidiMessageType::NoteOn:

        return ref new MidiNoteOnMessage(m.channel(), m.byte1(), m.byte2());
        break;

    case MidiMessageType::PolyphonicKeyPressure:

        return ref new MidiPolyphonicKeyPressureMessage(m.channel(), m.byte1(), m.byte2());
        break;

    case MidiMessageType::ControlChange:

        return ref new MidiControlChangeMessage(m.channel(), m.byte1(), m.byte2());
        break;

    case MidiMessageType::ProgramChange:

        return ref new MidiProgramChangeMessage(m.channel(), m.byte1());
        break;

    case MidiMessageType::ChannelPressure:

        return ref new MidiChannelPressureMessage(m.channel(), m.byte1());
        break;

    case MidiMessageType::PitchBendChange:

        return ref new MidiPitchBendChangeMessage(m.channel(), m.byte1());
        break;
    /*
    case MidiMessageType::SystemExclusive:
    {
        DataWriter^ dataWriter = ref new DataWriter();

        // expecting a string of format "NN NN NN NN...." where NN is a byte in hex
        int len = _sendMessageTextBox->Text->Length();
        if (0 == len)
        {
            return;
        }

        const wchar_t* startPointer = _sendMessageTextBox->Text->Data();
        wchar_t* endPointer = nullptr;

        do
        {
            byte midiByte = (byte)wcstoul(startPointer, &endPointer, 16);
            if (endPointer == startPointer)
            {
                // conversion failed, bail out
                break;
            }

            dataWriter->WriteByte(midiByte);

            // prep for next iteration
            startPointer = endPointer;
        } while (nullptr != endPointer);

        return ref new MidiSystemExclusiveMessage(dataWriter->DetachBuffer());
        break;
    }*/
    case MidiMessageType::MidiTimeCode:

        return ref new MidiTimeCodeMessage(m.byte1(), m.byte2());
        break;

    case MidiMessageType::SongPositionPointer:

        return ref new MidiSongPositionPointerMessage(m.byte1());
        break;

    case MidiMessageType::SongSelect:

        return ref new MidiSongSelectMessage(m.byte1());
        break;

    case MidiMessageType::TuneRequest:

        return ref new MidiTuneRequestMessage();
        break;

    case MidiMessageType::TimingClock:

        return ref new MidiTimingClockMessage();
        break;

    case MidiMessageType::Start:

        return ref new MidiStartMessage();
        break;

    case MidiMessageType::Continue:

        return ref new MidiContinueMessage();
        break;

    case MidiMessageType::Stop:

        return ref new MidiStopMessage();
        break;

    case MidiMessageType::ActiveSensing:

        return ref new MidiActiveSensingMessage();
        break;

    case MidiMessageType::SystemReset:

        return ref new MidiSystemResetMessage();
        break;

    case MidiMessageType::None:
    default:
        return nullptr;
    }
}

QT_END_NAMESPACE
