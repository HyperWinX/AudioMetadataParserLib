struct mp3_audio_tag{
    unsigned char title[31];
    unsigned char artist[31];
    unsigned char album[31];
    unsigned char year[5];
    unsigned char comment[31];
    unsigned char genre;
};