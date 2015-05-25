//
//  LHVideoOverlayView.m
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "LHVideoOverlayView.h"
#import "LHVideoPlayerImpl.h"

@implementation LHVideoOverlayView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
//        _btnSkip = nil;
        _labelSkip = Nil;
    }
    return self;
}

- (void)setSkipTitle: (NSString *)title
{
//    if (!_btnSkip) {
//        _btnSkip = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//        _btnSkip.frame = CGRectMake(self.frame.size.width - 60, self.frame.size.height - 45, 60, 45);
//        [_btnSkip addTarget:self action:@selector(skipAction:) forControlEvents:UIControlEventTouchUpInside];
//        [self addSubview:_btnSkip];
//    }
//    [_btnSkip setTitle:title forState:UIControlStateNormal];
//    [_btnSkip setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
//    [_btnSkip setTitleShadowColor:[UIColor blackColor] forState:UIControlStateNormal];
//    
    
    if(!_labelSkip)
    {
        _labelSkip = [[UILabel alloc] init];
        
        _labelSkip.frame = CGRectMake(self.frame.size.width/3, self.frame.size.height - 45, self.frame.size.width/3, self.frame.size.height/6);
        _labelSkip.textColor = [UIColor grayColor];
        _labelSkip.highlighted = YES;
        _labelSkip.adjustsFontSizeToFitWidth = YES;
        [_labelSkip setText:@"快速双击可跳过视频"];
        [self addSubview:_labelSkip];
        _labelSkip.alpha = 0.0f;
    }
}

-(void)skipAction:(id)sender {
    [LHVideoPlayerImpl skipVideo];
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

-(void)showHelpText:(NSString*) text
{
    [_labelSkip setText:text];
    [UIView animateWithDuration:3.f animations:^{
      _labelSkip.alpha = 1.0;
    } completion:^(BOOL finished) {
        _labelSkip.alpha = 0.0;
    }];
}


@end
#endif