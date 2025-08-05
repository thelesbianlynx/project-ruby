#pragma once

// Standard Library Includes.
#include "main.h"

// OpenGL Include.
#include <glad/glad.h>

struct window;
struct window_impl;

enum {
    KEY_STATE_PRESSED = 1,
    KEY_STATE_RELEASED = 0,
};

typedef void (*window_key_event_fn) (Window* window, uint32_t key, uint32_t state);
typedef void (*window_mouse_hover_event_fn) (Window* window, bool hover);
typedef void (*window_mouse_motion_event_fn) (Window* window, double x, double y);
typedef void (*window_mouse_button_event_fn) (Window* window, uint32_t button, uint32_t state);
typedef void (*window_mouse_scroll_event_fn) (Window* window, double sx, double sy);
typedef void (*window_focus_change_event_fn) (Window* window, bool focus);
typedef void (*window_resize_event_fn) (Window* window, int width, int height);


struct window {
    struct window_impl* impl;
    void* user;

    struct {
        window_key_event_fn on_key_event;
        window_mouse_hover_event_fn on_mouse_hover_event;
        window_mouse_motion_event_fn on_mouse_motion_event;
        window_mouse_button_event_fn on_mouse_button_event;
        window_mouse_scroll_event_fn on_mouse_scroll_event;
        window_focus_change_event_fn on_focus_change_event;
        window_resize_event_fn on_resize_event;
    } events;
};


int window_init_backend ();
void window_terminate_backend ();

Window* window_create (const char* title, int width, int height);
void window_destroy (Window* window);

void window_grab_mouse (Window* window, bool grabbed);

bool window_get_state (Window* window);
void window_get_size (Window* window, int* width, int* height);

void window_set_should_close (Window* window);

void window_run_events (Window* window);

void window_commit (Window* window);
