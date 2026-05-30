# ftxui-c

A C binding for [FTXUI](https://github.com/ArthurSonzogni/FTXUI), a C++ library for building terminal user interfaces. ftxui-c exposes the full FTXUI API through a stable C interface using opaque handles, making it usable from C and from any language with a C FFI.

## API Coverage

All 22 FTXUI modules are wrapped:

| §  | Module |
|----|--------|
| 1  | App |
| 2  | Terminal |
| 3  | Loop |
| 4  | Color |
| 5  | Color Info |
| 6  | Linear Gradient |
| 7  | Elements — Basic |
| 8  | Elements — Separators |
| 9  | Elements — Layout |
| 10 | Elements — Styling Decorators |
| 11 | Elements — Flex / Size |
| 12 | Elements — Frame / Scroll / Focus |
| 13 | Elements — Alignment & Utility |
| 14 | Canvas |
| 15 | Table |
| 16 | Table Selection |
| 17 | Components — Basic |
| 18 | Components — Containers |
| 19 | Components — Advanced |
| 20 | Component Decorators |
| 21 | Events |
| 22 | Animation |

See [API_MAPPING.md](API_MAPPING.md) for the full symbol-by-symbol mapping between the FTXUI C++ API and the ftxui-c C API.

## Installation

### Homebrew (macOS and Linux)

```sh
brew tap nassendelft/ftxui-c
brew install ftxui-c
```

This installs the shared library (`libftxui_c_binding.dylib`/`.so`) and header. The shared library is self-contained — FTXUI is statically linked in at build time, so no separate FTXUI install is needed.

### Pre-built releases

Download a pre-built archive for your platform from the [Releases](../../releases) page. Each archive contains:

```
include/
  ftxui_c_api.h
lib/
  libftxui_c_binding.so   (or .dylib on macOS)
  libftxui_c_binding.a
  libftxui-component.a    \
  libftxui-dom.a           } needed when linking statically
  libftxui-screen.a       /
```

**Shared library:**
```sh
cc myapp.c -Iinclude -Llib -lftxui_c_binding -o myapp
```

**Static library:**
```sh
cc myapp.c -Iinclude -Llib \
  -lftxui_c_binding -lftxui-component -lftxui-dom -lftxui-screen \
  -lstdc++ -o myapp
```

### Build from source

#### Prerequisites

- CMake 3.12+
- A C++17 compiler (GCC, Clang)

#### Steps

```sh
git clone --recurse-submodules https://github.com/nassendelft/ftxui-c
cd ftxui-c
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This produces:
- `build/libftxui_c_binding.so` — shared library
- `build/libftxui_c_binding.a` — static library
- `ftxui_c_api.h` — C header (in the source root)

## Example

```c
#include "ftxui_c_api.h"

static ftxui_app_handle_t app;

static void on_quit(void* userdata) {
    (void)userdata;
    ftxui_app_exit(app);
}

int main(void) {
    app = ftxui_app_create_fullscreen();

    ftxui_component_handle_t btn = ftxui_component_button("Quit", on_quit, NULL);

    ftxui_app_loop(app, btn);
    ftxui_app_destroy(app);
    return 0;
}
```

## License

GPL-3.0. See [LICENSE](LICENSE).

FTXUI itself is MIT-licensed. See `vendor/ftxui/LICENSE`.
