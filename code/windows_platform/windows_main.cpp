#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <shobjidl.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include <dsound.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <xinput.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <intrin.h>
#define AssertHR(HR) Assert(SUCCEEDED(HR))

#define STR2(x) #x
#define STR(x) STR2(x)

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_JPEG
#define STBI_NO_BMP
#define STBI_NO_TGA
#define STBI_NO_PSD
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_GIF
#define STBI_NO_PNM
#include "stb_image.h"

#include "windows_main.h"

global_variable IFileSaveDialog *FileSaveDialog;
global_variable s32 CursorDisplayCount;

inline void ShowSystemCursor()
{
    while (CursorDisplayCount <= 0)
    {
        CursorDisplayCount = ShowCursor(true);
    }
}

inline void HideSystemCursor()
{
    while (CursorDisplayCount >= 0)
    {
        CursorDisplayCount = ShowCursor(false);
    }
}

void FatalError(const char* message)
{
    MessageBoxA(NULL, message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}

#include "windows_file_io.h"
#include "windows_async_file_io.h"
#include "windows_renderer_setup.h"
#include "windows_gpu_transfer.h"
#include "windows_utility.h"

#include "../game_library/code/vario.cpp"
#include "../game_library/code/shared_input.cpp"
#include "../game_library/code/shared_renderer.cpp"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
typedef DIRECT_SOUND_CREATE(direct_sound_create);

bool quitting;
bool resizing;
HANDLE main_fiber;
DWORD update_interval;
DWORD next_update;
global_variable HANDLE GlobalStdHandle;
global_variable b32 GlobalPrintToConsole = false;
global_variable volatile b32 GlobalHasFocus = false;
global_variable volatile button_state GlobalMouseButtons[5] = {};
global_variable s64 GlobalPerformanceCountFrequency;

#define FIXED_DT (1.0f/60.0f)

PLATFORM_QUIT_GAME(PlatformQuitGame)
{
    ExitProcess(0);
}

PLATFORM_LOG_MESSAGE(PlatformLogMessage)
{
    if (GlobalPrintToConsole)
    {
        WriteFile(GlobalStdHandle, Message, (DWORD)strlen(Message), 0, 0);
    }
}

void WindowsDebugPrintf(char *Format, ...)
{
#ifndef NDEBUG
    va_list Args;
    va_start(Args, Format);
    char Str[1024] = "";
    vsprintf_s(Str, Format, Args);
    Str[ArrayCount(Str) - 1] = 0;
    PlatformLogMessage(Str);
    va_end(Args);
#endif
}

inline LARGE_INTEGER
WindowsGetTimeCounter()
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return Result;
}

inline r32
WindowsGetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    return (r32)(End.QuadPart - Start.QuadPart) / ((r32)GlobalPerformanceCountFrequency);
}

inline r32
WindowsGetSecondsElapsed(LARGE_INTEGER Start)
{
    return WindowsGetSecondsElapsed(Start, WindowsGetTimeCounter());
}

global_variable b32 WindowSized = false;

LRESULT CALLBACK
MainWindowCallback(HWND Window,
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT result = 0;
    switch (Message)
    {
        case WM_ACTIVATEAPP:
            GlobalHasFocus = (WParam == TRUE);
            break;
        case WM_DESTROY:
            quitting = true;
            break;
        case WM_SIZE:
            WindowSized = true;
            resizing = true;
            break;
        case WM_PAINT:
        case WM_TIMER:
            SwitchToFiber(main_fiber);
            break;
        case WM_ENTERMENULOOP:
        case WM_ENTERSIZEMOVE:
            SetTimer(Window, 0, 1, 0);
            break;
        case WM_EXITMENULOOP:
        case WM_EXITSIZEMOVE:
            KillTimer(Window, 0);
            break;
        default:
            result = DefWindowProcA(Window, Message, WParam, LParam);
            break;
    }
    return result;
}

game_input Input = {};

void CALLBACK MessageFiberProc(void *)
{
    // Load the US English keyboard layout without activating it so that
    // non-English keyboard users can use WASD without changing OS settings.
    HKL EnglishKeyboardLayout = LoadKeyboardLayout("00000409", 0);

    for (;;)
    {
        DWORD now = GetTickCount();
        if (now >= next_update || MsgWaitForMultipleObjects(0, 0, FALSE, next_update - now, QS_ALLEVENTS) == WAIT_OBJECT_0)
        {
            MSG Message;
            while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
            {
                u32 WMessage = Message.message;

                if (WMessage == WM_QUIT)
                {
                    break;
                }
                else if (WMessage == WM_KEYDOWN || WMessage == WM_KEYUP)
                {
                    u32 VKCode = (u32)Message.wParam;
                    b32 WasDown = ((Message.lParam & (1 << 30)) != 0);
                    b32 IsDown  = ((Message.lParam & (1 << 31)) == 0);

                    if (WasDown != IsDown)
                    {
                        if (VKCode == VK_UP)
                        {
                            ProcessButton(&Input.Keyboard.Up, IsDown);
                        }
                        else if (VKCode == VK_DOWN)
                        {
                            ProcessButton(&Input.Keyboard.Down, IsDown);
                        }
                        else if (VKCode == VK_LEFT)
                        {
                            ProcessButton(&Input.Keyboard.Left, IsDown);
                        }
                        else if (VKCode == VK_RIGHT)
                        {
                            ProcessButton(&Input.Keyboard.Right, IsDown);
                        }
                        else if (VKCode == VK_SPACE)
                        {
                            ProcessButton(&Input.Keyboard.Space, IsDown);
                            ProcessButton(&Input.Controller.A, IsDown);
                        }
                        else if (VKCode == VK_RETURN)
                        {
                            ProcessButton(&Input.Keyboard.Enter, IsDown);
                            ProcessButton(&Input.Controller.Start, IsDown);
                        }
                        else if (VKCode == VK_DELETE)
                        {
                            ProcessButton(&Input.Keyboard.Delete, IsDown);
                        }
                        else if (VKCode == VK_BACK)
                        {
                            ProcessButton(&Input.Keyboard.Backspace, IsDown);
                        }
                        else if (VKCode == VK_TAB)
                        {
                            ProcessButton(&Input.Keyboard.Tab, IsDown);
                        }
                        else if (VKCode == VK_SHIFT)
                        {
                            ProcessButton(&Input.Keyboard.Shift, IsDown);
                        }
                        else if (VKCode == VK_MENU)
                        {
                            ProcessButton(&Input.Keyboard.Alt, IsDown);
                        }
                        else if (VKCode == VK_CONTROL)
                        {
                            ProcessButton(&Input.Keyboard.Control, IsDown);
                        }
                        else if (VKCode == VK_ESCAPE)
                        {
                            ProcessButton(&Input.Keyboard.Escape, IsDown);
                        }
                        else if (VKCode >= 'A' && VKCode <= 'Z')
                        {
                            u32 ScanCode  = (Message.lParam >> 16) & 0xFF;
                            u32 PhysicalVK = EnglishKeyboardLayout
                                             ? MapVirtualKeyEx(ScanCode, MAPVK_VSC_TO_VK, EnglishKeyboardLayout)
                                             : VKCode;

                            if (PhysicalVK >= 'A' && PhysicalVK <= 'Z')
                            {
                                ProcessButton(&Input.Keyboard.Letters[PhysicalVK - 'A'], IsDown);
                            }

                            // Also mirror WASD to controller D-pad for legacy game code.
                            if (PhysicalVK == 'W') ProcessButton(&Input.Controller.Up,    IsDown);
                            if (PhysicalVK == 'S') ProcessButton(&Input.Controller.Down,  IsDown);
                            if (PhysicalVK == 'A') ProcessButton(&Input.Controller.Left,  IsDown);
                            if (PhysicalVK == 'D') ProcessButton(&Input.Controller.X,     IsDown);
                            if (PhysicalVK == 'Z') ProcessButton(&Input.Controller.LeftShoulder2, IsDown);
                        }
                        else if (VKCode >= '0' && VKCode <= '9')
                        {
                            ProcessButton(&Input.Keyboard.Numbers[VKCode - '0'], IsDown);
                        }
                        else if (VKCode >= VK_F1 && VKCode <= VK_F15)
                        {
                            ProcessButton(&Input.Keyboard.F[VKCode - VK_F1 + 1], IsDown);
                        }
                    }
                }
                else if (WMessage == WM_MOUSEWHEEL)
                {
                    s32 DWheel = GET_WHEEL_DELTA_WPARAM(Message.wParam) / WHEEL_DELTA;
                    Input.MouseWheel += DWheel;
                }
                else if (WMessage == WM_LBUTTONDOWN ||
                         WMessage == WM_MBUTTONDOWN ||
                         WMessage == WM_RBUTTONDOWN ||
                         WMessage == WM_XBUTTONDOWN)
                {
                    if (Message.wParam & MK_LBUTTON)  ProcessButton((button_state *)&GlobalMouseButtons[0], true);
                    if (Message.wParam & MK_MBUTTON)  ProcessButton((button_state *)&GlobalMouseButtons[1], true);
                    if (Message.wParam & MK_RBUTTON)  ProcessButton((button_state *)&GlobalMouseButtons[2], true);
                    if (Message.wParam & MK_XBUTTON1) ProcessButton((button_state *)&GlobalMouseButtons[3], true);
                    if (Message.wParam & MK_XBUTTON2) ProcessButton((button_state *)&GlobalMouseButtons[4], true);
                }

                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }

        SwitchToFiber(main_fiber);
    }
}

// ---- 3D draw helpers ----

global_variable ID3D11Buffer *WindowsFlatColorVertexBuffer;
global_variable ID3D11Buffer *WindowsTextureVertexBuffer;
global_variable ID3D11Buffer *WindowsLoadedModelVertexBuffer;

internal void
DrawMeshesFromInstanceBuffer(ID3D11DeviceContext *DeviceContext, ID3D11Buffer *ConstantsBuffer,
                              mesh_instance_buffer *MeshBuffer, game_vertex_buffer *VertexBuffer)
{
    for (u32 Index = 0; Index < MeshBuffer->MeshCount; Index++)
    {
        mesh_instance *MeshInstance = &MeshBuffer->Meshes[Index];

        {
            D3D11_MAPPED_SUBRESOURCE Mapped;
            HRESULT HR = DeviceContext->Map((ID3D11Resource*)ConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD,
                                             0, &Mapped);
            AssertHR(HR);
            memcpy(Mapped.pData, &MeshInstance->Constants, sizeof(game_constants));
            DeviceContext->Unmap((ID3D11Resource*)ConstantsBuffer, 0);
        }

        model_range Range = VertexBuffer->ModelRanges[MeshInstance->ModelIndex];
        DeviceContext->Draw(Range.VertexCount, Range.StartVertex);
    }
}

internal void
InitializeMeshInstanceBufferWindows(mesh_instance_buffer *InstanceBuffer, u32 MaxMeshes)
{
    InstanceBuffer->MeshCount = 0;
    InstanceBuffer->MeshMax   = MaxMeshes;
    InstanceBuffer->Meshes    = (mesh_instance *)VirtualAlloc(0, MaxMeshes * sizeof(mesh_instance),
                                                              MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

// ---- 2D sprite atlas helpers ----

#define MAX_SUPPORTED_TEXTURE_TYPES 2
global_variable ID3D11ShaderResourceView *ShaderResourceViews3D[MAX_SUPPORTED_TEXTURE_TYPES];

struct windows_sprite_draw_data
{
    ID3D11ShaderResourceView *ShaderView;
    v2 TextureSize;
};

windows_sprite_draw_data
GetSpriteDrawDataForAtlasType(texture_atlas_type AtlasType,
                              ID3D11ShaderResourceView **Views,
                              game_texture_buffer *TextureBuffers)
{
    windows_sprite_draw_data Result = {};
    u32 Index = (AtlasType == TextureAtlasType_Tile32) ? 1 : 0;
    Result.ShaderView   = Views[Index];
    Result.TextureSize  = V2((r32)TextureBuffers[Index].Textures[0].Width, (r32)TextureBuffers[Index].Textures[0].Height);
    return Result;
}

// ---- Fullscreen toggle ----

void
AdjustViewportWithUpdatedWindowSize(game_render_commands *RenderCommands, r32 WindowWidth, r32 WindowHeight)
{
    RenderCommands->ViewportWidth  = (s32)WindowWidth;
    RenderCommands->ViewportHeight = (s32)WindowHeight;
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    // Parse -console flag
    int argcW = 0;
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argcW);
    for (int i = 0; i < argcW; i++)
    {
        if (WideStringsAreEqual(argvW[i], L"-console"))
        {
            if (AttachConsole((DWORD)-1) == 0)
            {
                AllocConsole();
            }
            GlobalPrintToConsole = true;
        }
    }
    GlobalStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    LARGE_INTEGER PerformanceFrequency;
    QueryPerformanceFrequency(&PerformanceFrequency);
    GlobalPerformanceCountFrequency = PerformanceFrequency.QuadPart;

    main_fiber = ConvertThreadToFiber(0);
    HANDLE message_fiber = CreateFiber(0, MessageFiberProc, 0);

    game_memory GameMemory = {};
    GameMemory.PermanentStorageSize = Megabytes(1024);
    GameMemory.TransientStorageSize = Megabytes(128);
    GameMemory.PermanentStorage = VirtualAlloc(0, GameMemory.PermanentStorageSize,
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    GameMemory.TransientStorage = VirtualAlloc(0, GameMemory.TransientStorageSize,
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!GameMemory.PermanentStorage || !GameMemory.TransientStorage)
    {
        return 0;
    }

    GameSetupMemoryPartitions(&GameMemory);

    GameMemory.PlatformReadEntireFile  = PlatformReadEntireFile;
    GameMemory.PlatformWriteEntireFile = PlatformWriteEntireFile;
    GameMemory.PlatformFreeFileMemory  = PlatformFreeFileMemory;
    GameMemory.PlatformReadPNGFile     = PlatformReadPNGFile;
    GameMemory.PlatformOpenFileDialog  = PlatformOpenFileDialog;
    GameMemory.PlatformSaveFileDialog  = PlatformSaveFileDialog;
    GameMemory.PlatformLaunchAsyncSave = PlatformLaunchAsyncSave;
    GameMemory.PlatformLaunchAsyncLoad = PlatformLaunchAsyncLoad;
    GameMemory.PlatformLogMessage      = PlatformLogMessage;
    GameMemory.PlatformQuitGame        = PlatformQuitGame;

    Input.PrimaryInputMode = GameInputMode_KeyboardMouse;

    thread_context Thread = {};

// register window class
    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize        = sizeof(WindowClass);
    WindowClass.lpfnWndProc   = MainWindowCallback;
    WindowClass.hInstance     = Instance;
    WindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = L"d3d11_window_class";

    ATOM atom = RegisterClassExW(&WindowClass);
    Assert(atom && "Failed to register window class");

    DWORD ExStyle = WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP;
    DWORD Style   = WS_OVERLAPPED | WS_SYSMENU;

    RECT ScreenRect;
    GetWindowRect(GetDesktopWindow(), &ScreenRect);
    int ScreenHeight = ScreenRect.bottom - ScreenRect.top - 32;
    int ScreenWidth  = (ScreenRect.right - ScreenRect.left) + 18;

    RECT WindowRect = { 0, 0, ScreenWidth, ScreenHeight };
    AdjustWindowRectEx(&WindowRect, Style, FALSE, ExStyle);

    int WindowWidth  = WindowRect.right - WindowRect.left;
    int WindowHeight = WindowRect.bottom - WindowRect.top;

    HWND WindowHandle = CreateWindowExW(ExStyle, WindowClass.lpszClassName,
                                        (LPCWSTR)"Vario's Temple", Style,
                                        -10, 0, WindowWidth, WindowHeight,
                                        NULL, NULL, Instance, NULL);
    Assert(WindowHandle && "Failed to create window");

    HRESULT HR;

    DwmGetWindowAttribute(WindowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &WindowRect, sizeof(WindowRect));
    WindowWidth  = WindowRect.right - WindowRect.left;
    WindowHeight = WindowRect.bottom - WindowRect.top;

    game_render_commands RenderCommands = {};
    AdjustViewportWithUpdatedWindowSize(&RenderCommands, (r32)WindowWidth, (r32)WindowHeight);

    HDC RefreshDC       = GetDC(WindowHandle);
    int RefreshRate     = GetDeviceCaps(RefreshDC, VREFRESH);
    Input.dtForFrame    = 1.0f / (r32)RefreshRate;
    Input.FrameRateMultiplier = (r32)RefreshRate / 60.0f;

    // Initialize Direct Sound
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    LPDIRECTSOUNDBUFFER SecondaryBuffer = NULL;
    b32 SoundIsEnabled = false;

    game_startup_config StartupConfig = GameGetStartupConfig();
    u32 SamplesPerSecond    = StartupConfig.SoundSamplesPerSecond;
    u32 BytesPerSample      = StartupConfig.SoundBytesPerSample;
    u32 SecondaryBufferSize = StartupConfig.SoundBufferSize;

    if (DSoundLibrary)
    {
        direct_sound_create *DirectSoundCreate = (direct_sound_create *)GetProcAddress(DSoundLibrary, "DirectSoundCreate");

        if (DirectSoundCreate)
        {
            LPDIRECTSOUND DirectSound;
            HR = DirectSoundCreate(0, &DirectSound, 0);
            AssertHR(HR);

            HR = DirectSound->SetCooperativeLevel(WindowHandle, DSSCL_PRIORITY);
            AssertHR(HR);

            LPDIRECTSOUNDBUFFER PrimaryBuffer;
            DSBUFFERDESC PrimaryBufferDesc = {};
            PrimaryBufferDesc.dwSize  = sizeof(PrimaryBufferDesc);
            PrimaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

            HR = DirectSound->CreateSoundBuffer(&PrimaryBufferDesc, &PrimaryBuffer, 0);
            AssertHR(HR);

            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag      = WAVE_FORMAT_PCM;
            WaveFormat.nChannels       = 2;
            WaveFormat.wBitsPerSample  = 16;
            WaveFormat.nSamplesPerSec  = SamplesPerSecond;
            WaveFormat.nBlockAlign     = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nBlockAlign * WaveFormat.nSamplesPerSec;

            HR = PrimaryBuffer->SetFormat(&WaveFormat);
            AssertHR(HR);

            DSBUFFERDESC SecondaryBufferDesc = {};
            SecondaryBufferDesc.dwSize        = sizeof(SecondaryBufferDesc);
            SecondaryBufferDesc.dwFlags       = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
            SecondaryBufferDesc.dwBufferBytes = SecondaryBufferSize;
            SecondaryBufferDesc.lpwfxFormat   = &WaveFormat;

            HR = DirectSound->CreateSoundBuffer(&SecondaryBufferDesc, &SecondaryBuffer, 0);
            AssertHR(HR);

            HR = SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
            AssertHR(HR);

            SoundIsEnabled = true;
        }
    }

    game_sound_output_buffer SoundOutputBuffer = {};
    SoundOutputBuffer.Samples          = (s16 *)calloc(SamplesPerSecond, BytesPerSample);
    SoundOutputBuffer.SamplesPerSecond = SamplesPerSecond;

    // D3D11 device + swap chain
    IDXGISwapChain*      SwapChain;
    ID3D11Device*        D11Device;
    ID3D11DeviceContext* DeviceContext;

    {
        DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc.SampleDesc        = { 1, 0 };
        SwapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.BufferCount       = 2;
        SwapChainDesc.OutputWindow      = WindowHandle;
        SwapChainDesc.Windowed          = TRUE;
        SwapChainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        UINT Flags = 0;
#ifndef NDEBUG
        Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0 };
        HR = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, Flags, Levels,
                                           _countof(Levels), D3D11_SDK_VERSION, &SwapChainDesc,
                                           &SwapChain, &D11Device, NULL, &DeviceContext);
        AssertHR(HR);
    }

#ifndef NDEBUG
    {
        ID3D11Debug     *D3DDebug;
        ID3D11InfoQueue *D3DInfoQueue;
        HR = D11Device->QueryInterface(__uuidof(ID3D11Debug),     (void **)&D3DDebug);    AssertHR(HR);
        HR = D11Device->QueryInterface(__uuidof(ID3D11InfoQueue), (void **)&D3DInfoQueue); AssertHR(HR);
        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR,      true);
        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING,    true);
        D3DInfoQueue->Release();
        D3DDebug->Release();
    }
#endif

    // Disable Alt+Enter fullscreen toggle
    {
        IDXGIFactory *Factory;
        HR = SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&Factory);
        AssertHR(HR);
        Factory->MakeWindowAssociation(WindowHandle, DXGI_MWA_NO_ALT_ENTER);
        Factory->Release();
    }

    // ---- 2D sprite pipeline setup ----

    {
        u32 VertexCount = 6;
        RenderCommands.SpriteVertexBuffer.Max          = VertexCount;
        RenderCommands.SpriteVertexBuffer.Size         = sizeof(sprite_vertex) * VertexCount;
        RenderCommands.SpriteVertexBuffer.Vertices     =
            (sprite_vertex *)VirtualAlloc(0, RenderCommands.SpriteVertexBuffer.Size,
                                          MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        // Unit quad covering [0,1] x [0,1] (two triangles, CCW)
        sprite_vertex *V = RenderCommands.SpriteVertexBuffer.Vertices;
        V[0] = { V2(0.0f, 0.0f), V2(0.0f, 0.0f) };
        V[1] = { V2(1.0f, 0.0f), V2(1.0f, 0.0f) };
        V[2] = { V2(0.0f, 1.0f), V2(0.0f, 1.0f) };
        V[3] = { V2(0.0f, 1.0f), V2(0.0f, 1.0f) };
        V[4] = { V2(1.0f, 0.0f), V2(1.0f, 0.0f) };
        V[5] = { V2(1.0f, 1.0f), V2(1.0f, 1.0f) };
        RenderCommands.SpriteVertexBuffer.VertexCount = VertexCount;
    }

    ID3D11Buffer *WindowsSpriteVertexBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth      = RenderCommands.SpriteVertexBuffer.Size;
        Desc.Usage          = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA Initial = {};
        Initial.pSysMem = RenderCommands.SpriteVertexBuffer.Vertices;
        HR = D11Device->CreateBuffer(&Desc, &Initial, &WindowsSpriteVertexBuffer);
        AssertHR(HR);
    }

    // 2D sprite shader
    ID3D11InputLayout  *PixelArtShaderInputLayout;
    ID3D11VertexShader *PixelArtVertexShader;
    ID3D11PixelShader  *PixelArtFragmentShader;
    {
        D3D11_INPUT_ELEMENT_DESC Desc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
              offsetof(struct sprite_vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
              offsetof(struct sprite_vertex, UV), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "VMIN",      0, DXGI_FORMAT_R32G32_FLOAT, 1,
              offsetof(struct texture_draw_command_instance_uniforms, vMin), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "VMAX",      0, DXGI_FORMAT_R32G32_FLOAT, 1,
              offsetof(struct texture_draw_command_instance_uniforms, vMax), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "ROTATION",  0, DXGI_FORMAT_R32_FLOAT,    1,
              offsetof(struct texture_draw_command_instance_uniforms, Rotation), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEXTUREID", 0, DXGI_FORMAT_R32_UINT,     1,
              offsetof(struct texture_draw_command_instance_uniforms, TextureID), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "ALPHA",     0, DXGI_FORMAT_R32_FLOAT,    1,
              offsetof(struct texture_draw_command_instance_uniforms, Alpha), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "PROGRESS",  0, DXGI_FORMAT_R32_FLOAT,    1,
              offsetof(struct texture_draw_command_instance_uniforms, Progress), D3D11_INPUT_PER_INSTANCE_DATA, 1 }
        };

        #include "pixel_art_vertex_shader.h"
        HR = D11Device->CreateVertexShader(pixel_art_vertex_shader, sizeof(pixel_art_vertex_shader),
                                           NULL, &PixelArtVertexShader);
        AssertHR(HR);

        #include "pixel_art_fragment_shader.h"
        HR = D11Device->CreatePixelShader(pixel_art_fragment_shader, sizeof(pixel_art_fragment_shader),
                                          NULL, &PixelArtFragmentShader);
        AssertHR(HR);

        HR = D11Device->CreateInputLayout(Desc, _countof(Desc), pixel_art_vertex_shader,
                                          sizeof(pixel_art_vertex_shader), &PixelArtShaderInputLayout);
        AssertHR(HR);
    }

    RenderCommands.InstanceBuffer.InstanceMax = 5000;
    ID3D11Buffer *WindowsInstanceBuffer = SetupInstanceBuffer(D11Device, &RenderCommands.InstanceBuffer);

    ID3D11Buffer *ViewportSizeConstantsBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth      = 2 * 2 * sizeof(float);
        Desc.Usage          = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HR = D11Device->CreateBuffer(&Desc, NULL, &ViewportSizeConstantsBuffer);
        AssertHR(HR);
    }

    ID3D11Buffer *TextureSizeConstantBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth      = 4 * 4 * sizeof(float);
        Desc.Usage          = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HR = D11Device->CreateBuffer(&Desc, NULL, &TextureSizeConstantBuffer);
        AssertHR(HR);
    }

    ID3D11SamplerState *SpriteSampler;
    {
        D3D11_SAMPLER_DESC Desc = {};
        Desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        Desc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
        Desc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
        Desc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
        Desc.MinLOD         = -FLT_MAX;
        Desc.MaxLOD         =  FLT_MAX;
        Desc.MaxAnisotropy  = 1;
        Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        HR = D11Device->CreateSamplerState(&Desc, &SpriteSampler);
        AssertHR(HR);
    }

    ID3D11BlendState *SpriteBlendState;
    {
        D3D11_BLEND_DESC Desc = {};
        D3D11_RENDER_TARGET_BLEND_DESC RT = {};
        RT.BlendEnable           = TRUE;
        RT.SrcBlend              = D3D11_BLEND_SRC_ALPHA;
        RT.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
        RT.BlendOp               = D3D11_BLEND_OP_ADD;
        RT.SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;
        RT.DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
        RT.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        RT.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        Desc.RenderTarget[0] = RT;
        HR = D11Device->CreateBlendState(&Desc, &SpriteBlendState);
        AssertHR(HR);
    }

    ID3D11RasterizerState *SpriteRasterizerState;
    {
        D3D11_RASTERIZER_DESC Desc = {};
        Desc.FillMode = D3D11_FILL_SOLID;
        Desc.CullMode = D3D11_CULL_NONE;
        HR = D11Device->CreateRasterizerState(&Desc, &SpriteRasterizerState);
        AssertHR(HR);
    }

    // Placeholder sprite shader resource views (game does not yet load 2D atlases)
    ID3D11ShaderResourceView *SpriteShaderViews[2] = {};
    game_texture_buffer       SpriteTextureBuffers[2] = {};

    // ---- 3D pipeline setup ----

    u32 FlatColorVertexBufferSize = sizeof(game_flat_color_vertex) * 2000;
    game_flat_color_vertex *FlatColorVertices =
        (game_flat_color_vertex *)VirtualAlloc(0, FlatColorVertexBufferSize,
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.FlatColorVertexBuffer.Vertices = FlatColorVertices;
    WindowsFlatColorVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, FlatColorVertexBufferSize,
                                                                         FlatColorVertices);

    u32 TextureVertexBufferSize = sizeof(game_texture_vertex) * 2000;
    game_texture_vertex *TextureVertices =
        (game_texture_vertex *)VirtualAlloc(0, TextureVertexBufferSize,
                                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.TextureVertexBuffer.Vertices = TextureVertices;
    WindowsTextureVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, TextureVertexBufferSize,
                                                                       TextureVertices);

    u32 LoadedModelVertexBufferSize = sizeof(game_texture_vertex) * 10000;
    game_texture_vertex *LoadedModelVertices =
        (game_texture_vertex *)VirtualAlloc(0, LoadedModelVertexBufferSize,
                                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.LoadedModelVertexBuffer.Vertices = LoadedModelVertices;
    WindowsLoadedModelVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, LoadedModelVertexBufferSize,
                                                                           LoadedModelVertices);

    u32 IndexBufferSize  = sizeof(u32) * 10000;
    u32 *LoadedModelIndices = (u32 *)VirtualAlloc(0, IndexBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.LoadedModelVertexBuffer.Indices = LoadedModelIndices;

    ID3D11Buffer *WindowsLoadedModelIndexBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth      = IndexBufferSize;
        Desc.Usage          = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA Initial = {};
        Initial.pSysMem = RenderCommands.LoadedModelVertexBuffer.Indices;
        HR = D11Device->CreateBuffer(&Desc, &Initial, &WindowsLoadedModelIndexBuffer);
        AssertHR(HR);
    }

    // 3D flat-color shader
    ID3D11InputLayout  *FlatColorLayout;
    ID3D11VertexShader *FlatColorVShader;
    ID3D11PixelShader  *FlatColorPShader;
    {
        D3D11_INPUT_ELEMENT_DESC FlatColorLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              offsetof(struct game_flat_color_vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              offsetof(struct game_flat_color_vertex, Normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              offsetof(struct game_flat_color_vertex, Color),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        #include "d3d11_vshader_flat.h"
        HR = D11Device->CreateVertexShader(d3d11_vshader_flat, sizeof(d3d11_vshader_flat),
                                           NULL, &FlatColorVShader);
        AssertHR(HR);

        #include "d3d11_pshader_flat.h"
        HR = D11Device->CreatePixelShader(d3d11_pshader_flat, sizeof(d3d11_pshader_flat),
                                          NULL, &FlatColorPShader);
        AssertHR(HR);

        HR = D11Device->CreateInputLayout(FlatColorLayoutDesc, _countof(FlatColorLayoutDesc),
                                          d3d11_vshader_flat, sizeof(d3d11_vshader_flat), &FlatColorLayout);
        AssertHR(HR);
    }

    // 3D textured shader
    ID3D11InputLayout  *TexturedLayout;
    ID3D11VertexShader *TexturedVShader;
    ID3D11PixelShader  *TexturedPShader;
    {
        D3D11_INPUT_ELEMENT_DESC TexturedLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              offsetof(struct game_texture_vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              offsetof(struct game_texture_vertex, Normal),   D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "UV",       0, DXGI_FORMAT_R32G32_FLOAT,    0,
              offsetof(struct game_texture_vertex, UV),       D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        #include "d3d11_vshader_textured.h"
        HR = D11Device->CreateVertexShader(d3d11_vshader_textured, sizeof(d3d11_vshader_textured),
                                           NULL, &TexturedVShader);
        AssertHR(HR);

        #include "d3d11_pshader_textured.h"
        HR = D11Device->CreatePixelShader(d3d11_pshader_textured, sizeof(d3d11_pshader_textured),
                                          NULL, &TexturedPShader);
        AssertHR(HR);

        HR = D11Device->CreateInputLayout(TexturedLayoutDesc, _countof(TexturedLayoutDesc),
                                          d3d11_vshader_textured, sizeof(d3d11_vshader_textured), &TexturedLayout);
        AssertHR(HR);
    }

    // 3D constants buffer (per-draw transform / view / proj / light)
    ID3D11Buffer *ConstantsBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth      = (sizeof(game_constants) + 0xf) & 0xfffffff0;
        Desc.Usage          = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HR = D11Device->CreateBuffer(&Desc, NULL, &ConstantsBuffer);
        AssertHR(HR);
    }

    // 3D rasterizer (back-face cull, CCW front face to match OBJ winding)
    ID3D11RasterizerState *RasterizerState3D;
    {
        D3D11_RASTERIZER_DESC Desc = {};
        Desc.FillMode             = D3D11_FILL_SOLID;
        Desc.CullMode             = D3D11_CULL_BACK;
        Desc.FrontCounterClockwise = TRUE;
        HR = D11Device->CreateRasterizerState(&Desc, &RasterizerState3D);
        AssertHR(HR);
    }

    // Depth stencil state for 3D
    ID3D11DepthStencilState *DepthState;
    {
        D3D11_DEPTH_STENCIL_DESC Desc = {};
        Desc.DepthEnable    = TRUE;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        Desc.DepthFunc      = D3D11_COMPARISON_LESS;
        HR = D11Device->CreateDepthStencilState(&Desc, &DepthState);
        AssertHR(HR);
    }

    // 3D sampler (point filter, wrap — texel-perfect 3D textures)
    ID3D11SamplerState *Sampler3D;
    {
        D3D11_SAMPLER_DESC Desc = {};
        Desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
        Desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        HR = D11Device->CreateSamplerState(&Desc, &Sampler3D);
        AssertHR(HR);
    }

    GameClearMemoryArena(&GameMemory);

    // Load 3D textures
    game_texture_buffer TextureBuffer = {};
    TextureBuffer.Max = 2;
    GameLoadTextures(&GameMemory, &TextureBuffer);

    for (u32 TexIndex = 0; TexIndex < 2; TexIndex++)
    {
        ShaderResourceViews3D[TexIndex] = SetupShaderResourceViewFromTexture(D11Device,
                                                                              &TextureBuffer.Textures[TexIndex]);
    }

    GameClearMemoryArena(&GameMemory);

    // Load 3D models into vertex / index buffers
    u32 InstancedMeshBufferSize = 200;
    InitializeMeshInstanceBufferWindows(&RenderCommands.FlatColorMeshInstances,   InstancedMeshBufferSize);
    InitializeMeshInstanceBufferWindows(&RenderCommands.TexturedMeshInstances,    InstancedMeshBufferSize);
    InitializeMeshInstanceBufferWindows(&RenderCommands.LoadedModelMeshInstances, InstancedMeshBufferSize);

    GameLoad3DModels(&GameMemory, &RenderCommands);

    TransferVertexBufferContents(DeviceContext, WindowsFlatColorVertexBuffer,
                                 RenderCommands.FlatColorVertexBuffer.Vertices, FlatColorVertexBufferSize);
    TransferVertexBufferContents(DeviceContext, WindowsTextureVertexBuffer,
                                 RenderCommands.TextureVertexBuffer.Vertices, TextureVertexBufferSize);
    TransferVertexBufferContents(DeviceContext, WindowsLoadedModelVertexBuffer,
                                 RenderCommands.LoadedModelVertexBuffer.Vertices, LoadedModelVertexBufferSize);

    {
        D3D11_MAPPED_SUBRESOURCE Mapped;
        HR = DeviceContext->Map((ID3D11Resource*)WindowsLoadedModelIndexBuffer, 0,
                                 D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
        AssertHR(HR);
        memcpy(Mapped.pData, RenderCommands.LoadedModelVertexBuffer.Indices, IndexBufferSize);
        DeviceContext->Unmap((ID3D11Resource*)WindowsLoadedModelIndexBuffer, 0);
    }

    // Show window
    ShowWindow(WindowHandle, SW_SHOWDEFAULT);

    ID3D11RenderTargetView *RTView = NULL;
    ID3D11DepthStencilView *DSView = NULL;
    DWORD CurrentWidth  = 0;
    DWORD CurrentHeight = 0;

    LARGE_INTEGER C1 = WindowsGetTimeCounter();
    r32 dtAccumulator = 0.0f;
    r32 LastFrameDuration = 1.0f / 60.0f;
    LARGE_INTEGER TimeOfLastFrame = WindowsGetTimeCounter();

    update_interval = 10;
    next_update     = GetTickCount();

    while (!quitting)
    {
        SwitchToFiber(message_fiber);
        DWORD now = GetTickCount();

        if (now >= next_update)
        {
            // ---- XInput controller ----
            XINPUT_STATE XInputState;
            if (XInputGetState(0, &XInputState) == ERROR_SUCCESS)
            {
                game_controller_input *Ctrl = &Input.Controller;
                ProcessButton(&Ctrl->A,             (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A)              != 0);
                ProcessButton(&Ctrl->B,             (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B)              != 0);
                ProcessButton(&Ctrl->X,             (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X)              != 0);
                ProcessButton(&Ctrl->Y,             (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)              != 0);
                ProcessButton(&Ctrl->Up,            (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)        != 0);
                ProcessButton(&Ctrl->Down,          (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      != 0);
                ProcessButton(&Ctrl->Left,          (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      != 0);
                ProcessButton(&Ctrl->Right,         (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     != 0);
                ProcessButton(&Ctrl->RightShoulder1,(XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
                ProcessButton(&Ctrl->LeftShoulder1, (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0);
                ProcessButton(&Ctrl->RightShoulder2,(XInputState.Gamepad.bRightTrigger > 30));
                ProcessButton(&Ctrl->LeftShoulder2, (XInputState.Gamepad.bLeftTrigger  > 30));
                ProcessButton(&Ctrl->Select,        (XInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START)          != 0);

                r32 RightX    = (r32)XInputState.Gamepad.sThumbRX;
                r32 RightY    = (r32)XInputState.Gamepad.sThumbRY;
                r32 Magnitude = sqrtf(RightX * RightX + RightY * RightY);
                s32 Deadzone  = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

                if (Magnitude < (r32)Deadzone)
                {
                    Ctrl->RightThumb = V2(0.0f, 0.0f);
                }
                else
                {
                    r32 Max            = 32767.0f;
                    r32 AdjMagnitude   = (Magnitude - (r32)Deadzone) / (Max - (r32)Deadzone);
                    Ctrl->RightThumb   = V2((RightX / Max) * AdjMagnitude,
                                            (RightY / Max) * AdjMagnitude);
                }
            }

            // ---- Mouse input ----
            v2 WindowDim = WindowsGetWindowDimension(WindowHandle);
            POINT MousePoint;
            GetCursorPos(&MousePoint);
            ScreenToClient(WindowHandle, &MousePoint);
            Input.MousePos  = V2((r32)MousePoint.x, (r32)MousePoint.y);
            Input.WindowDim = WindowDim;

            b32 LocalHasFocus = GlobalHasFocus;
            if (GlobalHasFocus)
            {
                b32 MouseButtonsAreDown[5] = {
                    ((GetKeyState(VK_LBUTTON)  & (1 << 15)) != 0),
                    ((GetKeyState(VK_MBUTTON)  & (1 << 15)) != 0),
                    ((GetKeyState(VK_RBUTTON)  & (1 << 15)) != 0),
                    ((GetKeyState(VK_XBUTTON1) & (1 << 15)) != 0),
                    ((GetKeyState(VK_XBUTTON2) & (1 << 15)) != 0)
                };
                for (s32 i = 0; i < 5; i++)
                {
                    if (GlobalMouseButtons[i].EndedDown && !MouseButtonsAreDown[i])
                        ProcessButton((button_state *)&GlobalMouseButtons[i], false);
                }
            }
            else
            {
                for (s32 i = 0; i < 5; i++)
                    ProcessButton((button_state *)&GlobalMouseButtons[i], false);
            }

            for (s32 i = 0; i < 5; i++)
            {
                ProcessButton(&Input.MouseButtons[i], GlobalMouseButtons[i].EndedDown);
                GlobalMouseButtons[i].TransitionCount = 0;
            }

            // ---- Swap chain resize ----
            GetClientRect(WindowHandle, (RECT *)&WindowRect);
            int NewWidth  = WindowRect.right  - WindowRect.left;
            int NewHeight = WindowRect.bottom - WindowRect.top;

            if (RTView == NULL || (DWORD)NewWidth != CurrentWidth || (DWORD)NewHeight != CurrentHeight)
            {
                if (RTView)
                {
                    DeviceContext->ClearState();
                    RTView->Release();
                    DSView->Release();
                    RTView = NULL;
                    DSView = NULL;
                }

                WindowWidth  = NewWidth;
                WindowHeight = NewHeight;

                if (WindowWidth != 0 && WindowHeight != 0)
                {
                    HR = SwapChain->ResizeBuffers(0, WindowWidth, WindowHeight, DXGI_FORMAT_UNKNOWN, 0);
                    if (FAILED(HR)) FatalError("Failed to resize swap chain!");

                    D3D11_RENDER_TARGET_VIEW_DESC RTDesc = {};
                    RTDesc.Format        = DXGI_FORMAT_R8G8B8A8_UNORM;
                    RTDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

                    ID3D11Texture2D *Backbuffer;
                    HR = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&Backbuffer);
                    AssertHR(HR);
                    HR = D11Device->CreateRenderTargetView((ID3D11Resource*)Backbuffer, &RTDesc, &RTView);
                    AssertHR(HR);
                    Backbuffer->Release();

                    D3D11_TEXTURE2D_DESC DepthDesc = {};
                    DepthDesc.Width     = WindowWidth;
                    DepthDesc.Height    = WindowHeight;
                    DepthDesc.MipLevels = 1;
                    DepthDesc.ArraySize = 1;
                    DepthDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
                    DepthDesc.SampleDesc = { 1, 0 };
                    DepthDesc.Usage     = D3D11_USAGE_DEFAULT;
                    DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

                    ID3D11Texture2D *Depth;
                    HR = D11Device->CreateTexture2D(&DepthDesc, NULL, &Depth);
                    AssertHR(HR);
                    HR = D11Device->CreateDepthStencilView((ID3D11Resource*)Depth, NULL, &DSView);
                    AssertHR(HR);
                    Depth->Release();
                }

                CurrentWidth  = WindowWidth;
                CurrentHeight = WindowHeight;
            }

            AdjustViewportWithUpdatedWindowSize(&RenderCommands, (r32)WindowWidth, (r32)WindowHeight);

            if (RTView)
            {
                LARGE_INTEGER C2 = WindowsGetTimeCounter();
                r32 Delta = WindowsGetSecondsElapsed(C1, C2);
                C1 = C2;

                dtAccumulator += Delta;

                D3D11_VIEWPORT Viewport = {};
                Viewport.TopLeftX = 0;
                Viewport.TopLeftY = 0;
                Viewport.Width    = (FLOAT)WindowWidth;
                Viewport.Height   = (FLOAT)WindowHeight;
                Viewport.MinDepth = 0;
                Viewport.MaxDepth = 1;

                // Fixed-timestep update loop
                while (dtAccumulator >= FIXED_DT)
                {
                    dtAccumulator -= FIXED_DT;
                    Input.dtForFrame        = FIXED_DT;
                    Input.FrameRateMultiplier = 1.0f;
                    Input.HasFocusPrev      = Input.HasFocus;
                    Input.HasFocus          = LocalHasFocus;

                    GameUpdateAndRender(&Thread, &GameMemory, &Input, &RenderCommands);

                    LocalHasFocus = GlobalHasFocus;
                    ResetInput(&Input);
                }

                // Clear framebuffer and depth buffer
                clear_color *GameClearColor = &RenderCommands.ClearColor;
                FLOAT ClearColor[4] = {
                    GameClearColor->RGBA[0],
                    GameClearColor->RGBA[1],
                    GameClearColor->RGBA[2],
                    GameClearColor->RGBA[3]
                };
                DeviceContext->ClearRenderTargetView(RTView, ClearColor);
                DeviceContext->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

                // ---- DirectSound audio (TODO: enable when GameGetSoundSamples is implemented) ----
                // if (SoundIsEnabled) { ... }

                // ---- 3D render pass (depth-tested, no blending) ----
                {
                    DeviceContext->RSSetViewports(1, &Viewport);
                    DeviceContext->RSSetState(RasterizerState3D);
                    DeviceContext->OMSetDepthStencilState(DepthState, 0);
                    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
                    DeviceContext->OMSetRenderTargets(1, &RTView, DSView);

                    // Flat-color 3D meshes
                    DeviceContext->IASetInputLayout(FlatColorLayout);
                    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    {
                        UINT Stride = sizeof(game_flat_color_vertex), Offset = 0;
                        DeviceContext->IASetVertexBuffers(0, 1, &WindowsFlatColorVertexBuffer, &Stride, &Offset);
                    }
                    DeviceContext->VSSetShader(FlatColorVShader, NULL, 0);
                    DeviceContext->VSSetConstantBuffers(0, 1, &ConstantsBuffer);
                    DeviceContext->PSSetShader(FlatColorPShader, NULL, 0);

                    DrawMeshesFromInstanceBuffer(DeviceContext, ConstantsBuffer,
                                                 &RenderCommands.FlatColorMeshInstances,
                                                 &RenderCommands.FlatColorVertexBuffer);

                    // Textured 3D meshes
                    DeviceContext->IASetInputLayout(TexturedLayout);
                    {
                        UINT Stride = sizeof(game_texture_vertex), Offset = 0;
                        DeviceContext->IASetVertexBuffers(0, 1, &WindowsTextureVertexBuffer, &Stride, &Offset);
                    }
                    DeviceContext->VSSetShader(TexturedVShader, NULL, 0);
                    DeviceContext->VSSetConstantBuffers(0, 1, &ConstantsBuffer);
                    DeviceContext->PSSetShader(TexturedPShader, NULL, 0);
                    DeviceContext->PSSetSamplers(0, 1, &Sampler3D);
                    DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceViews3D[1]);

                    DrawMeshesFromInstanceBuffer(DeviceContext, ConstantsBuffer,
                                                 &RenderCommands.TexturedMeshInstances,
                                                 &RenderCommands.TextureVertexBuffer);

                    // Loaded OBJ models (indexed)
                    DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceViews3D[0]);
                    {
                        UINT Stride = sizeof(game_texture_vertex), Offset = 0;
                        DeviceContext->IASetVertexBuffers(0, 1, &WindowsLoadedModelVertexBuffer, &Stride, &Offset);
                    }
                    DeviceContext->IASetIndexBuffer(WindowsLoadedModelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

                    mesh_instance_buffer *LoadedModelInstances = &RenderCommands.LoadedModelMeshInstances;
                    for (u32 Index = 0; Index < LoadedModelInstances->MeshCount; Index++)
                    {
                        mesh_instance *MeshInstance = &LoadedModelInstances->Meshes[Index];
                        {
                            D3D11_MAPPED_SUBRESOURCE Mapped;
                            HR = DeviceContext->Map((ID3D11Resource*)ConstantsBuffer, 0,
                                                     D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
                            AssertHR(HR);
                            memcpy(Mapped.pData, &MeshInstance->Constants, sizeof(game_constants));
                            DeviceContext->Unmap((ID3D11Resource*)ConstantsBuffer, 0);
                        }
                        DeviceContext->DrawIndexed(RenderCommands.LoadedModelVertexBuffer.IndexCount, 0, 0);
                    }
                }

                // ---- 2D sprite render pass (no depth test, alpha blend) ----
                {
                    DeviceContext->RSSetState(SpriteRasterizerState);
                    DeviceContext->OMSetDepthStencilState(nullptr, 0);
                    DeviceContext->OMSetBlendState(SpriteBlendState, NULL, ~0U);
                    DeviceContext->OMSetRenderTargets(1, &RTView, NULL);

                    DeviceContext->IASetInputLayout(PixelArtShaderInputLayout);
                    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    DeviceContext->VSSetShader(PixelArtVertexShader, NULL, 0);
                    DeviceContext->VSSetConstantBuffers(0, 1, &ViewportSizeConstantsBuffer);
                    DeviceContext->PSSetShader(PixelArtFragmentShader, NULL, 0);
                    DeviceContext->PSSetConstantBuffers(1, 1, &TextureSizeConstantBuffer);
                    DeviceContext->PSSetSamplers(0, 1, &SpriteSampler);

                    {
                        UINT Stride = sizeof(sprite_vertex), Offset = 0;
                        DeviceContext->IASetVertexBuffers(0, 1, &WindowsSpriteVertexBuffer, &Stride, &Offset);
                    }

                    // Update viewport constant buffer
                    {
                        float VPSize[] = { (float)WindowWidth, (float)WindowHeight };
                        D3D11_MAPPED_SUBRESOURCE Mapped;
                        HR = DeviceContext->Map((ID3D11Resource*)ViewportSizeConstantsBuffer,
                                                 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
                        AssertHR(HR);
                        memcpy(Mapped.pData, VPSize, sizeof(VPSize));
                        DeviceContext->Unmap((ID3D11Resource*)ViewportSizeConstantsBuffer, 0);
                    }

                    TransferInstanceBufferToGPU(DeviceContext, WindowsInstanceBuffer,
                                                &RenderCommands.InstanceBuffer);

                    ResetDrawCommandBuffersBeforePlatformRender(&RenderCommands);

                    u32 VerticesInAQuad  = 6;
                    u32 PrevInstanceCount = 0;
                    u32 InstancesToDraw  = 0;

                    game_texture_draw_command_buffer *DrawBuf = &RenderCommands.DrawCommandsBuffer;
                    texture_atlas_type CurrentAtlasType = DrawBuf->BottomLayers[0].Commands
                                                          ? DrawBuf->BottomLayers[0].Commands[0].TextureAtlasType
                                                          : TextureAtlasType_Tile24;

                    UINT InstanceStride = sizeof(texture_draw_command_instance_uniforms);
                    UINT InstanceOffset = 0;

                    for (s32 LayerIndex = 0; LayerIndex < BOTTOM_LAYER_COUNT; LayerIndex++)
                    {
                        render_commands_array *Array = &DrawBuf->BottomLayers[LayerIndex];

                        for (s32 CmdIndex = 0; CmdIndex < (s32)Array->CommandCount; CmdIndex++)
                        {
                            game_texture_draw_command Cmd = Array->Commands[CmdIndex];

                            if (Cmd.TextureAtlasType != CurrentAtlasType && SpriteShaderViews[0])
                            {
                                windows_sprite_draw_data Data =
                                    GetSpriteDrawDataForAtlasType(CurrentAtlasType, SpriteShaderViews, SpriteTextureBuffers);
                                DeviceContext->PSSetShaderResources(0, 1, &Data.ShaderView);
                                DeviceContext->IASetVertexBuffers(1, 1, &WindowsInstanceBuffer,
                                                                   &InstanceStride, &InstanceOffset);
                                TransferTextureSizeToGPU(DeviceContext, TextureSizeConstantBuffer, Data.TextureSize);
                                DeviceContext->DrawInstanced(VerticesInAQuad, InstancesToDraw, 0, PrevInstanceCount);
                                PrevInstanceCount += InstancesToDraw;
                                InstancesToDraw    = 0;
                            }

                            CurrentAtlasType = Cmd.TextureAtlasType;
                            InstancesToDraw++;
                        }
                    }

                    if (InstancesToDraw > 0 && SpriteShaderViews[0])
                    {
                        windows_sprite_draw_data Data =
                            GetSpriteDrawDataForAtlasType(CurrentAtlasType, SpriteShaderViews, SpriteTextureBuffers);
                        DeviceContext->PSSetShaderResources(0, 1, &Data.ShaderView);
                        DeviceContext->IASetVertexBuffers(1, 1, &WindowsInstanceBuffer,
                                                           &InstanceStride, &InstanceOffset);
                        TransferTextureSizeToGPU(DeviceContext, TextureSizeConstantBuffer, Data.TextureSize);
                        DeviceContext->DrawInstanced(VerticesInAQuad, InstancesToDraw, 0, PrevInstanceCount);
                    }
                }
            }

            BOOL Vsync = TRUE;
            HR = SwapChain->Present(Vsync ? 1 : 0, 0);

            if (HR == DXGI_STATUS_OCCLUDED)
            {
                if (Vsync) Sleep(10);
            }
            else if (FAILED(HR))
            {
                FatalError("Failed to present swap chain! Device lost?");
            }

            {
                LARGE_INTEGER Now = WindowsGetTimeCounter();
                LastFrameDuration = WindowsGetSecondsElapsed(TimeOfLastFrame, Now);
                TimeOfLastFrame   = Now;
            }
        }
    }

    return 0;
}
