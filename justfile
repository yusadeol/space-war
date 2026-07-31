cc := "gcc"
cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"

raylib_cflags := `pkg-config --cflags raylib`
raylib_libs := `pkg-config --libs raylib`

iwyu-check:
    @sh -c 'for f in src/*.c; do include-what-you-use {{ cflags }} {{ raylib_cflags }} "$f" || exit 1; done'

clang-format:
    @clang-format -i src/*.c include/*.h

clang-check:
    @clang-format --dry-run --Werror src/*.c include/*.h

check: iwyu-check clang-check

build: clang-format
    @{{ cc }} {{ cflags }} {{ raylib_cflags }} src/*.c -o build/space-war {{ raylib_libs }}

run: build
    @build/space-war

clean:
    @rm -f build/space-war
