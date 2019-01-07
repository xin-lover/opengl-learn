#ifndef __GLWINDOW_HEAD__
#define __GLWINDOW_HEAD__

#include <functional>
#include <string>

using namespace std;

class GLFWwindow;

namespace luwu
{
    class GLWindow
    {
    public:
        GLWindow(string title,int widht,int height,int major=3,int minor=3);
        ~GLWindow();

        void SetUpdateCallback(function<void()> func)
        {
            updateCallback_ = func;
        }
        void SetSizeCallback(function<void(GLFWwindow*,int,int)> func)
        { 
            sizeCallback_ = func;
        }
        void SetMouseCallback(function<void(GLFWwindow*,double,double)> func)
        {
            mouseCallback_ = func;
        }
        void SetScrollCallback(function<void(GLFWwindow*,double,double)> func) 
        {
            scrollCallback_ = func;
        }

        void RunMainLoop();
        void Close();

        void SetCursorEnabled(bool enabled);

    protected:

    private:
        GLFWwindow *window_;
        int width_;
        int height_;

        function<void()> updateCallback_;
        function<void(GLFWwindow*,int,int)> sizeCallback_;
        function<void(GLFWwindow*,double,double)> mouseCallback_;
        function<void(GLFWwindow*,double,double)> scrollCallback_;

        static void size_callback(GLFWwindow* window,int width,int height)
        {
            // if(sizeCallback_)
            // {
            //     sizeCallback_(window,width,height);
            // }
        }

        static void mouse_callback(GLFWwindow *window,double x, double y)
        {

        }

        static void scroll_callback(GLFWwindow *window,double xoffset,double yoffset)
        {

        }
    };
}

#endif