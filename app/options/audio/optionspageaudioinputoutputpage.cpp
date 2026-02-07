/*****************************************************************************
 * Copyright 2016 Haye Hinrichsen, Christoph Wick
 *
 * This file is part of Entropy Piano Tuner.
 *
 * Entropy Piano Tuner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Entropy Piano Tuner is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Entropy Piano Tuner. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#include "optionspageaudioinputoutputpage.h"

#include <QMessageBox>
#include <QThread>

#include "audioforqt/audiointerfaceforqt.h"

#include "core/config.h"
#include "core/core.h"

#include "mainwindow/mainwindow.h"
#include "mainwindow/volumecontrollevel.h"
#include "widgets/progressoverlay.h"

namespace options {

PageAudioInputOutput::PageAudioInputOutput(OptionsDialog *optionsDialog,
                                           AudioInterfaceForQt::AudioMode mode)
    : mOptionsDialog(optionsDialog), mAudioInterface(nullptr), mMode(mode) {

  ProgressOverlay *overlay = nullptr;
  if (mode == AudioInterfaceForQt::AudioMode::Input) {
    mAudioInterface = dynamic_cast<AudioInterfaceForQt *>(
        optionsDialog->getCore()->getAudioInput());
    overlay = new ProgressOverlay(this, tr("Loading input devices"), true);
  } else {
    mAudioInterface = dynamic_cast<AudioInterfaceForQt *>(
        optionsDialog->getCore()->getAudioPlayer());
    overlay = new ProgressOverlay(this, tr("Loading output devices"), true);
  }
  QObject::connect(this, SIGNAL(updateProgress(int)), overlay,
                   SLOT(updatePercentage(int)));

  QGridLayout *inputLayout = new QGridLayout;
  this->setLayout(inputLayout);

  inputLayout->setColumnStretch(1, 1);

  mDeviceSelection = new QComboBox;
  mDeviceSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
  // devices will be loaded in separate thread

  // select input device
  QPushButton *selectDefaultInputDeviceButton = new QPushButton(tr("Default"));
  inputLayout->addWidget(new QLabel(tr("Device")), 0, 0);
  inputLayout->addWidget(mDeviceSelection, 0, 1);
  inputLayout->addWidget(selectDefaultInputDeviceButton, 0, 2);

  // sampling rate
  QPushButton *selectDefaultSamplingRateButton = new QPushButton(tr("Default"));
  inputLayout->addWidget(new QLabel(tr("Sampling rate ")), 1, 0);
  inputLayout->addWidget(mSamplingRates = new QComboBox, 1, 1);
  inputLayout->addWidget(selectDefaultSamplingRateButton, 1, 2);

  // add open sound settings button
  if (mOptionsDialog->getMainWindow()->isSoundControlSupported()) {
    QPushButton *openSystemSettingsButton =
        new QPushButton(tr("Open system settings"));
    inputLayout->addWidget(openSystemSettingsButton, 10, 0);
    QObject::connect(openSystemSettingsButton, SIGNAL(clicked()),
                     mOptionsDialog->getMainWindow(),
                     SLOT(onOpenSoundControl()));
  }

  // add stretch
  inputLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding,
                                       QSizePolicy::MinimumExpanding),
                       200, 0);

  // special widgets for output
  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    // Channels
    inputLayout->addWidget(new QLabel(tr("Channels")), 4, 0);

    mChannelsSelect = new QComboBox();
    mChannelsSelect->setSizePolicy(QSizePolicy::MinimumExpanding,
                                   QSizePolicy::Preferred);

    QPushButton *defaultChannels = new QPushButton(tr("Default"));
    QObject::connect(defaultChannels, SIGNAL(clicked(bool)), this,
                     SLOT(onDefaultChannel()));

    inputLayout->addWidget(mChannelsSelect, 4, 1);
    inputLayout->addWidget(defaultChannels, 4, 2);

    // Buffer size
    inputLayout->addWidget(new QLabel(tr("Buffer size")), 5, 0);

    mBufferSizeEdit = new QSpinBox();
    mBufferSizeEdit->setRange(10, 5000);
    mBufferSizeEdit->setSingleStep(10);
    mBufferSizeEdit->setSuffix("ms");
    mBufferSizeEdit->setSizePolicy(QSizePolicy::MinimumExpanding,
                                   QSizePolicy::Preferred);
    mBufferSizeEdit->setValue(mAudioInterface->getBufferSizeMS());

    QPushButton *defaultBufferSize = new QPushButton(tr("Default"));
    QObject::connect(defaultBufferSize, SIGNAL(clicked(bool)), this,
                     SLOT(onDefaultBufferSize()));

    inputLayout->addWidget(mBufferSizeEdit, 5, 1);
    inputLayout->addWidget(defaultBufferSize, 5, 2);
  }

  // set current values
  addDevice(mAudioInterface->getDeviceInfo());
  mDeviceSelection->setCurrentText(
      mAudioInterface->getDeviceInfo().description());
  onDeviceSelectionChanged(mDeviceSelection->currentIndex());
  mSamplingRates->setCurrentText(
      QString("%1").arg(mAudioInterface->getSamplingRate()));

  // connect widgets
  QObject::connect(mDeviceSelection, SIGNAL(currentIndexChanged(int)), this,
                   SLOT(onDeviceSelectionChanged(int)));

  QObject::connect(selectDefaultInputDeviceButton, SIGNAL(clicked()), this,
                   SLOT(onDefaultDevice()));
  QObject::connect(selectDefaultSamplingRateButton, SIGNAL(clicked()), this,
                   SLOT(onDefaultSamplingRate()));

  // notify if changes are made
  QObject::connect(mDeviceSelection, SIGNAL(currentIndexChanged(int)),
                   optionsDialog, SLOT(onChangesMade()));
  QObject::connect(mSamplingRates, SIGNAL(currentIndexChanged(int)),
                   optionsDialog, SLOT(onChangesMade()));

  // special audio output
  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    mChannelsSelect->setCurrentText(
        QString::number(mAudioInterface->getChannelCount()));
    // notify if changes are made
    QObject::connect(mChannelsSelect, SIGNAL(currentIndexChanged(int)),
                     optionsDialog, SLOT(onChangesMade()));
    QObject::connect(mBufferSizeEdit, SIGNAL(valueChanged(int)), optionsDialog,
                     SLOT(onChangesMade()));
  }

  // start thread to load devices
  DeviceLoaderThread *t = new DeviceLoaderThread(this, mode);
  QObject::connect(t, SIGNAL(updateProgress(int)), overlay,
                   SLOT(updatePercentage(int)));
  QObject::connect(t, SIGNAL(deviceReady(QAudioDevice)), this,
                   SLOT(addDevice(QAudioDevice)));
  t->start();
  mDeviceLoader = t;
}

PageAudioInputOutput::~PageAudioInputOutput() {
  mDeviceLoader->requestInterruption();
  mDeviceLoader->wait();
}

void PageAudioInputOutput::apply() {
  assert(mSamplingRates->currentIndex() != -1);
  assert(mSamplingRates->currentText().isEmpty() == false);

  // PCMDevice *writerInterfaceBackup = nullptr;
  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    // writerInterfaceBackup =
    // dynamic_cast<AudioPlayerAdapter*>(mAudioBase)->getWriter();
  }

  QAudioDevice info(mDeviceSelection->currentData().value<QAudioDevice>());
  int bufferSizeMS = -1;
  int channels = 1;
  const int samplingRate = mSamplingRates->currentText().toInt();
  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    bufferSizeMS = mBufferSizeEdit->value();
    channels = mChannelsSelect->currentData().toInt();
  } else {
  }
  mAudioInterface->reinitialize(samplingRate, channels, info, bufferSizeMS);
  mAudioInterface->start();

  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    // dynamic_cast<AudioPlayerAdapter*>(mAudioBase)->setWriter(writerInterfaceBackup);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)); // give waveform generator time
    if (mOptionsDialog->getCore()->getSoundGenerator()) {
      mOptionsDialog->getCore()->getSoundGenerator()->init();
    }
  }
}

void PageAudioInputOutput::onDeviceSelectionChanged(int row) {
  QAudioDevice info(mDeviceSelection->itemData(row).value<QAudioDevice>());

  mSamplingRates->clear();

  QList<int> standardRates = {8000,  11025, 22050, 44100,
                              48000, 88200, 96000, 192000};
  for (int rate : standardRates) {
    if (rate < 9000)
      continue; // Skip too low rates

    // In Qt 6, we check if the format is supported.
    // We assume Int16 and the current channel count (or 1/2).
    QAudioFormat format;
    format.setSampleRate(rate);
    format.setChannelCount(mMode == AudioInterfaceForQt::AudioMode::Input ? 1
                                                                          : 2);
    format.setSampleFormat(QAudioFormat::Int16);

    if (info.isFormatSupported(format)) {
      mSamplingRates->addItem(QString("%1").arg(rate));
    }
  }

  // Add preferred rate if not present
  int prefRate = info.preferredFormat().sampleRate();
  if (prefRate >= 9000 &&
      mSamplingRates->findText(QString::number(prefRate)) == -1) {
    mSamplingRates->addItem(QString::number(prefRate));
  }

  if (mMode == AudioInterfaceForQt::AudioMode::Output) {
    mChannelsSelect->clear();
    // Check 1 and 2 channels
    QAudioFormat format;
    format.setSampleRate(44100); // Check against standard rate
    format.setSampleFormat(QAudioFormat::Int16);

    format.setChannelCount(1);
    if (info.isFormatSupported(format))
      mChannelsSelect->addItem("1", 1);

    format.setChannelCount(2);
    if (info.isFormatSupported(format))
      mChannelsSelect->addItem("2", 2);

    mChannelsSelect->setCurrentIndex(mChannelsSelect->count() - 1);
  }

  onDefaultSamplingRate();
}

void PageAudioInputOutput::onDefaultDevice() {
  if (mMode == AudioInterfaceForQt::AudioMode::Input) {
    mDeviceSelection->setCurrentText(
        QMediaDevices::defaultAudioInput().description());
  } else {
    mDeviceSelection->setCurrentText(
        QMediaDevices::defaultAudioOutput().description());
  }
  onDefaultSamplingRate();
}

void PageAudioInputOutput::onDefaultSamplingRate() {
  QAudioDevice info(mDeviceSelection->currentData().value<QAudioDevice>());
  if (info.isNull()) {
    // no device found
    mSamplingRates->setCurrentText(QString());
    return;
  }

  // Use preferred format
  int prefRate = info.preferredFormat().sampleRate();

  // logic from old code: prefer 22050/44100 if available
  // We can iterate the combo box to see if they are there

  if (mMode == AudioInterfaceForQt::AudioMode::Output &&
      mSamplingRates->findText("22050") != -1) {
    mSamplingRates->setCurrentText("22050");
  } else if (mMode == AudioInterfaceForQt::AudioMode::Input &&
             mSamplingRates->findText("44100") != -1) {
    mSamplingRates->setCurrentText("44100");
  } else {
    mSamplingRates->setCurrentText(QString::number(prefRate));
    if (prefRate < 9000) {
      QMessageBox::warning(this, tr("Warning"),
                           tr("You need at least a sampling rate of %1 to "
                              "record and play all keys.")
                               .arg(11025));
    }
  }
}

void PageAudioInputOutput::addDevice(QAudioDevice info) {
  if (mDeviceSelection->findText(info.description()) == -1) {
    mDeviceSelection->addItem(info.description(),
                              QVariant::fromValue<QAudioDevice>(info));
  }
}

void PageAudioInputOutput::onDefaultChannel() {
  mChannelsSelect->setCurrentText(QString::number(2));
}

void PageAudioInputOutput::onDefaultBufferSize() {
  mBufferSizeEdit->setValue(AudioInterfaceForQt::DEFAULT_BUFFER_SIZE_MS);
}

DeviceLoaderThread::DeviceLoaderThread(QObject *parent,
                                       AudioInterfaceForQt::AudioMode mode)
    : QThread(parent), mMode(mode) {}

void DeviceLoaderThread::run() {
  QList<QAudioDevice> deviceInfos;
  if (mMode == AudioInterfaceForQt::AudioMode::Input) {
    deviceInfos = QMediaDevices::audioInputs();
  } else {
    deviceInfos = QMediaDevices::audioOutputs();
  }

  int progress = 0;
  for (const auto &info : deviceInfos) {
    if (isInterruptionRequested()) {
      break;
    }

    // In Qt 6, we assume devices listed are valid/supported to some extent.
    // The old code checked if preferred format is supported.
    emit updateProgress(progress);
    if (deviceInfos.size() > 0)
      progress += 100 / deviceInfos.size();

    LogI("%s is supported.", info.description().toStdString().c_str());

    emit deviceReady(info);
  }

  emit updateProgress(100);
}

} // namespace options
