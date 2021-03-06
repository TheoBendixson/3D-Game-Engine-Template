#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include <dsound.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <stdio.h>

//#define _USE_MATH_DEFINES
#include <string.h>
#include <intrin.h>
#define AssertHR(HR) Assert(SUCCEEDED(HR))

#define STR2(x) #x
#define STR(x) STR2(x)

#include <SDL.h>

#include "windows_main.h"

// TODO: (Ted)  Move to cross platform logic
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

PLATFORM_READ_PNG_FILE(PlatformReadPNGFile)
{
    read_file_result Result = {};

    int X,Y,N;
    u32 *ImageData = (u32 *)stbi_load(Filename, &X, &Y, &N, 0);

    if (X > 0 && Y > 0 && ImageData != NULL)
    {
        Result.Contents = ImageData;
        Result.ContentsSize = X*Y*sizeof(u32); 
    }

    return Result;
}

#include "../game_library/code/vario.cpp"

#include "windows_file_io.h"
#include "windows_d3d_resource_setup.h"
#include "windows_gpu_transfer.h"

static void FatalError(const char* message)
{
    MessageBoxA(NULL, message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}

PLATFORM_QUIT_GAME(PlatformQuitGame)
{
    ExitProcess(0);
}

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
typedef DIRECT_SOUND_CREATE(direct_sound_create);

bool quitting;
bool resizing;
HANDLE main_fiber;
DWORD update_interval;
DWORD next_update;

LRESULT CALLBACK
MainWindowCallback(HWND Window, 
                   UINT Message,
                   WPARAM WParam,
                   LPARAM LParam)
{
    LRESULT result = 0;
    switch (Message) {
        case WM_DESTROY:
            quitting = true;
            break;
        case WM_SIZE:
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

#define Z_KEY_CODE  0x5A
#define D_KEY_CODE  0x44
#define S_KEY_CODE  0x53

void CALLBACK MessageFiberProc(void *) 
{
    for (;;) 
    {
        DWORD now = GetTickCount();
        if (now >= next_update || MsgWaitForMultipleObjects(0, 0, FALSE, next_update - now, QS_ALLEVENTS) == WAIT_OBJECT_0) 
        {

            MSG Message;
            while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) 
            {
                u32 WMessage = Message.message;

                game_controller_input *KeyboardController = &Input.Controller;

                if (WMessage == WM_QUIT)
                {
                    break;
                } else if (WMessage == WM_SYSKEYDOWN)
                {

                } else if (WMessage == WM_SYSKEYUP)
                {

                } else if (WMessage == WM_KEYDOWN)
                {
                    u32 VirtualKeyCode = Message.wParam;

                    if (VirtualKeyCode == VK_RIGHT)
                    {
                        KeyboardController->Right.EndedDown = true;
                    } else if (VirtualKeyCode == VK_LEFT)
                    {
                        KeyboardController->Left.EndedDown = true;
                    } else if (VirtualKeyCode == VK_SPACE)
                    {
                        KeyboardController->A.EndedDown = true;
                    } else if (VirtualKeyCode == D_KEY_CODE)
                    {
                        KeyboardController->X.EndedDown = true;
                    } else if (VirtualKeyCode == S_KEY_CODE)
                    {
                        KeyboardController->B.EndedDown = true;
                    } else if (VirtualKeyCode == VK_UP)
                    {
                        KeyboardController->Up.EndedDown = true;
                    } 
                    else if (VirtualKeyCode == VK_DOWN)
                    {
                        KeyboardController->Down.EndedDown = true;
                    }
                    else if (VirtualKeyCode == VK_RETURN)
                    {
                        KeyboardController->Start.EndedDown = true;
                    }
                    else if (VirtualKeyCode == Z_KEY_CODE)
                    {
                       KeyboardController->LeftShoulder2.EndedDown = true;
                    }

                } else if (WMessage == WM_KEYUP)
                {
                    u32 VirtualKeyCode = Message.wParam;

                    if (VirtualKeyCode == VK_RIGHT)
                    {
                        KeyboardController->Right.EndedDown = false;
                    } else if (VirtualKeyCode == VK_LEFT)
                    {
                        KeyboardController->Left.EndedDown = false;
                    } else if (VirtualKeyCode == VK_SPACE)
                    {
                        KeyboardController->A.EndedDown = false;
                    } else if (VirtualKeyCode == D_KEY_CODE)
                    {
                        KeyboardController->X.EndedDown = false;
                    } else if (VirtualKeyCode == S_KEY_CODE)
                    {
                        KeyboardController->B.EndedDown = false;
                    } else if (VirtualKeyCode == VK_UP)
                    {
                        KeyboardController->Up.EndedDown = false;
                    } 
                    else if (VirtualKeyCode == VK_DOWN)
                    {
                        KeyboardController->Down.EndedDown = false;
                    }
                    else if (VirtualKeyCode == VK_RETURN)
                    {
                        KeyboardController->Start.EndedDown = false;
                    }
                    else if (VirtualKeyCode == Z_KEY_CODE)
                    {
                       KeyboardController->LeftShoulder2.EndedDown = false;
                    }

                }

                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }

        }


        SwitchToFiber(main_fiber);
    }
}

global_variable ID3D11Buffer *WindowsFlatColorVertexBuffer;
global_variable ID3D11Buffer *WindowsTextureVertexBuffer;
global_variable ID3D11Buffer *WindowsLoadedModelVertexBuffer;

internal
void DrawMeshesFromInstanceBuffer(ID3D11DeviceContext *DeviceContext, ID3D11Buffer *ConstantsBuffer,
                                  mesh_instance_buffer *MeshBuffer, game_vertex_buffer *VertexBuffer)
{
    for (u32 Index = 0;
         Index < MeshBuffer->MeshCount;
         Index++)
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
    InstanceBuffer->MeshMax = MaxMeshes;
    InstanceBuffer->Meshes = (mesh_instance *)VirtualAlloc(0, MaxMeshes*sizeof(mesh_instance),
                                                           MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

#define MAX_SUPPORTED_TEXTURE_TYPES 2
global_variable ID3D11ShaderResourceView* ShaderResourceViews[MAX_SUPPORTED_TEXTURE_TYPES];

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    main_fiber = ConvertThreadToFiber(0);
    HANDLE message_fiber = CreateFiber(0, MessageFiberProc, 0);

    game_memory GameMemory = {};

    // TODO: (Ted)  Set these up in the cross-platform code.
    GameMemory.PermanentStorageSize = Megabytes(1024);
    GameMemory.TransientStorageSize = Megabytes(128);
    GameMemory.PermanentStorage = VirtualAlloc(0, GameMemory.PermanentStorageSize,
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    GameMemory.TransientStorage = VirtualAlloc(0, GameMemory.TransientStorageSize,
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!GameMemory.PermanentStorage || !GameMemory.TransientStorage)
    {
        return(0);
    }

    GameSetupMemoryPartitions(&GameMemory);

    GameMemory.PlatformReadEntireFile = PlatformReadEntireFile;
    GameMemory.PlatformWriteEntireFile = PlatformWriteEntireFile;
    GameMemory.PlatformFreeFileMemory = PlatformFreeFileMemory;

    thread_context Thread = {};

// register window class to have custom WindowProc callback
    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = L"d3d11_window_class";

    ATOM atom = RegisterClassExW(&WindowClass);
    Assert(atom && "Failed to register window class");
    
// window properties - width, height and style
    DWORD Exstyle = WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP;
    DWORD Style = WS_OVERLAPPED | WS_SYSMENU;
   
    RECT ScreenRect;
    GetWindowRect(GetDesktopWindow(), &ScreenRect);
    int ScreenHeight = ScreenRect.bottom - ScreenRect.top - 32;
    int ScreenWidth = (ScreenRect.right - ScreenRect.left) + 18;

    // uncomment in case you want fixed size window
    RECT WindowRect = { 0, 0, ScreenWidth, ScreenHeight };
    AdjustWindowRectEx(&WindowRect, Style, FALSE, Exstyle);

    int WindowWidth = WindowRect.right - WindowRect.left;
    int WindowHeight = WindowRect.bottom - WindowRect.top;

    // create window
    HWND WindowHandle = CreateWindowExW(Exstyle, WindowClass.lpszClassName, (LPCWSTR)"Varios Temple", Style, 
                                        -10, 0, WindowWidth, WindowHeight, NULL, NULL, Instance, NULL);
    Assert(WindowHandle && "Failed to create window");

    HRESULT HR;

    DwmGetWindowAttribute(WindowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &WindowRect, sizeof(WindowRect));

    WindowWidth = WindowRect.right - WindowRect.left;
    WindowHeight = WindowRect.bottom - WindowRect.top;

    game_render_commands RenderCommands = {};
    RenderCommands.ViewportWidth = WindowWidth;
    RenderCommands.ViewportHeight = WindowHeight;

    // TODO: (Ted)  Once this is cross-platform, these buffers could be put on a single allocation up-front.
    u32 InstancedMeshBufferSize = 200;
    InitializeMeshInstanceBufferWindows(&RenderCommands.FlatColorMeshInstances, InstancedMeshBufferSize);
    InitializeMeshInstanceBufferWindows(&RenderCommands.TexturedMeshInstances, InstancedMeshBufferSize);
    InitializeMeshInstanceBufferWindows(&RenderCommands.LoadedModelMeshInstances, InstancedMeshBufferSize);

    HDC RefreshDC = GetDC(WindowHandle);
    int RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);
    r32 RefreshRateInFloat = (r32)RefreshRate;
    Input.dtForFrame = 1.0f/RefreshRateInFloat;
    Input.FrameRateMultiplier = RefreshRateInFloat/60.0f;

    // Initialize Direct Sound
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    LPDIRECTSOUNDBUFFER SecondaryBuffer;

    game_startup_config StartupConfig = GameGetStartupConfig();
    u32 SamplesPerSecond = StartupConfig.SoundSamplesPerSecond;
    u32 BytesPerSample = StartupConfig.SoundBytesPerSample; 
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
            DSBUFFERDESC PrimaryBufferDescription {};
            PrimaryBufferDescription.dwSize = sizeof(PrimaryBufferDescription);
            PrimaryBufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

            HR = DirectSound->CreateSoundBuffer(&PrimaryBufferDescription, &PrimaryBuffer, 0);
            AssertHR(HR);

            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 2;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
            WaveFormat.nBlockAlign = (WaveFormat.nChannels*WaveFormat.wBitsPerSample)/8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nBlockAlign*WaveFormat.nSamplesPerSec;
            WaveFormat.cbSize = 0;

            HR = PrimaryBuffer->SetFormat(&WaveFormat);
            AssertHR(HR);

            DSBUFFERDESC SecondaryBufferDescription {};
            SecondaryBufferDescription.dwSize = sizeof(SecondaryBufferDescription);
            SecondaryBufferDescription.dwFlags = 0;
            SecondaryBufferDescription.dwBufferBytes = SecondaryBufferSize;
            SecondaryBufferDescription.lpwfxFormat = &WaveFormat;

            HR = DirectSound->CreateSoundBuffer(&SecondaryBufferDescription, &SecondaryBuffer, 0);
            AssertHR(HR);

            HR = SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
            AssertHR(HR);

        } else 
        {
            // TODO: (Ted)  Handle Direct Sound Not Loaded
        }
    }

    u32 RunningSampleIndex = 0;

    game_sound_output_buffer SoundOutputBuffer = {};
    SoundOutputBuffer.Samples = (s16*)calloc(SamplesPerSecond, BytesPerSample); 
    SoundOutputBuffer.SamplesPerSecond = SamplesPerSecond;
    SoundOutputBuffer.SamplesToWriteThisFrame = 0;
    SoundOutputBuffer.SamplesWrittenThisFrame = 0;

// MARK: SDL Setup (Gamepad Only)

    SDL_Joystick *Joystick;

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
        OutputDebugString("SDL Failed To Initialize \n");
    } else
    {
        OutputDebugString("SDL Initialized \n");

        u32 JoystickCount = SDL_NumJoysticks();

        if (JoystickCount > 0)
        {
            SDL_JoystickEventState(SDL_ENABLE);
            Joystick = SDL_JoystickOpen(0);
        }
    }

    IDXGISwapChain* SwapChain;
    ID3D11Device* D11Device;
    ID3D11DeviceContext* DeviceContext;

 // create swap chain, device and context
    {
        DXGI_SWAP_CHAIN_DESC SwapChainDescription = {};
        SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDescription.SampleDesc = { 1, 0 };
        SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDescription.BufferCount = 2;
        SwapChainDescription.OutputWindow = WindowHandle;
        SwapChainDescription.Windowed = TRUE;
    
        // use more efficient flip model, available in Windows 10
        // if Windows 8 compatibility required, use DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL
        // if Windows 7/Vista compatibility required, use DXGI_SWAP_EFFECT_DISCARD
        // NOTE: flip models do not allow MSAA framebuffer, so if you want MSAA then
        // you'll need to render offscreen and afterwards resolve to non-MSAA framebuffer
        SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        UINT Flags = 0;
#ifndef NDEBUG
        Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0 };
        HR = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, Flags, Levels, 
                                           _countof(Levels), D3D11_SDK_VERSION, &SwapChainDescription, 
                                           &SwapChain, &D11Device, NULL, &DeviceContext);
        AssertHR(HR);
    }

#ifndef NDEBUG
    // for debug builds enable debug break on API errors
    {
        ID3D11Debug *D3DDebug;

        HR = D11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&D3DDebug);
        AssertHR(HR);

        ID3D11InfoQueue *D3DInfoQueue;
        HR = D11Device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&D3DInfoQueue);
        AssertHR(HR);

        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
        D3DInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);

        D3DInfoQueue->Release();
        D3DDebug->Release();
    }
#endif

    // disable stupid Alt+Enter changing monitor resolution to match window size
    {
        IDXGIFactory* Factory;
        HR = SwapChain->GetParent(__uuidof(IDXGIFactory), (void**) &Factory);
        AssertHR(HR);
        Factory->MakeWindowAssociation(WindowHandle, DXGI_MWA_NO_ALT_ENTER);
        Factory->Release();
    }

    u32 FlatColorVertexBufferSize = sizeof(game_flat_color_vertex)*2000;
    game_flat_color_vertex *FlatColorVertices = 
        (game_flat_color_vertex *)VirtualAlloc(0, FlatColorVertexBufferSize, 
                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.FlatColorVertexBuffer.Vertices = FlatColorVertices;
    WindowsFlatColorVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, FlatColorVertexBufferSize, 
                                                                         FlatColorVertices);

    u32 TextureVertexBufferSize = sizeof(game_texture_vertex)*2000;
    game_texture_vertex *TextureVertices =
        (game_texture_vertex *)VirtualAlloc(0, TextureVertexBufferSize, 
                                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.TextureVertexBuffer.Vertices = TextureVertices;
    WindowsTextureVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, TextureVertexBufferSize, 
                                                                       TextureVertices);

    u32 LoadedModelVertexBufferSize = sizeof(game_texture_vertex)*10000;
    game_texture_vertex *LoadedModelVertices =
        (game_texture_vertex *)VirtualAlloc(0, LoadedModelVertexBufferSize, 
                                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.LoadedModelVertexBuffer.Vertices = LoadedModelVertices;
    WindowsLoadedModelVertexBuffer = SetupVertexBufferFromGameVertexBuffer(D11Device, LoadedModelVertexBufferSize, 
                                                                           LoadedModelVertices);

    u32 IndexBufferSize = sizeof(u32)*10000;
    u32 *LoadedModelIndices = (u32 *)VirtualAlloc(0, IndexBufferSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    RenderCommands.LoadedModelVertexBuffer.Indices = LoadedModelIndices;

    ID3D11Buffer* WindowsLoadedModelIndexBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth = IndexBufferSize;
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA Initial = {};
        Initial.pSysMem = RenderCommands.LoadedModelVertexBuffer.Indices;
        HRESULT HR = D11Device->CreateBuffer(&Desc, &Initial, &WindowsLoadedModelIndexBuffer);
        AssertHR(HR);
    }
    
    // vertex & pixel shaders for drawing triangle, plus input layout for vertex input
    ID3D11InputLayout* FlatColorLayout;
    ID3D11VertexShader* FlatColorVShader;
    ID3D11PixelShader* FlatColorPShader;
    {
        // these must match vertex shader input layout
        D3D11_INPUT_ELEMENT_DESC FlatColorLayoutDesc[] =
        {
            { 
                "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                offsetof(struct game_flat_color_vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 
            },
            { 
                "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                offsetof(struct game_flat_color_vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 
            },
            { 
                "COLOR",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                offsetof(struct game_flat_color_vertex, Color), D3D11_INPUT_PER_VERTEX_DATA, 0 
            }
        };

        #include "d3d11_vshader_flat.h"
        HR = D11Device->CreateVertexShader(d3d11_vshader_flat, sizeof(d3d11_vshader_flat), NULL, &FlatColorVShader);
        AssertHR(HR);

        #include "d3d11_pshader_flat.h"
        HR = D11Device->CreatePixelShader(d3d11_pshader_flat, sizeof(d3d11_pshader_flat), NULL, &FlatColorPShader);
        AssertHR(HR);

        HR = D11Device->CreateInputLayout(FlatColorLayoutDesc, _countof(FlatColorLayoutDesc), d3d11_vshader_flat, 
                                          sizeof(d3d11_vshader_flat), &FlatColorLayout);
        AssertHR(HR);
    }

    ID3D11InputLayout* TexturedLayout;
    ID3D11VertexShader* TexturedVShader;
    ID3D11PixelShader* TexturedPShader;
    {
        // these must match vertex shader input layout
        D3D11_INPUT_ELEMENT_DESC TexturedLayoutDesc[] =
        {
            { 
                "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                offsetof(struct game_texture_vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 
            },
            { 
                "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                offsetof(struct game_texture_vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 
            },
            { 
                "UV",0, DXGI_FORMAT_R32G32_FLOAT, 0, 
                offsetof(struct game_texture_vertex, UV), D3D11_INPUT_PER_VERTEX_DATA, 0 
            }
        };

        #include "d3d11_vshader_textured.h"
        HR = D11Device->CreateVertexShader(d3d11_vshader_textured, sizeof(d3d11_vshader_textured), NULL, &TexturedVShader);
        AssertHR(HR);

        #include "d3d11_pshader_textured.h"
        HR = D11Device->CreatePixelShader(d3d11_pshader_textured, sizeof(d3d11_pshader_textured), NULL, &TexturedPShader);
        AssertHR(HR);

        HR = D11Device->CreateInputLayout(TexturedLayoutDesc, _countof(TexturedLayoutDesc), d3d11_vshader_textured, 
                                          sizeof(d3d11_vshader_textured), &TexturedLayout);
        AssertHR(HR);
    }

    ID3D11Buffer* ConstantsBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth = sizeof(game_constants) + 0xf & 0xfffffff0;
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        HR = D11Device->CreateBuffer(&Desc, NULL, &ConstantsBuffer);
        AssertHR(HR);
    }

    GameClearMemoryArena(&GameMemory);

    ID3D11RasterizerState* RasterizerState;
    {
        D3D11_RASTERIZER_DESC Desc = {};
        Desc.FillMode = D3D11_FILL_SOLID;
        Desc.CullMode = D3D11_CULL_BACK;
        Desc.FrontCounterClockwise = TRUE;
        HR = D11Device->CreateRasterizerState(&Desc, &RasterizerState);
        AssertHR(HR);
    }

    ID3D11DepthStencilState* DepthState;
    {
        D3D11_DEPTH_STENCIL_DESC Desc = {};
        Desc.DepthEnable = TRUE;
        Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        Desc.DepthFunc = D3D11_COMPARISON_LESS;
        HR = D11Device->CreateDepthStencilState(&Desc, &DepthState);
        AssertHR(HR);
    }

    ID3D11SamplerState* SamplerState;
    {
        D3D11_SAMPLER_DESC Desc = {};
        Desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
        Desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
        Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        HR = D11Device->CreateSamplerState(&Desc, &SamplerState);
        AssertHR(HR);
    }

    game_texture_buffer TextureBuffer = {};
    TextureBuffer.Max = 2;
    GameLoadTextures(&GameMemory, &TextureBuffer);

    for (u32 TextureIndex = 0;
         TextureIndex < 2;
         TextureIndex++)
    {
        ShaderResourceViews[TextureIndex] = SetupShaderResourceView(D11Device, &TextureBuffer.Textures[TextureIndex]);
    }

    GameClearMemoryArena(&GameMemory);

    ID3D11RenderTargetView* RTView = NULL;
    ID3D11DepthStencilView* DSView = NULL;

    // show the window
    ShowWindow(WindowHandle, SW_SHOWDEFAULT);

    LARGE_INTEGER Freq, C1;
    QueryPerformanceFrequency(&Freq);
    QueryPerformanceCounter(&C1);

    float Angle = 0;
    DWORD CurrentWidth = 0;
    DWORD CurrentHeight = 0;

    GameLoad3DModels(&GameMemory, &RenderCommands);

    TransferVertexBufferContents(DeviceContext, WindowsFlatColorVertexBuffer, 
                                 RenderCommands.FlatColorVertexBuffer.Vertices, FlatColorVertexBufferSize);

    TransferVertexBufferContents(DeviceContext, WindowsTextureVertexBuffer, 
                                 RenderCommands.TextureVertexBuffer.Vertices, TextureVertexBufferSize);


    TransferVertexBufferContents(DeviceContext, WindowsLoadedModelVertexBuffer, 
                                 RenderCommands.LoadedModelVertexBuffer.Vertices, LoadedModelVertexBufferSize);

    {
        D3D11_MAPPED_SUBRESOURCE Mapped;
        HRESULT HR = DeviceContext->Map((ID3D11Resource*)WindowsLoadedModelIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
        AssertHR(HR);
        memcpy(Mapped.pData, RenderCommands.LoadedModelVertexBuffer.Indices, IndexBufferSize);
        DeviceContext->Unmap((ID3D11Resource*)WindowsLoadedModelIndexBuffer, 0);
    }

    update_interval = 10;
    next_update = GetTickCount();

    while (!quitting)
    {
        SwitchToFiber(message_fiber);
        DWORD now = GetTickCount();

        if (now >= next_update) 
        {
            game_controller_input *GameController = &Input.Controller;

            SDL_Event Event;

            while(SDL_PollEvent(&Event)) 
            {
                if (Event.type == SDL_JOYBUTTONDOWN)
                {
                    if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
                    {
                        GameController->A.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_B)
                    {
                        GameController->B.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_X)
                    {
                        GameController->X.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_Y)
                    {
                        GameController->Y.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
                    {
                        GameController->LeftShoulder2.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    {
                        GameController->Right.EndedDown = true;
                    } else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    {
                        GameController->Left.EndedDown = true;
                    } else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                    {
                        GameController->Up.EndedDown = true;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
                    {
                        GameController->Start.EndedDown = true;
                    }

                } else if (Event.type == SDL_JOYBUTTONUP)
                {
                    if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_A)
                    {
                        GameController->A.EndedDown = false;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_B)
                    {
                        GameController->B.EndedDown = false;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_X)
                    {
                        GameController->X.EndedDown = false;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_Y)
                    {
                        GameController->Y.EndedDown = false;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
                    {
                        GameController->LeftShoulder2.EndedDown = false;
                    } 
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    {
                        GameController->Right.EndedDown = false;
                    } else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    {
                        GameController->Left.EndedDown = false;
                    } else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                    {
                        GameController->Up.EndedDown = false;
                    }
                    else if (Event.jbutton.button == SDL_CONTROLLER_BUTTON_START)
                    {
                        GameController->Start.EndedDown = false;
                    }
                } 
            }

            // get current window client area size
            RECT Rectangle;
            GetClientRect(WindowHandle, &Rectangle);
            WindowWidth = Rectangle.right - Rectangle.left;
            WindowHeight = Rectangle.bottom - Rectangle.top;
    
            // resize swap chain if needed
            if (RTView == NULL || WindowWidth != CurrentWidth || WindowHeight != CurrentHeight)
            {
                if (RTView)
                {
                    // release old swap chain buffers
                    DeviceContext->ClearState();
                    RTView->Release();
                    DSView->Release();
                    RTView = NULL;
                }

                if (WindowWidth != 0 && WindowHeight != 0)
                {
                    // resize to new size for non-zero sizes
                    HR = SwapChain->ResizeBuffers(0, WindowWidth, WindowHeight, DXGI_FORMAT_UNKNOWN, 0);

                    if (FAILED(HR))
                    {
                        FatalError("Failed to resize swap chain!");
                    }

                    D3D11_RENDER_TARGET_VIEW_DESC RTDesc = {};
                    RTDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // or use DXGI_FORMAT_R8G8B8A8_UNORM_SRGB for storing sRGB
                    RTDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

                    // create RenderTarget view for new backbuffer texture
                    ID3D11Texture2D* Backbuffer;
                    HR = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&Backbuffer);
                    AssertHR(HR);
                    HR = D11Device->CreateRenderTargetView((ID3D11Resource*)Backbuffer, &RTDesc, &RTView);
                    AssertHR(HR);
                    Backbuffer->Release();

                    D3D11_TEXTURE2D_DESC DepthDesc = {};
                    DepthDesc.Width = WindowWidth;
                    DepthDesc.Height = WindowHeight;
                    DepthDesc.MipLevels = 1;
                    DepthDesc.ArraySize = 1;
                    DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // or use DXGI_FORMAT_D32_FLOAT if you don't need stencil
                    DepthDesc.SampleDesc = { 1, 0 };
                    DepthDesc.Usage = D3D11_USAGE_DEFAULT;
                    DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

                    // create new depth stencil texture & DepthStencil view
                    ID3D11Texture2D* Depth;
                    HR = D11Device->CreateTexture2D(&DepthDesc, NULL, &Depth);
                    AssertHR(HR);
                    HR = D11Device->CreateDepthStencilView((ID3D11Resource*)Depth, NULL, &DSView);
                    AssertHR(HR);
                    Depth->Release();
                }

                CurrentWidth = WindowWidth;
                CurrentHeight = WindowHeight;
            }

            RenderCommands.ViewportWidth = WindowWidth;
            RenderCommands.ViewportHeight = WindowHeight;

            // can render only if window size is non-zero - we must have backbuffer & RenderTarget view created
            if (RTView)
            {
                LARGE_INTEGER C2;
                QueryPerformanceCounter(&C2);
                float Delta = (float)((double)(C2.QuadPart - C1.QuadPart) / Freq.QuadPart);

                /*
                char PerformanceCounterString[100];
                sprintf_s(PerformanceCounterString, "Performance Counter: %f \n", Delta);
                OutputDebugString(PerformanceCounterString);*/

                C1 = C2;

                LARGE_INTEGER WithinFrameCounter1;
                QueryPerformanceCounter(&WithinFrameCounter1);
    
                // output viewport covering all client area of window
                D3D11_VIEWPORT Viewport = {};
                Viewport.TopLeftX = 0;
                Viewport.TopLeftY = 0;
                Viewport.Width = (FLOAT)WindowWidth;
                Viewport.Height = (FLOAT)WindowHeight;
                Viewport.MinDepth = 0;
                Viewport.MaxDepth = 1;

                GameUpdateAndRender(&Thread, &GameMemory, 
                                    &Input, &RenderCommands);
                
                // clear screen
                clear_color GameClearColor = RenderCommands.ClearColor;
                FLOAT ClearColor[4]; 

                for (u32 Index = 0;
                     Index < 4;
                     Index++)
                {
                    ClearColor[Index] = GameClearColor.RGBA[Index];            
                }

                DeviceContext->ClearRenderTargetView(RTView, ClearColor);
                DeviceContext->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

                // MARK: Direct Sound Output
                // TODO: (Ted)  Enable Sound
                /*
                DWORD CurrentPlayCursor;
                DWORD CurrentWriteCursor;

                if (SUCCEEDED(SecondaryBuffer->GetCurrentPosition(&CurrentPlayCursor,
                                                                  &CurrentWriteCursor)))
                {
                    u32 SamplesPerFrameUpdate = SamplesPerSecond/60; 
                    u32 FramesAhead = 3;
                    u32 DesiredFrameBytesToWrite = SamplesPerFrameUpdate*FramesAhead*sizeof(s16)*2;
                    u32 TargetCursor = (CurrentPlayCursor + DesiredFrameBytesToWrite)%SecondaryBufferSize;
                    DWORD ByteToLock = (RunningSampleIndex*BytesPerSample)%SecondaryBufferSize; 

                    u32 BytesToWrite = 0;

                     if (ByteToLock > TargetCursor) {
                        // NOTE: (ted)  Play Cursor wrapped.

                        // Bytes to the end of the circular buffer.
                        BytesToWrite = (SecondaryBufferSize - ByteToLock);

                        // Bytes up to the target cursor.
                        BytesToWrite += TargetCursor;
                    } else {
                        BytesToWrite = TargetCursor - ByteToLock;
                    }

                    SoundOutputBuffer.SamplesToWriteThisFrame = (BytesToWrite/BytesPerSample);
                    SoundOutputBuffer.SamplesWrittenThisFrame = 0;

                    GameGetSoundSamples(&Thread, &GameMemory, &SoundOutputBuffer, 
                                        &GameSoundMixPanel, &Input);

                    VOID *Region1;
                    DWORD Region1Size;
                    VOID *Region2;
                    DWORD Region2Size;

                    if(SUCCEEDED(SecondaryBuffer->Lock(ByteToLock, BytesToWrite, 
                                                       &Region1, &Region1Size,
                                                       &Region2, &Region2Size, 0)))
                    {

                        s16* SoundSrc = SoundOutputBuffer.Samples;

                        s16 *SampleOut = (s16 *)Region1;
                        DWORD Region1SampleCount = Region1Size/BytesPerSample;

                        for (u32 SampleIndex = 0;
                             SampleIndex < Region1SampleCount;
                             SampleIndex++)
                        {
                            *SampleOut++ = *SoundSrc++;
                            *SampleOut++ = *SoundSrc++;
                            RunningSampleIndex++;
                        }

                        SampleOut = (s16 *)Region2;
                        DWORD Region2SampleCount = Region2Size/BytesPerSample;

                        for (u32 SampleIndex = 0;
                             SampleIndex < Region2SampleCount;
                             SampleIndex++)
                        {
                            *SampleOut++ = *SoundSrc++;
                            *SampleOut++ = *SoundSrc++;
                            RunningSampleIndex++;
                        }

                        SecondaryBuffer->Unlock(Region1, Region1Size,
                                                Region2, Region2Size);
                    }
                }*/
                

                // Input Assembler
                DeviceContext->IASetInputLayout(FlatColorLayout);
                DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                UINT Stride = sizeof(struct game_flat_color_vertex);
                UINT Offset = 0;

                // Vertex Shader
                DeviceContext->VSSetConstantBuffers(0, 1, &ConstantsBuffer);
                DeviceContext->VSSetShader(FlatColorVShader, NULL, 0);

                // Rasterizer Stage
                DeviceContext->RSSetViewports(1, &Viewport);
                DeviceContext->RSSetState(RasterizerState);

                // Pixel Shader
                DeviceContext->PSSetShader(FlatColorPShader, NULL, 0);

                // Output Merger
                DeviceContext->OMSetDepthStencilState(DepthState, 0);
                DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
                DeviceContext->OMSetRenderTargets(1, &RTView, DSView);

                DeviceContext->IASetVertexBuffers(0, 1, &WindowsFlatColorVertexBuffer, &Stride, &Offset);

                DrawMeshesFromInstanceBuffer(DeviceContext, ConstantsBuffer, &RenderCommands.FlatColorMeshInstances, 
                                             &RenderCommands.FlatColorVertexBuffer);

                DeviceContext->IASetInputLayout(TexturedLayout);
                Stride = sizeof(struct game_texture_vertex);
                DeviceContext->IASetVertexBuffers(0, 1, &WindowsTextureVertexBuffer, &Stride, &Offset);
                DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                DeviceContext->VSSetShader(TexturedVShader, NULL, 0);
                DeviceContext->VSSetConstantBuffers(0, 1, &ConstantsBuffer);

                DeviceContext->PSSetShader(TexturedPShader, NULL, 0);
                DeviceContext->PSSetSamplers(0, 1, &SamplerState);
                DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceViews[1]);

                DrawMeshesFromInstanceBuffer(DeviceContext, ConstantsBuffer, &RenderCommands.TexturedMeshInstances, 
                                             &RenderCommands.TextureVertexBuffer);

                DeviceContext->PSSetShaderResources(0, 1, &ShaderResourceViews[0]);
                DeviceContext->IASetVertexBuffers(0, 1, &WindowsLoadedModelVertexBuffer, &Stride, &Offset);
                DeviceContext->IASetIndexBuffer(WindowsLoadedModelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

                mesh_instance_buffer *LoadedModelMeshInstances = &RenderCommands.LoadedModelMeshInstances;
                game_indexed_vertex_buffer *LoadedModelVertexBuffer = &RenderCommands.LoadedModelVertexBuffer;

                for (u32 Index = 0;
                     Index < LoadedModelMeshInstances->MeshCount;
                     Index++)
                {
                    mesh_instance *MeshInstance = &LoadedModelMeshInstances->Meshes[Index];

                    {
                        D3D11_MAPPED_SUBRESOURCE Mapped;
                        HRESULT HR = DeviceContext->Map((ID3D11Resource*)ConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 
                                                         0, &Mapped);
                        AssertHR(HR);
                        memcpy(Mapped.pData, &MeshInstance->Constants, sizeof(game_constants));
                        DeviceContext->Unmap((ID3D11Resource*)ConstantsBuffer, 0);
                    }

                    DeviceContext->DrawIndexed(LoadedModelVertexBuffer->IndexCount, 0, 0);
                }

                LARGE_INTEGER WithinFrameCounter2;
                QueryPerformanceCounter(&WithinFrameCounter2);
                float WithinFrameDelta = 
                    (float)((double)(WithinFrameCounter2.QuadPart - WithinFrameCounter1.QuadPart) / Freq.QuadPart);

                /* 
                char PerformanceCounterString[100];
                sprintf_s(PerformanceCounterString, "Within Frame Performance Counter: %f \n", WithinFrameDelta);
                sprintf_s(PerformanceCounterString, "Within Frame Rate: %f \n", 1/WithinFrameDelta);
                OutputDebugString(PerformanceCounterString);*/
            }

            // change to FALSE to disable vsync
            BOOL Vsync = TRUE;
            HR = SwapChain->Present(Vsync ? 1:0, 0);

            if (HR == DXGI_STATUS_OCCLUDED)
            {
                // window is minimized, cannot vsync - instead sleep a bit
                if (Vsync)
                {
                    Sleep(10);
                }
            }
            else if (FAILED(HR))
            {
                FatalError("Failed to present swap chain! Device lost?");
            }
        }
    }

    return(0);
}
