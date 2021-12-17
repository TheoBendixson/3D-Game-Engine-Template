echo Building Varios Temple

MAC_BUILD_PATH="../../build/mac_os"

mkdir -p $MAC_BUILD_PATH
pushd $MAC_BUILD_PATH

MAC_PLATFORM_LAYER_PATH="../../code/mac_platform"
GAME_LIBRARY_CODE_PATH="../../code/game_library/code"

OSX_LD_FLAGS="-framework AppKit 
              -framework IOKit
              -framework Metal
              -framework MetalKit
              -framework QuartzCore
              -framework AudioToolbox"

COMMON_COMPILER_FLAGS="-DMACOS=1
                       ${OSX_LD_FLAGS}"

GAME_BUNDLE_RESOURCES_PATH="Vario.app/Contents/Resources"

PLATFORM_RESOURCES_PATH="../../code/mac_platform/resources"

echo Compiling Shader Libraries
xcrun -sdk macosx metal -mmacosx-version-min=10.14 -gline-tables-only -MO -g -c "${MAC_PLATFORM_LAYER_PATH}/shaders.metal" -o shaders.air
xcrun -sdk macosx metallib shaders.air -o shaders.metallib

echo Compiling Game Library \(Slow\)
clang -g -std=c++11 -o GameCode.dylib ${COMMON_COMPILER_FLAGS} -dynamiclib ${GAME_LIBRARY_CODE_PATH}/vario.cpp

echo Compiling Mac Platform Layer \(Slow\)
clang -g -lstdc++ ${COMMON_COMPILER_FLAGS} -mmacosx-version-min=10.14 -o Vario "${MAC_PLATFORM_LAYER_PATH}/mac_os_main.mm"

echo Building Game Application Bundle
rm -rf Vario.app
mkdir -p $GAME_BUNDLE_RESOURCES_PATH 

cp Vario Vario.app/Vario

cp GameCode.dylib ${GAME_BUNDLE_RESOURCES_PATH}/GameCode.dylib

echo Copying dSYM File Into Game Bundle
cp -r GameCode.dylib.dSYM ${GAME_BUNDLE_RESOURCES_PATH}/GameCode.dylib.dSYM

cp shaders.metallib ${GAME_BUNDLE_RESOURCES_PATH}/shaders.metallib
cp ${PLATFORM_RESOURCES_PATH}/GameInfo.plist Vario.app/Contents/Info.plist

popd
