cc := "gcc"
win_cc := "x86_64-w64-mingw32-gcc"

cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"

pkg_cflags := `pkg-config --cflags raylib yyjson`
win_pkg_cflags := `x86_64-w64-mingw32-pkg-config --cflags raylib yyjson`

pkg_libs := `pkg-config --libs raylib yyjson`
win_pkg_libs := `x86_64-w64-mingw32-pkg-config --libs raylib yyjson`

clang-format:
    @clang-format -i src/*.c include/*.h

iwyu-check:
    @sh -c 'flags="{{ cflags }} {{ pkg_cflags }}"; for f in src/*.c include/*.h; do include-what-you-use -Xiwyu --error=1 $flags "$f" || exit 1; done'

clang-format-check:
    @clang-format --dry-run --Werror src/*.c include/*.h

check: iwyu-check clang-format-check

build-windows: check
    @{{ win_cc }} {{ cflags }} {{ win_pkg_cflags }} src/*.c -o build/windows/space-war.exe {{ win_pkg_libs }}
    @cp -r assets/ build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libraylib.dll build/windows/
    @cp /usr/x86_64-w64-mingw32/bin/libssp-0.dll build/windows/

build: check
    @{{ cc }} {{ cflags }} {{ pkg_cflags }} src/*.c -o build/space-war {{ pkg_libs }}

run: build
    @build/space-war

clean:
    @rm -f build/space-war
