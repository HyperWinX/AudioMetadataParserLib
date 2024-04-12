#include <stdint.h>
#include <mp3.h>

typedef uint32_t _mp3_header;

struct _mp3_audio_tag{
    unsigned char tag[3];
    unsigned char title[30];
    unsigned char artist[30];
    unsigned char album[30];
    unsigned char year[4];
    unsigned char comment[30];
    unsigned char genre;
};
int read_mp3_audio_tag(mp3_audio_tag* tag, char* buffer, uint32_t size){
    _mp3_audio_tag _tag;

    if (size <= 128) return 1;

    memcpy(&_tag, buffer + size - sizeof(_mp3_audio_tag), sizeof(_mp3_audio_tag));

    if (_tag.tag[0] != 'T' || _tag.tag[1] != 'A' || _tag.tag[2] != 'G') return 1;

    memcpy(tag->title, _tag.title, sizeof(_tag.title));
    memcpy(tag->artist, _tag.artist, sizeof(_tag.artist));
    memcpy(tag->album, _tag.album, sizeof(_tag.album));
    memcpy(tag->year, _tag.year, sizeof(_tag.year));
    memcpy(tag->comment, _tag.comment, sizeof(_tag.comment));

    return 0;
}