#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// Button events
enum ButtonEvent {
    BTN_NONE,
    BTN_RED_PRESS,
    BTN_BLUE_PRESS,
    BTN_UNDO,          // Both held 1 second
    BTN_RESET_HOLD     // Both held 3 seconds
};

void buttons_init();
ButtonEvent buttons_poll();

#endif
