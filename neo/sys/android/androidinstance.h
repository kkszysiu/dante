

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/log.h>
#include <android_native_app_glue.h>

class AndroidApp
{
    private:
        AndroidApp() {}
        AndroidApp(const AndroidApp &);
        AndroidApp& operator=(const AndroidApp&);
    public:
        //std::string method() { return "singleton pattern"; }
        static AndroidApp& getInstance()
        {
            //Inicjalizacja statycznego obiektu.
            //Obiekt zostanie utworzony przy pierwszym wywołaniu tej metody
            //i tylko wtedy nastąpi inicjalizacja przy pomocy konstruktora.
            //Każde następne wywołanie jedynie zwróci referencję tego obiektu.
            static AndroidApp instance;
            return instance;
        }

        android_app* getApp () const { return app; }
        int getWidth () const { return width; }
        int getHeight() const { return height; }

        void setApp(android_app* app) {
            app = app;
        }

        android_app* app;

        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        int32_t width;
        int32_t height;
        bool windowCreated;
};