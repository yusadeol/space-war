cc := "gcc"
win_cc := "x86_64-w64-mingw32-gcc"

cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"
win_cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"

raylib_cflags := `pkg-config --cflags raylib`
win_raylib_cflags := "-I/usr/x86_64-w64-mingw32/include"

libs := `pkg-config --libs raylib yyjson`
win_libs := "-L/usr/x86_64-w64-mingw32/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lyyjson"

clang-format:
    @clang-format -i src/*.c include/*.h

iwyu-check:
    @sh -c 'flags="{{ cflags }} {{ raylib_cflags }}"; for f in src/*.c include/*.h; do include-what-you-use -Xiwyu --error=1 $flags "$f" || exit 1; done'

clang-check:
    @clang-format --dry-run --Werror src/*.c include/*.h

check: iwyu-check clang-check

build-windows: clang-format
    @{{ win_cc }} {{ win_cflags }} {{ win_raylib_cflags }} src/*.c -o build/windows/space-war.exe {{ win_libs }}
    @cp -r assets/ build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libraylib.dll build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libssp-0.dll build/windows/

build: clang-format
    @{{ cc }} {{ cflags }} {{ raylib_cflags }} src/*.c -o build/space-war {{ libs }}

run: build
    @build/space-war

clean:
    @rm -f build/space-war
