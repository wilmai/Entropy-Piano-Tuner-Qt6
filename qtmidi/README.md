# About

QtMidi is an unofficial Qt plugin that enables platform independent support (Windows, Windows Universal, Mac OS X, Linux, Android, iOS) for connecting to midi devices.

## Building Code and Examples

Open `qtmidi.pro` in the QtCreator to build the library and the examples.

## Using QtMidi in other projects

Add `qtmidi.pro` as a subproject.

### Linux Dependencies

On Ubuntu install

```
sudo apt install libasound2-dev
```

### WinRT

```
<PackageDependency Name="Microsoft.Midi.GmDls" MinVersion="1.0.0.0" Publisher="CN=Microsoft Corporation, O=Microsoft Corporation, L=Redmond, S=Washington, C=US" />
```

