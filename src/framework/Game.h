#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <LiteMath.h>
using namespace LiteMath;

struct GameStateCurrent
{
    double mouseX, mouseY;
    bool keyboard[GLFW_KEY_LAST+1];
    bool mouse[GLFW_MOUSE_BUTTON_LAST+1];
    double gameTime;
};

class GameState : public GameStateCurrent
{
public:
    GameState(GameStateCurrent curr, GameStateCurrent prev) :
                GameStateCurrent(curr),
                prev(prev){}

    GameStateCurrent prev;
};

class GameWindow
{
    int gl_major_v, gl_minor_v;
    int window_width, window_height;
    int fb_width, fb_height; // framebuffer size
    GLFWwindow *window;
    GameState state;

    GLFWkeyfun prevKeyboardCallback;
    void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    GLFWmousebuttonfun prevMouseButtonCallback;
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
public:
    GameWindow(int width, int height, const char *title, bool resizable = false,
               int gl_major_v = 3, int gl_minor_v = 3);

    int getWindowWidth() const { return window_width; };
    int getWindowHeight() const { return window_height; };
    int getViewSize() const { return fb_width; };
    int getViewSize() const { return fb_height; };

    GLFWwindow* get() const { return window; }

    void update();
    const GameState &getState() const { return state; }
};