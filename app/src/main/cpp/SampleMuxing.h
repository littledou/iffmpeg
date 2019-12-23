//
// Created by loki on 2019-12-23.
//

#ifndef IFFMPEG_SAMPLEMUXING_H
#define IFFMPEG_SAMPLEMUXING_H
#include "log.h"



#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC

extern "C" {
#include <libavcodec/avcodec.h>

#include <include/libavformat/avformat.h>
#include <include/libavutil/opt.h>
#include <include/libswresample/swresample.h>
#include <include/libswscale/swscale.h>
#include <include/libavutil/avassert.h>
#include <include/libavutil/timestamp.h>
}

typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;




class SampleMuxing {

public:
    int sampleMuxing(const char *path);

private:
    void add_stream(OutputStream *ost, AVFormatContext *oc,
                    AVCodec **codec,
                    enum AVCodecID codec_id);

    void open_audio(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);

    void open_video(AVFormatContext *oc, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg);

    AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                               uint64_t channel_layout,
                               int sample_rate, int nb_samples);

    AVFrame *alloc_picture(AVPixelFormat pix_fmt, int width, int height);

    void close_stream(AVFormatContext *oc, OutputStream *ost);

    AVFrame *get_video_frame(OutputStream *ost);

    int write_video_frame(AVFormatContext *oc, OutputStream *ost);

    void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height);

    int write_audio_frame(AVFormatContext *oc, OutputStream *ost);

    AVFrame *get_audio_frame(OutputStream *ost);

    int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt);

    void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);
};


#endif //IFFMPEG_SAMPLEMUXING_H
