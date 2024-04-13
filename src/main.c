#include <mp3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int read_mp3_audio_tag(mp3_audio_tag* tag, unsigned char* buffer, uint32_t size);
int read_first_frameheader(mp3_frame_header* frame_hdr, unsigned char* buffer, uint32_t size);
extern char* genres[];

int main(){
		puts("Opening file...");
		FILE* file = fopen("test.mp3", "rb");
		fseek(file, 0, SEEK_END);
		uint64_t size = ftell(file);
		fseek(file, 0, SEEK_SET);
		printf("File size: %lx\n", size);
		char* data = calloc(size, sizeof(char));
		fread(data, 1, size, file);
		printf("Trying to read tag at offset %lx\n", size - 128);
		mp3_audio_tag tag = {0};
		if (read_mp3_audio_tag(&tag, data, (uint32_t)size)){
				perror("Read failure");
				exit(1);
		}
		printf("Title: %s\n", tag.title);
		printf("Artist: %s\n", tag.artist);
		printf("Album: %s\n", tag.album);
		printf("Year: %s\n", tag.year);
		if (tag.genre > 125) puts("Genre: Unknown");
		else printf("Genre: %s\n", genres[tag.genre]);
		puts("\nTrying to read first frame...");
		mp3_frame_header hdr = {0};
		if (read_first_frameheader(&hdr, data, (uint32_t)size)){
				perror("First frame header parse error");
				exit(1);
		}
		switch(hdr.mpeg_version){
				case MPEG1: puts("MPEG version: MPEG1");
				case MPEG2: puts("MPEG version: MPEG2");
				case MPEG2_5: puts("MPEG version: MPEG2.5");
				default: return 1;
		}
		fclose(file);
		free(data);
}
