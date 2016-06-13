# Google Firebase SDK integration

Prerequisites
---

iOS and Android samples for the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup).

For more information, see [https://firebase.google.com/](https://firebase.google.com/).


Steps
---

0. add Firebase native SDK

    > iOS

    https://github.com/yinjimmy/firebase/blob/master/proj.ios_mac/Analytics
    https://github.com/yinjimmy/firebase/blob/master/GoogleService-Info.plist

    > Android

    https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/google-firebase.jar
    https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/gps
    https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/android-support-v4.jar


1. Put Firebase SDK `firebase_cpp_sdk` in `cocos2d/external/firebase_cpp_sdk`

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/external/firebase_cpp_sdk/Android.mk

2. Modified [Android.mk](https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk)

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L89
    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L130

3. Added to [JNIHelper](https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.h) function GetActivity to get Activity.

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.h#L49
    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.cpp#L126

4. Modified AndroidManifest.xml

    https://github.com/yinjimmy/firebase/blob/master/proj.android/AndroidManifest.xml#L30

Test
---

https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L4-L13
https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L156

