#include "sound_file.h"

#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"


namespace esphome {
namespace sound_file {

static const char *const TAG = "sound_file";

void SoundFile::setup() {


   full_buffer_time_ = (uint32_t)(((float)AUDIO_STREAM_BUFFER_SIZE / (float)stream.audio_frequency  )*1000000.0) / ( (stream.bits_per_sample /8) * stream.channels);

  next_time_ = micros() ;
  position_ = 0;

  return;
}

void SoundFile::loop() {

  while( micros() + full_buffer_time_ > next_time_)
  {
    ESP_LOGV(TAG,"%d + %d > %d",  micros() , full_buffer_time_ , next_time_);

    if(position_ + AUDIO_STREAM_BUFFER_SIZE > audio_buffer_size_){
      position_ =0;
    }

    memcpy(stream.audio_buffer , &(audio_buffer_[position_]),  AUDIO_STREAM_BUFFER_SIZE);

    stream.audio_buffer_size = AUDIO_STREAM_BUFFER_SIZE;

    position_ += AUDIO_STREAM_BUFFER_SIZE;

    next_time_ += full_buffer_time_;
    publish_audio();
  }
}

void SoundFile::dump_config() {
  ESP_LOGCONFIG(TAG, "Sound Generator:");
  ESP_LOGCONFIG(TAG, "  Audio Frequency: %d", this->stream.audio_frequency);
  ESP_LOGCONFIG(TAG, "  Bits Per Sample: %d", this->stream.bits_per_sample);
  ESP_LOGCONFIG(TAG, "  Channels: %d", this->stream.channels);

}



}  // namespace sound_generator
}  // namespace esphome

