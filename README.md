# ffmpeg_stream

### ffmpeg编译
```
PATH="$HOME/ffmpeg/ffmpeg_build/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg/ffmpeg_build/lib/pkgconfig"
./configure \
--prefix="$HOME/ffmpeg/ffmpeg_build" \
--extra-cflags="-fPIC" \
--extra-cxxflags="-fPIC" \
--extra-cflags="-I$HOME/ffmpeg/ffmpeg_build/include" \
--extra-ldflags="-L$HOME/ffmpeg/ffmpeg_build/lib" \
--extra-libs=-lpthread \
--extra-libs=-lm \
--bindir="$HOME/ffmpeg/ffmpeg_build/bin" \
--enable-pic \
--enable-gpl \
--enable-nonfree
```

### ffmpeg_stream编译
```
mkdir build && cd build
make -j2
cmake --install . --prefix .
```