cc := "gcc"
cflags := "-Wall -Wextra -Wpedantic -std=c23 -Iinclude"

raylib_cflags := `pkg-config --cflags raylib`
raylib_libs := `pkg-config --libs raylib`

clang-format:
    @clang-format -i src/*.c include/*.h

clang-check:
    @clang-format --dry-run --Werror src/*.c include/*.h

build: clang-format
    @{{cc}} {{cflags}} {{raylib_cflags}} src/*.c -o build/space-war {{raylib_libs}}

run: build
    @build/space-war

clean:
    @rm -f build/space-war
