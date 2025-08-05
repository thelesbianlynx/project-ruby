#include "window.h"

#include <GLFW/glfw3.h>

struct window_impl {
    GLFWwindow* glfw_window;
};

static void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void glfw_mouse_motion_callback (GLFWwindow* window, double xpos, double ypos);
static void glfw_mouse_scrolled_callback (GLFWwindow* window, double xoffset, double yoffset);
static void glfw_window_focus_callback (GLFWwindow* window, int focused);
static void glfw_window_resize_callback (GLFWwindow* window, int width, int height);


int window_init_backend () {
    if (glfwPlatformSupported(GLFW_PLATFORM_X11)) {
        // Force X11 Until GLFW's Wayland Support Gets Better.
        // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }
    return glfwInit() == GLFW_TRUE ? 0 : -1;
}

void window_terminate_backend () {
    glfwTerminate();
}


Window* window_create (const char* title, int width, int height) {
    // GLFW window parameters/hints.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    // Create GLFW window.
    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!glfw_window) {
        return NULL;
    }

    // Make OpenGL Context Current for the loader.
    glfwMakeContextCurrent(glfw_window);

    // OpenGL Loader.
    if (!gladLoadGL()) {
        printf("gladLoadGLLoader() Error\n");
        glfwDestroyWindow(glfw_window);
        return NULL;
    }

    // Check version.
    printf("%s\n", glGetString(GL_VERSION));

    // Set Viewport based on display scale.
    //      GLFW Provides no way of disserning which monitor the
    //      window is on so we kinda just guess.
    float xscale = 1, yscale = 1;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    glViewport(0, 0, xscale*width, yscale*height);
    printf("%f %f\n", xscale, yscale);

    // Set clear color.
    glClearColor(1,0,1,1);

    // Set GLFW Callbacks
    glfwSetKeyCallback(glfw_window, glfw_key_callback);
    glfwSetMouseButtonCallback(glfw_window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback (glfw_window, glfw_mouse_motion_callback);
    glfwSetScrollCallback (glfw_window, glfw_mouse_scrolled_callback);
    glfwSetWindowFocusCallback (glfw_window, glfw_window_focus_callback);
    glfwSetWindowSizeCallback (glfw_window, glfw_window_resize_callback);

    // Allocate and Initialize
    Window* new_window = malloc(sizeof(struct window));
    struct window_impl* impl = malloc(sizeof(struct window_impl));
    impl->glfw_window = glfw_window;
    new_window->impl = impl;
    new_window->user = NULL;
    new_window->events.on_key_event = NULL;
    new_window->events.on_mouse_hover_event = NULL;
    new_window->events.on_mouse_button_event = NULL;
    new_window->events.on_mouse_motion_event = NULL;
    new_window->events.on_mouse_scroll_event = NULL;
    new_window->events.on_focus_change_event = NULL;
    new_window->events.on_resize_event = NULL;

    // Set User-Pointer.
    glfwSetWindowUserPointer(glfw_window, new_window);

    // Return.
    return new_window;
}

void window_destroy (Window* window) {
    glfwDestroyWindow(window->impl->glfw_window);
    free(window->impl);
    free(window);
}

bool window_get_state (Window* window) {
    int shouldClose = glfwWindowShouldClose(window->impl->glfw_window);
    return shouldClose == GLFW_TRUE ? false : true;
}

void window_get_size (Window* window, int* width, int* height) {
    glfwGetWindowSize(window->impl->glfw_window, width, height);
}


void window_grab_mouse (Window* window, bool grabbed) {
    glfwSetInputMode(window->impl->glfw_window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void window_set_should_close (Window* window) {
    glfwSetWindowShouldClose(window->impl->glfw_window, GLFW_TRUE);
}

void window_run_events (Window* window) {
    glfwPollEvents();
}

void window_commit (Window* window) {
    glfwSwapBuffers(window->impl->glfw_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


static void glfw_key_callback (GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
    Window* window = glfwGetWindowUserPointer(glfw_window);
    if (window->events.on_key_event != NULL) {
        window->events.on_key_event(window, key, action);
    }
}

static void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods) {
    Window* window = glfwGetWindowUserPointer(glfw_window);
    if (window->events.on_mouse_button_event != NULL) {
        window->events.on_mouse_button_event(window, button, action);
    }
}

static void glfw_mouse_motion_callback (GLFWwindow* glfw_window, double xpos, double ypos) {
    Window* window = glfwGetWindowUserPointer(glfw_window);
    if (window->events.on_mouse_motion_event != NULL) {
        window->events.on_mouse_motion_event(window, xpos, ypos);
    }
}
static void glfw_mouse_scrolled_callback (GLFWwindow* glfw_window, double xoffset, double yoffset) {
    Window* window = glfwGetWindowUserPointer(glfw_window);
    if (window->events.on_mouse_scroll_event != NULL) {
        window->events.on_mouse_button_event(window, xoffset, yoffset);
    }
}

static void glfw_window_focus_callback (GLFWwindow* glfw_window, int focused) {
    Window* window = glfwGetWindowUserPointer(glfw_window);
    if (window->events.on_focus_change_event != NULL) {
        window->events.on_focus_change_event(window, focused == GLFW_TRUE ? true : false);
    }
}
static void glfw_window_resize_callback (GLFWwindow* glfw_window, int width, int height) {
    Window* window = glfwGetWindowUserPointer(glfw_window);

    // Update Viewport.
    float xscale = 1, yscale = 1;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    glViewport(0,0, xscale*width, yscale*height);

    if (window->events.on_resize_event != NULL) {
        window->events.on_resize_event(window, width, height);
    }
}
