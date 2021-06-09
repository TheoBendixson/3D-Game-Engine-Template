echo Building Varios Temple

pushd ../../build/mac_os

MAC_PLATFORM_LAYER_PATH="../../code/mac_platform_layer"
GAME_LIBRARY_CODE_PATH="../../code/game_library/code"

OSX_LD_FLAGS="-framework AppKit 
              -framework IOKit
              -framework Metal
              -framework MetalKit
              -framework QuartzCore
              -framework AudioToolbox"

COMMON_COMPILER_FLAGS="$OSX_LD_FLAGS"

echo Compiling Game Library \(Slow\)
clang -g -o GameCode.dylib ${COMMON_COMPILER_FLAGS} -dynamiclib ${GAME_LIBRARY_CODE_PATH}/vario.cpp

echo Compiling Mac Platform Layer \(Slow\)
clang -g -lstdc++ ${COMMON_COMPILER_FLAGS} -mmacosx-version-min=10.14 -o Vario "${MAC_PLATFORM_LAYER_PATH}/mac_os_main.mm"
