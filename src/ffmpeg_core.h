#ifndef _FFMPEG_CORE_H_
#define _FFMPEG_CORE_H_

class FFmpegCore
{
public:
    FFmpegCore();
    ~FFmpegCore();

public:
    int ffmpeg_core_init();
    int ffmpeg_core_input_stream();
    int ffmpeg_core_output_stream();

private:
    int input_stream_default();

private:
    AVDictionary* dict;
};

#endif // _FFMPEG_CORE_H_