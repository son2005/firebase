#include "HelloWorldScene.h"
#include "firebase/admob.h"

#define SCREENLOG_IMPLEMENTATION
#include "ScreenLog.h"

USING_NS_CC;
class Clickable : public CCNode
{
public:
    static void create(CCNode *node, std::function<void()> callback) {
        Clickable *item = new Clickable(node, callback);
        item->autorelease();
    }
    
    Clickable(CCNode *node, std::function<void()> callback) : CCNode() {
        _renderNode = node;
        _click = callback;
        
        if (_click != nullptr) {
            auto lis = EventListenerTouchOneByOne::create();
            lis->setSwallowTouches(true);
            lis->onTouchBegan = [&](Touch* touch, Event* event) {
                if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
                    _click();
                    return true;
                }
                
                return false;
            };
            CCDirector::sharedDirector()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, _renderNode);
        }
        
        node->addChild(this);
    }
    virtual ~Clickable() {
    }
    
private:
    CCNode *_renderNode;
    std::function<void()> _click;
};

cocos2d::CCNode *createButton(const std::string& text, std::function<void()> cb, int fontSize = 24) {
    auto label = Label::createWithSystemFont(text, "sans", fontSize);
    Clickable::create(label, cb);
    return label;
}

namespace rewarded_video = firebase::admob::rewarded_video;

// The AdMob app IDs.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdMobAppID = "ca-app-pub-1329374026572143~9713524918";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdMobAppID = "ca-app-pub-1329374026572143~5121874919";
#endif

// These ad units are configured to always serve test ads.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/6300978111";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
const char* kRewardedVideoAdUnit = "ca-app-pub-1329374026572143/2190258116";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/4411468910";
const char* kRewardedVideoAdUnit = "ca-app-pub-1329374026572143/6598608114";
#else
const char* kAdViewAdUnit = "";
const char* kInterstitialAdUnit = "";
const char* kRewardedVideoAdUnit = "YOUR_AD_UNIT_ID";
#endif

// Sample keywords to use in making the request.
static const char* kKeywords[] = {
//    "AdMob", "C++", "Fun"
};

// Sample test device IDs to use in making the request.
static const char* kTestDeviceIDs[] = {
#if TARGET_IPHONE_SIMULATOR
    "Simulator",
#endif
    "2D276EE68E1F3B6914011D46E1B6FC9E", // jimmy's android
    "2077ef9a63d2b398840261c8221a0c9b",
    "098fe087d987c9a878965454a65654d7"};

// Sample birthday value to use in making the request.
static const int kBirthdayDay = 10;
static const int kBirthdayMonth = 11;
static const int kBirthdayYear = 1976;


/// A simple listener that logs changes to rewarded video state.
class LoggingRewardedVideoListener : public rewarded_video::Listener {
public:
    LoggingRewardedVideoListener() {}
    void OnRewarded(rewarded_video::RewardItem reward) override {
        INFO("Rewarding user with %f %s.", reward.amount,
              reward.reward_type.c_str());
    }
    void OnPresentationStateChanged(rewarded_video::PresentationState state) override {
        INFO("Rewarded video PresentationState has changed to %d.",
              state);
    }
};

static std::string getErrorString(int errCode) {
    std::map<int, std::string> errMap = {
        {firebase::admob::kAdMobErrorNone,"Call completed successfully."},
        {firebase::admob::kAdMobErrorUninitialized,"The ad has not been fully initialized."},
        {firebase::admob::kAdMobErrorAlreadyInitialized,"The ad is already initialized (repeat call)."},
        {firebase::admob::kAdMobErrorLoadInProgress,"A call has failed because an ad is currently loading."},
        {firebase::admob::kAdMobErrorInternalError,"A call to load an ad has failed due to an internal SDK error."},
        {firebase::admob::kAdMobErrorInvalidRequest,"A call to load an ad has failed due to an invalid request."},
        {firebase::admob::kAdMobErrorNetworkError,"A call to load an ad has failed due to a network error."},
        {firebase::admob::kAdMobErrorNoFill,"A call to load an ad has failed because no ad was available to serve."},
        {firebase::admob::kAdMobErrorNoWindowToken, "An attempt has been made to show an ad on an Android Activity that has"
        "no window token (such as one that's not done initializing)."}
    };

    return errMap[errCode];
}

firebase::admob::AdRequest createAdRequest();
void onRewardedVideoInitializeCompletionCallback();
void onRewardedVideoLoadAdCompletionCallback();

/// This function is called when the Future for the last call to
/// rewarded_video::Initialize() method completes.
static void onRewardedVideoInitializeCompletionCallback(const firebase::Future<void>& future,
                                                        void* userData) {
    if (future.Error() == firebase::admob::kAdMobErrorNone) {
        INFO("Initializing rewarded video completed successfully.");
    } else {
        ERROR("Initializing rewarded video failed.");
        ERROR(
                          "ERROR: Action failed with error code %d and message \"%s\" (%s).",
                          future.Error(), future.ErrorMessage(), getErrorString(future.Error()).c_str());
    }
}
/// This function is called when the Future for the last call to
/// rewarded_video::LoadAd() method completes.
static void onRewardedVideoLoadAdCompletionCallback(const firebase::Future<void>& future,
                                                    void* userData) {
    INFO("kRewardedVideoAdUnit=%s", kRewardedVideoAdUnit);
    if (future.Error() == firebase::admob::kAdMobErrorNone) {
        INFO("Loading rewarded video completed successfully.");
    } else {
        ERROR("Loading rewarded video failed.");
        ERROR(
                          "ERROR: Action failed with error code %d and message \"%s\" (%s).",
                          future.Error(), future.ErrorMessage(), getErrorString(future.Error()).c_str());
    }
}

firebase::admob::AdRequest createAdRequest() {
    firebase::admob::AdRequest request;
    // If the app is aware of the user's gender, it can be added to the targeting
    // information. Otherwise, "unknown" should be used.
    request.gender = firebase::admob::kGenderUnknown;

    // This value allows publishers to specify whether they would like the request
    // to be treated as child-directed for purposes of the Children’s Online
    // Privacy Protection Act (COPPA).
    // See http://business.ftc.gov/privacy-and-security/childrens-privacy.
    request.tagged_for_child_directed_treatment =
    firebase::admob::kChildDirectedTreatmentStateTagged;

    // The user's birthday, if known. Note that months are indexed from one.
    request.birthday_day = kBirthdayDay;
    request.birthday_month = kBirthdayMonth;
    request.birthday_year = kBirthdayYear;

    // Additional keywords to be used in targeting.
    request.keyword_count = sizeof(kKeywords) / sizeof(kKeywords[0]);
    request.keywords = kKeywords;

    // "Extra" key value pairs can be added to the request as well. Typically
    // these are used when testing new features.
    static const firebase::admob::KeyValuePair kRequestExtras[] = {
        {"the_name_of_an_extra", "the_value_for_that_extra"}};
    request.extras_count = sizeof(kRequestExtras) / sizeof(kRequestExtras[0]);
    request.extras = kRequestExtras;

    // This example uses ad units that are specially configured to return test ads
    // for every request. When using your own ad unit IDs, however, it's important
    // to register the device IDs associated with any devices that will be used to
    // test the app. This ensures that regardless of the ad unit ID, those
    // devices will always receive test ads in compliance with AdMob policy.
    //
    // Device IDs can be obtained by checking the logcat or the Xcode log while
    // debugging. They appear as a long string of hex characters.
    request.test_device_id_count =
    sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
    request.test_device_ids = kTestDeviceIDs;

    return request;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a label shows "Hello World"
    // create and initialize a label

//    auto label = Label::createWithTTF("Hello World", "fonts/arial.ttf", 24);
    auto label = Label::createWithSystemFont("FireBase Analytics/AdMob Test", "sans", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label);

    auto rewardedVideoBtn = createButton("Load Rewarded Video", [&]() {
        rewarded_video::SetListener(_rewardedVideoListener);
        rewarded_video::LoadAd(kRewardedVideoAdUnit, createAdRequest());
        rewarded_video::LoadAdLastResult().OnCompletion(onRewardedVideoLoadAdCompletionCallback, this);
    });
    rewardedVideoBtn->setPosition(visibleSize.width/2, 100);
    addChild(rewardedVideoBtn);

    INFO("Initializing the AdMob with Firebase API.");
    firebase::admob::Initialize(*firebase::App::GetInstance(), kAdMobAppID);

    //
    // rewarded video
    //
    INFO("Initializing rewarded video.");
    rewarded_video::Initialize();
    _rewardedVideoListener = new LoggingRewardedVideoListener();
    rewarded_video::InitializeLastResult().OnCompletion(onRewardedVideoInitializeCompletionCallback, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

