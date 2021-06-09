#import <AppKit/AppKit.h>

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
}
