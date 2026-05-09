package org.uniwue.tp3.midi;

import android.content.Context;
import android.widget.Toast;
import android.hardware.usb.UsbDevice;
import android.util.Log;
import android.text.TextUtils;
import android.app.Activity;

import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;

import jp.kshoji.driver.midi.device.MidiInputDevice;
import jp.kshoji.driver.midi.device.MidiOutputDevice;
import jp.kshoji.driver.midi.util.UsbMidiDriver;

public class QAndroidMidiInterface {
    private static final String TAG = "QAndroidMidiInterface";

    public static QAndroidMidiInterface create(Activity activity) {
        Log.i(TAG, "Creating android QAndroidMidiInterface");
        return new QAndroidMidiInterface(activity);
    }

    private final UsbMidiDriver usbMidiDriver;
    private final Activity activity;
    private final Set<MidiInputDevice> inputDevices = new HashSet<>();

    private boolean mActiveInput = false;
    private boolean mActiveOutput = false;

    // Native methods
    public static native void java_midi_sendMidiMessage(String deviceName, int byte1);
    public static native void java_midi_sendMidiMessage(String deviceName, int byte1, int byte2);
    public static native void java_midi_sendMidiMessage(String deviceName, int byte1, int byte2, int byte3);
    public static native void java_midi_sendMidiMessage(String deviceName, byte[] bytes);
    public static native void java_midi_inputDeviceAttached(String deviceName);
    public static native void java_midi_inputDeviceDetached(String deviceName);
    public static native void java_midi_outputDeviceAttached(String deviceName);
    public static native void java_midi_outputDeviceDetached(String deviceName);

    public QAndroidMidiInterface(Activity activity) {
        this.activity = activity;
        usbMidiDriver = new UsbMidiDriver(activity) {
            @Override
            public void onDeviceAttached(UsbDevice usbDevice) {
                // deprecated method.
            }

            @Override
            public void onMidiInputDeviceAttached(MidiInputDevice midiInputDevice) {
                inputDevices.add(midiInputDevice);
                midiInputDevice.suspend();
                java_midi_inputDeviceAttached(getDeviceName(midiInputDevice));
            }

            @Override
            public void onMidiOutputDeviceAttached(final MidiOutputDevice midiOutputDevice) {
                midiOutputDevice.suspend();
                java_midi_outputDeviceAttached(getDeviceName(midiOutputDevice));
            }

            @Override
            public void onMidiReset(MidiInputDevice midiInputDevice, int id) {
            }

            @Override
            public void onDeviceDetached(UsbDevice usbDevice) {
                // deprecated method.
            }

            @Override
            public void onMidiInputDeviceDetached(MidiInputDevice midiInputDevice) {
                inputDevices.remove(midiInputDevice);
                java_midi_inputDeviceDetached(getDeviceName(midiInputDevice));
            }

            @Override
            public void onMidiOutputDeviceDetached(final MidiOutputDevice midiOutputDevice) {
                java_midi_outputDeviceDetached(getDeviceName(midiOutputDevice));
            }

            @Override
            public void onMidiNoteOff(final MidiInputDevice sender, int cable, int channel, int note, int velocity) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0x80 | channel, note, velocity);
            }

            @Override
            public void onMidiNoteOn(final MidiInputDevice sender, int cable, int channel, int note, int velocity) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0x90 | channel, note, velocity);
            }

            @Override
            public void onMidiPolyphonicAftertouch(final MidiInputDevice sender, int cable, int channel, int note, int pressure) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0xA0 | channel, note, pressure);
            }

            @Override
            public void onMidiControlChange(final MidiInputDevice sender, int cable, int channel, int function, int value) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0xB0 | channel, function, value);
            }

            @Override
            public void onMidiProgramChange(final MidiInputDevice sender, int cable, int channel, int program) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0xC0 | channel, program);
            }

            @Override
            public void onMidiChannelAftertouch(final MidiInputDevice sender, int cable, int channel, int pressure) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0xD0 | channel, pressure);
            }

            @Override
            public void onMidiPitchWheel(final MidiInputDevice sender, int cable, int channel, int amount) {
                java_midi_sendMidiMessage(getDeviceName(sender), 0xE0 | channel, amount);
            }

            @Override
            public void onMidiSystemExclusive(final MidiInputDevice sender, int cable, final byte[] systemExclusive) {
                java_midi_sendMidiMessage(getDeviceName(sender), systemExclusive);
            }

            @Override
            public void onMidiSystemCommonMessage(final MidiInputDevice sender, int cable, final byte[] bytes) {
                java_midi_sendMidiMessage(getDeviceName(sender), bytes);
            }

            @Override
            public void onMidiSingleByte(final MidiInputDevice sender, int cable, int byte1) {
                java_midi_sendMidiMessage(getDeviceName(sender), byte1);
            }

            @Override
            public void onMidiMiscellaneousFunctionCodes(final MidiInputDevice sender, int cable, int byte1, int byte2, int byte3) {
                java_midi_sendMidiMessage(getDeviceName(sender), byte1, byte2, byte3);
            }

            @Override
            public void onMidiCableEvents(final MidiInputDevice sender, int cable, int byte1, int byte2, int byte3) {
                java_midi_sendMidiMessage(getDeviceName(sender), byte1, byte2, byte3);
            }

            @Override
            public void onMidiActiveSensing(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xFE);
            }

            @Override
            public void onMidiStop(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xFC);
            }

            @Override
            public void onMidiContinue(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xFB);
            }

            @Override
            public void onMidiStart(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xFA);
            }

            @Override
            public void onMidiTimingClock(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xF8);
            }

            @Override
            public void onMidiTuneRequest(MidiInputDevice device, int cable) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xF6);
            }

            @Override
            public void onMidiSongPositionPointer(MidiInputDevice device, int cable, int byte1) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xF2, byte1);
            }

            @Override
            public void onMidiSongSelect(MidiInputDevice device, int cable, int byte1) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xF3, byte1);
            }

            @Override
            public void onMidiTimeCodeQuarterFrame(MidiInputDevice device, int cable, int byte1) {
                java_midi_sendMidiMessage(getDeviceName(device), 0xF1, byte1);
            }

        };
    }

    public void destroy() {
        usbMidiDriver.close();
    }

    public void init() {
        Log.i(TAG, "Initializing UsbMidiDriver");
        activity.runOnUiThread(new Runnable() {
            public void run() {
                usbMidiDriver.open();
            }
        });
    }

    public void exit() {
        Log.i(TAG, "Closing UsbMidiDriver");
        activity.runOnUiThread(new Runnable() {
            public void run() {
                usbMidiDriver.close();
            }
        });
    }

    public void initInput() {
        mActiveInput = true;
        init();
    }

    public void exitInput() {
        mActiveInput = false;
        if (!mActiveInput && !mActiveOutput) {
            exit();
        }
    }

    public void initOutput() {
        mActiveOutput = true;
        init();
    }

    public void exitOutput() {
        mActiveOutput = false;
        if (!mActiveInput && !mActiveOutput) {
            exit();
        }
    }


    public boolean connectInputDevice(String name) {
        MidiInputDevice dev = getInputDeviceByName(name);
        if (dev != null) {
            dev.resume();
            return true;
        } else {
            return false;
        }
    }

    public boolean disconnectInputDevice(String name) {
        MidiInputDevice device = getInputDeviceByName(name);
        if (device == null) {return false;}
        device.suspend();
        return true;
    }

    public boolean connectOutputDevice(String name) {
        MidiOutputDevice dev = getOutputDeviceByName(name);
        if (dev != null) {
            dev.resume();
            return true;
        } else {
            return false;
        }
    }

    public boolean disconnectOutputDevice(String name) {
        MidiOutputDevice device = getOutputDeviceByName(name);
        if (device == null) {return false;}
        device.suspend();
        return true;
    }

    public String getDeviceName(MidiInputDevice device) {
        if  (device == null) {return "";}
        String s = device.getProductName();
        if (s != null) {return s;}
        s = device.getManufacturerName();
        if (s != null) {return s;}
        return device.getDeviceAddress();
    }

    public String getDeviceName(MidiOutputDevice device) {
        if  (device == null) {return "";}
        String s = device.getProductName();
        if (s != null) {return s;}
        s = device.getManufacturerName();
        if (s != null) {return s;}
        return device.getDeviceAddress();
    }

    public String getInputDeviceNames() {
        List<String> names = new ArrayList<String>();
        for (MidiInputDevice dev : getMidiInputDevices()) {
            names.add(getDeviceName(dev));
        }

        return TextUtils.join("\n", names);

    }

    public String getOutputDeviceNames() {
        List<String> names = new ArrayList<String>();
        for (MidiOutputDevice dev : getMidiOutputDevices()) {
            names.add(getDeviceName(dev));
        }

        return TextUtils.join("\n", names);

    }

    public MidiInputDevice getInputDeviceByName(String s) {
        for (MidiInputDevice dev : getMidiInputDevices()) {
            if (getDeviceName(dev).equals(s)) {
                return dev;
            }
        }

        return null;
    }

    public MidiOutputDevice getOutputDeviceByName(String s) {
        for (MidiOutputDevice dev : getMidiOutputDevices()) {
            if (getDeviceName(dev).equals(s)) {
                return dev;
            }
        }

        return null;
    }

    private Set<MidiInputDevice> getMidiInputDevices() {
        if (usbMidiDriver == null) {
            return Collections.emptySet();
        }
        // return usbMidiDriver.getMidiOutputDevices();
        return inputDevices;
    }

    private Set<MidiOutputDevice> getMidiOutputDevices() {
        if (usbMidiDriver == null) {
            return Collections.emptySet();
        }
        return usbMidiDriver.getMidiOutputDevices();
    }


    public void receiveMidiOutputEvent(String s, int byte1, int byte2, int byte3) {
        MidiOutputDevice dev = getOutputDeviceByName(s);
        if (dev != null) {
            // cable is alwas 0
            dev.sendMidiMessage(0, byte1, byte2, byte3);
        }
    }

}
