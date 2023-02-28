#include "ffmpeg_stream.h"
#include "ffmpeg_client.h"
#include "ffmpeg_trans.h"
#include "ffmpeg_core.h"
#include "spdlog/spdlog.h"

static void auto_stream_thread(void *user) {
    int ret = FFMPEG_OK;
    int video_stream = -1, audio_stream = -1;
    FFmpegCore *core = nullptr;
    AVPacket   *pkg  = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    
    if (nullptr == user) {
        spdlog::error("param error, user={}", user);
        goto fail;
    }

    pkg = av_packet_alloc();
    if (!pkg) {
        spdlog::error("could not allocate packet, pkg={}", (void*)pkg);
        goto fail;
    }

    ctx = (ffmpeg_core_context*)user;
    if (nullptr == ctx || nullptr == ctx->ifmt_ctx) {
        spdlog::error("param error, ctx={}", (void*)ctx);
        goto fail;
    }

    ret = avformat_find_stream_info(ctx->ifmt_ctx, nullptr);
    if (ret < 0) {
        spdlog::error("can't find video stream in input stream, ret={}", ret);
        goto fail;
    }

    video_stream = av_find_best_stream(ctx->ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (video_stream < 0) {
        spdlog::error("can't find video stream in input stream");
        goto fail;
    }

    audio_stream = av_find_best_stream(ctx->ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (video_stream < 0) {
        spdlog::error("can't find video stream in input stream");
        goto fail;
    }

    while (true) {
        ret = av_read_frame(ctx->ifmt_ctx, pkg);
        spdlog::info("pkg size={}", pkg->size);
        av_packet_unref(pkg);
        if (ret < 0) {
            break;
        }
    }

fail:
    av_packet_free(&pkg);
    return;
}

static int auto_stream_before(void *user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    
    if (nullptr == user) {
        spdlog::error("param error, user={}", user);
        goto fail;
    }

    core = (FFmpegCore*)user;
    if (nullptr == core) {
        spdlog::error("param error, core={}", (void*)core);
        goto fail;
    }

    ctx = core->ffmpeg_core_get_context();
    if (nullptr == ctx) {
        spdlog::error("param error, ctx={}", (void*)ctx);
        goto fail;
    }

    ret = avformat_open_input(&ctx->ifmt_ctx, ctx->url.c_str(), nullptr, &ctx->dict);
    if (ret < 0) {
        spdlog::error("avformat_open_input failed, ret={}", ret);
        goto fail;
    }

    //av_dump_format(ctx->ifmt_ctx, 0, ctx->url.c_str(), 0);

fail:
    return ret;
}

static int auto_stream_start(void *user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    
    if (nullptr == user) {
        spdlog::error("param error, user={}", user);
        goto fail;
    }

    core = (FFmpegCore*)user;
    if (nullptr == core) {
        spdlog::error("param error, core={}", (void*)core);
        goto fail;
    }

    ctx = core->ffmpeg_core_get_context();
    if (nullptr == ctx || nullptr == ctx->ifmt_ctx) {
        spdlog::error("param error, ctx={}", (void*)ctx);
        goto fail;
    }

    if (ctx->ifmt_ctx->iformat)
        spdlog::info("video format={}", ctx->ifmt_ctx->iformat->name);

    if (ctx->ifmt_ctx->iformat
    &&  !strcmp(ctx->ifmt_ctx->iformat->name, "rtsp")) {
        av_read_play(ctx->ifmt_ctx);
    }

    ctx->read_thread = std::thread(auto_stream_thread, ctx);

fail:
    return ret;
}

static int auto_stream_stop(void *user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    
    if (nullptr == user) {
        spdlog::error("param error, user={}", user);
        goto fail;
    }

    core = (FFmpegCore*)user;
    if (nullptr == core) {
        spdlog::error("param error, core={}", (void*)core);
        goto fail;
    }

    ctx = core->ffmpeg_core_get_context();
    if (nullptr == ctx || nullptr == ctx->ifmt_ctx) {
        spdlog::error("param error, ctx={}", (void*)ctx);
        goto fail;
    }

    if (ctx->ifmt_ctx->iformat
    &&  !strcmp(ctx->ifmt_ctx->iformat->name, "rtsp")) {
        av_read_pause(ctx->ifmt_ctx);
    }
fail:
    return ret;
}


static void tran_stream_thread(void *user) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;
    AVPacket   *pkg = nullptr;
    ffmpeg_core_context *ctx = nullptr;
    
    if (nullptr == user) {
        spdlog::error("param error, user={}", user);
        goto fail;
    }

    pkg = av_packet_alloc();
    if (!pkg) {
        spdlog::error("could not allocate packet, pkg={}", (void*)pkg);
        goto fail;
    }

    ctx = (ffmpeg_core_context*)user;
    if (nullptr == ctx || nullptr == ctx->ifmt_ctx) {
        spdlog::error("param error, ctx={}", (void*)ctx);
        goto fail;
    }

    while (true) {
        ret = av_read_frame(ctx->ifmt_ctx, pkg);
        spdlog::info("pkg size={}", pkg->size);
        av_packet_unref(pkg);
        if (ret < 0) {
            break;
        }
    }

fail:
    av_packet_free(&pkg);
    return;
}

static int auto_stream_after(void *user) {
    int ret = FFMPEG_OK;
    return ret;
}

static int tran_stream_before(void *user) {
    int ret = FFMPEG_OK;
    return ret;
}

static int tran_stream_start(void *user) {
    int ret = FFMPEG_OK;
    return ret;
}

static int tran_stream_stop(void *user) {
    int ret = FFMPEG_OK;
    return ret;
}

static int tran_stream_after(void *user) {
    int ret = FFMPEG_OK;
    return ret;
}

const ffmpeg_stream ffmpeg_auto_stream = {
    .name = "auto",
    .ffmpeg_stream_before = auto_stream_before,
    .ffmpeg_stream_start  = auto_stream_start,
    .ffmpeg_stream_stop   = auto_stream_stop,
    .ffmpeg_stream_after  = auto_stream_after,
};

const ffmpeg_stream ffmpeg_tran_stream = {
    .name = "tran",
    .ffmpeg_stream_before = tran_stream_before,
    .ffmpeg_stream_start  = tran_stream_start,
    .ffmpeg_stream_stop   = tran_stream_stop,
    .ffmpeg_stream_after  = tran_stream_after,
};