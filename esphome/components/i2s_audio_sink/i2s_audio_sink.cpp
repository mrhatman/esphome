#include "i2s_audio_sink.h"

#ifdef USE_ESP32_FRAMEWORK_ARDUINO

#include "esphome/core/log.h"
#include <driver/i2s.h>


namespace esphome {
namespace i2s_audio_sink {

static const char *const TAG = "i2s_audio_sink";



void I2SAudioSink::setup() {

}


void I2SAudioSink::set_audio_source(audio_source::AudioSource *source){
    ESP_LOGCONFIG(TAG, "Setting up Audio...");
  esp_err_t err = ESP_OK;

  i2s_config_t i2s_config = {
	.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
	.sample_rate = source->stream.audio_frequency,
	.bits_per_sample = (i2s_bits_per_sample_t)source->stream.bits_per_sample, // is fixed at 12bit, stereo, MSB
	.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
	.communication_format = I2S_COMM_FORMAT_STAND_I2S,
	.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
	.dma_buf_count = 10,
	.dma_buf_len = 1024,
  .use_apll = false,
  .tx_desc_auto_clear = true,
  };


  //Mono sound
  if(  source->stream.channels == 1){
    i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
  }

  ESP_LOGV(TAG, "'%s': Installing driver", this->name_.c_str());
  err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  
  if( err != ESP_OK){
	   ESP_LOGE(TAG, "'%s': Received audio setup error: %d", this->name_.c_str(), err);
  }
  i2s_pin_config_t tx_pin_config;

  tx_pin_config.mck_io_num = 0;
  tx_pin_config.bck_io_num = this->bclk_pin_;
  tx_pin_config.ws_io_num = this->lrclk_pin_;
  tx_pin_config.data_out_num = this->dout_pin_;
  tx_pin_config.data_in_num = 0;


  
  ESP_LOGV(TAG, "'%s':setting clocks", this->name_.c_str());
  err = i2s_set_clk(I2S_NUM_0, source->stream.audio_frequency, (i2s_bits_per_sample_t)source->stream.bits_per_sample, (i2s_channel_t)source->stream.channels);
  //err = i2s_set_clk(I2S_NUM_0, source->stream.audio_frequency, (i2s_bits_per_sample_t)source->stream.bits_per_sample,I2S_CHANNEL_MONO);

  if( err != ESP_OK){
	   ESP_LOGE(TAG, "'%s': Received audio setup error: %d", this->name_.c_str(), err);
  }
  
  ESP_LOGV(TAG, "'%s': setting pins", this->name_.c_str());
  err = i2s_set_pin(I2S_NUM_0, &tx_pin_config);
  if( err != ESP_OK){
	   ESP_LOGE(TAG, "'%s': Received audio setup error: %d", this->name_.c_str(), err);
  }


  i2s_zero_dma_buffer(I2S_NUM_0);

  this->source_ = source;

  this->source_->add_audio_callback([this](esphome::audio_source::AudioStream * stream) {
    ESP_LOGD(TAG,"%d %d",((int16_t * )stream->audio_buffer)[0],((int16_t * )stream->audio_buffer)[stream->audio_buffer_size-1]);
    uint32_t byte_read;
    i2s_write(I2S_NUM_0, stream->audio_buffer, stream->audio_buffer_size, &byte_read, 0);

    ESP_LOGD(TAG,"%d %d",byte_read,stream->audio_buffer_size);
	
  });
  

}

void I2SAudioSink::dump_config() {
  ESP_LOGCONFIG(TAG, "I2S Audio Sink");
  ESP_LOGCONFIG(TAG, "  LRClk Pin: %d", this->lrclk_pin_);
  ESP_LOGCONFIG(TAG, "  BClk Pin: %d", this->bclk_pin_);
  ESP_LOGCONFIG(TAG, "  Dout Pin: %d", this->dout_pin_);

}

}  // namespace i2s_audio_sink
}  // namespace esphome

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
