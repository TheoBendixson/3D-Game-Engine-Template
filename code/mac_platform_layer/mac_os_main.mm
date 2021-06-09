#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "mac_os_main.h"
#include "mac_window.m"

@interface GameWindow: NSWindow
@end

@implementation GameWindow
{
}
@end

global_variable GameWindow *Window;

int main(int argc, const char * argv[]) 
{
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
    [Window toggleFullScreen: nil];

    NSString *ShaderLibraryFile = [[NSBundle mainBundle] pathForResource: @"shaders" ofType: @"metallib"];
    id<MTLLibrary> ShaderLibrary = [MetalKitView.device newLibraryWithFile: ShaderLibraryFile error: nil];
    id<MTLFunction> VertexFunction = [ShaderLibrary newFunctionWithName:@"vertexShader"];
    id<MTLFunction> FragmentFunction = [ShaderLibrary newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *PipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    PipelineStateDescriptor.label = @"3D Game Vertices";
    PipelineStateDescriptor.vertexFunction = VertexFunction;
    PipelineStateDescriptor.fragmentFunction = FragmentFunction;
    MTLRenderPipelineColorAttachmentDescriptor *RenderBufferAttachment = PipelineStateDescriptor.colorAttachments[0];
    RenderBufferAttachment.pixelFormat = MetalKitView.colorPixelFormat;

    NSError *error = NULL;
    id<MTLRenderPipelineState> RenderPipelineState = 
        [MetalKitView.device newRenderPipelineStateWithDescriptor: PipelineStateDescriptor 
                                                            error: &error];

    id<MTLCommandQueue> CommandQueue = [MetalKitView.device newCommandQueue]; 

    return NSApplicationMain(argc, argv);
}
