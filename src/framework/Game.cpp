#include <GLFW/glfw3.h>
#include "Game.h"

#include "HelperGL.h"
#include <glad/glad.h>
#include <iostream>
#include <iomanip>

void throwGLFWError()
{
  //TODO: Update GLFW
  //const char* description;
  //int code = glfwGetError(&description);
  //if (description) std::cout << "GLFW Error: " << description << std::endl;
  glfwTerminate();
  throw "GLFW Exception";
}

GameWindow::GameWindow(int width, int height, const char *title, bool resizable,
                       int gl_major_v, int gl_minor_v):
                       gl_major_v(gl_major_v), gl_minor_v(gl_minor_v),
                       window_width(width), window_height(height),
                       state({}, {})
{
  if (!glfwInit())
      throwGLFWError();

  // Ask for opengl context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major_v);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor_v);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, resizable);

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    throwGLFWError();
  }
  glfwMakeContextCurrent(window);


  // Load opengl functions with glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    const char* err = "Failed to initialize OpenGL context";
    std::cout << err << std::endl;
    throw std::runtime_error(err);
  }

  // Reset any OpenGL errors which could be present for some reason
  GLenum gl_error = glGetError();
  while (gl_error != GL_NO_ERROR)
    gl_error = glGetError();


  // Framebuffer size may be different from window size
  // For example Macbook has retina display with high dpi
  // And for him fb_size = 2 * window_size
  glfwGetFramebufferSize(window, &fb_width, &fb_height);
  glViewport(0, 0, fb_width, fb_height);


  // Print some information about driver, opengl context, window and framebuffer sizes
  std::cout << std::left;
  std::cout << std::setw(11) << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
  std::cout << std::setw(11) << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << std::setw(11) << "Version: "  << glGetString(GL_VERSION) << std::endl;
  std::cout << std::setw(11) << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << std::setw(11) << "WindowSize" << window_width << 'x' << window_height << std::endl;
  std::cout << std::setw(11) << "ViewSize"   << fb_width << 'x' << fb_height << std::endl;

  state.gameTime = state.prev.gameTime = -1;
}

GameWindow *current = NULL;
GLFWkeyfun prevKeyboardCallback = NULL;
GLFWmousebuttonfun prevMouseButtonCallback = NULL;

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (current && key != GLFW_KEY_UNKNOWN)
    current->state.keyboard[key] = (action != GLFW_RELEASE);

  if (prevKeyboardCallback != NULL)
    prevKeyboardCallback(window, key, scancode, action, mode);
}

void mouseButtonCallback(GLFWwindow *window, int key, int action, int mods)
{
  if (current && key != GLFW_KEY_UNKNOWN)
    current->state.mouse[key] = (action != GLFW_RELEASE);

  if (prevMouseButtonCallback != NULL)
    prevMouseButtonCallback(window, key, action, mods);
}

void GameWindow::update()
{
  state.prev = state;

  state.gameTime = glfwGetTime();
  glfwGetCursorPos(window, &state.mouseX, &state.mouseY);

  current = this;
  prevKeyboardCallback = glfwSetKeyCallback(window, keyboardCallback);
  prevMouseButtonCallback = glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwPollEvents();
  glfwSetKeyCallback(window, prevKeyboardCallback);
  glfwSetMouseButtonCallback(window, prevMouseButtonCallback);
  current = NULL;

  state.cursorCaptured = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

  // If the 1st frame
  if (state.prev.gameTime < 0)
    state.prev = state;
}
