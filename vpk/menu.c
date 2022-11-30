#include "string.h"
#include "stdio.h"
#include "settings.h"
#include "font.h"
#include "color.h"
#include "touchpad.h"
#include "modal.h"
#include "menu.h"
#include "utils.h"
#include "main.h"

#define MENU_START "START"
#define MENU_ACTIVE_DISPLAY "ACTIVE DISPLAY"
#define MENU_BUTTON_MAPPING "BUTTON MAPPING"
#define MENU_BUTTON_ORDER "BUTTON ORDER"
#define MENU_TOUCHPAD_CONTROLS "TOUCHPAD CONTROLS"
#define MENU_RESET_TOUCHPAD_CONTROLS "RESET TOUCHPAD CONTROLS"

char *MENU_ITEMS[6] = {MENU_START, MENU_ACTIVE_DISPLAY, MENU_BUTTON_MAPPING, MENU_BUTTON_ORDER, MENU_TOUCHPAD_CONTROLS, MENU_RESET_TOUCHPAD_CONTROLS};
int CURRENT_MENU_ITEM = 0;

int menuItemSelected(char *id) {
    return strcmp(MENU_ITEMS[CURRENT_MENU_ITEM], id) == 0;
}

int getMenuItemIndex(char *label) {
    for (int i = 0; i < (sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0])); ++i) {
        if (strcmp(MENU_ITEMS[i], label) == 0) return i;
    }
    return 0;
}

void setCurrentMenuItem(int index) {
    CURRENT_MENU_ITEM = index;
}

void handleMenuActions() {
    if (modalActive()) return;
    if (pressed_pad[PAD_CROSS]) {
        if (menuItemSelected(MENU_START)) {
            startPlugin();
        }
        if (menuItemSelected(MENU_BUTTON_MAPPING)) {
            toggleAltLayout();
            saveSettings();
        }
        if (menuItemSelected(MENU_BUTTON_ORDER)) {
            toggleAB();
            saveSettings();
        }
        if (menuItemSelected(MENU_ACTIVE_DISPLAY)) {
            toggleActiveDisplay();
            saveSettings();
        }
        if (menuItemSelected(MENU_TOUCHPAD_CONTROLS)) {
            setCurrentScreen(2);
        }
        if (menuItemSelected(MENU_RESET_TOUCHPAD_CONTROLS)) {
            openTouchPadControlResetToDefaultSettingsModal();
        }
    } else if (pressed_pad[PAD_UP]) {
        if (CURRENT_MENU_ITEM != 0) {
            CURRENT_MENU_ITEM = CURRENT_MENU_ITEM - 1;
        }
    } else if (pressed_pad[PAD_DOWN]) {
        if (CURRENT_MENU_ITEM < ((sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]) - 1))) {
            if (!checkTouchPadControlsChanged(DEFAULT_TOUCHPAD_CONTROL_SETTINGS, TOUCHPAD_CONTROL_SETTINGS) && CURRENT_MENU_ITEM == 4) {
                return;
            }
            CURRENT_MENU_ITEM += 1;
        }
    }
}

char* getControlText(int val) {
    if (val == 0)
        return "Original";
    if (val == 1)
        return "Web suggest";
    return "Xinput";
}

void displayMenu(int top) {
    for (int i = 0; i < (sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0])); ++i) {
        char text[MAX_MENU_ITEM_TITLE_LENGTH];

        if (strcmp(MENU_ITEMS[i], MENU_ACTIVE_DISPLAY) == 0) {
            sprintf(text, "%s %s", MENU_ITEMS[i], SETTINGS.ACTIVE_DISPLAY ? "ON" : "OFF");
        } else if (strcmp(MENU_ITEMS[i], MENU_BUTTON_MAPPING) == 0) { 
            sprintf(text, "%s %s", MENU_ITEMS[i], SETTINGS.BUTTON_MAPPING == 0 ? "default" : SETTINGS.BUTTON_MAPPING == 1 ? "web" : "xinput");
        } else if (strcmp(MENU_ITEMS[i], MENU_BUTTON_ORDER) == 0) { 
            sprintf(text, "%s %s", MENU_ITEMS[i], SETTINGS.BUTTON_ORDER ? "Xbox (O=B, X=A)" : "Nintendo (O=A, X=B)");
        } else {
            strcpy(text, MENU_ITEMS[i]);
        }

        int active = i == CURRENT_MENU_ITEM;
        int titleX = SCREEN_HALF_WIDTH - (pgf_text_width(text) / 2);
        int titleY = top + (i * (FONT_Y_SPACE + 20));

        if (strcmp(MENU_ITEMS[i], "RESET TOUCHPAD CONTROLS") == 0) {
            if (checkTouchPadControlsChanged(DEFAULT_TOUCHPAD_CONTROL_SETTINGS, TOUCHPAD_CONTROL_SETTINGS)) {
                pgf_draw_text(titleX, titleY, active ? BLACK : BLACK_OPACITY_30, text);
            }
        } else {
            pgf_draw_text(titleX, titleY, active ? BLACK : BLACK_OPACITY_30, text);
        }
    }
}
