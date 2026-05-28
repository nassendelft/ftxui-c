# FTXUI C++ ↔ ftxui-c API Mapping

This document lists every symbol in the FTXUI 6.x public C++ API and shows the corresponding ftxui-c C function, or marks it **Not exposed** if no mapping exists.

Symbols in the C API that have no direct FTXUI equivalent (helper types, handle lifecycle) are listed at the end.

---

## Table of Contents

1. [App](#1-app--ftxuicomponentapphpp)
2. [Terminal](#2-terminal--ftxuiscreenterminalhpp)
3. [Loop](#3-loop--ftxuicomponentloophpp)
4. [Color](#4-color--ftxuiscreencolorhpp)
5. [Color Info](#5-color-info--ftxuiscreencolor_infohpp)
6. [Linear Gradient](#6-linear-gradient--ftxuidomlinear_gradienthpp)
7. [Elements — Basic](#7-elements--basic--ftxuidomelementshpp)
8. [Elements — Separators](#8-elements--separators)
9. [Elements — Layout](#9-elements--layout)
10. [Elements — Styling Decorators](#10-elements--styling-decorators)
11. [Elements — Flex / Size](#11-elements--flex--size)
12. [Elements — Frame / Scroll / Focus](#12-elements--frame--scroll--focus)
13. [Elements — Alignment & Utility](#13-elements--alignment--utility)
14. [Canvas](#14-canvas--ftxuidomcanvashpp)
15. [Table](#15-table--ftxuidomtablehpp)
16. [Table Selection](#16-table-selection)
17. [Components — Basic](#17-components--basic)
18. [Components — Containers](#18-components--containers)
19. [Components — Advanced](#19-components--advanced)
20. [Component Decorators](#20-component-decorators)
21. [Events](#21-events--ftxuicomponenteventhpp)
22. [Animation](#22-animation--ftxuicomponentanimationhpp)
23. [C-only Types & Helpers](#23-c-only-types--helpers)

---

## 1. App (`ftxui/component/app.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `App::Fullscreen()` | `ftxui_app_create_fullscreen()` | |
| `App::FitComponent()` | `ftxui_app_create_fit_component()` | |
| `App::TerminalOutput()` | `ftxui_app_create_terminal_output()` | |
| `App::FixedSize(int, int)` | `ftxui_app_create_fixed_size(w, h)` | |
| `App::FullscreenPrimaryScreen()` | `ftxui_app_create_fullscreen_primary_screen()` | |
| `App::FullscreenAlternateScreen()` | `ftxui_app_create_fullscreen_alternate_screen()` | |
| `app.Loop(component)` | `ftxui_app_loop(app, component)` | |
| `app.Exit()` | `ftxui_app_exit(app)` | |
| `app.ExitLoopClosure()` | Not exposed | Use `ftxui_app_exit` |
| `app.RequestAnimationFrame()` | `ftxui_app_request_animation_frame(app)` | |
| `app.Post(task)` | `ftxui_app_post(app, callback, userdata)` | Thread-safe; posts a closure |
| `app.PostEvent(event)` | `ftxui_app_post_event(app, event)` | Copies the event |
| `app.GetSelection()` | `ftxui_app_get_selection(app)` | Returns `char*`; caller must free |
| `app.SelectionChange(callback)` | `ftxui_app_selection_change(app, cb, userdata)` | |
| `app.TrackMouse(bool)` | `ftxui_app_track_mouse(app, enable)` | Call before Loop |
| `app.HandlePipedInput(bool)` | `ftxui_app_handle_piped_input(app, enable)` | Call before Loop |
| `app.ForceHandleCtrlC(bool)` | `ftxui_app_force_handle_ctrl_c(app, force)` | |
| `app.ForceHandleCtrlZ(bool)` | `ftxui_app_force_handle_ctrl_z(app, force)` | |
| `app.WithRestoredIO(fn)` | `ftxui_app_with_restored_io(app, callback, userdata)` | Executes callback synchronously with terminal restored |
| `app.CaptureMouse()` | `ftxui_app_capture_mouse(app)` | Returns `ftxui_captured_mouse_handle_t`; NULL if already captured |
| `app.TerminalName()` | `ftxui_app_terminal_name(app)` | `const char*`; valid until app destroyed; do not free |
| `app.TerminalVersion()` | `ftxui_app_terminal_version(app)` | Returns `int` |
| `app.TerminalEmulatorName()` | `ftxui_app_terminal_emulator_name(app)` | `const char*`; valid until app destroyed; do not free |
| `app.TerminalEmulatorVersion()` | `ftxui_app_terminal_emulator_version(app)` | `const char*`; valid until app destroyed; do not free |
| `app.TerminalCapabilities()` | `ftxui_app_terminal_capabilities(app, &count)` | Returns malloc'd `int*`; caller must free |
| `app.TerminalCapabilityNames()` | Not exposed | |
| `App::Active()` | `ftxui_app_active()` | Returns handle of currently running app, or NULL |
| `App::PostEventOrExecute(fn)` | Not exposed | Edge-case; `ftxui_app_post` covers it |

---

## 2. Terminal (`ftxui/screen/terminal.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Terminal::Size().dimx` | `ftxui_terminal_width()` | |
| `Terminal::Size().dimy` | `ftxui_terminal_height()` | |
| `Terminal::SetFallbackSize(dims)` | Not exposed | |
| `Terminal::ColorSupport()` | Not exposed | |
| `Terminal::SetColorSupport(color)` | Not exposed | |
| `Terminal::GetQuirks()` | Not exposed | |
| `Terminal::SetQuirks(quirks)` | Not exposed | |

---

## 3. Loop (`ftxui/component/loop.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Loop(App*, Component)` | `ftxui_loop_create(app, component)` | |
| `loop.HasQuitted()` | `ftxui_loop_has_quitted(loop)` | |
| `loop.RunOnce()` | `ftxui_loop_run_once(loop)` | |
| `loop.RunOnceBlocking()` | Not exposed | |
| `loop.Run()` | Not exposed | Use `ftxui_app_loop` instead |
| *(destructor)* | `ftxui_loop_destroy(loop)` | |

---

## 4. Color (`ftxui/screen/color.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Color()` (default) | `ftxui_color_default()` | |
| `Color(Palette1 index)` | `ftxui_color_palette1(index)` | |
| `Color(Palette16 index)` | `ftxui_color_palette16(index)` | |
| `Color(Palette256 index)` | `ftxui_color_palette256(index)` | Named enum values |
| `Color(uint8_t r, g, b)` | `ftxui_color_rgb(r, g, b)` | |
| `Color(uint8_t r, g, b, a)` | `ftxui_color_rgba(r, g, b, a)` | |
| `Color::RGB(r, g, b)` | `ftxui_color_rgb(r, g, b)` | Same result |
| `Color::RGBA(r, g, b, a)` | `ftxui_color_rgba(r, g, b, a)` | Same result |
| `Color::HSV(h, s, v)` | `ftxui_color_hsv(h, s, v)` | |
| `Color::HSVA(h, s, v, a)` | `ftxui_color_hsva(h, s, v, a)` | |
| `Color::Interpolate(t, a, b)` | `ftxui_color_interpolate(t, a, b)` | |
| `Color::Blend(lhs, rhs)` | `ftxui_color_blend(lhs, rhs)` | |
| `color.IsOpaque()` | `ftxui_color_is_opaque(color)` | |
| `color.Print(is_bg)` | `ftxui_color_print(color, is_bg)` | Returns `char*`; caller must free |
| `color == other` | `ftxui_color_equals(lhs, rhs)` | |
| `color != other` | `ftxui_color_not_equals(lhs, rhs)` | |
| `operator""_rgb(combined)` | Not exposed | Literal syntax unavailable in C |
| `enum Color::Palette1` | `ftxui_palette1_t` | |
| `enum Color::Palette16` | `ftxui_palette16_t` | |
| `enum Color::Palette256` | `ftxui_palette256_t` | |
| *(raw index)* | `ftxui_color_palette256_raw(index)` | Takes a raw `uint8_t` index |

---

## 5. Color Info (`ftxui/screen/color_info.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `GetColorInfo(Palette256)` | Not exposed | |
| `GetColorInfo(Palette16)` | Not exposed | |
| `ColorInfoSorted2D()` | `ftxui_color_info_sorted_2d(num_rows, max_cols)` | Returns flat `ftxui_color_info_t*`; free with `ftxui_color_info_free` |

---

## 6. Linear Gradient (`ftxui/dom/linear_gradient.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `LinearGradient()` | `ftxui_linear_gradient_create()` | |
| `LinearGradient(begin, end)` | Not exposed | Use `create` + two `stop` calls |
| `LinearGradient(angle, begin, end)` | Not exposed | Use `create` + `angle` + two `stop` calls |
| `gradient.Angle(angle)` | `ftxui_linear_gradient_angle(gradient, angle)` | |
| `gradient.Stop(color)` | `ftxui_linear_gradient_stop(gradient, color)` | |
| `gradient.Stop(color, position)` | `ftxui_linear_gradient_stop_at(gradient, color, position)` | |
| *(destructor)* | `ftxui_linear_gradient_destroy(gradient)` | |

---

## 7. Elements — Basic (`ftxui/dom/elements.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `text(string_view)` | `ftxui_element_text(text)` | |
| `vtext(string_view)` | `ftxui_element_vtext(text)` | |
| `paragraph(string_view)` | `ftxui_element_paragraph(text)` | |
| `paragraphAlignLeft(string_view)` | `ftxui_element_paragraph_align_left(text)` | |
| `paragraphAlignRight(string_view)` | `ftxui_element_paragraph_align_right(text)` | |
| `paragraphAlignCenter(string_view)` | `ftxui_element_paragraph_align_center(text)` | |
| `paragraphAlignJustify(string_view)` | `ftxui_element_paragraph_align_justify(text)` | |
| `emptyElement()` | `ftxui_element_empty()` | |
| `filler()` | `ftxui_element_filler()` | |
| `gauge(float)` | `ftxui_element_gauge(value)` | |
| `gaugeLeft(float)` | `ftxui_element_gauge_left(value)` | |
| `gaugeRight(float)` | `ftxui_element_gauge_right(value)` | |
| `gaugeUp(float)` | `ftxui_element_gauge_up(value)` | |
| `gaugeDown(float)` | `ftxui_element_gauge_down(value)` | |
| `gaugeDirection(float, Direction)` | `ftxui_element_gauge_direction(value, direction)` | |
| `spinner(int, size_t)` | `ftxui_element_spinner(charset_index, image_index)` | |
| `window(title, content)` | `ftxui_element_window(title, content)` | |
| `window(title, content, BorderStyle)` | Not exposed | Third argument always defaults to ROUNDED |
| `graph(GraphFunction)` | `ftxui_element_graph(callback, userdata)` | |
| `canvas(ConstRef<Canvas>)` | `ftxui_element_canvas_ref(canvas)` | Takes `ftxui_canvas_handle_t` |
| `canvas(width, height, fn)` | Not exposed | Use `ftxui_canvas_create` + `ftxui_element_canvas_ref` |
| `canvas(fn)` | Not exposed | Same workaround |
| `enum BorderStyle` | `ftxui_border_style_t` | |
| `enum WidthOrHeight` | `ftxui_width_or_height_t` | |
| `enum Constraint` | `ftxui_constraint_t` | |
| `enum Direction` | `ftxui_direction_t` | |

---

## 8. Elements — Separators

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `separator()` | `ftxui_element_separator()` | |
| `separatorLight()` | `ftxui_element_separator_light()` | |
| `separatorDashed()` | `ftxui_element_separator_dashed()` | |
| `separatorHeavy()` | `ftxui_element_separator_heavy()` | |
| `separatorDouble()` | `ftxui_element_separator_double()` | |
| `separatorEmpty()` | `ftxui_element_separator_empty()` | |
| `separatorStyled(BorderStyle)` | `ftxui_element_separator_styled(style)` | |
| `separatorCharacter(string_view)` | `ftxui_element_separator_character(char)` | |
| `separatorHSelector(l, r, unsel, sel)` | `ftxui_element_separator_hselector(l, r, unsel, sel)` | |
| `separatorVSelector(u, d, unsel, sel)` | `ftxui_element_separator_vselector(u, d, unsel, sel)` | |
| `separator(Cell)` | Not exposed | Requires `Cell` struct |

---

## 9. Elements — Layout

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `hbox(Elements)` | `ftxui_element_hbox(elements, count)` | |
| `vbox(Elements)` | `ftxui_element_vbox(elements, count)` | |
| `dbox(Elements)` | `ftxui_element_dbox(elements, count)` | Depth / overlay |
| `hflow(Elements)` | `ftxui_element_hflow(elements, count)` | |
| `vflow(Elements)` | `ftxui_element_vflow(elements, count)` | |
| `gridbox(vector<Elements>)` | `ftxui_element_gridbox(cells, total, row_lengths, rows)` | Flat array + row-length array |
| `flexbox(Elements, FlexboxConfig)` | `ftxui_element_flexbox(elements, count, config)` | `ftxui_flexbox_config_t` struct |
| `struct FlexboxConfig` | `ftxui_flexbox_config_t` | All fields and enums mapped |

---

## 10. Elements — Styling Decorators

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `bold(element)` | `ftxui_element_bold(element)` | |
| `dim(element)` | `ftxui_element_dim(element)` | |
| `italic(element)` | `ftxui_element_italic(element)` | |
| `inverted(element)` | `ftxui_element_inverted(element)` | |
| `underlined(element)` | `ftxui_element_underlined(element)` | |
| `underlinedDouble(element)` | `ftxui_element_underlined_double(element)` | |
| `blink(element)` | `ftxui_element_blink(element)` | |
| `strikethrough(element)` | `ftxui_element_strikethrough(element)` | |
| `color(Color, element)` | `ftxui_element_color(element, color)` | |
| `bgcolor(Color, element)` | `ftxui_element_bgcolor(element, color)` | |
| `color(LinearGradient, element)` | `ftxui_element_color_linear_gradient(element, gradient)` | |
| `bgcolor(LinearGradient, element)` | `ftxui_element_bgcolor_linear_gradient(element, gradient)` | |
| `border(element)` | `ftxui_element_border(element)` | |
| `borderLight(element)` | `ftxui_element_border_light(element)` | |
| `borderDashed(element)` | `ftxui_element_border_dashed(element)` | |
| `borderHeavy(element)` | `ftxui_element_border_heavy(element)` | |
| `borderDouble(element)` | `ftxui_element_border_double(element)` | |
| `borderRounded(element)` | `ftxui_element_border_rounded(element)` | |
| `borderEmpty(element)` | `ftxui_element_border_empty(element)` | |
| `borderStyled(BorderStyle)` | `ftxui_element_border_styled(element, style)` | |
| `borderStyled(BorderStyle, Color)` | `ftxui_element_border_styled_color(element, style, color)` | |
| `borderStyled(Color)` | `ftxui_element_border_colored(element, color)` | |
| `borderWith(const Cell&)` | Not exposed | Requires low-level `Cell` struct |
| `automerge(element)` | `ftxui_element_automerge(element)` | |
| `nothing(element)` | `ftxui_element_nothing(element)` | |
| `hyperlink(link, element)` | `ftxui_element_hyperlink(link, element)` | |
| `clear_under(element)` | `ftxui_element_clear_under(element)` | |
| `selectionStyleReset(element)` | `ftxui_element_selection_style_reset(element)` | |
| `selectionColor(Color)` | `ftxui_element_selection_color(element, color)` | |
| `selectionBackgroundColor(Color)` | `ftxui_element_selection_background_color(element, color)` | |
| `selectionForegroundColor(Color)` | `ftxui_element_selection_foreground_color(element, color)` | |
| `selectionStyle(fn)` | `ftxui_element_selection_style(element, callback, userdata)` | Callback receives `ftxui_cell_t*` |

---

## 11. Elements — Flex / Size

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `flex(element)` | `ftxui_element_flex(element)` | |
| `flex_grow(element)` | `ftxui_element_flex_grow(element)` | |
| `flex_shrink(element)` | `ftxui_element_flex_shrink(element)` | |
| `xflex(element)` | `ftxui_element_xflex(element)` | |
| `xflex_grow(element)` | `ftxui_element_xflex_grow(element)` | |
| `xflex_shrink(element)` | `ftxui_element_xflex_shrink(element)` | |
| `yflex(element)` | `ftxui_element_yflex(element)` | |
| `yflex_grow(element)` | `ftxui_element_yflex_grow(element)` | |
| `yflex_shrink(element)` | `ftxui_element_yflex_shrink(element)` | |
| `notflex(element)` | `ftxui_element_notflex(element)` | |
| `size(WidthOrHeight, Constraint, int)` | `ftxui_element_set_size(element, woh, constraint, value)` | |

---

## 12. Elements — Frame / Scroll / Focus

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `frame(element)` | `ftxui_element_frame(element)` | |
| `xframe(element)` | `ftxui_element_xframe(element)` | |
| `yframe(element)` | `ftxui_element_yframe(element)` | |
| `focus(element)` | `ftxui_element_focus(element)` | |
| `vscroll_indicator(element)` | `ftxui_element_vscroll_indicator(element)` | |
| `hscroll_indicator(element)` | `ftxui_element_hscroll_indicator(element)` | |
| `focusPosition(x, y)` | `ftxui_element_focus_position(element, x, y)` | |
| `focusPositionRelative(x, y)` | `ftxui_element_focus_position_relative(element, x, y)` | |
| `focusCursorBlock(element)` | `ftxui_element_focus_cursor_block(element)` | |
| `focusCursorBlockBlinking(element)` | `ftxui_element_focus_cursor_block_blinking(element)` | |
| `focusCursorBar(element)` | `ftxui_element_focus_cursor_bar(element)` | |
| `focusCursorBarBlinking(element)` | `ftxui_element_focus_cursor_bar_blinking(element)` | |
| `focusCursorUnderline(element)` | `ftxui_element_focus_cursor_underline(element)` | |
| `focusCursorUnderlineBlinking(element)` | `ftxui_element_focus_cursor_underline_blinking(element)` | |

---

## 13. Elements — Alignment & Utility

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `hcenter(element)` | `ftxui_element_hcenter(element)` | |
| `vcenter(element)` | `ftxui_element_vcenter(element)` | |
| `center(element)` | `ftxui_element_center(element)` | |
| `align_right(element)` | `ftxui_element_align_right(element)` | |

---

## 14. Canvas (`ftxui/dom/canvas.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Canvas(width, height)` | `ftxui_canvas_create(width, height)` | |
| `canvas.width()` | Not exposed | |
| `canvas.height()` | Not exposed | |
| `canvas.GetCell(x, y)` | Not exposed | |
| `canvas.DrawText(x, y, text)` | `ftxui_canvas_draw_text(canvas, x, y, text)` | |
| `canvas.DrawText(x, y, text, Color)` | `ftxui_canvas_draw_text_color(canvas, x, y, text, color)` | |
| `canvas.DrawText(x, y, text, Stylizer)` | Not exposed | |
| `canvas.DrawPointOn(x, y)` | Not exposed | |
| `canvas.DrawPointOff(x, y)` | Not exposed | |
| `canvas.DrawPointToggle(x, y)` | Not exposed | |
| `canvas.DrawPoint(x, y, bool)` | Not exposed | |
| `canvas.DrawPoint(x, y, bool, Color)` | Not exposed | |
| `canvas.DrawPoint(x, y, bool, Stylizer)` | Not exposed | |
| `canvas.DrawPointLine(x1, y1, x2, y2)` | Not exposed | Color-less overload |
| `canvas.DrawPointLine(x1, y1, x2, y2, Color)` | `ftxui_canvas_draw_point_line(canvas, x1, y1, x2, y2, color)` | |
| `canvas.DrawPointLine(x1, y1, x2, y2, Stylizer)` | Not exposed | |
| `canvas.DrawPointCircle(x, y, r)` | `ftxui_canvas_draw_point_circle(canvas, x, y, r)` | |
| `canvas.DrawPointCircle(x, y, r, Color)` | Not exposed | |
| `canvas.DrawPointCircle(x, y, r, Stylizer)` | Not exposed | |
| `canvas.DrawPointCircleFilled(x, y, r)` | `ftxui_canvas_draw_point_circle_filled(canvas, x, y, r)` | |
| `canvas.DrawPointCircleFilled(x, y, r, Color)` | Not exposed | |
| `canvas.DrawPointCircleFilled(x, y, r, Stylizer)` | Not exposed | |
| `canvas.DrawPointEllipse(x, y, r1, r2)` | `ftxui_canvas_draw_point_ellipse(canvas, x, y, rx, ry)` | |
| `canvas.DrawPointEllipse(x, y, r1, r2, Color)` | Not exposed | |
| `canvas.DrawPointEllipse(x, y, r1, r2, Stylizer)` | Not exposed | |
| `canvas.DrawPointEllipseFilled(x, y, r1, r2)` | `ftxui_canvas_draw_point_ellipse_filled(canvas, x, y, rx, ry)` | |
| `canvas.DrawPointEllipseFilled(x, y, r1, r2, Color)` | Not exposed | |
| `canvas.DrawPointEllipseFilled(x, y, r1, r2, Stylizer)` | Not exposed | |
| `canvas.DrawBlockOn(x, y)` | Not exposed | |
| `canvas.DrawBlockOff(x, y)` | Not exposed | |
| `canvas.DrawBlockToggle(x, y)` | Not exposed | |
| `canvas.DrawBlock(x, y, bool)` | Not exposed | |
| `canvas.DrawBlock(x, y, bool, Color)` | Not exposed | |
| `canvas.DrawBlock(x, y, bool, Stylizer)` | Not exposed | |
| `canvas.DrawBlockLine(x1, y1, x2, y2)` | Not exposed | Color-less overload |
| `canvas.DrawBlockLine(x1, y1, x2, y2, Color)` | `ftxui_canvas_draw_block_line(canvas, x1, y1, x2, y2, color)` | |
| `canvas.DrawBlockLine(x1, y1, x2, y2, Stylizer)` | Not exposed | |
| `canvas.DrawBlockCircle(x, y, r)` | `ftxui_canvas_draw_block_circle(canvas, x, y, r)` | |
| `canvas.DrawBlockCircle(x, y, r, Color)` | Not exposed | |
| `canvas.DrawBlockCircle(x, y, r, Stylizer)` | Not exposed | |
| `canvas.DrawBlockCircleFilled(x, y, r)` | `ftxui_canvas_draw_block_circle_filled(canvas, x, y, r)` | |
| `canvas.DrawBlockCircleFilled(x, y, r, Color)` | Not exposed | |
| `canvas.DrawBlockCircleFilled(x, y, r, Stylizer)` | Not exposed | |
| `canvas.DrawBlockEllipse(x, y, r1, r2)` | `ftxui_canvas_draw_block_ellipse(canvas, x, y, rx, ry)` | |
| `canvas.DrawBlockEllipse(x, y, r1, r2, Color)` | Not exposed | |
| `canvas.DrawBlockEllipse(x, y, r1, r2, Stylizer)` | Not exposed | |
| `canvas.DrawBlockEllipseFilled(x, y, r1, r2)` | `ftxui_canvas_draw_block_ellipse_filled(canvas, x, y, rx, ry)` | |
| `canvas.DrawBlockEllipseFilled(x, y, r1, r2, Color)` | Not exposed | |
| `canvas.DrawBlockEllipseFilled(x, y, r1, r2, Stylizer)` | Not exposed | |
| `canvas.DrawCell(x, y, Cell)` | Not exposed | |
| `canvas.DrawSurface(x, y, Surface)` | Not exposed | |
| `canvas.Style(x, y, Stylizer)` | Not exposed | |
| *(destructor)* | `ftxui_canvas_destroy(canvas)` | |

---

## 15. Table (`ftxui/dom/table.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Table(vector<vector<string>>)` | `ftxui_table_create(cells, rows, cols)` | C takes flat `const char**` + dimensions |
| `Table(vector<vector<Element>>)` | Not exposed | |
| `Table(initializer_list)` | Not exposed | |
| `table.Render()` | `ftxui_table_render(table)` | |
| `table.SelectAll()` | `ftxui_table_select_all(table)` | |
| `table.SelectCell(col, row)` | `ftxui_table_select_cell(table, col, row)` | |
| `table.SelectRow(row)` | `ftxui_table_select_row(table, row)` | |
| `table.SelectRows(min, max)` | `ftxui_table_select_rows(table, from, to)` | |
| `table.SelectColumn(col)` | `ftxui_table_select_column(table, col)` | |
| `table.SelectColumns(min, max)` | Not exposed | |
| `table.SelectRectangle(c_min, c_max, r_min, r_max)` | Not exposed | |
| *(destructor)* | `ftxui_table_destroy(table)` | |

---

## 16. Table Selection

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `sel.Border(BorderStyle)` | `ftxui_table_selection_border(sel, style)` | |
| `sel.Border(BorderStyle, Decorator)` | `ftxui_table_selection_border_color(sel, style, color)` | Only color decorator |
| `sel.BorderLeft(BorderStyle)` | Not exposed | |
| `sel.BorderRight(BorderStyle)` | Not exposed | |
| `sel.BorderTop(BorderStyle)` | Not exposed | |
| `sel.BorderBottom(BorderStyle)` | Not exposed | |
| `sel.Separator(BorderStyle)` | Not exposed | |
| `sel.SeparatorVertical(BorderStyle)` | `ftxui_table_selection_separator_vertical(sel, style)` | |
| `sel.SeparatorHorizontal(BorderStyle)` | Not exposed | |
| `sel.Decorate(Decorator)` | Not exposed | |
| `sel.DecorateAlternateRow(Decorator, modulo, shift)` | Not exposed | |
| `sel.DecorateAlternateColumn(Decorator, modulo, shift)` | Not exposed | |
| `sel.DecorateBorder(Decorator)` | Not exposed | |
| `sel.DecorateBorderLeft(Decorator)` | Not exposed | |
| `sel.DecorateBorderRight(Decorator)` | Not exposed | |
| `sel.DecorateBorderTop(Decorator)` | Not exposed | |
| `sel.DecorateBorderBottom(Decorator)` | Not exposed | |
| `sel.DecorateSeparator(Decorator)` | Not exposed | |
| `sel.DecorateSeparatorVertical(Decorator)` | Not exposed | |
| `sel.DecorateSeparatorHorizontal(Decorator)` | Not exposed | |
| `sel.DecorateCells(Decorator)` | Not exposed | |
| `sel.DecorateCellsAlternateColumn(Decorator, modulo, shift)` | Not exposed | |
| `sel.DecorateCellsAlternateRow(Decorator, modulo, shift)` | `ftxui_table_selection_decorate_cells_color_alternate_row(sel, color, modulo, offset)` | Only color; no general Decorator |
| *(bold shortcut)* | `ftxui_table_selection_decorate_bold(sel)` | Convenience — calls `Decorate(bold)` |
| *(align_right shortcut)* | `ftxui_table_selection_decorate_cells_align_right(sel)` | Convenience |
| *(color shortcut)* | `ftxui_table_selection_decorate_cells_color(sel, color)` | Convenience |
| *(destructor)* | `ftxui_table_selection_destroy(sel)` | |

---

## 17. Components — Basic

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Button(label, on_click)` | `ftxui_component_button(label, on_click, userdata)` | |
| `Button(label, on_click, ButtonOption)` | `ftxui_component_button_with_options(label, on_click, userdata, options)` | |
| `ButtonOption::Ascii()` | `ftxui_button_option_ascii()` | |
| `ButtonOption::Simple()` | `ftxui_button_option_simple()` | |
| `ButtonOption::Border()` | `ftxui_button_option_border()` | |
| `ButtonOption::Animated(...)` | `ftxui_button_option_animated(bg, fg, bg_active, fg_active)` | |
| `Checkbox(label, bool*)` | `ftxui_component_checkbox(label, checked)` | |
| `Checkbox(label, bool*, on_change)` | Not exposed | `on_change` callback not available |
| `Input(content, placeholder)` | `ftxui_component_input(content, placeholder)` | Uses `ftxui_string_handle_t` for content |
| `Input(content, placeholder, {password:true})` | `ftxui_component_input_password(content, placeholder)` | |
| `Input` option: `on_change` | Not exposed | |
| `Input` option: `on_enter` | Not exposed | |
| `Input` option: `multiline` | Not exposed | Always true |
| `Input` option: `insert` | Not exposed | |
| `Input` option: `cursor_position` | Not exposed | |
| `Toggle(entries, selected)` | `ftxui_component_toggle(entries, count, selected)` | |
| `Radiobox(entries, selected)` | `ftxui_component_radiobox(entries, count, selected)` | |
| `Radiobox` option: `on_change` | Not exposed | |
| `Menu(entries, selected)` | `ftxui_component_menu(entries, count, selected)` | |
| `Menu(entries, selected, Horizontal())` | `ftxui_component_menu_horizontal(entries, count, selected)` | |
| `Menu(entries, selected, HorizontalAnimated())` | `ftxui_component_menu_horizontal_animated(entries, count, selected)` | |
| `Menu(entries, selected, Toggle())` | `ftxui_component_menu_toggle(entries, count, selected)` | |
| `Menu` option: `on_change` | Not exposed | |
| `Menu` option: `on_enter` | Not exposed | |
| `MenuEntry(label)` | `ftxui_component_menu_entry(label)` | |
| `MenuEntry(label, {animated_colors})` | `ftxui_component_menu_entry_animated(label, colors)` | |
| `Dropdown(entries, selected)` | `ftxui_component_dropdown(entries, count, selected)` | |
| `Dropdown(DropdownOption)` | `ftxui_component_dropdown_custom(entries, count, selected, transform, ...)` | Partial — transform + entry transform only |
| `Slider(label, int*, min, max, inc)` | `ftxui_component_slider(label, value, min, max, increment)` | |
| `Slider(label, float*, min, max, inc)` | `ftxui_component_slider_float(label, value, min, max, increment)` | |
| `Slider(SliderOption<int>)` | `ftxui_component_slider_int_direction(value, min, max, inc, direction)` | No `on_change` |
| `Slider(SliderOption<float>)` | `ftxui_component_slider_float_direction(value, min, max, inc, direction, color_active, color_inactive)` | No `on_change` |
| `Slider` option: `on_change` | Not exposed | |
| `component.Render()` | `ftxui_component_render(component)` | Returns `ftxui_element_handle_t` |

---

## 18. Components — Containers

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Container::Vertical(children)` | `ftxui_component_container_vertical()` | Children added via `ftxui_container_add` |
| `Container::Vertical(children, selector)` | Not exposed | `selector` int* not available |
| `Container::Horizontal(children)` | `ftxui_component_container_horizontal()` | |
| `Container::Horizontal(children, selector)` | Not exposed | |
| `Container::Tab(children, selector)` | `ftxui_component_container_tab(selected)` | |
| `Container::Stacked(children)` | `ftxui_component_container_stacked()` | |
| `component.Add(child)` | `ftxui_container_add(container, child)` | |

---

## 19. Components — Advanced

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `Collapsible(label, child, show)` | `ftxui_component_collapsible(label, child, show)` | |
| `Maybe(child, bool*)` | `ftxui_component_maybe(child, show)` | |
| `Maybe(child, fn)` | Not exposed | Function overload not available |
| `Modal(main, modal, show)` | `ftxui_component_modal(main, modal, show)` | |
| `Hoverable(component, bool*)` | `ftxui_component_hoverable(component, hover)` | |
| `Hoverable(component, on_enter, on_leave)` | Not exposed | |
| `Hoverable(component, on_change)` | Not exposed | |
| `Window(WindowOptions)` | `ftxui_component_window(options)` | `ftxui_window_options_t` struct |
| `Renderer(child, fn)` | `ftxui_component_renderer(component, callback, userdata)` | |
| `Renderer(fn)` | Not exposed | Use `ftxui_component_renderer` with a null component |
| `Renderer(fn(bool focused))` | `ftxui_component_renderer_focusable(callback, userdata)` | |
| `Renderer(child, fn(inner_element))` | `ftxui_component_renderer_with_inner(component, callback, userdata)` | |
| `CatchEvent(child, fn)` | `ftxui_component_catch_event(component, callback, userdata)` | |
| `ResizableSplitLeft(main, back, size)` | `ftxui_component_resizable_split_left(main, back, size)` | |
| `ResizableSplitRight(main, back, size)` | `ftxui_component_resizable_split_right(main, back, size)` | |
| `ResizableSplitTop(main, back, size)` | `ftxui_component_resizable_split_top(main, back, size)` | |
| `ResizableSplitBottom(main, back, size)` | `ftxui_component_resizable_split_bottom(main, back, size)` | |
| `ResizableSplit(ResizableSplitOption)` | `ftxui_component_resizable_split_opt(option)` | `ftxui_resizable_split_option_t` struct |
| *(poll helper)* | `ftxui_component_poll(app, on_poll, userdata)` | No C++ equivalent; renders empty, calls `on_poll` each frame |

---

## 20. Component Decorators

All component decorators wrap the component's `Render()` output with the corresponding element decorator. The C API provides a parallel `ftxui_component_*` function for each.

| FTXUI C++ pattern | ftxui-c |
|-------------------|---------|
| `component \| border` | `ftxui_component_border(component)` |
| `component \| borderLight` | `ftxui_component_border_light(component)` |
| `component \| borderDashed` | `ftxui_component_border_dashed(component)` |
| `component \| borderHeavy` | `ftxui_component_border_heavy(component)` |
| `component \| borderDouble` | `ftxui_component_border_double(component)` |
| `component \| borderRounded` | `ftxui_component_border_rounded(component)` |
| `component \| borderEmpty` | `ftxui_component_border_empty(component)` |
| `component \| borderStyled(style)` | `ftxui_component_border_styled(component, style)` |
| `component \| bold` | `ftxui_component_bold(component)` |
| `component \| inverted` | `ftxui_component_inverted(component)` |
| `component \| underlined` | `ftxui_component_underlined(component)` |
| `component \| dim` | `ftxui_component_dim(component)` |
| `component \| blink` | `ftxui_component_blink(component)` |
| `component \| strikethrough` | `ftxui_component_strikethrough(component)` |
| `component \| nothing` | `ftxui_component_nothing(component)` |
| `component \| color(c)` | `ftxui_component_color(component, color)` |
| `component \| bgcolor(c)` | `ftxui_component_bgcolor(component, color)` |
| `component \| vscroll_indicator` | `ftxui_component_vscroll_indicator(component)` |
| `component \| frame` | `ftxui_component_frame(component)` |
| `component \| hcenter` | `ftxui_component_hcenter(component)` |
| `component \| vcenter` | `ftxui_component_vcenter(component)` |
| `component \| center` | `ftxui_component_center(component)` |
| `component \| align_right` | `ftxui_component_align_right(component)` |
| `component \| size(woh, c, v)` | `ftxui_component_set_size(component, woh, constraint, value)` |

---

## 21. Events (`ftxui/component/event.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `event.input()` | `ftxui_event_input(event)` | Returns `const char*` (internal storage) |
| `event.DebugString()` | `ftxui_event_debug_string(event)` | Returns `const char*` |
| `event.is_character()` | `ftxui_event_is_character(event)` | |
| `event.character()` | `ftxui_event_character(event)` | Returns `const char*` |
| `event.is_mouse()` | `ftxui_event_is_mouse(event)` | |
| `event.mouse().x` | `ftxui_event_mouse_x(event)` | |
| `event.mouse().y` | `ftxui_event_mouse_y(event)` | |
| `event.mouse().button` | Not exposed | |
| `event.mouse().motion` | Not exposed | |
| `event.mouse().shift/meta/control` | Not exposed | |
| `event.is_cursor_position()` | Not exposed | |
| `event.cursor_x()` / `cursor_y()` | Not exposed | |
| `event.is_cursor_shape()` | Not exposed | |
| `event.cursor_shape()` | Not exposed | |
| `event.IsTerminalNameVersion()` | Not exposed | |
| `event.TerminalName()` | Not exposed | |
| `event.TerminalVersion()` | Not exposed | |
| `event.IsTerminalEmulator()` | Not exposed | |
| `event.IsTerminalCapabilities()` | Not exposed | |
| `Event::ArrowLeft/Right/Up/Down` | Not exposed | Static constants — C has no way to construct or compare |
| `Event::Backspace/Delete/Return/Escape/Tab` | Not exposed | Same — use `event.input()` to compare raw bytes |
| `Event::F1`–`F12` | Not exposed | |
| `Event::CtrlX`, `Event::AltX`, etc. | Not exposed | |
| `Event::Custom` | Not exposed | |
| `Event::Character(char)` | Not exposed | Events are received, not constructed, in C |
| `Event::Special(string_view)` | Not exposed | |
| `Event::Mouse(string_view, Mouse)` | Not exposed | |

---

## 22. Animation (`ftxui/component/animation.hpp`)

| FTXUI C++ | ftxui-c | Notes |
|-----------|---------|-------|
| `animation::RequestAnimationFrame()` | `ftxui_app_request_animation_frame(app)` | |
| `animation::easing::Linear` | `ftxui_easing_function_get(FTXUI_EASING_LINEAR)` | Returns a C function pointer |
| `animation::easing::QuadraticIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_QUADRATIC_*)` | |
| `animation::easing::CubicIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_CUBIC_*)` | |
| `animation::easing::QuarticIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_QUARTIC_*)` | |
| `animation::easing::QuinticIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_QUINTIC_*)` | |
| `animation::easing::SineIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_SINE_*)` | |
| `animation::easing::CircularIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_CIRCULAR_*)` | |
| `animation::easing::ExponentialIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_EXPONENTIAL_*)` | |
| `animation::easing::ElasticIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_ELASTIC_*)` | |
| `animation::easing::BackIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_BACK_*)` | |
| `animation::easing::BounceIn/Out/InOut` | `ftxui_easing_function_get(FTXUI_EASING_BOUNCE_*)` | |
| `animation::Animator` class | Not exposed | Internal animation state machine |
| `animation::Params` class | Not exposed | Passed internally to `OnAnimation` hooks |

---

## 23. C-only Types & Helpers

These types and functions exist in the C API with no direct FTXUI C++ equivalent — they are infrastructure for bridging the C / C++ boundary.

| ftxui-c | Purpose |
|---------|---------|
| `ftxui_string_handle_t` | Mutable `std::string` wrapper for `Input` content |
| `ftxui_string_create(initial)` | Allocate a managed string |
| `ftxui_string_get(str)` | Read current value |
| `ftxui_string_set(str, value)` | Write new value |
| `ftxui_string_destroy(str)` | Free |
| `ftxui_component_destroy(component)` | Free component handle |
| `ftxui_element_destroy(element)` | Free element handle |
| `ftxui_app_destroy(app)` | Free app handle |
| `ftxui_color_destroy(color)` | Free color handle |
| `ftxui_captured_mouse_handle_t` | Scoped mouse capture; destroy to release |
| `ftxui_captured_mouse_destroy(handle)` | Release mouse capture |
| `ftxui_canvas_destroy(canvas)` | Free canvas handle |
| `ftxui_linear_gradient_destroy(gradient)` | Free gradient handle |
| `ftxui_table_destroy(table)` | Free table handle |
| `ftxui_table_selection_destroy(sel)` | Free selection handle |
| `ftxui_loop_destroy(loop)` | Free loop handle |
| `ftxui_color_info_free(data)` | Free color info array |
| `ftxui_cell_t` | Flat struct mirroring `Cell` for use in callbacks |
