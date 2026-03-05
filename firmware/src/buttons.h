#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// Button events
enum ButtonEvent {
    BTN_NONE,
    BTN_RED_PRESS,
    BTN_BLUE_PRESS,
    BTN_RESET_HOLD,
    BTN_UNDO       // Both red+blue held together
};

void buttons_init();
ButtonEvent buttons_poll();

#endif
