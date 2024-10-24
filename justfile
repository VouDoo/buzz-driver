# Define variables for compiler, directories, and library names
CC := if os_family() == "windows" { "clang.exe" } else { "clang" }
CFLAGS := "-Wall -I./includes"
LDFLAGS := "-L./libs -lhidapi"

src_dir := "."
libs_dir := "./libs"
build_dir := "./build"
target := build_dir / if os_family() == "windows" { "buzzer.exe" } else { "buzzer" }

default: build

build:
    mkdir -p {{build_dir}}
    cp {{libs_dir}}/*.dll {{build_dir}}
    {{CC}} -o {{target}} {{src_dir}}/main.c {{CFLAGS}} {{LDFLAGS}}

clean:
    rm -rf {{build_dir}}

run: build
    ./{{target}}
