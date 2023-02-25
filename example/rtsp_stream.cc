#include <unistd.h>
#include <iomanip>
#include "cmdline.h"
#include "ffmpeg_client.h"
#include "spdlog/spdlog.h"

static void option(int argc, char **argv, cmdline::parser &parser) {
    parser.add<std::string>("url", 'u', "rtsp url", true);
    parser.add<std::string>("out", 'o', "out file path", true);
    parser.add<std::string>("transport",    '\0', "set RTSP transport protocols.", false);
    parser.add<unsigned int>("min_port",    '\0', "set minimum local udp port. default value is 5000.", false);
    parser.add<unsigned int>("max_port",    '\0', "set maximum local udp port. default value is 65000.", false);
    parser.add<unsigned int>("timeout",     '\0', "set socket tcp i/o timeout in microseconds.", false);
    parser.add<unsigned int>("queue_size",  '\0', "set number of packets to buffer for handling of reordered packets.", false);
    parser.add<unsigned int>("buffer_size", '\0', "set the maximum socket buffer size in bytes.", false);
    parser.parse_check(argc, argv);
}

static void progress_bar(const int progress) {
    std::cout << "\r";
    std::cout << std::setw(progress) << std::setfill('=') << "=";
    std::cout << std::setw(100 - progress) << std::setfill(' ') << "|";
    std::cout << progress << "%|" << std::flush;
}

static void ffmpeg_msg_callback(HANDLE h, ffmpeg_client_callback_type type, void *data, int len, void *user) {
    spdlog::info("ffmpeg_msg_callback h={} type={} data={} len={} user={}", h, (int)type, data, len, user);
}

static void ffmpeg_data_callback(HANDLE h, ffmpeg_client_callback_type type, void *data, int len, void *user) {
    spdlog::info("ffmpeg_data_callback h={} type={} data={} len={} user={}", h, (int)type, data, len, user);
}

int main(int argc, char **argv) {
    int ret = FFMPEG_OK;
    int progress = 0;
    char buf[102] = {0};
    const char *label = "|/-\\";
    HANDLE h = INVALID_HANDLE;
    std::string url, out;
    ffmpeg_client_rtsp_param rtsp_param = {};
    ffmpeg_client_config config = {};

    cmdline::parser parser;
    option(argc, argv, parser);

    if (parser.exist("url") && parser.exist("out")) {
        url = parser.get<std::string>("url");
        out = parser.get<std::string>("out");
    } else {
        spdlog::error("param error, ./%s -h for help", argv[0]);
        goto fail;
    }

    ret = ffmpeg_client_create(&h, url.c_str());
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_create failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_create success!");

    if (parser.exist("transport")
        && parser.get<std::string>("transport").compare("udp") == 0) {
        rtsp_param.rtsp_transport = ffmpeg_client_udp_type;
    } else {
        rtsp_param.rtsp_transport = ffmpeg_client_tcp_type;
    }
    if (parser.exist("min_port")) {
        rtsp_param.min_port = parser.get<unsigned int>("min_port");
    }
    if (parser.exist("max_port")) {
        rtsp_param.max_port = parser.get<unsigned int>("max_port");
    }
    if (parser.exist("timeout")) {
        rtsp_param.timeout = parser.get<unsigned int>("timeout");
    }
    if (parser.exist("queue_size")) {
        rtsp_param.queue_size = parser.get<unsigned int>("queue_size");
    }
    if (parser.exist("buffer_size")) {
        rtsp_param.buffer_size = parser.get<unsigned int>("buffer_size");
    }
    config.protocol_type = ffmpeg_client_rtsp_type;
    config.param         = &rtsp_param;
    config.len           = sizeof(rtsp_param);
    ret = ffmpeg_client_setconfig(h, &config);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_setconfig failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_setconfig success!");

    ret = ffmpeg_client_setcallback(h, ffmpeg_client_data_type, ffmpeg_data_callback, nullptr);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_setcallback data failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_setcallback data success!");

    ret = ffmpeg_client_setcallback(h, ffmpeg_client_msg_type, ffmpeg_msg_callback, nullptr);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_setcallback msg failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_setcallback msg success!");

    ret = ffmpeg_client_start(h);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_start failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_start success!");

    while (progress++ < 100) {
        progress_bar(progress);
        usleep(5 * 60 * 1000 * 1000 / 100);
    }

    ret = ffmpeg_client_stop(h);
    if (FFMPEG_OK != ret) {
        spdlog::error("ffmpeg_client_stop failed, ret={}", ret);
        goto fail;
    }
    spdlog::info("ffmpeg_client_stop success!");

fail:
    if (INVALID_HANDLE != h) {
        ret = ffmpeg_client_destroy(h);
        if (FFMPEG_OK != ret) {
            spdlog::error("ffmpeg_client_destroy failed, ret={}", ret);
        }
    }
    return ret;
}