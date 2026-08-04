cc := "gcc"
win_cc := "x86_64-w64-mingw32-gcc"

cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"
win_cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"

raylib_cflags := `pkg-config --cflags raylib`
win_raylib_cflags := "-I/usr/x86_64-w64-mingw32/include"

win_raylib_libs := "-L/usr/x86_64-w64-mingw32/lib -lraylib -lopengl32 -lgdi32 -lwinmm"
raylib_libs := `pkg-config --libs raylib`

iwyu-check:
    @sh -c 'flags="{{ cflags }} $(pkg-config --cflags raylib)"; for f in src/*.c include/*.h; do include-what-you-use -Xiwyu --error=1 $flags "$f" || exit 1; done'

clang-format:
    @clang-format -i src/*.c include/*.h

clang-check:
    @clang-format --dry-run --Werror src/*.c include/*.h

check: iwyu-check clang-check

build: clang-format
    @{{ cc }} {{ cflags }} {{ raylib_cflags }} src/*.c -o build/space-war {{ raylib_libs }}

build-windows: clang-format
    @{{ win_cc }} {{ win_cflags }} {{ win_raylib_cflags }} src/*.c -o build/windows/space-war.exe {{ win_raylib_libs }}
    @cp -r assets/ build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libraylib.dll build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libssp-0.dll build/windows/

run: build
    @build/space-war

clean:
    @rm -f build/space-war
