import esphome.codegen as cg
from esphome.components import audio_source
import esphome.config_validation as cv

from esphome import pins

from esphome.const import CONF_ID, CONF_MODE
from esphome.core import CORE

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["esp32"]

i2s_audio_sink_ns = cg.esphome_ns.namespace("i2s_audio_sink")

I2SAudioSink = i2s_audio_sink_ns.class_("I2SAudioSink", cg.Component)

CONF_AUDIO_SOURCE = "audio_source"
CONF_I2S_DOUT_PIN = "i2s_dout_pin"
CONF_I2S_BCLK_PIN = "i2s_bclk_pin"
CONF_I2S_LRCLK_PIN = "i2s_lrclk_pin"

CONFIG_SCHEMA = cv.ENTITY_BASE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(I2SAudioSink),
        cv.Required(CONF_I2S_DOUT_PIN): pins.internal_gpio_input_pin_number,
        cv.Required(CONF_I2S_BCLK_PIN): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_I2S_LRCLK_PIN): pins.internal_gpio_output_pin_number,
        cv.Required(CONF_AUDIO_SOURCE): cv.use_id(audio_source.AudioSource),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_dout_pin(config[CONF_I2S_DOUT_PIN]))
    cg.add(var.set_bclk_pin(config[CONF_I2S_BCLK_PIN]))
    cg.add(var.set_lrclk_pin(config[CONF_I2S_LRCLK_PIN]))

    source = await cg.get_variable(config[CONF_AUDIO_SOURCE])
    cg.add(var.set_audio_source(source))
