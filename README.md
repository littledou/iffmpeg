#### 获取源码
网络条件允许的情况下可以直接在官网下载，如git仓库速度过慢，可以将git仓库fork至国内源码管理网站，推荐Gitee或者腾讯工蜂。
源码下载完成后，将分支切换至4.2.1
```
git checkout n4.2.1
```
#### 配置编译脚本
##### 默认编译选项
所有的编译选项位于ffmpeg根目录configure文件中，查看所有编译选项：
```
./configure -h
```

##### 配置编译Android动态库工具链
ffmpeg编译到不同的平台使用需使用不同的工具链
``` shell
#NDK路径
NDK=/Users/loki/Library/Android/sdk/ndk/20.0.5594570
#支持最低的Android版本
API=21
# arm aarch64 i686 x86_64 CPU类型
ARCH=arm
# armv7a aarch64 i686 x86_64 cpu架构
CPU=armv7a
#编译时所需要使用的Android框架下的库和头文件
SYSROOT=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

```

在ffmpeg根目录创建shell脚本config_android.sh
Android NDK在r18版本移除了gcc编译器，目前Android Studio最新NDK版本为20.0.5594570,需配置clang编译器。
完整脚本如下：
``` shell
#!/bin/bash
NDK=/Users/loki/Library/Android/sdk/ndk/20.0.5594570
echo "$NDK"

API=21
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/bin

# arm aarch64 i686 x86_64 CPU类型
ARCH=arm
# armv7a aarch64 i686 x86_64 cpu架构
CPU=armv7a

CC=$TOOLCHAIN/$CPU-linux-androideabi$API-clang
CXX=$TOOLCHAIN/$CPU-linux-androideabi$API-clang++
SYSROOT=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot
PREFIX=/Users/loki/Desktop/work/soft/ffmpeg/build/android/$CPU

CFLAG="-D__ANDROID_API__=$API -U_FILE_OFFSET_BITS -DBIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD -Os -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -marm"

./configure \
--prefix=$PREFIX \
--target-os=android \
--arch=$CPU \
--enable-shared --disable-static \
--disable-doc \
--disable-ffmpeg --disable-ffplay --disable-ffprobe --disable-avdevice --disable-symver \
--enable-cross-compile \
--cross-prefix=$TOOLCHAIN/$ARCH-linux-androideabi- \
--cc=$CC --cxx=$CXX \
--sysroot=$SYSROOT \
--extra-cflags="-I$SYSROOT/usr/include" \
--extra-ldflags="-L$SYSROOT/usr/lib"

make clean
make -j4
make install
```
##### 编译产物
![ffmpeg-android-out.png](https://i.loli.net/2019/12/20/mPhZoUBxNK1XFib.png)

#### 应用
- 创建支持native工程的Android Studio应用.
- 将编译产物lib下动态库复制到app中jniLibs/armeabi-v7a目录.
- 将编译产物include目录整个复制到app中main/cpp目录下.
- 修改CMakeLists.txt，链接avcodec动态库
- 增加测试代码，获取avcodec_configuration()信息

执行结果：
```
--prefix=/Users/loki/Desktop/work/soft/ffmpeg/build/android/armv7a --target-os=android --arch=armv7a --enable-shared --disable-static --disable-doc --disable-ffmpeg --disable-ffplay --disable-ffprobe --disable-avdevice --disable-symver --enable-cross-compile --cross-prefix=/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/bin/arm-linux-androideabi- --cc=/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/bin/armv7a-linux-androideabi21-clang --cxx=/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/bin/armv7a-linux-androideabi21-clang++ --sysroot=/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/sysroot --extra-cflags=-I/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include --extra-ldflags=-L/Users/loki/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib
```
[Githup 示例](https://github.com/littledou/iffmpeg)


#### 遇到的错误


``` C
error: undefined reference to 'avcodec_configuration()'
描述：ffmpeg源代码均为c编写，在c++文件中需 extren "C"包裹才能正常调用
```
``` C
java.lang.UnsatisfiedLinkError: dalvik.system.PathClassLoader[DexPathList[[zip file "/data/app/cn.readsense.iffmpeg-2/base.apk"],nativeLibraryDirectories=[/data/app/cn.readsense.iffmpeg-2/lib/arm64, /system/lib64, /vendor/lib64]]] couldn't find "libiffmpeg.so"
描述：在64位机器上运行32位库时，需指定应用仅支持32位库,abiFilters "armeabi-v7a"
```
``` C
java.lang.UnsatisfiedLinkError: dlopen failed: cannot locate symbol "iconv_open" referenced by "/data/app/cn.readsense.iffmpeg-1/lib/arm/libavcodec.so"...
描述：经查询，iconv_open方法位于“sysroot/usr/include/iconv.h”头文件中，ANDROID_API版本大于28，该方法未实现，使用编译的工具链版本大于28，编译产物在小于28的安卓设备上运行出现，需降低至目标设备之下版本的API
```
