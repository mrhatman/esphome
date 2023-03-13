#pragma once


#include "esphome/components/audio_source/audio_source.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/helpers.h"



namespace esphome {
namespace sound_file {

class SoundFile : public Component, public audio_source::AudioSource {
 public:
  SoundFile(const uint8_t * buffer ,size_t size, uint32_t frequency,uint8_t bits, uint8_t channels) {
    this->stream.audio_frequency = frequency;
    this->stream.bits_per_sample = bits;
    this->stream.channels = channels;
    audio_buffer_ = buffer;
    audio_buffer_size_ = size;
  }
  void setup() override;
  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

  void loop() override;

  void dump_config() override;

 protected:

  const uint8_t * audio_buffer_;
  size_t audio_buffer_size_;

  uint32_t next_time_;
  uint32_t full_buffer_time_;
  uint32_t position_;

enum SoundFileCommand : uint8_t {
  SOUND_FILE_COMMAND_PLAY = 0,
  SOUND_FILE_COMMAND_PAUSE = 1,
  SOUND_FILE_COMMAND_STOP = 2,
};

};



}  // namespace i2s_audio_source
}  // namespace esphome

