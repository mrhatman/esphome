import esphome.codegen as cg
from esphome.components import audio_source
import esphome.config_validation as cv

from esphome import pins

from esphome.const import CONF_ID, CONF_MODE
from esphome.core import CORE, HexInt

from esphome import automation
from esphome.automation import maybe_simple_id

from esphome.const import (
    CONF_DITHER,
    CONF_FILE,
    CONF_ID,
    CONF_RAW_DATA_ID,
    CONF_RESIZE,
    CONF_TYPE,
)

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["esp32"]

sound_file_ns = cg.esphome_ns.namespace("sound_file")

SoundFile = sound_file_ns.class_("SoundFile", cg.Component, audio_source.AudioSource)

SOUND_FILE_ACTION_SCHEMA = maybe_simple_id({cv.GenerateID(): cv.use_id(SoundFile)})

PlayAction = sound_file_ns.class_(
    "PlayAction", automation.Action, cg.Parented.template(SoundFile)
)
CONF_BITS_PER_SAMPLE = "bits_per_sample"
CONF_AUDIO_FREQUENCY = "audio_frequency"
CHANNEL_OPTIONS = ["mono", "stereo"]

# @automation.register_action("media_player.play", PlayAction, SOUND_FILE_ACTION_SCHEMA)


CONFIG_SCHEMA = audio_source.AUDIO_SOURCE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SoundFile),
        cv.GenerateID(CONF_RAW_DATA_ID): cv.declare_id(cg.uint8),
        cv.Required(CONF_FILE): cv.file_,
    }
)


async def to_code(config):

    rhs = []
    SampleRate = 0
    BitsPerSample = 0
    NumChannels = 0

    path = CORE.relative_config_path(config[CONF_FILE])

    try:
        with open(path, "rb") as f:
            ChunkID = int.from_bytes(f.read(4), byteorder="little")
            print(f"Chunk ID = {ChunkID}")
            ChunkSize = int.from_bytes(f.read(4), byteorder="little")
            print(f"Chunk Size = {ChunkSize}")
            Format = int.from_bytes(f.read(4), byteorder="little")
            print(f"Format = {Format}")
            Subchunk1ID = int.from_bytes(f.read(4), byteorder="little")
            print(f"Subchunk1ID = {Subchunk1ID}")
            Subchunk1Size = int.from_bytes(f.read(4), byteorder="little")
            print(f"Subchunk1Size = {Subchunk1Size}")
            AudioFormat = int.from_bytes(f.read(2), byteorder="little")
            print(f"AudioFormat = {AudioFormat}")
            NumChannels = int.from_bytes(f.read(2), byteorder="little")
            print(f"NumChannels = {NumChannels}")
            SampleRate = int.from_bytes(f.read(4), byteorder="little")
            print(f"SampleRate = {SampleRate}")
            ByteRate = int.from_bytes(f.read(4), byteorder="little")
            print(f"ByteRate = {ByteRate}")
            BlockAlign = int.from_bytes(f.read(2), byteorder="little")
            print(f"BlockAlign = {BlockAlign}")
            BitsPerSample = int.from_bytes(f.read(2), byteorder="little")
            print(f"BitsPerSample = {BitsPerSample}")
            Subchunk2ID = int.from_bytes(f.read(4), byteorder="little")
            print(f"Subchunk2ID = {Subchunk2ID}")
            Subchunk2Size = int.from_bytes(f.read(4), byteorder="little")
            print(f"Subchunk2Size = {Subchunk2Size}")

            byte = f.read(1)
            while byte:
                # Do stuff with byte.
                rhs.append(HexInt(byte[0]))
                byte = f.read(1)
    except OSError:
        print("Error While Opening the file!")

    prog_arr = cg.progmem_array(config[CONF_RAW_DATA_ID], rhs)

    # (uint8_t * buffer , uint32_t frequency,uint8_t bits, uint8_t channels)
    var = cg.new_Pvariable(
        config[CONF_ID], prog_arr, len(rhs), SampleRate, BitsPerSample, NumChannels
    )
    await cg.register_component(var, config)
