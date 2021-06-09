
@interface MainWindowDelegate: NSObject<NSWindowDelegate>
@end

@implementation MainWindowDelegate 
- (void)windowWillClose:(id)sender {
    [NSApp terminate: nil];
}
@end
