#include "ftxui_c_api.h"
#include <stdio.h>
#include <assert.h>

int destructor_count = 0;

void mock_destructor(void* userdata) {
    int* val = (int*)userdata;
    (*val)++;
    destructor_count++;
}

void dummy_callback(void* userdata) {
    (void)userdata;
}

static ftxui_element_handle_t bold_test_decorator(ftxui_element_handle_t el, void* userdata) {
    (void)userdata;
    return ftxui_element_bold(el);
}

int main() {
    printf("Running destructor verification tests...\n");

    // 1. Test Button Destructor
    {
        int local_count = 0;
        ftxui_component_handle_t btn = ftxui_component_button("Test", dummy_callback, &local_count, mock_destructor);
        assert(local_count == 0);
        ftxui_component_destroy(btn);
        assert(local_count == 1);
        printf("Button destructor: PASSED\n");
    }

    // 2. Test Checkbox Destructor
    {
        int local_count = 0;
        bool checked = false;
        ftxui_component_handle_t cb = ftxui_component_checkbox_with_change("Check", &checked, dummy_callback, &local_count, mock_destructor);
        assert(local_count == 0);
        ftxui_component_destroy(cb);
        assert(local_count == 1);
        printf("Checkbox destructor: PASSED\n");
    }

    // 3. Test Slider Destructor
    {
        int local_count = 0;
        int val = 50;
        ftxui_component_handle_t slider = ftxui_component_slider_int_with_change(&val, 0, 100, 1, dummy_callback, &local_count, mock_destructor);
        assert(local_count == 0);
        ftxui_component_destroy(slider);
        assert(local_count == 1);
        printf("Slider destructor: PASSED\n");
    }

    // 4. Test Radiobox Destructor
    {
        int local_count = 0;
        const char* entries[] = {"A", "B"};
        int selected = 0;
        ftxui_component_handle_t rb = ftxui_component_radiobox_with_change(entries, 2, &selected, dummy_callback, &local_count, mock_destructor);
        assert(local_count == 0);
        ftxui_component_destroy(rb);
        assert(local_count == 1);
        printf("Radiobox destructor: PASSED\n");
    }

    // 5. Test Menu with multiple Destructors
    {
        int change_count = 0;
        int enter_count = 0;
        const char* entries[] = {"Item 1"};
        int selected = 0;
        ftxui_component_handle_t menu = ftxui_component_menu_with_callbacks(
            entries, 1, &selected,
            dummy_callback, &change_count, mock_destructor,
            dummy_callback, &enter_count, mock_destructor
        );
        assert(change_count == 0);
        assert(enter_count == 0);
        ftxui_component_destroy(menu);
        assert(change_count == 1);
        assert(enter_count == 1);
        printf("Menu multiple destructors: PASSED\n");
    }

    // 6. Test Input with multiple Destructors
    {
        int change_count = 0;
        int enter_count = 0;
        ftxui_string_handle_t str = ftxui_string_create("");
        ftxui_input_options_t opts = {0};
        opts.content = str;
        opts.on_change = dummy_callback;
        opts.on_change_userdata = &change_count;
        opts.on_change_destructor = mock_destructor;
        opts.on_enter = dummy_callback;
        opts.on_enter_userdata = &enter_count;
        opts.on_enter_destructor = mock_destructor;

        ftxui_component_handle_t input = ftxui_component_input_with_options(opts);
        assert(change_count == 0);
        assert(enter_count == 0);
        ftxui_component_destroy(input);
        assert(change_count == 1);
        assert(enter_count == 1);
        ftxui_string_destroy(str);
        printf("Input multiple destructors: PASSED\n");
    }

    // 7. Test Renderer Destructor
    {
        int local_count = 0;
        ftxui_component_handle_t renderer = ftxui_component_renderer(NULL, (ftxui_render_callback_t)dummy_callback, &local_count, mock_destructor);
        // Note: dummy_callback signature matches ftxui_callback_t, ftxui_render_callback_t returns ftxui_element_handle_t.
        // But for destruction test, we just need it to be created.
        assert(local_count == 0);
        ftxui_component_destroy(renderer);
        assert(local_count == 1);
        printf("Renderer destructor: PASSED\n");
    }

    // 8. Test ftxui_component_destroy with NULL
    {
        printf("Testing ftxui_component_destroy(NULL) (smoke test)...\n");
        ftxui_component_destroy(NULL); 
        printf("ftxui_component_destroy(NULL): PASSED\n");
    }

    // 9. Test Table Selection Decorator Destructor
    {
        int local_count = 0;
        const char* cells[] = {"A"};
        ftxui_table_handle_t t = ftxui_table_create(cells, 1, 1);
        ftxui_table_selection_handle_t sel = ftxui_table_select_all(t);
        
        ftxui_table_selection_decorate(sel, bold_test_decorator, &local_count, mock_destructor);
        assert(local_count == 1); // Callback is executed synchronously and decorator destroyed immediately
        ftxui_table_selection_destroy(sel);
        ftxui_table_destroy(t);
        printf("Table selection decorator destructor: PASSED\n");
    }

    // 10. Test Selection Style Destructor
    {
        int local_count = 0;
        ftxui_element_handle_t text = ftxui_element_text("Hello");
        ftxui_element_handle_t styled = ftxui_element_selection_style(text, (ftxui_cell_style_callback_t)dummy_callback, &local_count, mock_destructor);
        
        assert(local_count == 0);
        ftxui_element_destroy(styled); // Destroying the element destroys selectionStyle and triggers cleanup
        assert(local_count == 1);
        printf("Selection style decorator destructor: PASSED\n");
    }

    // 11. Test Graph Destructor
    {
        int local_count = 0;
        ftxui_element_handle_t graph = ftxui_element_graph((ftxui_graph_callback_t)dummy_callback, &local_count, mock_destructor);
        
        assert(local_count == 0);
        ftxui_element_destroy(graph); // Destroying the graph element triggers cleanup
        assert(local_count == 1);
        printf("Graph element destructor: PASSED\n");
    }

    printf("All destructor tests PASSED. Total destructor calls: %d\n", destructor_count);
    return 0;
}
