#include "ftxui_c_api.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ftxui_element_handle_t render_empty(void* userdata) {
    (void)userdata;
    return ftxui_element_empty();
}

static void set_flag(void* p) { *(bool*)p = true; }
static void noop(void* p)     { (void)p; }

// ---------------------------------------------------------------------------
// 1. App creation — all six creators return non-NULL
// ---------------------------------------------------------------------------
static void test_app_creation(void) {
    printf("test_app_creation...");

    ftxui_app_handle_t app;

    app = ftxui_app_create_fullscreen();
    assert(app != NULL);
    ftxui_app_destroy(app);

    app = ftxui_app_create_fit_component();
    assert(app != NULL);
    ftxui_app_destroy(app);

    app = ftxui_app_create_terminal_output();
    assert(app != NULL);
    ftxui_app_destroy(app);

    app = ftxui_app_create_fixed_size(80, 24);
    assert(app != NULL);
    ftxui_app_destroy(app);

    app = ftxui_app_create_fullscreen_primary_screen();
    assert(app != NULL);
    ftxui_app_destroy(app);

    app = ftxui_app_create_fullscreen_alternate_screen();
    assert(app != NULL);
    ftxui_app_destroy(app);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 2. ftxui_app_active() returns NULL when no loop is running
// ---------------------------------------------------------------------------
static void test_app_active_no_loop(void) {
    printf("test_app_active_no_loop...");
    assert(ftxui_app_active() == NULL);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 3. Configuration flags — smoke (must not crash, called before loop)
// ---------------------------------------------------------------------------
static void test_app_configuration(void) {
    printf("test_app_configuration...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    ftxui_app_track_mouse(app, true);
    ftxui_app_track_mouse(app, false);
    ftxui_app_handle_piped_input(app, true);
    ftxui_app_handle_piped_input(app, false);
    ftxui_app_force_handle_ctrl_c(app, true);
    ftxui_app_force_handle_ctrl_c(app, false);
    ftxui_app_force_handle_ctrl_z(app, true);
    ftxui_app_force_handle_ctrl_z(app, false);

    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 4. Terminal size helpers (global, no app needed)
// ---------------------------------------------------------------------------
static void test_terminal_size(void) {
    printf("test_terminal_size...");

    int w = ftxui_terminal_width();
    int h = ftxui_terminal_height();
    assert(w >= 0);
    assert(h >= 0);

    printf(" %dx%d OK\n", w, h);
}

// ---------------------------------------------------------------------------
// 5. Terminal color support — round-trip
// ---------------------------------------------------------------------------
static void test_terminal_color_support(void) {
    printf("test_terminal_color_support...");

    ftxui_terminal_color_t original = ftxui_terminal_color_support();
    assert(original >= FTXUI_TERMINAL_COLOR_PALETTE1);
    assert(original <= FTXUI_TERMINAL_COLOR_TRUE_COLOR);

    ftxui_terminal_set_color_support(FTXUI_TERMINAL_COLOR_PALETTE1);
    assert(ftxui_terminal_color_support() == FTXUI_TERMINAL_COLOR_PALETTE1);

    ftxui_terminal_set_color_support(FTXUI_TERMINAL_COLOR_TRUE_COLOR);
    assert(ftxui_terminal_color_support() == FTXUI_TERMINAL_COLOR_TRUE_COLOR);

    ftxui_terminal_set_color_support(original);
    assert(ftxui_terminal_color_support() == original);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 6. Terminal quirks — round-trip across all four fields
// ---------------------------------------------------------------------------
static void test_terminal_quirks(void) {
    printf("test_terminal_quirks...");

    ftxui_quirks_t original = ftxui_terminal_get_quirks();

    ftxui_quirks_t modified;
    modified.block_characters = true;
    modified.cursor_hiding    = false;
    modified.component_ascii  = true;
    modified.color_support    = FTXUI_TERMINAL_COLOR_PALETTE256;

    ftxui_terminal_set_quirks(modified);

    ftxui_quirks_t readback = ftxui_terminal_get_quirks();
    assert(readback.block_characters == true);
    assert(readback.cursor_hiding    == false);
    assert(readback.component_ascii  == true);
    assert(readback.color_support    == FTXUI_TERMINAL_COLOR_PALETTE256);

    ftxui_terminal_set_quirks(original);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 7. Terminal fallback size — smoke test (no observable getter)
// ---------------------------------------------------------------------------
static void test_terminal_fallback_size(void) {
    printf("test_terminal_fallback_size...");

    ftxui_terminal_set_fallback_size(80, 24);
    ftxui_terminal_set_fallback_size(132, 50);
    ftxui_terminal_set_fallback_size(0, 0);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 8. Terminal info — values may be empty before loop, but must not be NULL
// ---------------------------------------------------------------------------
static void test_terminal_info(void) {
    printf("test_terminal_info...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    const char* name = ftxui_app_terminal_name(app);
    assert(name != NULL);

    int version = ftxui_app_terminal_version(app);
    (void)version;

    const char* emu_name = ftxui_app_terminal_emulator_name(app);
    assert(emu_name != NULL);

    const char* emu_ver = ftxui_app_terminal_emulator_version(app);
    assert(emu_ver != NULL);

    int count = -1;
    int* caps = ftxui_app_terminal_capabilities(app, &count);
    assert(count >= 0);
    if (caps != NULL) free(caps);

    ftxui_app_destroy(app);
    printf(" name='%s' version=%d emu='%s' caps=%d OK\n",
           name, version, emu_name, count);
}

// ---------------------------------------------------------------------------
// 6. Selection API — before loop: empty result, must not crash
// ---------------------------------------------------------------------------
static void test_selection_api(void) {
    printf("test_selection_api...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    char* sel = ftxui_app_get_selection(app);
    assert(sel != NULL);
    free(sel);

    ftxui_app_selection_change(app, noop, NULL);

    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 7. Mouse capture — before loop: may return NULL; must not crash either way
// ---------------------------------------------------------------------------
static void test_mouse_capture(void) {
    printf("test_mouse_capture...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    ftxui_captured_mouse_handle_t cap = ftxui_app_capture_mouse(app);
    if (cap != NULL) ftxui_captured_mouse_destroy(cap);

    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 8. ftxui_app_post — verified via a single non-blocking loop iteration
// ---------------------------------------------------------------------------
static void test_app_post(void) {
    printf("test_app_post...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    ftxui_component_handle_t comp = ftxui_component_container_vertical();
    assert(comp != NULL);

    ftxui_loop_handle_t loop = ftxui_loop_create(app, comp);
    assert(loop != NULL);

    bool called = false;
    ftxui_app_post(app, set_flag, &called);
    ftxui_loop_run_once(loop);
    assert(called);

    ftxui_loop_destroy(loop);
    ftxui_component_destroy(comp);
    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 9. Loop lifecycle
// ---------------------------------------------------------------------------
static void test_loop_lifecycle(void) {
    printf("test_loop_lifecycle...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    ftxui_component_handle_t comp = ftxui_component_container_vertical();
    assert(comp != NULL);

    ftxui_loop_handle_t loop = ftxui_loop_create(app, comp);
    assert(loop != NULL);

    assert(!ftxui_loop_has_quitted(loop));
    ftxui_loop_run_once(loop);

    ftxui_loop_destroy(loop);
    ftxui_component_destroy(comp);
    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 10. NULL-safety for all new functions
// ---------------------------------------------------------------------------
static void test_null_safety(void) {
    printf("test_null_safety...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    // NULL app handle — must not crash
    ftxui_app_track_mouse(NULL, true);
    ftxui_app_handle_piped_input(NULL, true);
    ftxui_app_force_handle_ctrl_c(NULL, true);
    ftxui_app_force_handle_ctrl_z(NULL, true);
    ftxui_app_post(NULL, noop, NULL);

    // NULL callback with valid app — must not crash
    ftxui_app_post(app, NULL, NULL);

    // Terminal info with NULL app — must return safe defaults
    assert(strcmp(ftxui_app_terminal_name(NULL), "") == 0);
    assert(ftxui_app_terminal_version(NULL) == 0);
    assert(strcmp(ftxui_app_terminal_emulator_name(NULL), "") == 0);
    assert(strcmp(ftxui_app_terminal_emulator_version(NULL), "") == 0);

    int count = -1;
    assert(ftxui_app_terminal_capabilities(NULL, &count) == NULL);

    // Mouse capture with NULL app — must return NULL without crashing
    assert(ftxui_app_capture_mouse(NULL) == NULL);

    // Destroy NULL handle — must not crash
    ftxui_captured_mouse_destroy(NULL);

    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------

int main(void) {
    printf("=== test_app_api ===\n");
    test_app_creation();
    test_app_active_no_loop();
    test_app_configuration();
    test_terminal_size();
    test_terminal_color_support();
    test_terminal_quirks();
    test_terminal_fallback_size();
    test_terminal_info();
    test_selection_api();
    test_mouse_capture();
    test_app_post();
    test_loop_lifecycle();
    test_null_safety();
    printf("=== all tests passed ===\n");
    return 0;
}
