//
//  HeyzapSDK.h
//
//  Copyright 2011 Smart Balloon, Inc. All Rights Reserved
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "HZAdsDelegate.h"

typedef enum {
    HZDebugLevelVerbose = 3,
    HZDebugLevelInfo = 2,
    HZDebugLevelError = 1,
    HZDebugLevelSilent = 0
} HZDebugLevel;

typedef enum {
    HZOptionsNone    = 0,
    HZOptionsHideStartScreen   = 1 << 1,
    HZOptionsHideDeleteScore   = 1 << 2,
    HZOptionsShowErrors        = 1 << 3,
} HZOptions;

@protocol HeyzapAchievementProtocol <NSObject>
- (NSString *)heyzapAchievementIdentifier;
@end

@class HZCheckinButton;
@class HZScore;
@class HZLeaderboardRank;

/** `HeyzapSDK` is a singleton object which handles all the functionality of the Heyzap SDK. */
@interface HeyzapSDK : NSObject

/** Accessing the singleton `HeyzapSDK` */
+ (id) sharedSDK;

/** The App Store ID for your app. Instructions for getting this value: http://support.heyzap.com/entries/23201783-How-do-I-find-my-game-s-App-Store-ID- . If you haven't submitted your app to the store, see here: http://developers.heyzap.com/docs/ios_sdk_advanced*/
@property (nonatomic, strong) NSString *appId;
/** The URL the Heyzap SDK uses to return to your app. For information, see here: http://developers.heyzap.com/docs/ios_sdk_advanced*/
@property (nonatomic, strong) NSURL *appURL;

/** Whether or not Heyzap supports the current version of iOS. Heyzap will run fine on iOS versions below 5.0, but will not show Leaderboards, Achievements, or Ads on those versions.
 @return `YES` if supported
 */
+ (BOOL) isSupported;

#pragma mark - Initialization

+ (void) setAppName: (NSString *) passedAppName;
+ (void) startHeyzapWithAppId: (NSString *) appId andAppURL: (NSURL *) url andShowPopup:(BOOL)showPopup;
+ (void) startHeyzapWithAppId:(NSString *) appId andShowPopup:(BOOL)showPopup;

+ (void) startHeyzapWithAppId:(NSString *)appId andOptions: (HZOptions) options;
+ (void) startHeyzapWithAppId: (NSString *) appId andAppURL: (NSURL *) url andOptions: (HZOptions) options;


+ (HZCheckinButton*) getCheckinButtonWithLocation: (CGPoint) location;
+ (HZCheckinButton*) getCheckinButtonWithLocation: (CGPoint) location andMessage: (NSString *) message;

#pragma mark - Debug
/** For development purposes, the Heyzap SDK will print helpful information. This method controls how much is logged. The default level is `HZDebugLevelSilent`.
 @param debugLevel The `HZDebugLevel` to use for print statements.
 */
- (void) setDebugLevel: (HZDebugLevel) debugLevel;

#pragma mark - Checkin Methods

- (IBAction) checkin;
- (IBAction) checkinWithMessage: (NSString *) message;

#pragma mark - Level
- (void) onStartLevel:(void (^)(NSString *))block;

#pragma mark - Leaderboard/Score Methods
/**---------------------------------------------------------------------------------------
 * @name Leaderboard/Score Methods
 *  ---------------------------------------------------------------------------------------
 */

/** Submit a new score to Heyzap. See the `HZScore` header for how to create an `HZScore`. If the score isn't a personal best, this method will show a small notification at the top of the screen. If the score is a personal best, shows a full leaderboard.
 
 @param completionBlock A block with following arguments:
   rank The `HZLeaderboardRank` describing the user's position on the leaderboard. You do not need to use this object.
   error If there was an error submitting the score, `error` describes the issue. Otherwise, `nil`.
 */
- (IBAction) submitScore:(HZScore *)score withCompletion: (void(^)(HZLeaderboardRank *rank, NSError *error))completionBlock;

/** Shows the leaderboard for the specified level ID
 @param levelID The string for the level ID. You can find these values at https://developers.heyzap.com/dashboard
 */
- (IBAction) openLeaderboardLevel: (NSString *) levelID;

/** Shows the leaderboard for the default level ID */
- (IBAction) openLeaderboard;

#pragma mark - Parsing URLs coming from Heyzap
extern NSString * const kHeyzapRequestTypeKey;
extern NSString * const kHeyzapRequestArgumentsKey;
+ (BOOL)canParseURL:(NSURL *)url;
+ (NSDictionary *)parseURL:(NSURL *)url;

/**---------------------------------------------------------------------------------------
 * @name Achievements
 *  ---------------------------------------------------------------------------------------
 */

#pragma mark - Unlocking Achievements

/** Unlocks achievements and then displays a popup showing the achievements the user has unlocked. This method will only display UI if there are new achievements to show. Because this method displays UI over the screen, call it at e.g. the end of a level. To unlock achievements without interrupting gameplay, see `silentlyUnlockAchievements:`
 
 @param achievementIDs An array of achievement ID strings.
 As a convenience, if you internally represent achievements as objects within your application, you can pass those objects. Just have them conform to the `HeyzapAchievementProtocol`. You can create achievements and find their achievement identifier at https://developers.heyzap.com/dashboard
 @param block A completion block with the following arguments:
 
   `achievements` An `NSArray` of `HZAchievement` model objects. You do not need to use these objects unless you wish to disable Heyzap's Achievement UI and create your own.
 
   `error` If the request sent to Heyzap was a success, this value will be `nil`. Otherwise, it will contain an `NSError` object describing the error.
 
   `showPopup` A reference to a Boolean value. The block can set this value to `NO` to not show the Achievement popup (if for example the user has already started a new game).
 */
- (void)unlockAchievementsWithIDs:(NSArray *)achievementIDs completion:(void(^)(NSArray *achievements, NSError *error, BOOL *showPopup))block;

/** Stores achievement IDs in `NSUserDefaults` that should be unlocked on the next call of `unlockAchievementsWithIDs:completion:` without adding UI to the screen. Use this method to unlock an achievement without interrupting gameplay.
 
 @param achievementIDs This parameter has the same semantics as in the `unlockAchievementsWithIDs:completion:`. It is safe to call this method multiple times with the same achievement and to pass duplicate achievements -- the UI displayed by `unlockAchievementsWithIDs:completion:` will always be accurate at displaying what achievements are new.
 */
- (void)silentlyUnlockAchievements:(NSArray *)achievementIDs;

#pragma mark - Showing all possible achievements

/** Shows all achievements for the game.
 @param block A completion block called after finishing a Heyzap network request. The block arguments are identical to those used in `unlockAchievementsWithIDs:completion:`.
 */
- (void)showAllAchievementsWithCompletion:(void(^)(NSArray *achievements, NSError *error, BOOL *showPopup))block;


/**---------------------------------------------------------------------------------------
 * @name Heyzap Ads
 *  ---------------------------------------------------------------------------------------
 */

#pragma mark - Enabling Ads & Ad-Related Callbacks

/** You must call this method, or `enableAds:`, before showing an ad. This method prefetches a Heyzap ad. You only need to call this method once. */
- (void)enableAds;

/** You must call this method, or `enableAds`, before showing an ad. This method prefetches a Heyzap ad. You only need to call this method once.
 @param delegate Pass an object conforming to the `HZAdsDelegate` protocol to receive ad-related callbacks.
 */
- (void)enableAds:(id<HZAdsDelegate>)delegate;

/** Call this method to set a new delegate to respond to ad-related callbacks.
 @param delegate Pass an object conforming to the `HZAdsDelegate` protocol to receive ad-related callbacks.
 */
- (void)setAdsDelegate:(id<HZAdsDelegate>)delegate;


#pragma mark - Showing Ads

/** Shows a Heyzap Ad. If one has not been preloaded, this makes another request to prefetch an ad and returns immediately. */
- (void)showAd;

/** This method is equivalent to `showAd`, but you may pass a string describing the context of where the ad is shown. Heyzap does not currently support this feature on its Dashboard, but this string will be used to compare the effectiveness of ads depending on where they are shown within the app.
 @param tag A string describing where the ad is being shown, e.g. "After Level 1"
 */
- (void)showAd:(NSString *)tag;



@end
