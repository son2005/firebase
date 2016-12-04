#include "AppDelegate.h"
#include "HelloWorldScene.h"

// firebase
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/analytics/user_property_names.h"
#include "firebase/app.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#endif

#include "kaguya.hpp"
void HandleError(int errCode, const char * szError)
{  //customize your error handling, eg. write to file...
    CCLOG("errCode=%d, szError=%s", errCode, szError);
}

static int register_firebase_analytics(kaguya::State& state)
{
    kaguya::LuaTable firebase_table = state.newTable();
    state["firebase"] = firebase_table;
    kaguya::LuaTable analytics_table = state.newTable();
    firebase_table["analytics"] = analytics_table;

    // constant
    analytics_table["kUserPropertySignUpMethod"] = firebase::analytics::kUserPropertySignUpMethod;
    analytics_table["kEventLogin"]               = firebase::analytics::kEventLogin;
    analytics_table["kEventPostScore"]           = firebase::analytics::kEventPostScore;
    analytics_table["kEventJoinGroup"]           = firebase::analytics::kEventJoinGroup;
    analytics_table["kParameterLevel"]           = firebase::analytics::kParameterLevel;
    analytics_table["kParameterCharacter"]       = firebase::analytics::kParameterCharacter;
    analytics_table["kEventLevelUp"]             = firebase::analytics::kEventLevelUp;
    analytics_table["kParameterScore"]           = firebase::analytics::kParameterScore;


    // function
    analytics_table["SetAnalyticsCollectionEnabled"] = &firebase::analytics::SetAnalyticsCollectionEnabled;
    analytics_table["SetMinimumSessionDuration"]     = &firebase::analytics::SetMinimumSessionDuration;
    analytics_table["SetSessionTimeoutDuration"]     = &firebase::analytics::SetSessionTimeoutDuration;
    analytics_table["SetUserProperty"]               = &firebase::analytics::SetUserProperty;
    analytics_table["SetUserId"]                     = &firebase::analytics::SetUserId;
    analytics_table["LogEvent"]                      = kaguya::overload(
            [](const char *name) {
                firebase::analytics::LogEvent(name);
            },
            [](const char *name, const char *parameter_name, float parameter_value) {
                firebase::analytics::LogEvent(name, parameter_name, parameter_value);
            }
        );


    return 0;
}

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

static void firebase_test()
{
    namespace analytics = ::firebase::analytics;
    ::firebase::App* app = NULL;

    CCLOG("Initialize the Analytics library");
#if defined(__ANDROID__)
    ::firebase::AppOptions options;
    options.set_app_id("281343926607-8740isl9kao0n7d87vd5tp2a8lo8cvuk.apps.googleusercontent.com");
    options.set_api_key("AIzaSyCouO_YdowioS2n7m5vBr1n5RqhSKo3s_8");
    app = ::firebase::App::Create(options, JniHelper::getEnv(), JniHelper::GetActivity());
#else
    app = ::firebase::App::Create(::firebase::AppOptions());
#endif  // defined(__ANDROID__)

    CCLOG("Created the firebase app %x",
               static_cast<int>(reinterpret_cast<intptr_t>(app)));
    analytics::Initialize(*app);
    CCLOG("Initialized the firebase analytics API");

    CCLOG("Enabling data collection.");
    analytics::SetAnalyticsCollectionEnabled(true);
    // App needs to be open at least 1s before logging a valid session.
    analytics::SetMinimumSessionDuration(1000);
    // App session times out after 5s.
    analytics::SetSessionTimeoutDuration(5000);

    CCLOG("Set user properties.");
    // Set the user's sign up method.
    analytics::SetUserProperty(analytics::kUserPropertySignUpMethod, "Google");
    // Set the user ID.
    analytics::SetUserId("uber_user_510");

    // Log an event with no parameters.
    CCLOG("Log login event.");
    analytics::LogEvent(analytics::kEventLogin);

    // Log an event with a floating point parameter.
    CCLOG("Log progress event.");
    analytics::LogEvent("progress", "percent", 0.4f);

    // Log an event with an integer parameter.
    CCLOG("Log post score event.");
    analytics::LogEvent(analytics::kEventPostScore, analytics::kParameterScore,
                        42);

    // Log an event with a string parameter.
    CCLOG("Log group join event.");
    analytics::LogEvent(analytics::kEventJoinGroup, analytics::kParameterGroupID,
                        "spoon_welders");

    // Log an event with multiple parameters.
    CCLOG("Log level up event.");
    {
      const analytics::Parameter kLevelUpParameters[] = {
          analytics::Parameter(analytics::kParameterLevel, 5),
          analytics::Parameter(analytics::kParameterCharacter, "mrspoon"),
          analytics::Parameter("hit_accuracy", 3.14f),
      };
      analytics::LogEvent(
          analytics::kEventLevelUp, kLevelUpParameters,
          sizeof(kLevelUpParameters) / sizeof(kLevelUpParameters[0]));
    }

    CCLOG("Complete");
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("firebase", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("firebase");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    firebase_test();

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    // lua test
    {
        kaguya::State state;
        state["myprint"] = kaguya::function([](const char *msg) {
            cocos2d::log("%s", msg);
        });
        state.setErrorHandler(HandleError);
        register_firebase_analytics(state);

        state.dostring("myprint('# hello lua')");
        state.dostring("myprint(string.format('kEventLogin=%s', firebase.analytics.kEventLogin))");
        state.dostring("firebase.analytics.SetAnalyticsCollectionEnabled(false)");
        state.dostring("firebase.analytics.LogEvent(firebase.analytics.kEventLogin)");
        state.dostring("firebase.analytics.LogEvent(firebase.analytics.kEventPostScore, firebase.analytics.kParameterScore, 42)");
    }


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
