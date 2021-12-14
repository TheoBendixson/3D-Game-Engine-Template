@echo off

set optimizer_flags= -Zi 

if "%~1" == "-release" (
set optimizer_flags= -O2 -DNDEBUG=1
) 

set ignored_warnings=-wd4805 -wd4244 -wd4018 -wd4838 -wd4700
set windows_build_path=..\..\build\windows
set resources_path=..\..\resources
set included_libraries=user32.lib d3d11.lib gdi32.lib dxguid.lib ole32.lib dwmapi.lib 
set sdl_library_path=c:/SDL2-devel-2.0.16-VC/SDL2-2.0.16
set sdl_dll_path=.\resources\SDL2.dll

:: Compile the game's shaders
fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh d3d11_vshader_flat.h /Vn d3d11_vshader_flat^
        /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_flat.hlsl
fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh d3d11_pshader_flat.h /Vn d3d11_pshader_flat^
        /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_flat.hlsl

fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh d3d11_vshader_textured.h /Vn d3d11_vshader_textured^
        /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_textured.hlsl
fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh d3d11_pshader_textured.h /Vn d3d11_pshader_textured^
        /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_textured.hlsl

mkdir %windows_build_path%
pushd %windows_build_path%

:: Compile the game
cl -WX -W3 %ignored_warnings% %optimizer_flags% -DWINDOWS=1 -DGAME=1 /FeVariosTemple /I %sdl_library_path%/include^
            ..\..\code\windows_platform\windows_main.cpp^
           /link /LIBPATH %sdl_library_path%/lib/x64/SDL2.lib^
    %included_libraries%

copy ..\..\code\windows_platform\resources\SDL2.dll SDL2.dll
popd
