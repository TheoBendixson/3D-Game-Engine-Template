#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#include <mach/mach_init.h>
#include <mach/mach_time.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <sys/stat.h>

#include "mac_os_main.h"
#include "mac_window.m"
#include "mac_file_path.cpp"
#include "mac_file.cpp"
#include "mac_game_code.h"
#include "mac_recording.h"
#include "mac_game_controller.h"

global_variable b32 LKeyWasPressed = false;
global_variable b32 MouseCursorHideAllowed = true;
global_variable b32 ExternalMouseCursorFlag = false;

@interface GameWindow: NSWindow
-(void) setMacKeyboardControllerPtr:(mac_game_controller *)MacKeyboardControllerPtr;
@end

@implementation GameWindow
{
    mac_game_controller *_MacKeyboardControllerPtr;
    game_input *_GameInputPtr;
}

-(void) setMacKeyboardControllerPtr:(mac_game_controller *)MacKeyboardControllerPtr
{
    _MacKeyboardControllerPtr = MacKeyboardControllerPtr;
}

- (void) setGameInputPtr:(game_input *)GameInputPtr
{
    _GameInputPtr = GameInputPtr;
}

// TODO: (Ted)  Consider looking at this in more detail.
//              Technically, the mouse cursor doesn't always hide
//              after five seconds. It is variable depending on when
//              the user started moving the mouse.
//
//              However, just playing with it, this feels fine to me.
- (void)mouseMoved: (NSEvent *)event
{
    [NSCursor unhide];

    if (!ExternalMouseCursorFlag)
    {
        MouseCursorHideAllowed = true;

        NSTimeInterval FiveSeconds = 5;
        [NSTimer scheduledTimerWithTimeInterval: FiveSeconds  
                                        repeats: false 
                                          block: ^(NSTimer *timer) {
            if (MouseCursorHideAllowed && !ExternalMouseCursorFlag)
            {
                [NSCursor hide];                                                      
                MouseCursorHideAllowed = false;
            }
        }];
    }

}

- (void)keyDown:(NSEvent *)theEvent 
{ 
    if (theEvent.modifierFlags & NSEventModifierFlagCommand)
    {
        return;
    }

    unsigned short KeyCode = theEvent.keyCode;

    if (KeyCode == LeftArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadX = -1;
    }
    else if (KeyCode == RightArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadX = 1;
    }
    else if (KeyCode == DownArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadY = -1;
    }
    else if (KeyCode == UpArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadY = 1;
    }
    else if (KeyCode == SpacebarKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonAState = 1;
    }
    else if (KeyCode == SKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonBState = 1;
    }
    else if (KeyCode == FKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonYState = 1;
    }
    else if (KeyCode == RKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonSelectState = 1;
    } 
    else if (KeyCode == WKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonLeftShoulder1State = 1;
    } else if (KeyCode == LKeyCode)
    {
        LKeyWasPressed = true;
    } else if (KeyCode == ReturnKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonStartState = 1;
    } else if (KeyCode == F1KeyCode)
    {
        _MacKeyboardControllerPtr->ButtonRightShoulder1State = 1;
    } else if (KeyCode == ZKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonLeftShoulder2State = 1;
    } else if (KeyCode == YKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonRightShoulder2State = 1;
    }
}

- (void)keyUp:(NSEvent *)theEvent
{
    unsigned short KeyCode = theEvent.keyCode;

    if (KeyCode == LeftArrowKeyCode || KeyCode == RightArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadX = 0;
    } 
    else if (KeyCode == DownArrowKeyCode || KeyCode == UpArrowKeyCode)
    {
        _MacKeyboardControllerPtr->DPadY = 0;
    }
    else if (KeyCode == SpacebarKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonAState = 0;
    }
    else if (KeyCode == SKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonBState = 0;
    }
    else if (KeyCode == DKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonXState = 0;
    }
    else if (KeyCode == FKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonYState = 0;
    }
    else if (KeyCode == ZKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonLeftShoulder2State = 0;
    }
    else if (KeyCode == RKeyCode) 
    {
        _MacKeyboardControllerPtr->ButtonSelectState = 0;
    } 
    else if (KeyCode == WKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonLeftShoulder1State = 0;
    }
    else if (KeyCode == ReturnKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonStartState = 0;
    } else if (KeyCode == F1KeyCode)
    {
        _MacKeyboardControllerPtr->ButtonRightShoulder1State = 0;
    } else if (KeyCode == YKeyCode)
    {
        _MacKeyboardControllerPtr->ButtonRightShoulder2State = 0;
    }
}

@end

// NOTE: (Ted)  Not sure how much we will need this. The file dialog is nicer.
PLATFORM_WRITE_ENTIRE_FILE(PlatformWriteEntireFile)
{
    b32 Result = false;

    mac_app_path Path = {};
    MacBuildAppFilePath(&Path);

    char BundleFilename[MAC_MAX_FILENAME_SIZE];
    char LocalFilename[MAC_MAX_FILENAME_SIZE];

    sprintf(LocalFilename, "Contents/%s", Filename);

    MacBuildAppPathFileName(&Path, LocalFilename,
                            sizeof(BundleFilename), BundleFilename);

    Result = MacWriteEntireFile(BundleFilename, FileSize, Memory);

    return(Result);
}

PLATFORM_FREE_FILE_MEMORY(PlatformFreeFileMemory) 
{
    if (Memory) {
        free(Memory);
    }
}

@interface MetalViewDelegate: NSObject<MTKViewDelegate>

@property (retain) NSString *SourceGameCodeDLLFullPath;
@property mac_game_code Game;
@property game_memory GameMemory;
@property game_render_commands RenderCommands;
@property (retain) id<MTLRenderPipelineState> FlatColorPipelineState;
@property (retain) id<MTLRenderPipelineState> TexturePipelineState;
@property (retain) id<MTLCommandQueue> CommandQueue;
@property (retain) id<MTLTexture> SampleTexture;
@property (retain) id<MTLBuffer> FlatColorVertexBuffer;
@property (retain) id<MTLBuffer> TextureVertexBuffer;
@property (retain) id<MTLDepthStencilState> DepthStencilState;

- (void)configureMetal;
- (void)setKeyboardControllerPtr: (mac_game_controller *)KeyboardControllerPtr;
- (void)setMacStatePtr: (mac_state *)MacStatePtr;
- (void)setNewInputPtr: (game_input *)NewInputPtr;
- (void)setOldInputPtr: (game_input *)OldInputPtr;

@end

static const NSUInteger kMaxInflightBuffers = 3;

@implementation MetalViewDelegate
{
    mac_game_controller *_KeyboardControllerPtr;
    mac_state *_MacStatePtr;
    game_input *_NewInputPtr;
    game_input *_OldInputPtr;
    dispatch_semaphore_t _frameBoundarySemaphore;
    NSUInteger _currentFrameIndex;
}

- (void)configureMetal
{
    _frameBoundarySemaphore = dispatch_semaphore_create(kMaxInflightBuffers);
    _currentFrameIndex = 0;
}

- (void)setKeyboardControllerPtr: (mac_game_controller *)KeyboardControllerPtr
{
    _KeyboardControllerPtr = KeyboardControllerPtr;
}

- (void)setMacStatePtr: (mac_state *)MacStatePtr
{
    _MacStatePtr = MacStatePtr;
}

- (void)setNewInputPtr: (game_input *)NewInputPtr
{
    _NewInputPtr = NewInputPtr;
}

- (void)setOldInputPtr: (game_input *)OldInputPtr
{
    _OldInputPtr = OldInputPtr;
}

- (void)drawInMTKView:(MTKView *)view 
{
    dispatch_semaphore_wait(_frameBoundarySemaphore, DISPATCH_TIME_FOREVER);
    _currentFrameIndex = (_currentFrameIndex + 1) % kMaxInflightBuffers;

    game_render_commands *RenderCommandsPtr = &_RenderCommands;
    RenderCommandsPtr->FrameIndex = (u32)_currentFrameIndex;

    game_memory *GameMemoryPtr = &_GameMemory;
    mac_state MacState = *_MacStatePtr;

    thread_context Thread = {};

#if INTERNAL
    if (LKeyWasPressed)
    {
        if (_MacStatePtr->InputPlayingIndex == 0)
        {
            if (_MacStatePtr->InputRecordingIndex == 0)
            {
                MacBeginRecordingInput(&Thread, _MacStatePtr, 1);
                _MacStatePtr->InputRecordingIndex = 1;
            } else 
            {
                MacEndRecordingInput(_MacStatePtr);
                MacBeginInputPlayback(&Thread, _MacStatePtr, 1);
                _MacStatePtr->InputRecordingIndex = 0;
                _MacStatePtr->InputPlayingIndex = 1;
            }
        } else {
            MacEndInputPlayback(_MacStatePtr);
        }

        LKeyWasPressed = false;

        GameMemoryPtr->PermanentStorage = MacState.GameMemoryBlock; 
    }
#endif

    _NewInputPtr->dtForFrame = 1.0f/view.preferredFramesPerSecond;

    mac_game_controller *MacController;
    MacController = _KeyboardControllerPtr;

    game_controller_input *OldController = &_OldInputPtr->Controller;
    game_controller_input *NewController = &_NewInputPtr->Controller;

    MacProcessGameControllerButton(&(OldController->A),
                                     &(NewController->A),
                                     MacController->ButtonAState); 

    MacProcessGameControllerButton(&(OldController->B),
                                     &(NewController->B),
                                     MacController->ButtonBState); 

    MacProcessGameControllerButton(&(OldController->X),
                                     &(NewController->X),
                                     MacController->ButtonXState); 

    MacProcessGameControllerButton(&(OldController->Y),
                                     &(NewController->Y),
                                     MacController->ButtonYState); 

    MacProcessGameControllerButton(&(OldController->LeftShoulder1),
                                     &(NewController->LeftShoulder1),
                                     MacController->ButtonLeftShoulder1State); 

    MacProcessGameControllerButton(&(OldController->LeftShoulder2),
                                     &(NewController->LeftShoulder2),
                                     MacController->ButtonLeftShoulder2State); 
   
    MacProcessGameControllerButton(&(OldController->RightShoulder1),
                                     &(NewController->RightShoulder1),
                                     MacController->ButtonRightShoulder1State); 

    MacProcessGameControllerButton(&(OldController->RightShoulder2),
                                     &(NewController->RightShoulder2),
                                     MacController->ButtonRightShoulder2State); 

    MacProcessGameControllerButton(&(OldController->Start),
                                     &(NewController->Start),
                                     MacController->ButtonStartState); 

    MacProcessGameControllerButton(&(OldController->Select),
                                     &(NewController->Select),
                                     MacController->ButtonSelectState); 

    b32 Right = MacController->DPadX > 0 ? true:false;
    b32 Left = MacController->DPadX < 0 ? true:false;
    b32 Up = MacController->DPadY > 0 ? true:false;
    b32 Down = MacController->DPadY < 0 ? true:false;

    MacProcessGameControllerButton(&(OldController->Right),
                                   &(NewController->Right),
                                   Right);
    MacProcessGameControllerButton(&(OldController->Left),
                                   &(NewController->Left),
                                   Left);
    MacProcessGameControllerButton(&(OldController->Up),
                                   &(NewController->Up),
                                   Up);
    MacProcessGameControllerButton(&(OldController->Down),
                                   &(NewController->Down),
                                   Down);

    char *GameCodeFilePath = (char *)[[self SourceGameCodeDLLFullPath] 
                                       cStringUsingEncoding: NSUTF8StringEncoding];
    time_t NewDLLWriteTime = MacGetLastWriteTime(GameCodeFilePath);
    if(NewDLLWriteTime > _Game.DLLLastWriteTime)
    {
        MacUnloadGameCode(&_Game);
        [self setGame: MacLoadGameCode(GameCodeFilePath)];
    }

#if INTERNAL
    if (MacState.InputRecordingIndex)
    {
        MacRecordInput(_MacStatePtr, _NewInputPtr); 
    }

    if (MacState.InputPlayingIndex)
    {
        MacPlaybackInput(&Thread, _MacStatePtr, _NewInputPtr); 
    }
#endif

    if (_Game.UpdateAndRender)
    {
        _Game.UpdateAndRender(&Thread, GameMemoryPtr, 
                              _NewInputPtr, RenderCommandsPtr);
    }

    game_input *Temp = _NewInputPtr;
    _NewInputPtr = _OldInputPtr;
    _OldInputPtr = Temp;

    CGFloat BackingScaleFactor = [[NSScreen mainScreen] backingScaleFactor];
    NSUInteger Width = (NSUInteger)(RenderCommandsPtr->ViewportWidth*BackingScaleFactor);
    NSUInteger Height = (NSUInteger)(RenderCommandsPtr->ViewportHeight*BackingScaleFactor);
    MTLViewport Viewport = (MTLViewport){0.0, 0.0, (r64)Width, (r64)Height, -1.0, 1.0 };

    @autoreleasepool 
    {
        id<MTLCommandBuffer> CommandBuffer = [[self CommandQueue] commandBuffer];
        MTLRenderPassDescriptor *RenderPassDescriptor = [view currentRenderPassDescriptor];
        RenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;

        clear_color ClearColor = RenderCommandsPtr->ClearColor;
        MTLClearColor MetalClearColor = MTLClearColorMake(ClearColor.RGBA[0], ClearColor.RGBA[1], ClearColor.RGBA[2], 
                                                          ClearColor.RGBA[3]);
        RenderPassDescriptor.colorAttachments[0].clearColor = MetalClearColor;

        vector_uint2 ViewportSize = { (u32)RenderCommandsPtr->ViewportWidth, 
                                      (u32)RenderCommandsPtr->ViewportHeight };

        id<MTLRenderCommandEncoder> RenderEncoder = 
            [CommandBuffer renderCommandEncoderWithDescriptor: RenderPassDescriptor];
        RenderEncoder.label = @"RenderEncoder";
        [RenderEncoder setViewport: Viewport];
        [RenderEncoder setCullMode: MTLCullModeBack];
        [RenderEncoder setFrontFacingWinding: MTLWindingCounterClockwise];
        [RenderEncoder setDepthStencilState: [self DepthStencilState]];

// MARK:    Render Flat Shaded Geometry
        [RenderEncoder setRenderPipelineState: [self FlatColorPipelineState]];
        [RenderEncoder setVertexBuffer: [self FlatColorVertexBuffer] offset: 0 atIndex: 0];

        mesh_instance_buffer *MeshBuffer = &RenderCommandsPtr->FlatColorMeshInstances;
        game_vertex_buffer *FlatColorVertexBuffer = &RenderCommandsPtr->FlatColorVertexBuffer;

        for (u32 Index = 0;
             Index < MeshBuffer->MeshCount;
             Index++)
        {
            mesh_instance *MeshInstance = &MeshBuffer->Meshes[Index];

            [RenderEncoder setVertexBytes: &MeshInstance->Constants
                                   length: sizeof(game_constants)
                                  atIndex: 1];

            model_range Range = FlatColorVertexBuffer->ModelRanges[MeshInstance->ModelIndex];
            [RenderEncoder drawPrimitives: MTLPrimitiveTypeTriangle
                              vertexStart: Range.StartVertex
                              vertexCount: Range.VertexCount];
        }

        // TODO: (Ted)  Render textured cube primitives here.

        [RenderEncoder endEncoding];

        // Schedule a present once the framebuffer is complete using the current drawable
        id<CAMetalDrawable> NextDrawable = [view currentDrawable];
        [CommandBuffer presentDrawable: NextDrawable];

        __block dispatch_semaphore_t semaphore = _frameBoundarySemaphore;
        [CommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer) {
            dispatch_semaphore_signal(semaphore);
        }];

        [CommandBuffer commit];
    }
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{

}

@end

global_variable GameWindow *Window;

int main(int argc, const char * argv[]) 
{
    mac_state MacState = {};
    mac_app_path Path = {};
    MacState.Path = &Path;
    MacBuildAppFilePath(MacState.Path);

	char SourceGameCodeDLLFullPath[MAC_MAX_FILENAME_SIZE];
    MacBuildAppPathFileName(MacState.Path, "Contents/Resources/GameCode.dylib",
                               sizeof(SourceGameCodeDLLFullPath), SourceGameCodeDLLFullPath);
    mac_game_code Game = MacLoadGameCode(SourceGameCodeDLLFullPath);

    if (!Game.IsValid)
    {
        [NSException raise: @"Game Code Not Loaded"
                     format: @"Game Code Not Loaded"];
    }

    MainWindowDelegate *WindowDelegate = [[MainWindowDelegate alloc] init];

    NSRect ScreenRect = [[NSScreen mainScreen] frame];

    r32 GlobalRenderWidth = (r32)ScreenRect.size.width;
    r32 GlobalRenderHeight = (r32)ScreenRect.size.height;

    NSRect InitialFrame = NSMakeRect(0, 0, GlobalRenderWidth, GlobalRenderHeight);
  
    Window = [[GameWindow alloc] 
                initWithContentRect: InitialFrame
                styleMask: NSWindowStyleMaskTitled |
                           NSWindowStyleMaskClosable |
                           NSWindowStyleMaskResizable
                  backing: NSBackingStoreBuffered
                    defer: NO];    

    Window.acceptsMouseMovedEvents = true;
    NSTrackingArea *MouseTrackingArea = [[NSTrackingArea alloc] initWithRect: [Window.contentView bounds]
                                                                     options: (NSTrackingActiveAlways | 
                                                                               NSTrackingMouseMoved)
                                                                       owner: Window
                                                                    userInfo: nil];

    [Window.contentView addTrackingArea: MouseTrackingArea];

    [Window setBackgroundColor: NSColor.blackColor];
    [Window setTitle: @"Vario's Temple"];
    [Window makeKeyAndOrderFront: nil];
    [Window setDelegate: WindowDelegate];

    MTKView *MetalKitView = [[MTKView alloc] init];
    MetalKitView.frame = CGRectMake(0, 0, 
                                    GlobalRenderWidth, GlobalRenderHeight); 

    MetalKitView.device = MTLCreateSystemDefaultDevice(); 
    MetalKitView.framebufferOnly = false;
    MetalKitView.layer.contentsGravity = kCAGravityCenter;
    MetalKitView.preferredFramesPerSecond = 60;
    [Window setContentView: MetalKitView];

    game_render_commands RenderCommands = {}; 
    RenderCommands.ViewportWidth = (int)GlobalRenderWidth;
    RenderCommands.ViewportHeight = (int)GlobalRenderHeight;
    RenderCommands.FrameIndex = 0;
    RenderCommands.ScreenScaleFactor = (r32)([[NSScreen mainScreen] backingScaleFactor]);

    u32 PageSize = getpagesize();
    u32 VertexBufferSize = PageSize*1000;

    RenderCommands.FlatColorVertexBuffer.Vertices = mmap(0, VertexBufferSize,
                                                         PROT_READ | PROT_WRITE,
                                                         MAP_PRIVATE | MAP_ANON, -1, 0);

    id<MTLBuffer> MacFlatColorVertexBuffer = [MetalKitView.device 
                                                newBufferWithBytesNoCopy: RenderCommands.FlatColorVertexBuffer.Vertices
                                                                  length: VertexBufferSize 
                                                                 options: MTLResourceStorageModeShared
                                                             deallocator: nil];

    RenderCommands.FlatColorVertexBuffer.VertexCount = 0;

    RenderCommands.TextureVertexBuffer.Vertices = mmap(0, VertexBufferSize,
                                                       PROT_READ | PROT_WRITE,
                                                       MAP_PRIVATE | MAP_ANON, -1, 0);

    id<MTLBuffer> MacTextureVertexBuffer = [MetalKitView.device 
                                                newBufferWithBytesNoCopy: RenderCommands.TextureVertexBuffer.Vertices
                                                                  length: VertexBufferSize 
                                                                 options: MTLResourceStorageModeShared
                                                             deallocator: nil];

    RenderCommands.TextureVertexBuffer.VertexCount = 0;

    NSString *ShaderLibraryFile = [[NSBundle mainBundle] pathForResource: @"Shaders" ofType: @"metallib"];
    id<MTLLibrary> ShaderLibrary = [MetalKitView.device newLibraryWithFile: ShaderLibraryFile 
                                                                     error: nil];
    id<MTLFunction> FlatColorVertexFunction = [ShaderLibrary newFunctionWithName:@"flatColorVertexShader"];
    id<MTLFunction> FlatColorFragmentFunction = [ShaderLibrary newFunctionWithName:@"flatColorFragmentShader"];
    id<MTLFunction> TextureVertexFunction = [ShaderLibrary newFunctionWithName:@"textureVertexShader"];
    id<MTLFunction> TextureFragmentFunction = [ShaderLibrary newFunctionWithName:@"textureFragmentShader"];

    MTLRenderPipelineDescriptor *FlatColorPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    FlatColorPipelineDescriptor.label = @"Flat Shaded Colored Vertices";
    FlatColorPipelineDescriptor.vertexFunction = FlatColorVertexFunction;
    FlatColorPipelineDescriptor.fragmentFunction = FlatColorFragmentFunction; 
    MTLRenderPipelineColorAttachmentDescriptor *FlatColorRenderBufferAttachment = 
        FlatColorPipelineDescriptor.colorAttachments[0];
    FlatColorRenderBufferAttachment.pixelFormat = MetalKitView.colorPixelFormat;

    NSError *error = NULL;
    id<MTLRenderPipelineState> FlatColorPipelineState = 
        [MetalKitView.device newRenderPipelineStateWithDescriptor: FlatColorPipelineDescriptor
                                                            error: &error];

    if (error != nil)
    {
        NSLog(@"Error creating flat shaded geometry pipeline state");
    }

    MTLRenderPipelineDescriptor *TexturePipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    TexturePipelineDescriptor.label = @"Texture Sampled Vertices";
    TexturePipelineDescriptor.vertexFunction = TextureVertexFunction;
    TexturePipelineDescriptor.fragmentFunction = TextureFragmentFunction; 
    MTLRenderPipelineColorAttachmentDescriptor *TextureRenderBufferAttachment = 
        TexturePipelineDescriptor.colorAttachments[0];
    TextureRenderBufferAttachment.pixelFormat = MetalKitView.colorPixelFormat;

    id<MTLRenderPipelineState> TexturePipelineState = 
        [MetalKitView.device newRenderPipelineStateWithDescriptor: TexturePipelineDescriptor
                                                            error: &error];

    if (error != nil)
    {
        NSLog(@"Error creating flat shaded geometry pipeline state");
    }

    MTLDepthStencilDescriptor *DepthStencilDesc = [[MTLDepthStencilDescriptor alloc] init];
    DepthStencilDesc.depthCompareFunction = MTLCompareFunctionLess;
    DepthStencilDesc.label = @"Depth Stencil";
    id<MTLDepthStencilState> DepthStencilState = [MetalKitView.device newDepthStencilStateWithDescriptor: DepthStencilDesc];

    id<MTLCommandQueue> CommandQueue = [MetalKitView.device newCommandQueue]; 

#if INTERNAL
    char* BaseAddress = (char*)Gigabytes(8);
    u32 AllocationFlags = MAP_PRIVATE | MAP_ANON | MAP_FIXED;
#else
    void* BaseAddress = 0;
    u32 AllocationFlags = MAP_PRIVATE | MAP_ANON;
#endif

    MetalViewDelegate *ViewDelegate = [[MetalViewDelegate alloc] init];

	MacBuildAppPathFileName(MacState.Path, "../Resources/",
							sizeof(MacState.ResourcesDirectory), MacState.ResourcesDirectory);
	MacState.ResourcesDirectorySize = StringLength(MacState.ResourcesDirectory);

    game_memory GameMemory = {};
    GameMemory.PermanentStorageSize = Megabytes(1024);
    GameMemory.TransientStorageSize = Megabytes(128);

    MacState.PermanentStorageSize = GameMemory.PermanentStorageSize;
    MacState.GameMemoryBlock = mmap(BaseAddress,
                                    GameMemory.PermanentStorageSize,
                                    PROT_READ | PROT_WRITE,
                                    AllocationFlags, -1, 0); 

    GameMemory.PermanentStorage = MacState.GameMemoryBlock; 

    if (GameMemory.PermanentStorage == MAP_FAILED) 
    {
		printf("mmap error: %d  %s", errno, strerror(errno));
        [NSException raise: @"Game Memory Not Allocated"
                     format: @"Failed to allocate permanent storage"];
    }

    u8* TransientStorageAddress = ((u8*)GameMemory.PermanentStorage + GameMemory.PermanentStorageSize);
    GameMemory.TransientStorage = mmap(TransientStorageAddress,
                                       GameMemory.TransientStorageSize,
                                       PROT_READ | PROT_WRITE,
                                       AllocationFlags, -1, 0); 

    if (GameMemory.TransientStorage == MAP_FAILED) {
		printf("mmap error: %d  %s", errno, strerror(errno));
        [NSException raise: @"Game Memory Not Allocated"
                     format: @"Failed to allocate transient storage"];
    }

    GameMemory.PlatformReadEntireFile = PlatformReadEntireFile;
    GameMemory.PlatformWriteEntireFile = PlatformWriteEntireFile;
    GameMemory.PlatformFreeFileMemory = PlatformFreeFileMemory;

    thread_context Thread = {};

#if INTERNAL
    for(int ReplayIndex = 0;
        ReplayIndex < ArrayCount(MacState.ReplayBuffers);
        ++ReplayIndex)
    {
        mac_replay_buffer *ReplayBuffer = &MacState.ReplayBuffers[ReplayIndex];
        int FileDescriptor;

        mode_t Mode = S_IRUSR | S_IWUSR;
        char Filename[MAC_MAX_FILENAME_SIZE];
        char LocalFilename[MAC_MAX_FILENAME_SIZE];
        sprintf(LocalFilename, "Contents/Resources/ReplayBuffer%d", ReplayIndex);
        MacBuildAppPathFileName(MacState.Path, LocalFilename,
                                sizeof(Filename), Filename);
        FileDescriptor = open(Filename, O_CREAT | O_RDWR, Mode);
        int Result = truncate(Filename, GameMemory.PermanentStorageSize);

        if (Result < 0)
        {
            NSLog(@"Failed to setup replay buffers. Errno: %d", Result);
        }

        ReplayBuffer->MemoryBlock = mmap(0, GameMemory.PermanentStorageSize,
                                         PROT_READ | PROT_WRITE,
                                         MAP_PRIVATE, FileDescriptor, 0);
        ReplayBuffer->FileHandle = fopen(Filename, "r+");
        fseek(ReplayBuffer->FileHandle, MacState.PermanentStorageSize, SEEK_SET);
        if (ReplayBuffer->MemoryBlock)
        {
        } else 
        {
            NSLog(@"Failed to setup replay buffer memory block.");
        }
    }
#endif

    [ViewDelegate setMacStatePtr: &MacState];

    mac_game_controller MacKeyboardController = {};
    MacKeyboardController.UsesHatSwitch = false;
    [Window setMacKeyboardControllerPtr: &MacKeyboardController]; 
    [ViewDelegate setKeyboardControllerPtr: &MacKeyboardController];

    game_input Input[2] = {};
    game_input *NewInputPtr = &Input[0];
    game_input *OldInputPtr = &Input[1];

    [ViewDelegate setNewInputPtr: NewInputPtr];
    [ViewDelegate setOldInputPtr: OldInputPtr];

    [Window setGameInputPtr: NewInputPtr]; 

    if (Game.Load3DModels)
    {
        Game.Load3DModels(&RenderCommands);
    } else
    {
        [NSException raise: @"3D Models Not Loaded"
                     format: @"Failed to load the game's 3D models"];
    }

    NSString *GameCodeDLLPath = [[NSString alloc] initWithCString: SourceGameCodeDLLFullPath
                                                         encoding: NSUTF8StringEncoding];
    ViewDelegate.SourceGameCodeDLLFullPath = GameCodeDLLPath;
    ViewDelegate.Game = Game; 
    ViewDelegate.GameMemory = GameMemory; 
    ViewDelegate.RenderCommands = RenderCommands; 
    ViewDelegate.TexturePipelineState = TexturePipelineState;
    ViewDelegate.FlatColorPipelineState = FlatColorPipelineState;
    ViewDelegate.CommandQueue = CommandQueue;
    ViewDelegate.FlatColorVertexBuffer = MacFlatColorVertexBuffer;
    ViewDelegate.TextureVertexBuffer = MacTextureVertexBuffer;
    ViewDelegate.DepthStencilState = DepthStencilState;

    [ViewDelegate configureMetal];
    [MetalKitView setDelegate: ViewDelegate];

    [NSCursor hide];                                                      

    return NSApplicationMain(argc, argv);
}
