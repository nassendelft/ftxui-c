#ifndef FTXUI_C_API_H
#define FTXUI_C_API_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handles
typedef void* ftxui_app_handle_t;
typedef void* ftxui_component_handle_t;
typedef void* ftxui_element_handle_t;
typedef void* ftxui_color_handle_t;
typedef void* ftxui_captured_mouse_handle_t;
typedef void* ftxui_event_handle_t;

typedef ftxui_element_handle_t (*ftxui_render_callback_t)(void* userdata);
typedef void (*ftxui_callback_t)(void* userdata);
typedef bool (*ftxui_predicate_callback_t)(void* userdata);

// =============================================================================
// Forward-declared types  (used by earlier sections than their logical home)
// =============================================================================
// ftxui_easing_function_t and ftxui_easing_function_type_t belong to §22
// but are needed here because ftxui_animated_color_option_t in §17 references them.
// ftxui_border_style_t belongs to §8 but is needed in §10, §15, and §16.

typedef float (*ftxui_easing_function_t)(float progress);

typedef enum {
    FTXUI_EASING_LINEAR,
    FTXUI_EASING_QUADRATIC_IN,
    FTXUI_EASING_QUADRATIC_OUT,
    FTXUI_EASING_QUADRATIC_IN_OUT,
    FTXUI_EASING_CUBIC_IN,
    FTXUI_EASING_CUBIC_OUT,
    FTXUI_EASING_CUBIC_IN_OUT,
    FTXUI_EASING_QUARTIC_IN,
    FTXUI_EASING_QUARTIC_OUT,
    FTXUI_EASING_QUARTIC_IN_OUT,
    FTXUI_EASING_QUINTIC_IN,
    FTXUI_EASING_QUINTIC_OUT,
    FTXUI_EASING_QUINTIC_IN_OUT,
    FTXUI_EASING_SINE_IN,
    FTXUI_EASING_SINE_OUT,
    FTXUI_EASING_SINE_IN_OUT,
    FTXUI_EASING_CIRCULAR_IN,
    FTXUI_EASING_CIRCULAR_OUT,
    FTXUI_EASING_CIRCULAR_IN_OUT,
    FTXUI_EASING_EXPONENTIAL_IN,
    FTXUI_EASING_EXPONENTIAL_OUT,
    FTXUI_EASING_EXPONENTIAL_IN_OUT,
    FTXUI_EASING_ELASTIC_IN,
    FTXUI_EASING_ELASTIC_OUT,
    FTXUI_EASING_ELASTIC_IN_OUT,
    FTXUI_EASING_BACK_IN,
    FTXUI_EASING_BACK_OUT,
    FTXUI_EASING_BACK_IN_OUT,
    FTXUI_EASING_BOUNCE_IN,
    FTXUI_EASING_BOUNCE_OUT,
    FTXUI_EASING_BOUNCE_IN_OUT,
} ftxui_easing_function_type_t;

typedef enum {
    FTXUI_BORDER_STYLE_LIGHT,
    FTXUI_BORDER_STYLE_DASHED,
    FTXUI_BORDER_STYLE_HEAVY,
    FTXUI_BORDER_STYLE_DOUBLE,
    FTXUI_BORDER_STYLE_ROUNDED,
    FTXUI_BORDER_STYLE_EMPTY,
} ftxui_border_style_t;

// =============================================================================
// §1  App  (ftxui/component/app.hpp)
// =============================================================================

ftxui_app_handle_t ftxui_app_create_fullscreen();
ftxui_app_handle_t ftxui_app_create_fit_component();
ftxui_app_handle_t ftxui_app_create_terminal_output();
ftxui_app_handle_t ftxui_app_create_fixed_size(int w, int h);
ftxui_app_handle_t ftxui_app_create_fullscreen_primary_screen();
ftxui_app_handle_t ftxui_app_create_fullscreen_alternate_screen();

void ftxui_app_loop(ftxui_app_handle_t app, ftxui_component_handle_t component);
void ftxui_app_exit(ftxui_app_handle_t app);
void ftxui_app_destroy(ftxui_app_handle_t app);

// --- App Configuration (call before ftxui_app_loop) ---

/**
 * @brief Enable or disable mouse event tracking. Enabled by default.
 */
void ftxui_app_track_mouse(ftxui_app_handle_t app, bool enable);

/**
 * @brief Enable or disable automatic piped input handling. Enabled by default.
 */
void ftxui_app_handle_piped_input(ftxui_app_handle_t app, bool enable);

/**
 * @brief Force FTXUI to handle or not handle Ctrl-C regardless of component event handling.
 */
void ftxui_app_force_handle_ctrl_c(ftxui_app_handle_t app, bool force);

/**
 * @brief Force FTXUI to handle or not handle Ctrl-Z regardless of component event handling.
 */
void ftxui_app_force_handle_ctrl_z(ftxui_app_handle_t app, bool force);

// --- App Operations ---

/**
 * @brief Post a closure to be executed on the main loop thread. Safe to call from any thread.
 */
void ftxui_app_post(ftxui_app_handle_t app, void (*callback)(void*), void* userdata);

/**
 * @brief Post an event to the main loop. The event is copied.
 */
void ftxui_app_post_event(ftxui_app_handle_t app, ftxui_event_handle_t event);

/**
 * @brief Execute callback with the terminal temporarily restored to its original state.
 */
void ftxui_app_with_restored_io(ftxui_app_handle_t app, void (*callback)(void*), void* userdata);

/**
 * @brief Return the currently active app handle, or NULL if none is running.
 */
ftxui_app_handle_t ftxui_app_active();

/**
 * @brief Request an animation frame; causes the component to re-render.
 */
void ftxui_app_request_animation_frame(ftxui_app_handle_t app);

// --- Terminal Info (via App) ---

/**
 * @brief Return the terminal name. Pointer is valid until the app is destroyed. Do not free.
 */
const char* ftxui_app_terminal_name(ftxui_app_handle_t app);

/**
 * @brief Return the terminal version number.
 */
int ftxui_app_terminal_version(ftxui_app_handle_t app);

/**
 * @brief Return the terminal emulator name. Pointer is valid until the app is destroyed. Do not free.
 */
const char* ftxui_app_terminal_emulator_name(ftxui_app_handle_t app);

/**
 * @brief Return the terminal emulator version string. Pointer is valid until the app is destroyed. Do not free.
 */
const char* ftxui_app_terminal_emulator_version(ftxui_app_handle_t app);

/**
 * @brief Return the terminal capabilities as a malloc'd int array. Sets *count to the number of elements.
 * The caller must free() the returned pointer. Returns NULL if there are no capabilities.
 */
int* ftxui_app_terminal_capabilities(ftxui_app_handle_t app, int* count);

// --- Mouse Capture ---

/**
 * @brief Try to capture the mouse exclusively. Returns NULL if already captured.
 * Destroy the handle with ftxui_captured_mouse_destroy to release the capture.
 */
ftxui_captured_mouse_handle_t ftxui_app_capture_mouse(ftxui_app_handle_t app);

/**
 * @brief Release a captured mouse handle obtained from ftxui_app_capture_mouse.
 */
void ftxui_captured_mouse_destroy(ftxui_captured_mouse_handle_t handle);

// --- Selection ---

/**
 * @brief Registers a callback invoked whenever the terminal text selection changes.
 */
void ftxui_app_selection_change(ftxui_app_handle_t app, void (*callback)(void*), void* userdata);

/**
 * @brief Returns the currently selected text. Caller must free() the returned string.
 */
char* ftxui_app_get_selection(ftxui_app_handle_t app);

// =============================================================================
// §2  Terminal  (ftxui/screen/terminal.hpp)
// =============================================================================

/**
 * @brief Terminal color support level, matching ftxui::Terminal::Color.
 */
typedef enum {
    FTXUI_TERMINAL_COLOR_PALETTE1   = 0,
    FTXUI_TERMINAL_COLOR_PALETTE16  = 1,
    FTXUI_TERMINAL_COLOR_PALETTE256 = 2,
    FTXUI_TERMINAL_COLOR_TRUE_COLOR = 3,
} ftxui_terminal_color_t;

/**
 * @brief Flat representation of ftxui::Terminal::Quirks.
 * Retrieve with ftxui_terminal_get_quirks, modify fields, then apply with
 * ftxui_terminal_set_quirks.
 */
typedef struct {
    bool block_characters;
    bool cursor_hiding;
    bool component_ascii;
    ftxui_terminal_color_t color_support;
} ftxui_quirks_t;

/**
 * @brief Returns the current terminal width in columns.
 */
int ftxui_terminal_width();

/**
 * @brief Returns the current terminal height in rows.
 */
int ftxui_terminal_height();

/**
 * @brief Set the fallback terminal size used when auto-detection fails.
 */
void ftxui_terminal_set_fallback_size(int w, int h);

/**
 * @brief Return the global terminal color support level.
 */
ftxui_terminal_color_t ftxui_terminal_color_support(void);

/**
 * @brief Override the global terminal color support level.
 */
void ftxui_terminal_set_color_support(ftxui_terminal_color_t color);

/**
 * @brief Return the current terminal quirks as a flat struct.
 */
ftxui_quirks_t ftxui_terminal_get_quirks(void);

/**
 * @brief Apply terminal quirks from a flat struct.
 */
void ftxui_terminal_set_quirks(ftxui_quirks_t quirks);

// =============================================================================
// §3  Loop  (ftxui/component/loop.hpp)
// =============================================================================

typedef void* ftxui_loop_handle_t;
ftxui_loop_handle_t ftxui_loop_create(ftxui_app_handle_t app, ftxui_component_handle_t component);
bool ftxui_loop_has_quitted(ftxui_loop_handle_t loop);
void ftxui_loop_run_once(ftxui_loop_handle_t loop);
void ftxui_loop_run_once_blocking(ftxui_loop_handle_t loop);
void ftxui_loop_destroy(ftxui_loop_handle_t loop);

// =============================================================================
// §4  Color  (ftxui/screen/color.hpp)
// =============================================================================

typedef enum {
  FTXUI_PALETTE1_DEFAULT = 0, // Transparent
} ftxui_palette1_t;

typedef enum {
  FTXUI_PALETTE16_BLACK = 0,
  FTXUI_PALETTE16_RED = 1,
  FTXUI_PALETTE16_GREEN = 2,
  FTXUI_PALETTE16_YELLOW = 3,
  FTXUI_PALETTE16_BLUE = 4,
  FTXUI_PALETTE16_MAGENTA = 5,
  FTXUI_PALETTE16_CYAN = 6,
  FTXUI_PALETTE16_GRAY_LIGHT = 7,
  FTXUI_PALETTE16_GRAY_DARK = 8,
  FTXUI_PALETTE16_RED_LIGHT = 9,
  FTXUI_PALETTE16_GREEN_LIGHT = 10,
  FTXUI_PALETTE16_YELLOW_LIGHT = 11,
  FTXUI_PALETTE16_BLUE_LIGHT = 12,
  FTXUI_PALETTE16_MAGENTA_LIGHT = 13,
  FTXUI_PALETTE16_CYAN_LIGHT = 14,
  FTXUI_PALETTE16_WHITE = 15,
} ftxui_palette16_t;

typedef enum {
  FTXUI_PALETTE256_AQUAMARINE1 = 122,
  FTXUI_PALETTE256_AQUAMARINE1BIS = 86,
  FTXUI_PALETTE256_AQUAMARINE3 = 79,
  FTXUI_PALETTE256_BLUE1 = 21,
  FTXUI_PALETTE256_BLUE3 = 19,
  FTXUI_PALETTE256_BLUE3BIS = 20,
  FTXUI_PALETTE256_BLUEVIOLET = 57,
  FTXUI_PALETTE256_CADETBLUE = 72,
  FTXUI_PALETTE256_CADETBLUEBIS = 73,
  FTXUI_PALETTE256_CHARTREUSE1 = 118,
  FTXUI_PALETTE256_CHARTREUSE2 = 112,
  FTXUI_PALETTE256_CHARTREUSE2BIS = 82,
  FTXUI_PALETTE256_CHARTREUSE3 = 70,
  FTXUI_PALETTE256_CHARTREUSE3BIS = 76,
  FTXUI_PALETTE256_CHARTREUSE4 = 64,
  FTXUI_PALETTE256_CORNFLOWERBLUE = 69,
  FTXUI_PALETTE256_CORNSILK1 = 230,
  FTXUI_PALETTE256_CYAN1 = 51,
  FTXUI_PALETTE256_CYAN2 = 50,
  FTXUI_PALETTE256_CYAN3 = 43,
  FTXUI_PALETTE256_DARKBLUE = 18,
  FTXUI_PALETTE256_DARKCYAN = 36,
  FTXUI_PALETTE256_DARKGOLDENROD = 136,
  FTXUI_PALETTE256_DARKGREEN = 22,
  FTXUI_PALETTE256_DARKKHAKI = 143,
  FTXUI_PALETTE256_DARKMAGENTA = 90,
  FTXUI_PALETTE256_DARKMAGENTABIS = 91,
  FTXUI_PALETTE256_DARKOLIVEGREEN1 = 191,
  FTXUI_PALETTE256_DARKOLIVEGREEN1BIS = 192,
  FTXUI_PALETTE256_DARKOLIVEGREEN2 = 155,
  FTXUI_PALETTE256_DARKOLIVEGREEN3 = 107,
  FTXUI_PALETTE256_DARKOLIVEGREEN3BIS = 113,
  FTXUI_PALETTE256_DARKOLIVEGREEN3TER = 149,
  FTXUI_PALETTE256_DARKORANGE = 208,
  FTXUI_PALETTE256_DARKORANGE3 = 130,
  FTXUI_PALETTE256_DARKORANGE3BIS = 166,
  FTXUI_PALETTE256_DARKRED = 52,
  FTXUI_PALETTE256_DARKREDBIS = 88,
  FTXUI_PALETTE256_DARKSEAGREEN = 108,
  FTXUI_PALETTE256_DARKSEAGREEN1 = 158,
  FTXUI_PALETTE256_DARKSEAGREEN1BIS = 193,
  FTXUI_PALETTE256_DARKSEAGREEN2 = 151,
  FTXUI_PALETTE256_DARKSEAGREEN2BIS = 157,
  FTXUI_PALETTE256_DARKSEAGREEN3 = 115,
  FTXUI_PALETTE256_DARKSEAGREEN3BIS = 150,
  FTXUI_PALETTE256_DARKSEAGREEN4 = 65,
  FTXUI_PALETTE256_DARKSEAGREEN4BIS = 71,
  FTXUI_PALETTE256_DARKSLATEGRAY1 = 123,
  FTXUI_PALETTE256_DARKSLATEGRAY2 = 87,
  FTXUI_PALETTE256_DARKSLATEGRAY3 = 116,
  FTXUI_PALETTE256_DARKTURQUOISE = 44,
  FTXUI_PALETTE256_DARKVIOLET = 128,
  FTXUI_PALETTE256_DARKVIOLETBIS = 92,
  FTXUI_PALETTE256_DEEPPINK1 = 198,
  FTXUI_PALETTE256_DEEPPINK1BIS = 199,
  FTXUI_PALETTE256_DEEPPINK2 = 197,
  FTXUI_PALETTE256_DEEPPINK3 = 161,
  FTXUI_PALETTE256_DEEPPINK3BIS = 162,
  FTXUI_PALETTE256_DEEPPINK4 = 125,
  FTXUI_PALETTE256_DEEPPINK4BIS = 89,
  FTXUI_PALETTE256_DEEPPINK4TER = 53,
  FTXUI_PALETTE256_DEEPSKYBLUE1 = 39,
  FTXUI_PALETTE256_DEEPSKYBLUE2 = 38,
  FTXUI_PALETTE256_DEEPSKYBLUE3 = 31,
  FTXUI_PALETTE256_DEEPSKYBLUE3BIS = 32,
  FTXUI_PALETTE256_DEEPSKYBLUE4 = 23,
  FTXUI_PALETTE256_DEEPSKYBLUE4BIS = 24,
  FTXUI_PALETTE256_DEEPSKYBLUE4TER = 25,
  FTXUI_PALETTE256_DODGERBLUE1 = 33,
  FTXUI_PALETTE256_DODGERBLUE2 = 27,
  FTXUI_PALETTE256_DODGERBLUE3 = 26,
  FTXUI_PALETTE256_GOLD1 = 220,
  FTXUI_PALETTE256_GOLD3 = 142,
  FTXUI_PALETTE256_GOLD3BIS = 178,
  FTXUI_PALETTE256_GREEN1 = 46,
  FTXUI_PALETTE256_GREEN3 = 34,
  FTXUI_PALETTE256_GREEN3BIS = 40,
  FTXUI_PALETTE256_GREEN4 = 28,
  FTXUI_PALETTE256_GREENYELLOW = 154,
  FTXUI_PALETTE256_GREY0 = 16,
  FTXUI_PALETTE256_GREY100 = 231,
  FTXUI_PALETTE256_GREY11 = 234,
  FTXUI_PALETTE256_GREY15 = 235,
  FTXUI_PALETTE256_GREY19 = 236,
  FTXUI_PALETTE256_GREY23 = 237,
  FTXUI_PALETTE256_GREY27 = 238,
  FTXUI_PALETTE256_GREY3 = 232,
  FTXUI_PALETTE256_GREY30 = 239,
  FTXUI_PALETTE256_GREY35 = 240,
  FTXUI_PALETTE256_GREY37 = 59,
  FTXUI_PALETTE256_GREY39 = 241,
  FTXUI_PALETTE256_GREY42 = 242,
  FTXUI_PALETTE256_GREY46 = 243,
  FTXUI_PALETTE256_GREY50 = 244,
  FTXUI_PALETTE256_GREY53 = 102,
  FTXUI_PALETTE256_GREY54 = 245,
  FTXUI_PALETTE256_GREY58 = 246,
  FTXUI_PALETTE256_GREY62 = 247,
  FTXUI_PALETTE256_GREY63 = 139,
  FTXUI_PALETTE256_GREY66 = 248,
  FTXUI_PALETTE256_GREY69 = 145,
  FTXUI_PALETTE256_GREY7 = 233,
  FTXUI_PALETTE256_GREY70 = 249,
  FTXUI_PALETTE256_GREY74 = 250,
  FTXUI_PALETTE256_GREY78 = 251,
  FTXUI_PALETTE256_GREY82 = 252,
  FTXUI_PALETTE256_GREY84 = 188,
  FTXUI_PALETTE256_GREY85 = 253,
  FTXUI_PALETTE256_GREY89 = 254,
  FTXUI_PALETTE256_GREY93 = 255,
  FTXUI_PALETTE256_HONEYDEW2 = 194,
  FTXUI_PALETTE256_HOTPINK = 205,
  FTXUI_PALETTE256_HOTPINK2 = 169,
  FTXUI_PALETTE256_HOTPINK3 = 132,
  FTXUI_PALETTE256_HOTPINK3BIS = 168,
  FTXUI_PALETTE256_HOTPINKBIS = 206,
  FTXUI_PALETTE256_INDIANRED = 131,
  FTXUI_PALETTE256_INDIANRED1 = 203,
  FTXUI_PALETTE256_INDIANRED1BIS = 204,
  FTXUI_PALETTE256_INDIANREDBIS = 167,
  FTXUI_PALETTE256_KHAKI1 = 228,
  FTXUI_PALETTE256_KHAKI3 = 185,
  FTXUI_PALETTE256_LIGHTCORAL = 210,
  FTXUI_PALETTE256_LIGHTCYAN1BIS = 195,
  FTXUI_PALETTE256_LIGHTCYAN3 = 152,
  FTXUI_PALETTE256_LIGHTGOLDENROD1 = 227,
  FTXUI_PALETTE256_LIGHTGOLDENROD2 = 186,
  FTXUI_PALETTE256_LIGHTGOLDENROD2BIS = 221,
  FTXUI_PALETTE256_LIGHTGOLDENROD2TER = 222,
  FTXUI_PALETTE256_LIGHTGOLDENROD3 = 179,
  FTXUI_PALETTE256_LIGHTGREEN = 119,
  FTXUI_PALETTE256_LIGHTGREENBIS = 120,
  FTXUI_PALETTE256_LIGHTPINK1 = 217,
  FTXUI_PALETTE256_LIGHTPINK3 = 174,
  FTXUI_PALETTE256_LIGHTPINK4 = 95,
  FTXUI_PALETTE256_LIGHTSALMON1 = 216,
  FTXUI_PALETTE256_LIGHTSALMON3 = 137,
  FTXUI_PALETTE256_LIGHTSALMON3BIS = 173,
  FTXUI_PALETTE256_LIGHTSEAGREEN = 37,
  FTXUI_PALETTE256_LIGHTSKYBLUE1 = 153,
  FTXUI_PALETTE256_LIGHTSKYBLUE3 = 109,
  FTXUI_PALETTE256_LIGHTSKYBLUE3BIS = 110,
  FTXUI_PALETTE256_LIGHTSLATEBLUE = 105,
  FTXUI_PALETTE256_LIGHTSLATEGREY = 103,
  FTXUI_PALETTE256_LIGHTSTEELBLUE = 147,
  FTXUI_PALETTE256_LIGHTSTEELBLUE1 = 189,
  FTXUI_PALETTE256_LIGHTSTEELBLUE3 = 146,
  FTXUI_PALETTE256_LIGHTYELLOW3 = 187,
  FTXUI_PALETTE256_MAGENTA1 = 201,
  FTXUI_PALETTE256_MAGENTA2 = 165,
  FTXUI_PALETTE256_MAGENTA2BIS = 200,
  FTXUI_PALETTE256_MAGENTA3 = 127,
  FTXUI_PALETTE256_MAGENTA3BIS = 163,
  FTXUI_PALETTE256_MAGENTA3TER = 164,
  FTXUI_PALETTE256_MEDIUMORCHID = 134,
  FTXUI_PALETTE256_MEDIUMORCHID1 = 171,
  FTXUI_PALETTE256_MEDIUMORCHID1BIS = 207,
  FTXUI_PALETTE256_MEDIUMORCHID3 = 133,
  FTXUI_PALETTE256_MEDIUMPURPLE = 104,
  FTXUI_PALETTE256_MEDIUMPURPLE1 = 141,
  FTXUI_PALETTE256_MEDIUMPURPLE2 = 135,
  FTXUI_PALETTE256_MEDIUMPURPLE2BIS = 140,
  FTXUI_PALETTE256_MEDIUMPURPLE3 = 97,
  FTXUI_PALETTE256_MEDIUMPURPLE3BIS = 98,
  FTXUI_PALETTE256_MEDIUMPURPLE4 = 60,
  FTXUI_PALETTE256_MEDIUMSPRINGGREEN = 49,
  FTXUI_PALETTE256_MEDIUMTURQUOISE = 80,
  FTXUI_PALETTE256_MEDIUMVIOLETRED = 126,
  FTXUI_PALETTE256_MISTYROSE1 = 224,
  FTXUI_PALETTE256_MISTYROSE3 = 181,
  FTXUI_PALETTE256_NAVAJOWHITE1 = 223,
  FTXUI_PALETTE256_NAVAJOWHITE3 = 144,
  FTXUI_PALETTE256_NAVYBLUE = 17,
  FTXUI_PALETTE256_ORANGE1 = 214,
  FTXUI_PALETTE256_ORANGE3 = 172,
  FTXUI_PALETTE256_ORANGE4 = 58,
  FTXUI_PALETTE256_ORANGE4BIS = 94,
  FTXUI_PALETTE256_ORANGERED1 = 202,
  FTXUI_PALETTE256_ORCHID = 170,
  FTXUI_PALETTE256_ORCHID1 = 213,
  FTXUI_PALETTE256_ORCHID2 = 212,
  FTXUI_PALETTE256_PALEGREEN1 = 121,
  FTXUI_PALETTE256_PALEGREEN1BIS = 156,
  FTXUI_PALETTE256_PALEGREEN3 = 114,
  FTXUI_PALETTE256_PALEGREEN3BIS = 77,
  FTXUI_PALETTE256_PALETURQUOISE1 = 159,
  FTXUI_PALETTE256_PALETURQUOISE4 = 66,
  FTXUI_PALETTE256_PALEVIOLETRED1 = 211,
  FTXUI_PALETTE256_PINK1 = 218,
  FTXUI_PALETTE256_PINK3 = 175,
  FTXUI_PALETTE256_PLUM1 = 219,
  FTXUI_PALETTE256_PLUM2 = 183,
  FTXUI_PALETTE256_PLUM3 = 176,
  FTXUI_PALETTE256_PLUM4 = 96,
  FTXUI_PALETTE256_PURPLE = 129,
  FTXUI_PALETTE256_PURPLE3 = 56,
  FTXUI_PALETTE256_PURPLE4 = 54,
  FTXUI_PALETTE256_PURPLE4BIS = 55,
  FTXUI_PALETTE256_PURPLEBIS = 93,
  FTXUI_PALETTE256_RED1 = 196,
  FTXUI_PALETTE256_RED3 = 124,
  FTXUI_PALETTE256_RED3BIS = 160,
  FTXUI_PALETTE256_ROSYBROWN = 138,
  FTXUI_PALETTE256_ROYALBLUE1 = 63,
  FTXUI_PALETTE256_SALMON1 = 209,
  FTXUI_PALETTE256_SANDYBROWN = 215,
  FTXUI_PALETTE256_SEAGREEN1 = 84,
  FTXUI_PALETTE256_SEAGREEN1BIS = 85,
  FTXUI_PALETTE256_SEAGREEN2 = 83,
  FTXUI_PALETTE256_SEAGREEN3 = 78,
  FTXUI_PALETTE256_SKYBLUE1 = 117,
  FTXUI_PALETTE256_SKYBLUE2 = 111,
  FTXUI_PALETTE256_SKYBLUE3 = 74,
  FTXUI_PALETTE256_SLATEBLUE1 = 99,
  FTXUI_PALETTE256_SLATEBLUE3 = 61,
  FTXUI_PALETTE256_SLATEBLUE3BIS = 62,
  FTXUI_PALETTE256_SPRINGGREEN1 = 48,
  FTXUI_PALETTE256_SPRINGGREEN2 = 42,
  FTXUI_PALETTE256_SPRINGGREEN2BIS = 47,
  FTXUI_PALETTE256_SPRINGGREEN3 = 35,
  FTXUI_PALETTE256_SPRINGGREEN3BIS = 41,
  FTXUI_PALETTE256_SPRINGGREEN4 = 29,
  FTXUI_PALETTE256_STEELBLUE = 67,
  FTXUI_PALETTE256_STEELBLUE1 = 75,
  FTXUI_PALETTE256_STEELBLUE1BIS = 81,
  FTXUI_PALETTE256_STEELBLUE3 = 68,
  FTXUI_PALETTE256_TAN = 180,
  FTXUI_PALETTE256_THISTLE1 = 225,
  FTXUI_PALETTE256_THISTLE3 = 182,
  FTXUI_PALETTE256_TURQUOISE2 = 45,
  FTXUI_PALETTE256_TURQUOISE4 = 30,
  FTXUI_PALETTE256_VIOLET = 177,
  FTXUI_PALETTE256_WHEAT1 = 229,
  FTXUI_PALETTE256_WHEAT4 = 101,
  FTXUI_PALETTE256_YELLOW1 = 226,
  FTXUI_PALETTE256_YELLOW2 = 190,
  FTXUI_PALETTE256_YELLOW3 = 148,
  FTXUI_PALETTE256_YELLOW3BIS = 184,
  FTXUI_PALETTE256_YELLOW4 = 100,
  FTXUI_PALETTE256_YELLOW4BIS = 106,
} ftxui_palette256_t;

/**
 * @brief Creates a default (transparent) Color object.
 */
ftxui_color_handle_t ftxui_color_default();
/**
 * @brief Creates a Color object from RGB values.
 */
ftxui_color_handle_t ftxui_color_rgb(uint8_t r, uint8_t g, uint8_t b);
/**
 * @brief Creates a Color object from RGBA values.
 */
ftxui_color_handle_t ftxui_color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
/**
 * @brief Creates a Color object from HSV values (h, s, v each 0-255).
 */
ftxui_color_handle_t ftxui_color_hsv(uint8_t h, uint8_t s, uint8_t v);
/**
 * @brief Creates a Color object from HSVA values.
 */
ftxui_color_handle_t ftxui_color_hsva(uint8_t h, uint8_t s, uint8_t v, uint8_t a);
ftxui_color_handle_t ftxui_color_palette1(ftxui_palette1_t index);
ftxui_color_handle_t ftxui_color_palette16(ftxui_palette16_t index);
ftxui_color_handle_t ftxui_color_palette256(ftxui_palette256_t index);
/**
 * @brief Creates a Color object from a raw 256-color palette index (0-255).
 */
ftxui_color_handle_t ftxui_color_palette256_raw(int index);
ftxui_color_handle_t ftxui_color_interpolate(float t, ftxui_color_handle_t a, ftxui_color_handle_t b);
ftxui_color_handle_t ftxui_color_blend(ftxui_color_handle_t lhs, ftxui_color_handle_t rhs);
bool ftxui_color_is_opaque(ftxui_color_handle_t color);
bool ftxui_color_equals(ftxui_color_handle_t lhs, ftxui_color_handle_t rhs);
bool ftxui_color_not_equals(ftxui_color_handle_t lhs, ftxui_color_handle_t rhs);
/**
 * @brief Returns a string representation of the color. The returned string must be freed using free().
 */
char* ftxui_color_print(ftxui_color_handle_t color, bool is_background_color);
void ftxui_color_destroy(ftxui_color_handle_t color);

// =============================================================================
// §5  Color Info  (ftxui/screen/color_info.hpp)
// =============================================================================

typedef struct {
    int         index_256;   // -1 for padding entries in ftxui_color_info_sorted_2d
    uint8_t     index_16;
    const char* name;
    uint8_t     red;
    uint8_t     green;
    uint8_t     blue;
    uint8_t     hue;
    uint8_t     saturation;
    uint8_t     value;
} ftxui_color_info_t;

// Returns a flat row-major array of (num_rows * max_cols) entries.
// Entries with index_256 == -1 are padding.
// Caller must call ftxui_color_info_free() on the returned pointer.
ftxui_color_info_t* ftxui_color_info_sorted_2d(int* num_rows, int* max_cols);
void ftxui_color_info_free(ftxui_color_info_t* data);

// Returns color info for a single Palette256 or Palette16 entry by value.
// The name pointer is valid for the lifetime of the program.
ftxui_color_info_t ftxui_color_info_get_256(ftxui_palette256_t index);
ftxui_color_info_t ftxui_color_info_get_16(ftxui_palette16_t index);

// =============================================================================
// §6  Linear Gradient  (ftxui/dom/linear_gradient.hpp)
// =============================================================================

typedef void* ftxui_linear_gradient_handle_t;
ftxui_linear_gradient_handle_t ftxui_linear_gradient_create();
void ftxui_linear_gradient_destroy(ftxui_linear_gradient_handle_t gradient);
void ftxui_linear_gradient_angle(ftxui_linear_gradient_handle_t gradient, float angle);
void ftxui_linear_gradient_stop(ftxui_linear_gradient_handle_t gradient, ftxui_color_handle_t color);
void ftxui_linear_gradient_stop_at(ftxui_linear_gradient_handle_t gradient, ftxui_color_handle_t color, float position);
ftxui_element_handle_t ftxui_element_color_linear_gradient(ftxui_element_handle_t element, ftxui_linear_gradient_handle_t gradient);
ftxui_element_handle_t ftxui_element_bgcolor_linear_gradient(ftxui_element_handle_t element, ftxui_linear_gradient_handle_t gradient);

// =============================================================================
// §7  Elements — Basic  (ftxui/dom/elements.hpp)
// =============================================================================

ftxui_element_handle_t ftxui_element_text(const char* text);
ftxui_element_handle_t ftxui_element_vtext(const char* text);
ftxui_element_handle_t ftxui_element_paragraph(const char* text);
ftxui_element_handle_t ftxui_element_paragraph_align_left(const char* text);
ftxui_element_handle_t ftxui_element_paragraph_align_right(const char* text);
ftxui_element_handle_t ftxui_element_paragraph_align_center(const char* text);
ftxui_element_handle_t ftxui_element_paragraph_align_justify(const char* text);
ftxui_element_handle_t ftxui_element_empty();
ftxui_element_handle_t ftxui_element_filler();
ftxui_element_handle_t ftxui_element_gauge(double value);
ftxui_element_handle_t ftxui_element_gauge_left(double value);
ftxui_element_handle_t ftxui_element_gauge_right(double value);
ftxui_element_handle_t ftxui_element_gauge_up(double value);
ftxui_element_handle_t ftxui_element_gauge_down(double value);

typedef enum {
    FTXUI_DIRECTION_UP,
    FTXUI_DIRECTION_DOWN,
    FTXUI_DIRECTION_LEFT,
    FTXUI_DIRECTION_RIGHT,
} ftxui_direction_t;

ftxui_element_handle_t ftxui_element_gauge_direction(double value, ftxui_direction_t direction);
ftxui_element_handle_t ftxui_element_spinner(int charset_index, int image_index);

// The callback fills `output` (pre-allocated array of `width` ints) with graph values [0..height].
typedef void (*ftxui_graph_callback_t)(int width, int height, int* output, void* userdata);
ftxui_element_handle_t ftxui_element_graph(ftxui_graph_callback_t callback, void* userdata);

/**
 * @brief Wraps an element with a window title bar.
 */
ftxui_element_handle_t ftxui_element_window(ftxui_element_handle_t title, ftxui_element_handle_t element);

void ftxui_element_destroy(ftxui_element_handle_t element);

// =============================================================================
// §8  Elements — Separators
// =============================================================================
// NOTE: ftxui_border_style_t is declared above in the forward-declared types block.

ftxui_element_handle_t ftxui_element_separator();
ftxui_element_handle_t ftxui_element_separator_light();
ftxui_element_handle_t ftxui_element_separator_dashed();
ftxui_element_handle_t ftxui_element_separator_heavy();
ftxui_element_handle_t ftxui_element_separator_double();
ftxui_element_handle_t ftxui_element_separator_empty();
ftxui_element_handle_t ftxui_element_separator_styled(ftxui_border_style_t style);
ftxui_element_handle_t ftxui_element_separator_character(const char* character);
ftxui_element_handle_t ftxui_element_separator_hselector(float left, float right, ftxui_color_handle_t unselected_color, ftxui_color_handle_t selected_color);
ftxui_element_handle_t ftxui_element_separator_vselector(float up, float down, ftxui_color_handle_t unselected_color, ftxui_color_handle_t selected_color);

// =============================================================================
// §9  Elements — Layout
// =============================================================================

ftxui_element_handle_t ftxui_element_hbox(ftxui_element_handle_t* elements, int count);
ftxui_element_handle_t ftxui_element_vbox(ftxui_element_handle_t* elements, int count);
ftxui_element_handle_t ftxui_element_dbox(ftxui_element_handle_t* elements, int count);
ftxui_element_handle_t ftxui_element_hflow(ftxui_element_handle_t* elements, int count);
ftxui_element_handle_t ftxui_element_vflow(ftxui_element_handle_t* elements, int count);
// cells: flat row-major array; row_lengths[i] = number of cells in row i
ftxui_element_handle_t ftxui_element_gridbox(ftxui_element_handle_t* cells, int total_cells, int* row_lengths, int row_count);

typedef enum {
    FTXUI_FLEXBOX_DIRECTION_ROW,
    FTXUI_FLEXBOX_DIRECTION_ROW_INVERSED,
    FTXUI_FLEXBOX_DIRECTION_COLUMN,
    FTXUI_FLEXBOX_DIRECTION_COLUMN_INVERSED,
} ftxui_flexbox_direction_t;

typedef enum {
    FTXUI_FLEXBOX_WRAP_NO_WRAP,
    FTXUI_FLEXBOX_WRAP_WRAP,
    FTXUI_FLEXBOX_WRAP_WRAP_INVERSED,
} ftxui_flexbox_wrap_t;

typedef enum {
    FTXUI_FLEXBOX_JUSTIFY_FLEX_START,
    FTXUI_FLEXBOX_JUSTIFY_FLEX_END,
    FTXUI_FLEXBOX_JUSTIFY_CENTER,
    FTXUI_FLEXBOX_JUSTIFY_STRETCH,
    FTXUI_FLEXBOX_JUSTIFY_SPACE_BETWEEN,
    FTXUI_FLEXBOX_JUSTIFY_SPACE_AROUND,
    FTXUI_FLEXBOX_JUSTIFY_SPACE_EVENLY,
} ftxui_flexbox_justify_t;

typedef enum {
    FTXUI_FLEXBOX_ALIGN_ITEMS_FLEX_START,
    FTXUI_FLEXBOX_ALIGN_ITEMS_FLEX_END,
    FTXUI_FLEXBOX_ALIGN_ITEMS_CENTER,
    FTXUI_FLEXBOX_ALIGN_ITEMS_STRETCH,
} ftxui_flexbox_align_items_t;

typedef enum {
    FTXUI_FLEXBOX_ALIGN_CONTENT_FLEX_START,
    FTXUI_FLEXBOX_ALIGN_CONTENT_FLEX_END,
    FTXUI_FLEXBOX_ALIGN_CONTENT_CENTER,
    FTXUI_FLEXBOX_ALIGN_CONTENT_STRETCH,
    FTXUI_FLEXBOX_ALIGN_CONTENT_SPACE_BETWEEN,
    FTXUI_FLEXBOX_ALIGN_CONTENT_SPACE_AROUND,
    FTXUI_FLEXBOX_ALIGN_CONTENT_SPACE_EVENLY,
} ftxui_flexbox_align_content_t;

typedef struct {
    ftxui_flexbox_direction_t direction;
    ftxui_flexbox_wrap_t wrap;
    ftxui_flexbox_justify_t justify_content;
    ftxui_flexbox_align_items_t align_items;
    ftxui_flexbox_align_content_t align_content;
    int gap_x;
    int gap_y;
} ftxui_flexbox_config_t;

ftxui_element_handle_t ftxui_element_flexbox(ftxui_element_handle_t* elements, int count, ftxui_flexbox_config_t config);

// =============================================================================
// §10  Elements — Styling Decorators
// =============================================================================

// --- Border decorators ---
ftxui_element_handle_t ftxui_element_border(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_light(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_dashed(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_heavy(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_double(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_rounded(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_empty(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_border_styled(ftxui_element_handle_t element, ftxui_border_style_t style);
ftxui_element_handle_t ftxui_element_border_styled_color(ftxui_element_handle_t element, ftxui_border_style_t style, ftxui_color_handle_t color);
ftxui_element_handle_t ftxui_element_border_colored(ftxui_element_handle_t element, ftxui_color_handle_t color);

// --- Text style decorators ---
ftxui_element_handle_t ftxui_element_bold(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_dim(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_italic(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_inverted(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_underlined(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_underlined_double(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_blink(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_strikethrough(ftxui_element_handle_t element);

// --- Color decorators ---
ftxui_element_handle_t ftxui_element_color(ftxui_element_handle_t element, ftxui_color_handle_t color);
ftxui_element_handle_t ftxui_element_bgcolor(ftxui_element_handle_t element, ftxui_color_handle_t color);

// --- Misc decorators ---
ftxui_element_handle_t ftxui_element_automerge(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_hyperlink(const char* link, ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_clear_under(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_nothing(ftxui_element_handle_t element);

// --- Selection style ---
// Represents the style of a terminal cell. Used by selectionStyle and canvas stylizer callbacks.
// When used as a canvas stylizer: foreground_color and background_color are temporary handles —
// the callback may read or replace them (setting to NULL leaves the color unchanged), but must NOT
// call ftxui_color_destroy() on the handles it receives.
typedef struct {
    bool blink;
    bool bold;
    bool dim;
    bool italic;
    bool inverted;
    bool underlined;
    bool underlined_double;
    bool strikethrough;
    bool automerge;
    ftxui_color_handle_t foreground_color;
    ftxui_color_handle_t background_color;
} ftxui_cell_t;

typedef void (*ftxui_cell_style_callback_t)(ftxui_cell_t* cell, void* userdata);

ftxui_element_handle_t ftxui_element_selection_style_reset(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_selection_color(ftxui_element_handle_t element, ftxui_color_handle_t color);
ftxui_element_handle_t ftxui_element_selection_background_color(ftxui_element_handle_t element, ftxui_color_handle_t color);
ftxui_element_handle_t ftxui_element_selection_foreground_color(ftxui_element_handle_t element, ftxui_color_handle_t color);
ftxui_element_handle_t ftxui_element_selection_style(ftxui_element_handle_t element, ftxui_cell_style_callback_t callback, void* userdata);

// =============================================================================
// §11  Elements — Flex / Size
// =============================================================================

/**
 * @brief Specifies whether to apply size constraints to width or height.
 */
typedef enum {
    FTXUI_WIDTH_OR_HEIGHT_WIDTH,  ///< Apply constraints to the width.
    FTXUI_WIDTH_OR_HEIGHT_HEIGHT, ///< Apply constraints to the height.
} ftxui_width_or_height_t;

/**
 * @brief Specifies the type of size constraint to apply.
 */
typedef enum {
    FTXUI_CONSTRAINT_LESS_THAN,      ///< The size must be less than the given value.
    FTXUI_CONSTRAINT_GREATER_THAN,   ///< The size must be greater than the given value.
    FTXUI_CONSTRAINT_EQUAL,          ///< The size must be equal to the given value.
} ftxui_constraint_t;

ftxui_element_handle_t ftxui_element_flex(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_flex_grow(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_flex_shrink(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_xflex(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_xflex_grow(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_xflex_shrink(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_yflex(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_yflex_grow(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_yflex_shrink(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_notflex(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_set_size(ftxui_element_handle_t element, ftxui_width_or_height_t width_or_height, ftxui_constraint_t constraint_type, int value);

// =============================================================================
// §12  Elements — Frame / Scroll / Focus
// =============================================================================

ftxui_element_handle_t ftxui_element_frame(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_xframe(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_yframe(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_vscroll_indicator(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_hscroll_indicator(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_block(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_block_blinking(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_bar(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_bar_blinking(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_underline(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_cursor_underline_blinking(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_focus_position(ftxui_element_handle_t element, int x, int y);
ftxui_element_handle_t ftxui_element_focus_position_relative(ftxui_element_handle_t element, float x, float y);

// =============================================================================
// §13  Elements — Alignment & Utility
// =============================================================================

ftxui_element_handle_t ftxui_element_hcenter(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_vcenter(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_center(ftxui_element_handle_t element);
ftxui_element_handle_t ftxui_element_align_right(ftxui_element_handle_t element);

// =============================================================================
// §14  Canvas  (ftxui/dom/canvas.hpp)
// =============================================================================

typedef void* ftxui_canvas_handle_t;
ftxui_canvas_handle_t ftxui_canvas_create(int width, int height);
void ftxui_canvas_destroy(ftxui_canvas_handle_t canvas);
int ftxui_canvas_width(ftxui_canvas_handle_t canvas);
int ftxui_canvas_height(ftxui_canvas_handle_t canvas);
void ftxui_canvas_draw_text(ftxui_canvas_handle_t canvas, int x, int y, const char* text);
void ftxui_canvas_draw_text_color(ftxui_canvas_handle_t canvas, int x, int y, const char* text, ftxui_color_handle_t color);
// Draws text with a stylizer callback. The callback may modify cell style/color fields.
// The foreground_color and background_color fields in ftxui_cell_t are temporary handles — do NOT free them.
void ftxui_canvas_draw_text_stylizer(ftxui_canvas_handle_t canvas, int x, int y, const char* text, ftxui_cell_style_callback_t cb, void* userdata);
// Boolean point drawing (braille characters)
void ftxui_canvas_draw_point_on(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_point_off(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_point_toggle(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_point(ftxui_canvas_handle_t canvas, int x, int y, bool value);
void ftxui_canvas_draw_point_color(ftxui_canvas_handle_t canvas, int x, int y, bool value, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_stylizer(ftxui_canvas_handle_t canvas, int x, int y, bool value, ftxui_cell_style_callback_t cb, void* userdata);
// Line drawing (color=NULL draws with default color)
void ftxui_canvas_draw_point_line(ftxui_canvas_handle_t canvas, int x1, int y1, int x2, int y2, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_line_stylizer(ftxui_canvas_handle_t canvas, int x1, int y1, int x2, int y2, ftxui_cell_style_callback_t cb, void* userdata);
// Circle drawing — point (braille)
void ftxui_canvas_draw_point_circle(ftxui_canvas_handle_t canvas, int x, int y, int radius);
void ftxui_canvas_draw_point_circle_color(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_circle_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_cell_style_callback_t cb, void* userdata);
void ftxui_canvas_draw_point_circle_filled(ftxui_canvas_handle_t canvas, int x, int y, int radius);
void ftxui_canvas_draw_point_circle_filled_color(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_circle_filled_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_cell_style_callback_t cb, void* userdata);
// Ellipse drawing — point (braille)
void ftxui_canvas_draw_point_ellipse(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry);
void ftxui_canvas_draw_point_ellipse_color(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_ellipse_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_cell_style_callback_t cb, void* userdata);
void ftxui_canvas_draw_point_ellipse_filled(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry);
void ftxui_canvas_draw_point_ellipse_filled_color(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_color_handle_t color);
void ftxui_canvas_draw_point_ellipse_filled_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_cell_style_callback_t cb, void* userdata);
// Boolean block drawing (box characters)
void ftxui_canvas_draw_block_on(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_block_off(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_block_toggle(ftxui_canvas_handle_t canvas, int x, int y);
void ftxui_canvas_draw_block(ftxui_canvas_handle_t canvas, int x, int y, bool value);
void ftxui_canvas_draw_block_color(ftxui_canvas_handle_t canvas, int x, int y, bool value, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_stylizer(ftxui_canvas_handle_t canvas, int x, int y, bool value, ftxui_cell_style_callback_t cb, void* userdata);
// Line drawing — block (color=NULL draws with default color)
void ftxui_canvas_draw_block_line(ftxui_canvas_handle_t canvas, int x1, int y1, int x2, int y2, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_line_stylizer(ftxui_canvas_handle_t canvas, int x1, int y1, int x2, int y2, ftxui_cell_style_callback_t cb, void* userdata);
// Circle drawing — block
void ftxui_canvas_draw_block_circle(ftxui_canvas_handle_t canvas, int x, int y, int radius);
void ftxui_canvas_draw_block_circle_color(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_circle_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_cell_style_callback_t cb, void* userdata);
void ftxui_canvas_draw_block_circle_filled(ftxui_canvas_handle_t canvas, int x, int y, int radius);
void ftxui_canvas_draw_block_circle_filled_color(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_circle_filled_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int radius, ftxui_cell_style_callback_t cb, void* userdata);
// Ellipse drawing — block
void ftxui_canvas_draw_block_ellipse(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry);
void ftxui_canvas_draw_block_ellipse_color(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_ellipse_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_cell_style_callback_t cb, void* userdata);
void ftxui_canvas_draw_block_ellipse_filled(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry);
void ftxui_canvas_draw_block_ellipse_filled_color(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_color_handle_t color);
void ftxui_canvas_draw_block_ellipse_filled_stylizer(ftxui_canvas_handle_t canvas, int x, int y, int rx, int ry, ftxui_cell_style_callback_t cb, void* userdata);
// Apply a style decorator at a cell position (x must be multiple of 2, y multiple of 4)
void ftxui_canvas_style(ftxui_canvas_handle_t canvas, int x, int y, ftxui_cell_style_callback_t cb, void* userdata);
// Creates an element from the canvas. Caller keeps ownership of the canvas handle.
ftxui_element_handle_t ftxui_element_canvas_ref(ftxui_canvas_handle_t canvas);

// =============================================================================
// §15  Table  (ftxui/dom/table.hpp)
// =============================================================================

typedef void* ftxui_table_handle_t;
typedef void* ftxui_table_selection_handle_t;

// A C-callable decorator: receives an owned element handle, returns a new one.
// The input handle is consumed by the call — do not free it separately.
typedef ftxui_element_handle_t (*ftxui_decorator_callback_t)(
    ftxui_element_handle_t element, void* userdata);

// cells is a flat row-major array of (rows * cols) strings
ftxui_table_handle_t ftxui_table_create(const char** cells, int rows, int cols);
void ftxui_table_destroy(ftxui_table_handle_t table);
ftxui_element_handle_t ftxui_table_render(ftxui_table_handle_t table);

// =============================================================================
// §16  Table Selection
// =============================================================================

ftxui_table_selection_handle_t ftxui_table_select_all(ftxui_table_handle_t table);
ftxui_table_selection_handle_t ftxui_table_select_row(ftxui_table_handle_t table, int row);
ftxui_table_selection_handle_t ftxui_table_select_rows(ftxui_table_handle_t table, int from, int to);
ftxui_table_selection_handle_t ftxui_table_select_column(ftxui_table_handle_t table, int col);
ftxui_table_selection_handle_t ftxui_table_select_cell(ftxui_table_handle_t table, int col, int row);
ftxui_table_selection_handle_t ftxui_table_select_columns(ftxui_table_handle_t table, int from, int to);
ftxui_table_selection_handle_t ftxui_table_select_rectangle(ftxui_table_handle_t table, int col_min, int col_max, int row_min, int row_max);
void ftxui_table_selection_destroy(ftxui_table_selection_handle_t sel);

// Border / separator styling
void ftxui_table_selection_border(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_border_color(ftxui_table_selection_handle_t sel, ftxui_border_style_t style, ftxui_color_handle_t color);
void ftxui_table_selection_border_left(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_border_right(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_border_top(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_border_bottom(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_separator(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_separator_vertical(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);
void ftxui_table_selection_separator_horizontal(ftxui_table_selection_handle_t sel, ftxui_border_style_t style);

// Generic decorator callbacks
void ftxui_table_selection_decorate(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_alternate_row(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata, int modulo, int shift);
void ftxui_table_selection_decorate_alternate_column(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata, int modulo, int shift);
void ftxui_table_selection_decorate_border(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_border_left(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_border_right(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_border_top(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_border_bottom(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_separator(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_separator_vertical(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_separator_horizontal(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_cells(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata);
void ftxui_table_selection_decorate_cells_alternate_row(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata, int modulo, int shift);
void ftxui_table_selection_decorate_cells_alternate_column(ftxui_table_selection_handle_t sel, ftxui_decorator_callback_t cb, void* userdata, int modulo, int shift);

// Convenience shortcuts
void ftxui_table_selection_decorate_bold(ftxui_table_selection_handle_t sel);
void ftxui_table_selection_decorate_cells_align_right(ftxui_table_selection_handle_t sel);
void ftxui_table_selection_decorate_cells_color(ftxui_table_selection_handle_t sel, ftxui_color_handle_t color);
void ftxui_table_selection_decorate_cells_color_alternate_row(ftxui_table_selection_handle_t sel, ftxui_color_handle_t color, int modulo, int offset);

// =============================================================================
// §17  Components — Basic
// =============================================================================
// NOTE: ftxui_easing_function_t and ftxui_easing_function_type_t are declared
// above in the forward-declared types block.

typedef struct {
    bool enabled;
    ftxui_color_handle_t inactive;
    ftxui_color_handle_t active;
    int duration_ms;
    ftxui_easing_function_type_t easing_function_type;
} ftxui_animated_color_option_t;

typedef struct {
    ftxui_animated_color_option_t background;
    ftxui_animated_color_option_t foreground;
} ftxui_animated_colors_option_t;

typedef struct {
    const char* label;
    bool state;
    bool active;
    bool focused;
    int index;
} ftxui_entry_state_t;

typedef ftxui_element_handle_t (*ftxui_button_transform_t)(ftxui_entry_state_t state, void* userdata);

typedef struct {
    ftxui_animated_colors_option_t animated_colors;
    ftxui_button_transform_t transform;
    void* transform_userdata;
} ftxui_button_option_t;

// --- Button ---
ftxui_component_handle_t ftxui_component_button(const char* label, void (*on_click)(void*), void* userdata);
ftxui_component_handle_t ftxui_component_button_with_options(const char* label, void (*on_click)(void*), void* userdata, ftxui_button_option_t options);
ftxui_button_option_t ftxui_button_option_simple();
ftxui_button_option_t ftxui_button_option_ascii();
ftxui_button_option_t ftxui_button_option_border();
ftxui_button_option_t ftxui_button_option_animated(ftxui_color_handle_t background, ftxui_color_handle_t foreground, ftxui_color_handle_t background_active, ftxui_color_handle_t foreground_active);

// --- Checkbox ---
ftxui_component_handle_t ftxui_component_checkbox(const char* label, bool* checked);
ftxui_component_handle_t ftxui_component_checkbox_with_change(const char* label, bool* checked, ftxui_callback_t on_change, void* userdata);

// --- Input ---
// String handle wraps a mutable std::string for use with Input components.
typedef void* ftxui_string_handle_t;
ftxui_string_handle_t ftxui_string_create(const char* initial);
const char* ftxui_string_get(ftxui_string_handle_t str);
void ftxui_string_set(ftxui_string_handle_t str, const char* value);
void ftxui_string_destroy(ftxui_string_handle_t str);

ftxui_component_handle_t ftxui_component_input(ftxui_string_handle_t content, const char* placeholder);
ftxui_component_handle_t ftxui_component_input_password(ftxui_string_handle_t content, const char* placeholder);

// Input with full options. NULL pointer fields use FTXUI defaults.
typedef struct {
    ftxui_string_handle_t content;
    const char*           placeholder;
    bool*                 multiline;
    bool*                 insert;
    int*                  cursor_position;
    ftxui_callback_t      on_change;
    void*                 on_change_userdata;
    ftxui_callback_t      on_enter;
    void*                 on_enter_userdata;
} ftxui_input_options_t;
ftxui_component_handle_t ftxui_component_input_with_options(ftxui_input_options_t opts);

// --- Toggle ---
ftxui_component_handle_t ftxui_component_toggle(const char** entries, int count, int* selected);

// --- Slider ---
ftxui_component_handle_t ftxui_component_slider(const char* label, int* value, int min, int max, int increment);
// Slider with on_change callback (no label). Use ftxui_component_slider for a labeled variant.
ftxui_component_handle_t ftxui_component_slider_int_with_change(int* value, int min, int max, int increment, ftxui_callback_t on_change, void* userdata);
ftxui_component_handle_t ftxui_component_slider_float_with_change(float* value, float min, float max, float increment, ftxui_callback_t on_change, void* userdata);
ftxui_component_handle_t ftxui_component_slider_int_direction(int* value, int min, int max, int increment, ftxui_direction_t direction);
ftxui_component_handle_t ftxui_component_slider_float(const char* label, float* value, float min, float max, float increment);
ftxui_component_handle_t ftxui_component_slider_float_direction(float* value, float min, float max, float increment, ftxui_direction_t direction, ftxui_color_handle_t color_active, ftxui_color_handle_t color_inactive);

// --- Radiobox ---
ftxui_component_handle_t ftxui_component_radiobox(const char** entries, int count, int* selected);
ftxui_component_handle_t ftxui_component_radiobox_with_change(const char** entries, int count, int* selected, ftxui_callback_t on_change, void* userdata);

// --- Menu ---
ftxui_component_handle_t ftxui_component_menu(const char** entries, int count, int* selected);
ftxui_component_handle_t ftxui_component_menu_with_callbacks(const char** entries, int count, int* selected, ftxui_callback_t on_change, void* on_change_userdata, ftxui_callback_t on_enter, void* on_enter_userdata);
ftxui_component_handle_t ftxui_component_menu_entry(const char* label);
ftxui_component_handle_t ftxui_component_menu_entry_animated(const char* label, ftxui_animated_colors_option_t animated_colors);
ftxui_component_handle_t ftxui_component_menu_horizontal(const char** entries, int count, int* selected);
ftxui_component_handle_t ftxui_component_menu_horizontal_animated(const char** entries, int count, int* selected);
ftxui_component_handle_t ftxui_component_menu_toggle(const char** entries, int count, int* selected);

// --- Dropdown ---
ftxui_component_handle_t ftxui_component_dropdown(const char** entries, int count, int* selected);

typedef ftxui_element_handle_t (*ftxui_dropdown_transform_callback_t)(
    bool open,
    ftxui_element_handle_t checkbox,
    ftxui_element_handle_t radiobox,
    void* userdata
);

// entry_transform may be null (uses default radiobox entry rendering).
ftxui_component_handle_t ftxui_component_dropdown_custom(
    const char** entries, int count, int* selected,
    ftxui_dropdown_transform_callback_t transform, void* transform_userdata,
    ftxui_button_transform_t entry_transform, void* entry_transform_userdata
);

// =============================================================================
// §18  Components — Containers
// =============================================================================

ftxui_component_handle_t ftxui_component_container_vertical();
ftxui_component_handle_t ftxui_component_container_vertical_focused(int* selector);
ftxui_component_handle_t ftxui_component_container_horizontal();
ftxui_component_handle_t ftxui_component_container_horizontal_focused(int* selector);
ftxui_component_handle_t ftxui_component_container_tab(int* selected);
ftxui_component_handle_t ftxui_component_container_stacked();
void ftxui_container_add(ftxui_component_handle_t container, ftxui_component_handle_t child);

// =============================================================================
// §19  Components — Advanced
// =============================================================================

// --- Renderer variants ---
ftxui_component_handle_t ftxui_component_renderer(ftxui_component_handle_t component, ftxui_render_callback_t callback, void* userdata);

typedef ftxui_element_handle_t (*ftxui_focused_render_callback_t)(bool focused, void* userdata);
ftxui_component_handle_t ftxui_component_renderer_focusable(ftxui_focused_render_callback_t callback, void* userdata);

typedef ftxui_element_handle_t (*ftxui_inner_render_callback_t)(ftxui_element_handle_t inner, void* userdata);
ftxui_component_handle_t ftxui_component_renderer_with_inner(ftxui_component_handle_t component, ftxui_inner_render_callback_t callback, void* userdata);

ftxui_element_handle_t ftxui_component_render(ftxui_component_handle_t component);

/**
 * @brief Creates a component that calls on_poll on every render frame. C-only; no FTXUI equivalent.
 */
ftxui_component_handle_t ftxui_component_poll(ftxui_app_handle_t app, void (*on_poll)(void*), void* userdata);

// --- Visibility ---
ftxui_component_handle_t ftxui_component_collapsible(const char* label, ftxui_component_handle_t child, bool* show);
ftxui_component_handle_t ftxui_component_maybe(ftxui_component_handle_t child, const bool* show);
ftxui_component_handle_t ftxui_component_maybe_fn(ftxui_component_handle_t child, ftxui_predicate_callback_t predicate, void* userdata);
ftxui_component_handle_t ftxui_component_modal(ftxui_component_handle_t main, ftxui_component_handle_t modal, const bool* show_modal);

// --- ResizableSplit ---
ftxui_component_handle_t ftxui_component_resizable_split_left(ftxui_component_handle_t main, ftxui_component_handle_t back, int* main_size);
ftxui_component_handle_t ftxui_component_resizable_split_right(ftxui_component_handle_t main, ftxui_component_handle_t back, int* main_size);
ftxui_component_handle_t ftxui_component_resizable_split_top(ftxui_component_handle_t main, ftxui_component_handle_t back, int* main_size);
ftxui_component_handle_t ftxui_component_resizable_split_bottom(ftxui_component_handle_t main, ftxui_component_handle_t back, int* main_size);

typedef ftxui_element_handle_t (*ftxui_separator_func_t)(void* userdata);

typedef struct {
    ftxui_component_handle_t main;
    ftxui_component_handle_t back;
    ftxui_direction_t direction;
    int* main_size;
    int* min_size;
    int* max_size;
    ftxui_separator_func_t separator_func;
    void* separator_userdata;
} ftxui_resizable_split_option_t;

ftxui_component_handle_t ftxui_component_resizable_split_opt(ftxui_resizable_split_option_t option);

// --- Window ---
typedef struct {
    ftxui_component_handle_t inner;  // nullable
    const char* title;               // nullable
    int* left;    // nullable (use left_default if null)
    int* top;     // nullable
    int* width;   // nullable
    int* height;  // nullable
    int left_default;
    int top_default;
    int width_default;
    int height_default;
} ftxui_window_options_t;

ftxui_component_handle_t ftxui_component_window(ftxui_window_options_t options);

// --- Hoverable ---
ftxui_component_handle_t ftxui_component_hoverable(ftxui_component_handle_t component, bool* hover);
ftxui_component_handle_t ftxui_component_hoverable_callbacks(ftxui_component_handle_t component, ftxui_callback_t on_enter, void* on_enter_userdata, ftxui_callback_t on_leave, void* on_leave_userdata);
ftxui_component_handle_t ftxui_component_hoverable_change(ftxui_component_handle_t component, void (*on_change)(bool hovered, void* userdata), void* userdata);

// --- CatchEvent ---
typedef bool (*ftxui_catch_event_callback_t)(ftxui_event_handle_t event, void* userdata);
ftxui_component_handle_t ftxui_component_catch_event(ftxui_component_handle_t component, ftxui_catch_event_callback_t callback, void* userdata);

void ftxui_component_destroy(ftxui_component_handle_t component);

// =============================================================================
// §20  Component Decorators
// =============================================================================

ftxui_component_handle_t ftxui_component_border(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_light(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_dashed(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_heavy(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_double(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_rounded(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_border_empty(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_bold(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_dim(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_inverted(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_underlined(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_blink(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_strikethrough(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_nothing(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_color(ftxui_component_handle_t component, ftxui_color_handle_t color);
ftxui_component_handle_t ftxui_component_bgcolor(ftxui_component_handle_t component, ftxui_color_handle_t color);
ftxui_component_handle_t ftxui_component_vscroll_indicator(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_frame(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_flex(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_hcenter(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_vcenter(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_center(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_align_right(ftxui_component_handle_t component);
ftxui_component_handle_t ftxui_component_set_size(ftxui_component_handle_t component, ftxui_width_or_height_t width_or_height, ftxui_constraint_t constraint_type, int value);

// =============================================================================
// §21  Events  (ftxui/component/event.hpp)
// =============================================================================

typedef enum {
    FTXUI_MOUSE_BUTTON_LEFT        = 0,
    FTXUI_MOUSE_BUTTON_MIDDLE      = 1,
    FTXUI_MOUSE_BUTTON_RIGHT       = 2,
    FTXUI_MOUSE_BUTTON_NONE        = 3,
    FTXUI_MOUSE_BUTTON_WHEEL_UP    = 4,
    FTXUI_MOUSE_BUTTON_WHEEL_DOWN  = 5,
    FTXUI_MOUSE_BUTTON_WHEEL_LEFT  = 6,
    FTXUI_MOUSE_BUTTON_WHEEL_RIGHT = 7,
} ftxui_mouse_button_t;

typedef enum {
    FTXUI_MOUSE_MOTION_RELEASED = 0,
    FTXUI_MOUSE_MOTION_PRESSED  = 1,
    FTXUI_MOUSE_MOTION_MOVED    = 2,
} ftxui_mouse_motion_t;

// Basic event accessors
const char* ftxui_event_input(ftxui_event_handle_t event);
const char* ftxui_event_debug_string(ftxui_event_handle_t event);
bool        ftxui_event_is_character(ftxui_event_handle_t event);
const char* ftxui_event_character(ftxui_event_handle_t event);

// Mouse event accessors (all return 0/false if !is_mouse())
bool                 ftxui_event_is_mouse(ftxui_event_handle_t event);
int                  ftxui_event_mouse_x(ftxui_event_handle_t event);
int                  ftxui_event_mouse_y(ftxui_event_handle_t event);
ftxui_mouse_button_t ftxui_event_mouse_button(ftxui_event_handle_t event);
ftxui_mouse_motion_t ftxui_event_mouse_motion(ftxui_event_handle_t event);
bool                 ftxui_event_mouse_shift(ftxui_event_handle_t event);
bool                 ftxui_event_mouse_meta(ftxui_event_handle_t event);
bool                 ftxui_event_mouse_control(ftxui_event_handle_t event);

// Cursor-position and cursor-shape events (return 0 if guard is false)
bool ftxui_event_is_cursor_position(ftxui_event_handle_t event);
int  ftxui_event_cursor_x(ftxui_event_handle_t event);
int  ftxui_event_cursor_y(ftxui_event_handle_t event);
bool ftxui_event_is_cursor_shape(ftxui_event_handle_t event);
int  ftxui_event_cursor_shape(ftxui_event_handle_t event);

// Terminal-info events
bool        ftxui_event_is_terminal_name_version(ftxui_event_handle_t event);
const char* ftxui_event_terminal_name(ftxui_event_handle_t event);
int         ftxui_event_terminal_version(ftxui_event_handle_t event);
bool        ftxui_event_is_terminal_emulator(ftxui_event_handle_t event);
const char* ftxui_event_terminal_emulator_name(ftxui_event_handle_t event);
const char* ftxui_event_terminal_emulator_version(ftxui_event_handle_t event);
bool        ftxui_event_is_terminal_capabilities(ftxui_event_handle_t event);
// Returns malloc'd int[count]; caller must free(). NULL if not a capabilities event.
int*        ftxui_event_terminal_capabilities(ftxui_event_handle_t event, int* count);

// Lifecycle for factory-created (caller-owned) event handles
void ftxui_event_destroy(ftxui_event_handle_t event);
bool ftxui_event_equal(ftxui_event_handle_t a, ftxui_event_handle_t b);

// Named-constant factory functions — caller must ftxui_event_destroy the result
ftxui_event_handle_t ftxui_event_arrow_left(void);
ftxui_event_handle_t ftxui_event_arrow_right(void);
ftxui_event_handle_t ftxui_event_arrow_up(void);
ftxui_event_handle_t ftxui_event_arrow_down(void);
ftxui_event_handle_t ftxui_event_arrow_left_ctrl(void);
ftxui_event_handle_t ftxui_event_arrow_right_ctrl(void);
ftxui_event_handle_t ftxui_event_arrow_up_ctrl(void);
ftxui_event_handle_t ftxui_event_arrow_down_ctrl(void);
ftxui_event_handle_t ftxui_event_backspace(void);
ftxui_event_handle_t ftxui_event_delete(void);
ftxui_event_handle_t ftxui_event_return(void);
ftxui_event_handle_t ftxui_event_escape(void);
ftxui_event_handle_t ftxui_event_tab(void);
ftxui_event_handle_t ftxui_event_tab_reverse(void);
ftxui_event_handle_t ftxui_event_insert(void);
ftxui_event_handle_t ftxui_event_home(void);
ftxui_event_handle_t ftxui_event_end(void);
ftxui_event_handle_t ftxui_event_page_up(void);
ftxui_event_handle_t ftxui_event_page_down(void);
ftxui_event_handle_t ftxui_event_f1(void);
ftxui_event_handle_t ftxui_event_f2(void);
ftxui_event_handle_t ftxui_event_f3(void);
ftxui_event_handle_t ftxui_event_f4(void);
ftxui_event_handle_t ftxui_event_f5(void);
ftxui_event_handle_t ftxui_event_f6(void);
ftxui_event_handle_t ftxui_event_f7(void);
ftxui_event_handle_t ftxui_event_f8(void);
ftxui_event_handle_t ftxui_event_f9(void);
ftxui_event_handle_t ftxui_event_f10(void);
ftxui_event_handle_t ftxui_event_f11(void);
ftxui_event_handle_t ftxui_event_f12(void);
ftxui_event_handle_t ftxui_event_custom(void);

// General event construction — caller must ftxui_event_destroy the result
ftxui_event_handle_t ftxui_event_character_from_char(char c);
ftxui_event_handle_t ftxui_event_special(const char* input);
// c must be a-z or A-Z; returns NULL otherwise
ftxui_event_handle_t ftxui_event_ctrl_char(char c);
ftxui_event_handle_t ftxui_event_alt_char(char c);

// =============================================================================
// §22  Animation  (ftxui/component/animation.hpp)
// =============================================================================
// NOTE: ftxui_easing_function_t and ftxui_easing_function_type_t are declared
// above in the forward-declared types block due to use in §17.

/**
 * @brief Returns the easing function for a given type.
 */
ftxui_easing_function_t ftxui_easing_function_get(ftxui_easing_function_type_t type);

#ifdef __cplusplus
}
#endif

#endif // FTXUI_C_API_H
