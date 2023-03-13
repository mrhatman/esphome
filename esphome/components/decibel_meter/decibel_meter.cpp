#include "decibel_meter.h"
#include "esphome/core/log.h"
#include "esphome/components/audio_source/audio_source.h"

namespace esphome {
namespace decibel_meter {

static const char *const TAG = "decibel_meter";

void DecibelMeterSensor::setup() {
}

void DecibelMeterSensor::set_audio_source(audio_source::AudioSource *source){
  this->source_ = source;

  this->source_->add_audio_callback([this](esphome::audio_source::AudioStream * stream) {
	
	//Convert to 16 bits
	//TODO fixme for not 16 nit streams

	int BYTES_PER_SAMPLE = 2;
	
	//Collect max 
	
	int32_t max = 0;
	int32_t max_sample = (1<< (stream->bits_per_sample -1))-1;
	int32_t sample_count = stream->get_sample_count();
	
	for(int q =0; q< sample_count; q++){
		for(int w =0; w < stream->channels; w++){

			if(stream->get_sample(q,w) > max){
				max = stream->get_sample(q,w);
			}
		}

	}
	

	
	float decibels = (20.0* log10( (float)max / (float)max_sample)) + this->sensitivity_ + 95.0 ;
	
	this->publish_state((float)decibels);
	
  });
}

void DecibelMeterSensor::dump_config() {
  LOG_SENSOR("", "Decibel Meter", this);
  ESP_LOGCONFIG(TAG, "  Sensitivity %.11f", this->sensitivity_);
}

}  // namespace decibel_meter
}  // namespace esphome
