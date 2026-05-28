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
// 8. Color info — GetColorInfo and sorted_2d with full struct fields
// ---------------------------------------------------------------------------
static void test_color_info(void) {
    printf("test_color_info...");

    // Palette256 lookup: RED1 (index 196) is pure red — verify name and RGB
    ftxui_color_info_t r256 = ftxui_color_info_get_256(FTXUI_PALETTE256_RED1);
    assert(r256.name != NULL);
    assert(r256.name[0] != '\0');
    assert(r256.index_256 == 196);
    assert(r256.red > 0);

    // Palette16 lookup: RED (index 1) — verify name is populated
    ftxui_color_info_t r16 = ftxui_color_info_get_16(FTXUI_PALETTE16_RED);
    assert(r16.name != NULL);
    assert(r16.name[0] != '\0');
    assert(r16.index_16 == 1);

    // sorted_2d: verify at least one non-padding entry has full fields
    int rows = 0, cols = 0;
    ftxui_color_info_t* grid = ftxui_color_info_sorted_2d(&rows, &cols);
    assert(grid != NULL);
    assert(rows > 0 && cols > 0);
    bool found_real = false;
    bool found_nonblack = false;
    for (int i = 0; i < rows * cols; i++) {
        if (grid[i].index_256 != -1) {
            assert(grid[i].name != NULL);
            assert(grid[i].name[0] != '\0');
            found_real = true;
            if (grid[i].red != 0 || grid[i].green != 0 || grid[i].blue != 0) {
                found_nonblack = true;
            }
        }
    }
    assert(found_real);
    assert(found_nonblack);
    ftxui_color_info_free(grid);

    printf(" rows=%d cols=%d name256='%s' name16='%s' OK\n",
           rows, cols, r256.name, r16.name);
}

// ---------------------------------------------------------------------------
// 9. Table API — selection, border/separator styling, and decorator callbacks
// ---------------------------------------------------------------------------

static ftxui_element_handle_t bold_decorator(ftxui_element_handle_t el, void* userdata) {
    (void)userdata;
    return ftxui_element_bold(el);
}

static void test_table_api(void) {
    printf("test_table_api...");

    const char* cells[] = {"A", "B", "C", "D"};

    // --- SelectColumns and SelectRectangle (smoke) ---
    {
        ftxui_table_handle_t t = ftxui_table_create(cells, 2, 2);
        assert(t != NULL);

        ftxui_table_selection_handle_t sc = ftxui_table_select_columns(t, 0, 1);
        assert(sc != NULL);
        ftxui_table_selection_destroy(sc);

        ftxui_table_selection_handle_t sr = ftxui_table_select_rectangle(t, 0, 1, 0, 1);
        assert(sr != NULL);
        ftxui_table_selection_destroy(sr);

        ftxui_table_destroy(t);
    }

    // --- BorderLeft/Right/Top/Bottom, Separator, SeparatorHorizontal ---
    {
        ftxui_table_handle_t t = ftxui_table_create(cells, 2, 2);
        ftxui_table_selection_handle_t sel = ftxui_table_select_all(t);
        ftxui_table_selection_border_left(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_border_right(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_border_top(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_border_bottom(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_separator(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_separator_horizontal(sel, FTXUI_BORDER_STYLE_LIGHT);
        ftxui_table_selection_destroy(sel);

        ftxui_element_handle_t el = ftxui_table_render(t);
        assert(el != NULL);
        ftxui_element_destroy(el);
        ftxui_table_destroy(t);
    }

    // --- Decorate (general callback) ---
    {
        ftxui_table_handle_t t = ftxui_table_create(cells, 2, 2);
        ftxui_table_selection_handle_t sel = ftxui_table_select_all(t);
        ftxui_table_selection_decorate(sel, bold_decorator, NULL);
        ftxui_table_selection_destroy(sel);

        ftxui_element_handle_t el = ftxui_table_render(t);
        assert(el != NULL);
        ftxui_element_destroy(el);
        ftxui_table_destroy(t);
    }

    // --- DecorateCells (general callback) ---
    {
        ftxui_table_handle_t t = ftxui_table_create(cells, 2, 2);
        ftxui_table_selection_handle_t sel = ftxui_table_select_all(t);
        ftxui_table_selection_decorate_cells(sel, bold_decorator, NULL);
        ftxui_table_selection_destroy(sel);

        ftxui_element_handle_t el = ftxui_table_render(t);
        assert(el != NULL);
        ftxui_element_destroy(el);
        ftxui_table_destroy(t);
    }

    // --- DecorateCellsAlternateRow (general callback) ---
    {
        ftxui_table_handle_t t = ftxui_table_create(cells, 2, 2);
        ftxui_table_selection_handle_t sel = ftxui_table_select_all(t);
        ftxui_table_selection_decorate_cells_alternate_row(sel, bold_decorator, NULL, 2, 0);
        ftxui_table_selection_destroy(sel);

        ftxui_element_handle_t el = ftxui_table_render(t);
        assert(el != NULL);
        ftxui_element_destroy(el);
        ftxui_table_destroy(t);
    }

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 10. Event API — factory functions, equality, construction, mouse guards
// ---------------------------------------------------------------------------
static void test_event_api(void) {
    printf("test_event_api...");

    // Named-constant factories + equality
    ftxui_event_handle_t left1 = ftxui_event_arrow_left();
    ftxui_event_handle_t left2 = ftxui_event_arrow_left();
    ftxui_event_handle_t right = ftxui_event_arrow_right();
    assert(left1 != NULL && left2 != NULL && right != NULL);
    assert(ftxui_event_equal(left1, left2));
    assert(!ftxui_event_equal(left1, right));
    ftxui_event_destroy(left1);
    ftxui_event_destroy(left2);
    ftxui_event_destroy(right);

    // Spot-check a selection of named factories
    ftxui_event_handle_t evs[] = {
        ftxui_event_arrow_up(), ftxui_event_arrow_down(),
        ftxui_event_arrow_left_ctrl(), ftxui_event_arrow_right_ctrl(),
        ftxui_event_backspace(), ftxui_event_delete(), ftxui_event_return(),
        ftxui_event_escape(), ftxui_event_tab(), ftxui_event_tab_reverse(),
        ftxui_event_insert(), ftxui_event_home(), ftxui_event_end(),
        ftxui_event_page_up(), ftxui_event_page_down(),
        ftxui_event_f1(), ftxui_event_f12(), ftxui_event_custom(),
    };
    for (int i = 0; i < (int)(sizeof(evs)/sizeof(evs[0])); i++) {
        assert(evs[i] != NULL);
        ftxui_event_destroy(evs[i]);
    }

    // character_from_char
    ftxui_event_handle_t ch = ftxui_event_character_from_char('a');
    assert(ch != NULL);
    assert(ftxui_event_is_character(ch));
    assert(ftxui_event_input(ch)[0] == 'a');
    ftxui_event_destroy(ch);

    // ctrl_char
    ftxui_event_handle_t ctrl_c1 = ftxui_event_ctrl_char('c');
    ftxui_event_handle_t ctrl_c2 = ftxui_event_ctrl_char('C');
    assert(ctrl_c1 != NULL && ctrl_c2 != NULL);
    assert(ftxui_event_equal(ctrl_c1, ctrl_c2));
    assert(ftxui_event_ctrl_char('1') == NULL); // non-letter returns NULL
    ftxui_event_destroy(ctrl_c1);
    ftxui_event_destroy(ctrl_c2);

    // alt_char
    ftxui_event_handle_t alt_a = ftxui_event_alt_char('a');
    assert(alt_a != NULL);
    assert(ftxui_event_alt_char('!') == NULL);
    ftxui_event_destroy(alt_a);

    // special
    ftxui_event_handle_t sp = ftxui_event_special("hello");
    assert(sp != NULL);
    assert(strcmp(ftxui_event_input(sp), "hello") == 0);
    ftxui_event_destroy(sp);

    // Mouse guard — no live mouse event, verify accessors don't crash
    ftxui_event_handle_t esc = ftxui_event_escape();
    assert(!ftxui_event_is_mouse(esc));
    assert(ftxui_event_mouse_button(esc) == FTXUI_MOUSE_BUTTON_NONE);
    assert(ftxui_event_mouse_motion(esc) == FTXUI_MOUSE_MOTION_RELEASED);
    assert(!ftxui_event_mouse_shift(esc));
    assert(!ftxui_event_mouse_meta(esc));
    assert(!ftxui_event_mouse_control(esc));
    assert(!ftxui_event_is_cursor_position(esc));
    assert(ftxui_event_cursor_x(esc) == 0);
    assert(ftxui_event_cursor_y(esc) == 0);
    assert(!ftxui_event_is_cursor_shape(esc));
    assert(ftxui_event_cursor_shape(esc) == 0);
    assert(!ftxui_event_is_terminal_name_version(esc));
    assert(strcmp(ftxui_event_terminal_name(esc), "") == 0);
    assert(ftxui_event_terminal_version(esc) == 0);
    assert(!ftxui_event_is_terminal_emulator(esc));
    assert(strcmp(ftxui_event_terminal_emulator_name(esc), "") == 0);
    assert(!ftxui_event_is_terminal_capabilities(esc));
    int cap_count = -1;
    assert(ftxui_event_terminal_capabilities(esc, &cap_count) == NULL);
    assert(cap_count == 0);
    ftxui_event_destroy(esc);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// 11. Terminal info — values may be empty before loop, but must not be NULL
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
// Canvas new API
// ---------------------------------------------------------------------------
static int stylizer_call_count;

static void counting_stylizer(ftxui_cell_t* cell, void* userdata) {
    (void)cell;
    (void)userdata;
    stylizer_call_count++;
}

static void bold_stylizer(ftxui_cell_t* cell, void* userdata) {
    (void)userdata;
    cell->bold = true;
}

static void test_canvas_new_api(void) {
    printf("test_canvas_new_api...");

    ftxui_canvas_handle_t cv = ftxui_canvas_create(80, 40);
    assert(cv != NULL);
    assert(ftxui_canvas_width(cv) == 80);
    assert(ftxui_canvas_height(cv) == 40);

    // Boolean point ops — no crash
    ftxui_canvas_draw_point_on(cv, 2, 4);
    ftxui_canvas_draw_point_off(cv, 2, 4);
    ftxui_canvas_draw_point_toggle(cv, 2, 4);
    ftxui_canvas_draw_point(cv, 4, 8, true);
    ftxui_canvas_draw_point(cv, 4, 8, false);

    // Point with color
    ftxui_color_handle_t red = ftxui_color_rgb(255, 0, 0);
    ftxui_canvas_draw_point_color(cv, 4, 8, true, red);

    // Point with stylizer — verify callback fires
    stylizer_call_count = 0;
    ftxui_canvas_draw_point_stylizer(cv, 4, 8, true, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // Boolean block ops
    ftxui_canvas_draw_block_on(cv, 2, 2);
    ftxui_canvas_draw_block_off(cv, 2, 2);
    ftxui_canvas_draw_block_toggle(cv, 2, 2);
    ftxui_canvas_draw_block(cv, 4, 4, true);
    ftxui_canvas_draw_block_color(cv, 4, 4, true, red);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_stylizer(cv, 4, 4, true, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // Line stylizer
    stylizer_call_count = 0;
    ftxui_canvas_draw_point_line_stylizer(cv, 0, 0, 10, 10, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_line_stylizer(cv, 0, 0, 10, 10, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // Circle color and stylizer
    ftxui_canvas_draw_point_circle_color(cv, 20, 20, 8, red);
    ftxui_canvas_draw_point_circle_filled_color(cv, 20, 20, 5, red);
    ftxui_canvas_draw_block_circle_color(cv, 20, 20, 8, red);
    ftxui_canvas_draw_block_circle_filled_color(cv, 20, 20, 5, red);

    stylizer_call_count = 0;
    ftxui_canvas_draw_point_circle_stylizer(cv, 20, 20, 8, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_point_circle_filled_stylizer(cv, 20, 20, 5, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_circle_stylizer(cv, 20, 20, 8, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_circle_filled_stylizer(cv, 20, 20, 5, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // Ellipse color and stylizer
    ftxui_canvas_draw_point_ellipse_color(cv, 20, 20, 8, 4, red);
    ftxui_canvas_draw_point_ellipse_filled_color(cv, 20, 20, 6, 3, red);
    ftxui_canvas_draw_block_ellipse_color(cv, 20, 20, 8, 4, red);
    ftxui_canvas_draw_block_ellipse_filled_color(cv, 20, 20, 6, 3, red);

    stylizer_call_count = 0;
    ftxui_canvas_draw_point_ellipse_stylizer(cv, 20, 20, 8, 4, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_point_ellipse_filled_stylizer(cv, 20, 20, 6, 3, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_ellipse_stylizer(cv, 20, 20, 8, 4, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    stylizer_call_count = 0;
    ftxui_canvas_draw_block_ellipse_filled_stylizer(cv, 20, 20, 6, 3, counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // DrawText stylizer
    stylizer_call_count = 0;
    ftxui_canvas_draw_text_stylizer(cv, 0, 0, "Hello", counting_stylizer, NULL);
    assert(stylizer_call_count > 0);

    // Canvas Style at specific position (sets bold on text drawn at 0,0)
    ftxui_canvas_style(cv, 0, 0, bold_stylizer, NULL);

    // Stylizer modifies the cell color
    ftxui_color_handle_t blue = ftxui_color_rgb(0, 0, 255);
    static ftxui_color_handle_t captured_fg = NULL;
    // We can't easily capture via static in C without a global, so just verify no crash
    ftxui_canvas_draw_text_stylizer(cv, 0, 0, "X", bold_stylizer, NULL);

    ftxui_color_destroy(blue);
    ftxui_color_destroy(red);
    ftxui_canvas_destroy(cv);

    // NULL safety
    assert(ftxui_canvas_width(NULL) == 0);
    assert(ftxui_canvas_height(NULL) == 0);
    ftxui_canvas_draw_point_on(NULL, 0, 0);
    ftxui_canvas_draw_point_off(NULL, 0, 0);
    ftxui_canvas_draw_point_toggle(NULL, 0, 0);
    ftxui_canvas_draw_point(NULL, 0, 0, true);
    ftxui_canvas_draw_block_on(NULL, 0, 0);
    ftxui_canvas_draw_block_off(NULL, 0, 0);
    ftxui_canvas_draw_block_toggle(NULL, 0, 0);
    ftxui_canvas_draw_block(NULL, 0, 0, true);
    ftxui_canvas_style(NULL, 0, 0, bold_stylizer, NULL);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// Component callbacks
// ---------------------------------------------------------------------------
static void test_component_callbacks(void) {
    printf("test_component_callbacks...");

    // Checkbox with on_change
    bool checked = false;
    bool cb_called = false;
    ftxui_component_handle_t chk = ftxui_component_checkbox_with_change("chk", &checked, set_flag, &cb_called);
    assert(chk != NULL);
    ftxui_component_destroy(chk);

    // Input with options
    ftxui_string_handle_t content = ftxui_string_create("hello");
    bool multiline = true;
    bool insert = true;
    int cursor = 0;
    bool on_change_called = false;
    bool on_enter_called = false;
    ftxui_input_options_t opts;
    opts.content = content;
    opts.placeholder = "type here";
    opts.multiline = &multiline;
    opts.insert = &insert;
    opts.cursor_position = &cursor;
    opts.on_change = set_flag;
    opts.on_change_userdata = &on_change_called;
    opts.on_enter = set_flag;
    opts.on_enter_userdata = &on_enter_called;
    ftxui_component_handle_t inp = ftxui_component_input_with_options(opts);
    assert(inp != NULL);
    ftxui_component_destroy(inp);
    ftxui_string_destroy(content);

    // Radiobox with on_change
    const char* entries[] = {"a", "b", "c"};
    int selected = 0;
    bool radio_cb = false;
    ftxui_component_handle_t rb = ftxui_component_radiobox_with_change(entries, 3, &selected, set_flag, &radio_cb);
    assert(rb != NULL);
    ftxui_component_destroy(rb);

    // Menu with callbacks
    bool menu_change_called = false;
    bool menu_enter_called = false;
    ftxui_component_handle_t mn = ftxui_component_menu_with_callbacks(entries, 3, &selected, set_flag, &menu_change_called, set_flag, &menu_enter_called);
    assert(mn != NULL);
    ftxui_component_destroy(mn);

    // Slider int with on_change
    int ival = 50;
    bool slider_cb = false;
    ftxui_component_handle_t sl = ftxui_component_slider_int_with_change(&ival, 0, 100, 5, set_flag, &slider_cb);
    assert(sl != NULL);
    ftxui_component_destroy(sl);

    // Slider float with on_change
    float fval = 0.5f;
    ftxui_component_handle_t slfl = ftxui_component_slider_float_with_change(&fval, 0.0f, 1.0f, 0.05f, set_flag, &slider_cb);
    assert(slfl != NULL);
    ftxui_component_destroy(slfl);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// Container with selector
// ---------------------------------------------------------------------------
static void test_container_with_selector(void) {
    printf("test_container_with_selector...");

    int sel_v = 0;
    ftxui_component_handle_t v = ftxui_component_container_vertical_focused(&sel_v);
    assert(v != NULL);

    int sel_h = 0;
    ftxui_component_handle_t h = ftxui_component_container_horizontal_focused(&sel_h);
    assert(h != NULL);

    // Add a child to verify it works
    ftxui_component_handle_t child = ftxui_component_button("X", noop, NULL);
    assert(child != NULL);
    ftxui_container_add(v, child);
    ftxui_container_add(h, child);

    ftxui_component_destroy(h);
    ftxui_component_destroy(v);
    // child owned by containers; don't destroy separately

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// Loop RunOnceBlocking
// ---------------------------------------------------------------------------
static void test_loop_run_once_blocking(void) {
    printf("test_loop_run_once_blocking...");

    ftxui_app_handle_t app = ftxui_app_create_terminal_output();
    assert(app != NULL);

    ftxui_component_handle_t comp = ftxui_component_container_vertical();
    assert(comp != NULL);

    ftxui_loop_handle_t loop = ftxui_loop_create(app, comp);
    assert(loop != NULL);

    // Post a callback so the blocking call has an event to process and returns.
    bool called = false;
    ftxui_app_post(app, set_flag, &called);
    ftxui_loop_run_once_blocking(loop);
    assert(called);

    ftxui_loop_destroy(loop);
    ftxui_component_destroy(comp);
    ftxui_app_destroy(app);
    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// Maybe with function predicate
// ---------------------------------------------------------------------------
static bool always_true(void* ud)  { (void)ud; return true;  }
static bool always_false(void* ud) { (void)ud; return false; }

static void test_maybe_fn(void) {
    printf("test_maybe_fn...");

    ftxui_component_handle_t child = ftxui_component_button("test", noop, NULL);
    assert(child != NULL);

    ftxui_component_handle_t m1 = ftxui_component_maybe_fn(child, always_true, NULL);
    assert(m1 != NULL);
    ftxui_component_destroy(m1);

    // Re-create child since it was moved into m1
    child = ftxui_component_button("test", noop, NULL);
    ftxui_component_handle_t m2 = ftxui_component_maybe_fn(child, always_false, NULL);
    assert(m2 != NULL);
    ftxui_component_destroy(m2);

    // NULL safety
    assert(ftxui_component_maybe_fn(NULL, always_true, NULL) == NULL);

    printf(" OK\n");
}

// ---------------------------------------------------------------------------
// Hoverable callbacks
// ---------------------------------------------------------------------------
static void on_hover_change(bool hovered, void* userdata) {
    *(bool*)userdata = hovered;
}

static void test_hoverable_callbacks(void) {
    printf("test_hoverable_callbacks...");

    ftxui_component_handle_t base;
    bool enter_called = false;
    bool leave_called = false;

    base = ftxui_component_button("btn", noop, NULL);
    assert(base != NULL);
    ftxui_component_handle_t hov1 = ftxui_component_hoverable_callbacks(base, set_flag, &enter_called, set_flag, &leave_called);
    assert(hov1 != NULL);
    ftxui_component_destroy(hov1);

    // ftxui_component_hoverable_change with a proper callback
    base = ftxui_component_button("btn2", noop, NULL);
    assert(base != NULL);
    bool hover_state = false;
    ftxui_component_handle_t hov2 = ftxui_component_hoverable_change(base, on_hover_change, &hover_state);
    assert(hov2 != NULL);
    ftxui_component_destroy(hov2);

    // NULL safety
    assert(ftxui_component_hoverable_callbacks(NULL, NULL, NULL, NULL, NULL) == NULL);
    assert(ftxui_component_hoverable_change(NULL, NULL, NULL) == NULL);

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
    test_color_info();
    test_table_api();
    test_event_api();
    test_terminal_info();
    test_selection_api();
    test_mouse_capture();
    test_app_post();
    test_loop_lifecycle();
    test_null_safety();
    test_canvas_new_api();
    test_component_callbacks();
    test_container_with_selector();
    test_loop_run_once_blocking();
    test_maybe_fn();
    test_hoverable_callbacks();
    printf("=== all tests passed ===\n");
    return 0;
}
