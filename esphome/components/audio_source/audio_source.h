#pragma once

#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/entity_base.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace audio_source {

  #define AUDIO_STREAM_BUFFER_SIZE 9000

 class  AudioStream{
  public:
    uint32_t audio_frequency;
    uint8_t bits_per_sample;
    uint8_t channels;
    uint8_t audio_buffer[AUDIO_STREAM_BUFFER_SIZE];
    size_t audio_buffer_size;
    
    int32_t get_sample(int index, int channel);
    int32_t get_sample_count();

};

class AudioSource : public EntityBase {
 public:
  explicit AudioSource();


  AudioStream stream;
  
  
  /** Publish a new state to the front-end.
   *
   * First, the new state will be assigned to the raw_value. Then it's passed through all filters
   * until it finally lands in the .value member variable and a callback is issued.
   *
   */
  void publish_audio();

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Add a callback that will be called every time a filtered value arrives.
  void add_audio_callback(std::function<void(AudioStream *)> &&callback);
  


  /** A unique ID for this sensor, empty for no unique id. See unique ID requirements:
   * https://developers.home-assistant.io/docs/en/entity_registry_index.html#unique-id-requirements
   *
   * @return The unique id as a string.
   */
  virtual std::string unique_id();


 protected:
 
  CallbackManager<void(AudioStream *)> callback_;      ///< Storage for audio buffer callbacks.
                        
};

}  // namespace audio_source
}  // namespace esphome
