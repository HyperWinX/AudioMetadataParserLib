#include <stdint.h>
#include <string.h>
#include <stdio.h>
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
}__attribute__((packed));

__attribute__((visibility("hidden"))) uint16_t _calculate_bitrate_v1_l1(uint8_t bits){
  switch(bits){
    case 1: return 32;
    case 2: return 64;
    case 3: return 96;
    case 4: return 128;
    case 5: return 160;
    case 6: return 192;
    case 7: return 224;
    case 8: return 256;
    case 9: return 288;
    case 10: return 320;
    case 11: return 352;
    case 12: return 384;
    case 13: return 416;
    case 14: return 448;
    default: return 0xFFFF;
  }
}

__attribute__((visibility("hidden"))) uint16_t _calculate_bitrate_v1_l2(uint8_t bits){
  switch(bits){
    case 1: return 32;
    case 2: return 48;
    case 3: return 56;
    case 4: return 64;
    case 5: return 80;
    case 6: return 96;
    case 7: return 112;
    case 8: return 128;
    case 9: return 160;
    case 10: return 192;
    case 11: return 224;
    case 12: return 256;
    case 13: return 320;
    case 14: return 384;
    default: return 0xFFFF;
  }
}

__attribute__((visibility("hidden"))) uint16_t _calculate_bitrate_v1_l3(uint8_t bits){
  switch(bits){
    case 1: return 32;
    case 2: return 40;
    case 3: return 48;
    case 4: return 56;
    case 5: return 64;
    case 6: return 80;
    case 7: return 96;
    case 8: return 112;
    case 9: return 128;
    case 10: return 160;
    case 11: return 192;
    case 12: return 224;
    case 13: return 256;
    case 14: return 320;
    default: return 0xFFFF;
  }
}

__attribute__((visibility("hidden"))) uint16_t _calculate_bitrate_v2_l1(uint8_t bits){
  switch(bits){
    case 1: return 32;
    case 2: return 48;
    case 3: return 56;
    case 4: return 64;
    case 5: return 80;
    case 6: return 96;
    case 7: return 112;
    case 8: return 128;
    case 9: return 144;
    case 10: return 160;
    case 11: return 176;
    case 12: return 192;
    case 13: return 224;
    case 14: return 256;
    default: return 0xFFFF;
  }
}

__attribute__((visibility("hidden"))) uint16_t _calculate_bitrate_v2_l23(uint8_t bits){
  switch(bits){
    case 1: return 8;
    case 2: return 16;
    case 3: return 24;
    case 4: return 32;
    case 5: return 40;
    case 6: return 48;
    case 7: return 56;
    case 8: return 64;
    case 9: return 80;
    case 10: return 96;
    case 11: return 112;
    case 12: return 128;
    case 13: return 133;
    case 14: return 160;
    default: return 0xFFFF;
  }
}

__attribute__((visibility("hidden"))) enum bitrt _get_bitrate(mp3_frame_header* frame_hdr, uint8_t bits){
  if (frame_hdr->mpeg_version == MPEG1){
    if (frame_hdr->layer_type == LAYER_I)
      return _calculate_bitrate_v1_l1(bits);
    else if (frame_hdr->layer_type == LAYER_II)
      return _calculate_bitrate_v1_l2(bits);
    else if (frame_hdr->layer_type == LAYER_III)
      return _calculate_bitrate_v1_l3(bits);
  }
  else{
    if (frame_hdr->layer_type == LAYER_I)
      return _calculate_bitrate_v2_l1(bits);
    else
      return _calculate_bitrate_v2_l23(bits);
  }
}

__attribute__((visibility("hidden"))) uint16_t _get_sampling_rate(mp3_frame_header* frame_hdr, uint8_t bits){
  switch(frame_hdr->mpeg_version){
    case MPEG1:
      if (bits == 0) return 44100;
      else if (bits == 1) return 48000;
      else if (bits == 2) return 32000;
      else return 0xFFFF;
    case MPEG2:
      if (bits == 0) return 22050;
      else if (bits == 1) return 24000;
      else if (bits == 2) return 16000;
      else return 0xFFFF;
    case MPEG2_5:
      if (bits == 0) return 11025;
      else if (bits == 1) return 12000;
      else if (bits == 2) return 8000;
      else return 0xFFFF;
  }
}

int read_mp3_audio_tag(mp3_audio_tag* tag, unsigned char* buffer, uint32_t size){
    struct _mp3_audio_tag _tag;

    if (size <= 128) return 1;

    memcpy(&_tag, buffer + size - sizeof(struct _mp3_audio_tag), sizeof(struct _mp3_audio_tag));
		printf("Read at offset %lx\n", size - sizeof(struct _mp3_audio_tag));
    if (_tag.tag[0] != 'T' || _tag.tag[1] != 'A' || _tag.tag[2] != 'G'){
		printf("Tag: %x%c%c\n", *(buffer + size - sizeof(struct _mp3_audio_tag)), _tag.tag[1], _tag.tag[2]);
		return 1;
	}

    memcpy(tag->title, _tag.title, sizeof(_tag.title));
    memcpy(tag->artist, _tag.artist, sizeof(_tag.artist));
    memcpy(tag->album, _tag.album, sizeof(_tag.album));
    memcpy(tag->year, _tag.year, sizeof(_tag.year));
    memcpy(tag->comment, _tag.comment, sizeof(_tag.comment));

    return 0;
}

int read_first_frameheader(mp3_frame_header* frame_hdr, unsigned char* buffer, uint32_t size){
  uint32_t _hdr = *(uint32_t*)buffer;

  // Set MPEG version
  if ((frame_hdr->mpeg_version = (enum mpeg_ver)(((unsigned char*)_hdr)[1] & 0b00011000) >> 3) == mv_RESERVED){
    frame_hdr->mpeg_version = 0;
    return 1;
  }

  // Set layer type
  if ((frame->layer_type = (enum layer_tp)(((unsigned char*)_hdr)[1] & 0b00000110) >> 1) == ltp_RESERVED){
    frame->layer_type = 0;
    return 1;
  }

  // Set protection bit
  frame->protection_enabled = ((unsigned char*)_hdr)[1] & 0b00000001;

  // Set bitrate
  frame_hdr->bitrate = _get_bitrate(frame_hdr, ((unsigned char*)_hdr)[2] >> 4);

  // Set sampling rate
  frame_hdr->sampling_rate = _get_sampling_rate(frame_hdr, (((unsigned char*)_hdr)[2] & 0b00001100) >> 2);

  // Set padding bit
  frame_hdr->padding_enabled = ((unsigned char*)_hdr)[2] & 0b00000001;
  
  // Set channel mode
  frame_hdr->channel_mode = (enum chnl_mode)((((unsigned char*)_hdr)[3] & 0b11000000) >> 6);

  
}