# Google Firebase SDK (1.0.0 version) integration with Cocos2d-x v3

Prerequisites
---

iOS and Android samples for the [Firebase C++ SDK](https://firebase.google.com/docs/cpp/setup).

[https://firebase.google.com/docs/reference/gradle/#processing_the_json_file](https://firebase.google.com/docs/reference/gradle/#processing_the_json_file)

For more information, see [https://firebase.google.com/](https://firebase.google.com/).


Steps
---

0. add Firebase native SDK

    > iOS

    [proj.ios_mac/Analytics](https://github.com/yinjimmy/firebase/blob/master/proj.ios_mac/Analytics)
    [proj.ios_mac/GoogleService-Info.plist](https://github.com/yinjimmy/firebase/blob/master/proj.ios_mac/GoogleService-Info.plist)

    > Android

    [proj.android/libs/google-firebase.jar](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/google-firebase.jar)
    [proj.android/libs/gps](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/gps)
    [proj.android/libs/android-support-v4.jar](https://github.com/yinjimmy/firebase/blob/master/proj.android/libs/android-support-v4.jar)


1. Put Firebase SDK `firebase_cpp_sdk` in `cocos2d/external/firebase_cpp_sdk`

    [cocos2d/external/firebase_cpp_sdk/Android.mk](https://github.com/yinjimmy/firebase/blob/master/cocos2d/external/firebase_cpp_sdk/Android.mk)

2. Modified `cocos2d/cocos/Android.mk`

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L89
    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/Android.mk#L130

3. Added to `cocos2d/cocos/platform/android/jni/JniHelper.h` function GetActivity to get Activity.

    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.h#L49
    https://github.com/yinjimmy/firebase/blob/master/cocos2d/cocos/platform/android/jni/JniHelper.cpp#L126

4. Modified `proj.android/AndroidManifest.xml`

    https://github.com/yinjimmy/firebase/blob/master/proj.android/AndroidManifest.xml#L11-L21
    https://github.com/yinjimmy/firebase/blob/master/proj.android/AndroidManifest.xml#L45-L107

5. Convert `google-services.json` to `res/values/google-services.xml`

    ```
    ./generate_xml_from_google_services_json.py -i google-services.json -o proj.android/res/values/google-services.xml
    ```

Test
---

https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L4-L13
https://github.com/yinjimmy/firebase/blob/master/Classes/AppDelegate.cpp#L156


Error
---

```
12-02 22:16:47.683 24451-24490/org.cocosbox.firebase E/FirebaseCrash: Failed to initialize crash reporting
                                                                      java.lang.RuntimeException: Can't create handler inside thread that has not called Looper.prepare()
                                                                          at android.os.Handler.<init>(Handler.java:209)
                                                                          at android.os.Handler.<init>(Handler.java:123)
                                                                          at com.google.firebase.crash.internal.api.FirebaseCrashApiImpl.init(:com.google.android.gms.DynamiteModulesC:1175)
                                                                          at bot.onTransact(:com.google.android.gms.DynamiteModulesC:60)
                                                                          at android.os.Binder.transact(Binder.java:385)
                                                                          at com.google.firebase.crash.internal.zzd$zza$zza.zza(Unknown Source)
                                                                          at com.google.firebase.crash.FirebaseCrash.<init>(Unknown Source)
                                                                          at com.google.firebase.crash.FirebaseCrash.getInstance(Unknown Source)
                                                                          at java.lang.reflect.Method.invoke(Native Method)
                                                                          at java.lang.reflect.Method.invoke(Method.java:372)
                                                                          at com.google.firebase.FirebaseApp.zza(Unknown Source)
                                                                          at com.google.firebase.FirebaseApp.initializeApp(Unknown Source)
                                                                          at com.google.firebase.FirebaseApp.initializeApp(Unknown Source)
                                                                          at org.cocos2dx.lib.Cocos2dxRenderer.nativeInit(Native Method)
                                                                          at org.cocos2dx.lib.Cocos2dxRenderer.onSurfaceCreated(Cocos2dxRenderer.java:72)
                                                                          at android.opengl.GLSurfaceView$GLThread.guardedRun(GLSurfaceView.java:1548)
                                                                          at android.opengl.GLSurfaceView$GLThread.run(GLSurfaceView.java:1286)

...
...
...

12-02 22:16:47.737 24451-24490/org.cocosbox.firebase E/FA: GoogleService failed to initialize, status: 10, Missing google app id value from from string resources with name google_app_id.
```

missing step 4 and 5

got bellow message if success

```
12-02 22:11:00.693 20050-20104/org.cocosbox.firebase I/firebase: firebase::App initializing app org.cocosbox.firebase (default 1).
12-02 22:11:00.694 20050-20104/org.cocosbox.firebase W/firebase: firebase::AppOptions will be ignored as the default app has already been initialized.  To disable automatic app initialization remove or rename resources derived from google-services.json.
```

![android](https://raw.githubusercontent.com/yinjimmy/firebase/master/ss/firebase_android.png)
![ios](https://raw.githubusercontent.com/yinjimmy/firebase/master/ss/firebase_ios.png)
