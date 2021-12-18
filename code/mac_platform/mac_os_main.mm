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
#include "mac_game_code.h"

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

    return NSApplicationMain(argc, argv);
}
