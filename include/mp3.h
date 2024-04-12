enum {
    MPEG1=0,
    MPEG2=1,
    mv_RESERVED=2,
    MPEG2_5=3
} mpeg_ver;

enum {
    ltp_RESERVED=0,
    LAYER_I=1,
    LAYER_II=2,
    LAYER_III=3
} layer_tp;

enum {
    STEREO=0,
    JOINT_STEREO=1,
    DUAL_CHANNEL=2,
    SINGLE_CHANNEL=3
} chnl_mode;

enum {
    NONE=0,
    FIFTY_FIFTEEN=2,
    CCIT=3
} emphs;

typedef struct mp3_audio_tag{
    unsigned char title[31];
    unsigned char artist[31];
    unsigned char album[31];
    unsigned char year[5];
    unsigned char comment[31];
    unsigned char genre;
} mp3_audio_tag;

typedef struct _mp3_frame_header{
    enum mpeg_ver mpeg_version;
    enum layer_tp layer_type;
    int protection_enabled;
    uint16_t bitrate;
    uint16_t sampling_rate;
    int padding_enabled;
    enum chnl_mode channel_mode;
    unsigned char mode_extension;
    unsigned char copyright;
    unsigned char original;
    enum emphs emphasis;
} mp3_frame_header;