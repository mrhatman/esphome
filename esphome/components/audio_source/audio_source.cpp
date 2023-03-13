#include "audio_source.h"
#include "esphome/core/log.h"

namespace esphome {
namespace audio_source {

static const char *const TAG = "audio_source";

AudioSource::AudioSource() {}


void AudioSource::publish_audio() {
  ESP_LOGV(TAG, "'%s': Published new audio data %d", this->name_.c_str(), stream.audio_buffer_size);
	
  this->callback_.call(&stream);
}

void AudioSource::add_audio_callback(std::function<void(AudioStream * )> &&callback) { this->callback_.add(std::move(callback)); }

std::string AudioSource::unique_id() { return ""; }

int AudioStream::get_sample_count() {
  return audio_buffer_size / (channels * (bits_per_sample /8));
}

int32_t AudioStream::get_sample(int index, int channel){

  int32_t sample = 0;

  switch (bits_per_sample)
  {
  case 8:
    sample = (int32_t)((int8_t *)audio_buffer)[(channels * index) + channel];
    break;
  case 16:
    sample = (int32_t)((int16_t *)audio_buffer)[(channels * index) + channel];
    break;
  case 24:
    sample |= (int32_t)((int8_t *)audio_buffer)[(channels * 3 * index) + (3*channel)];
    sample |= (int32_t)((int8_t *)audio_buffer)[(channels * 3 * index) + (3*channel) +1] << 8;
    sample |= (int32_t)((int8_t *)audio_buffer)[(channels * 3 * index) + (3*channel) +2] << 16;
    break;
  case 32:
    sample = (int32_t)((int32_t *)audio_buffer)[(channels * index) + channel];
    break;
  
  default:
    break;
  } 

  return sample;
}

}  // namespace audio_source
}  // namespace esphome
