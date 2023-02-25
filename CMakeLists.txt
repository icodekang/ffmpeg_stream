cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(ffmpeg_stream VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wl,-Bsymbolic")

# spdlog lib
list(APPEND EXTRA_INCS "${PROJECT_SOURCE_DIR}/third_party/spdlog/inc")
list(APPEND EXTRA_LIBS "${PROJECT_SOURCE_DIR}/third_party/spdlog/lib")

# ffmpeg lib
list(APPEND EXTRA_INCS "${PROJECT_SOURCE_DIR}/third_party/ffmpeg/inc")
list(APPEND EXTRA_LIBS "${PROJECT_SOURCE_DIR}/third_party/ffmpeg/lib")

# search all src code
aux_source_directory("${PROJECT_SOURCE_DIR}/src" MAIN_SRC)

add_library(ffmpeg_stream SHARED ${MAIN_SRC})

target_include_directories(ffmpeg_stream PUBLIC "${PROJECT_SOURCE_DIR}")
target_include_directories(ffmpeg_stream PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(ffmpeg_stream PUBLIC "${EXTRA_INCS}")
target_link_directories(ffmpeg_stream PUBLIC "${EXTRA_LIBS}")
target_link_libraries(ffmpeg_stream PUBLIC spdlog avdevice avfilter postproc avformat avcodec avutil swresample swscale z m bz2 pthread)

add_subdirectory(example)

install(TARGETS ffmpeg_stream DESTINATION .)
install(TARGETS rtsp_stream   DESTINATION .)
install(TARGETS tran_stream   DESTINATION .)