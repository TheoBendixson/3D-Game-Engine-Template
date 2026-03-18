@echo off

set optimizer_flags= -Zi 

if "%~1" == "-release" (
set optimizer_flags= -O2 -DNDEBUG=1
) 

set compile_shaders=true

for %%A in (%*) do (
    if "%%A" == "-no-shaders" set compile_shaders=false
    if "%%A" == "-no-shader"  set compile_shaders=false
)

set ignored_warnings=-wd4805 -wd4244 -wd4018 -wd4838 -wd4700 -wd4996
set windows_build_path=..\..\build\windows
set resources_path=..\..\resources
set included_libraries=user32.lib d3d11.lib gdi32.lib dxguid.lib ole32.lib dwmapi.lib shell32.lib XInput.lib

:: Compile the game's shaders
if %compile_shaders% == true (
    fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh pixel_art_vertex_shader.h /Vn pixel_art_vertex_shader^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv pixel_art_shader.hlsl
    fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh pixel_art_fragment_shader.h /Vn pixel_art_fragment_shader^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv pixel_art_shader.hlsl

    fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh d3d11_vshader_flat.h /Vn d3d11_vshader_flat^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_flat.hlsl
    fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh d3d11_pshader_flat.h /Vn d3d11_pshader_flat^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_flat.hlsl

    fxc.exe /nologo /T vs_5_0 /E vs /O3 /WX /Zpc /Ges /Fh d3d11_vshader_textured.h /Vn d3d11_vshader_textured^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_textured.hlsl
    fxc.exe /nologo /T ps_5_0 /E ps /O3 /WX /Zpc /Ges /Fh d3d11_pshader_textured.h /Vn d3d11_pshader_textured^
            /Qstrip_reflect /Qstrip_debug /Qstrip_priv shader_textured.hlsl
)

if not exist %windows_build_path% mkdir %windows_build_path%
pushd %windows_build_path%

:: Compile the game
cl -WX -W3 -nologo %ignored_warnings% %optimizer_flags% -DWINDOWS=1 -DGAME=1 /FeVariosTemple^
            ..\..\code\windows_platform\windows_main.cpp^
           /link %included_libraries%

if exist windows_main.obj del windows_main.obj >nul

copy %resources_path%\game_character.png game_character.png
copy %resources_path%\grid_surface.png grid_surface.png
copy %resources_path%\cartoon.obj cartoon.obj
copy %resources_path%\testmodel.obj testmodel.obj
popd

:: Manifest stuff for DPI-awareness
mt.exe -manifest "VariosTemple.exe.manifest" -outputresource:"%windows_build_path%\VariosTemple.exe;1" -nologo

