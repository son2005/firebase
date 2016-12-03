# Cocos2d-x v3 集成 Google Firebase SDK 简略

预备
---

iOS and Android samples for the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup)。

[https://firebase.google.com/docs/reference/gradle/#processing_the_json_file](https://firebase.google.com/docs/reference/gradle/#processing_the_json_file)

访问 [https://firebase.google.com/](https://firebase.google.com/) 获取更多关于 Firebase 的资料。


步骤
---

0. 添加 Firebase 原生 SDK

    > iOS 平台

    -   [proj.ios_mac/Analytics](https://github.com/yinjimmy/firebase/blob/master/proj.ios_mac/Analytics)
    -   [proj.ios_mac/GoogleService-Info.plist](https://github.com/yinjimmy/firebase/blob/master/proj.ios_mac/GoogleService-Info.plist)

    > Android 平台

    -   [proj.android/libs/google-firebase.jar](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/google-firebase.jar)
    -   [proj.android/libs/gps](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/gps)
    -   [proj.android/libs/android-support-v4.jar](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/android-support-v4.jar)


1. 把 Firebase SDK 文件夹 `firebase_cpp_sdk` 放到 `cocos2d/external/firebase_cpp_sdk` 目录

    [cocos2d/external/firebase_cpp_sdk/Android.mk](https://github.com/yinjimmy/firebase/blob/master/cocos2d/external/firebase_cpp_sdk/Android.mk)

2. 修改 `cocos2d/cocos/Android.mk`

    -   https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L89
    -   https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L130

3. 获取 Activity， `cocos2d/cocos/platform/android/jni/JniHelper.h` 添加 GetActivity 方法

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.h#L49
    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.cpp#L126

4. 修改 `proj.android/AndroidManifest.xml` 文件

    https://github.com/yinjimmy/firebase/blob/master/proj.android/AndroidManifest.xml#L11-L21
    https://github.com/yinjimmy/firebase/blob/master/proj.android/AndroidManifest.xml#L45-L107

5. 把 `google-services.json` 转化为 `res/values/google-services.xml` 文件

    ```
    ./generate_xml_from_google_services_json.py -i google-services.json -o proj.android/res/values/google-services.xml
    ```
    
测试代码
---

https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L4-L13
https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L156


安卓上，logcat 输出：
```
I/firebase: firebase::App initializing app org.cocosbox.firebase (default 1).
```

表示集成成功。


问题
---

如果碰到一下错误:
```
E/FirebaseCrash: Failed to initialize crash reporting

......

E/FA: GoogleService failed to initialize, status: 10, Missing google app id value from from string resources with name google_app_id.
```

检查第4，5步骤。

