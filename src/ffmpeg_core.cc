#include "ffmpeg_core.h"
#include "ffmpeg_client.h"
#include "ffmpeg_trans.h"
#include "ffmpeg_protocol.h"
#include "ffmpeg_stream.h"
#include "spdlog/spdlog.h"

std::mutex                 FFmpegCore::m_xhandle;
std::map<int, FFmpegCore*> FFmpegCore::m_mhandle;

FFmpegCore::FFmpegCore() {
    m_ctx = {};
}

FFmpegCore::FFmpegCore(std::string url) {
    m_ctx = {};
    m_ctx.url = url;
}

FFmpegCore::~FFmpegCore() {
}

int FFmpegCore::ffmpeg_core_init() {
    int ret = FFMPEG_OK;
    avdevice_register_all();
    avformat_network_init();
    return ret;
}

int FFmpegCore::ffmpeg_core_setconfig(void *param, int len, void *protocol) {
    int ret = FFMPEG_OK;
    ffmpeg_protocol *p = nullptr;

    if (nullptr == param || 0 >= len || nullptr == protocol) {
        spdlog::error("param error, param={} len={} protocol", param, len, protocol);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    p = (ffmpeg_protocol*)protocol;
    if (nullptr == p) {
        spdlog::error("param error, p={}", (void*)p);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ret = p->ffmpeg_set_config(param, len, this);
    if (FFMPEG_OK != ret) {
        spdlog::error("param error,ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int FFmpegCore::ffmpeg_core_start_stream(void *stream) {
    int ret = FFMPEG_OK;
    ffmpeg_stream *s = nullptr;

    if (nullptr == stream) {
        spdlog::error("param error, stream={}", stream);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    s = (ffmpeg_stream*)stream;
    if (nullptr == s) {
        spdlog::error("param error, s={}", (void*)s);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ret = s->ffmpeg_stream_before(this);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_stream_before failed, ret={}", ret);
        goto fail;
    }

    ret = s->ffmpeg_stream_start(this);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_stream_start failed, ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int FFmpegCore::ffmpeg_core_stop_stream(void *stream) {
    int ret = FFMPEG_OK;
    ffmpeg_stream *s = nullptr;

    if (nullptr == stream) {
        spdlog::error("param error, stream={}", stream);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    s = (ffmpeg_stream*)stream;
    if (nullptr == s) {
        spdlog::error("param error, s={}", (void*)s);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    ret = s->ffmpeg_stream_stop(this);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_stream_stop failed, ret={}", ret);
        goto fail;
    }

    ret = s->ffmpeg_stream_after(this);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_stream_after failed, ret={}", ret);
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int FFmpegCore::ffmpeg_core_regist_callback(ffmpeg_core_callback_type type, ffmpeg_callback func, void *user) {
    int ret = FFMPEG_OK;

    if (nullptr == func) {
        spdlog::error("param error, func={}", (void*)func);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    switch (type) {
    case ffmpeg_core_data_type:
        m_ctx.callback.data_callback = func;
        m_ctx.callback.data_user     = user;
        break;
    case ffmpeg_core_msg_type:
        m_ctx.callback.msg_callback = func;
        m_ctx.callback.msg_user     = user;
        break;
    default:
        spdlog::error("param error, type={}", (int)type);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int FFmpegCore::ffmpeg_core_input_data(void *data, int len)
{
    int ret = FFMPEG_OK;

    if (nullptr == data || len <= 0) {
        spdlog::error("param error, data={} len={}", data, len);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }
    return ret;

fail:
    return ret;
}

int FFmpegCore::ffmpeg_core_create_handle() {
    std::unique_lock<std::mutex> lock(m_xhandle);
    for (auto info : m_mhandle)
        if (info.second == this) 
            return info.first;

    size_t size = m_mhandle.size();
    while (m_mhandle.find(size) != m_mhandle.end())
        size++;

    m_mhandle[size] = this;

    return size;
}

ffmpeg_core_context *FFmpegCore::ffmpeg_core_get_context() {
    return &m_ctx;
}

int FFmpegCore::ffmpeg_core_release_handle(int h) {
    int ret = FFMPEG_OK;
    FFmpegCore *core = nullptr;

    std::unique_lock<std::mutex> lock(m_xhandle);
    auto it = m_mhandle.find(h);
    if (it == m_mhandle.end()) {
        spdlog::error("ffmpeg_core_release_handle find h={} failed", h);
        ret = FFMPEG_ERR_PARAM;
        goto fail;
    }

    core = it->second;
    if (core) {
        delete core;
        core = nullptr;
    }
    m_mhandle.erase(it);
    return ret;

fail:
    return ret;
}

FFmpegCore *FFmpegCore::ffmpeg_core_instance(int h) {
    std::unique_lock<std::mutex> lock(m_xhandle);
    auto it = m_mhandle.find(h);
    if (it == m_mhandle.end()) return nullptr;
    return it->second;
}