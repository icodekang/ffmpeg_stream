add_executable(rtsp_stream rtsp_stream.cc)
target_link_libraries(rtsp_stream PUBLIC ffmpeg_stream)

add_executable(tran_stream tran_stream.cc)
target_link_libraries(tran_stream PUBLIC ffmpeg_stream)