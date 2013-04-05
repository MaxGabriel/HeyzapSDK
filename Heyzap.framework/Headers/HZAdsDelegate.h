//
//  HZAdsDelegate.h
//  Heyzap
//
//  Created by David Stumm on 1/30/13.
//
//

#import <Foundation/Foundation.h>

@protocol HZAdsDelegate <NSObject>
@optional
- (void)didReceiveAd;
@optional
- (void)didFailToReceiveAd;
@optional
- (void)didShowAd;
@optional
- (void)didClickAd;
@optional
- (void)didHideAd;
@end
