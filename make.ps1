$should_link = 0;

mkdir obj -erroraction ignore >$null 2>&1

if (-not (Test-Path -Path obj/dep/sqlite3/sqlite3.o)) {
    echo "Compiling sqlite3.c..."
    mkdir obj/dep -erroraction ignore >$null 2>&1
    mkdir obj/dep/sqlite3 -erroraction ignore >$null 2>&1
    clang dep/sqlite3/src/sqlite3.c -I lib -Wno-deprecated-declarations -O3 -c -o obj/dep/sqlite3/sqlite3.o
    $should_link = 1;
}

$source_files = ls src/*.cpp
ForEach ($file in $source_files) {
    $base = $file.BaseName
    $name = $file.Name
    mkdir obj/lib -erroraction ignore >$null 2>&1
    if (-not (Test-Path -Path obj/lib/$base.o)) {
        echo "Compiling $name..."
        clang $file -std=c++17 -I lib -I dep/sqlite3/lib -O3 -c -o "obj/lib/$base.o"
        $should_link = 1;
    }
}

$main_files = ls *.cpp
ForEach ($file in $main_files) {
    $base = $file.BaseName
    $name = $file.Name
    mkdir obj/main -erroraction ignore >$null 2>&1
    if (-not (Test-Path -Path obj/main/$base.o)) {
        echo "Compiling $name..."
        clang $file -std=c++17 -I lib -I dep/sqlite3/lib -O3 -c -o "obj/main/$base.o"
        $should_link = 1;
    }
}

mkdir bin -erroraction ignore >$null 2>&1
ForEach ($file in $main_files) {
    $base = $file.BaseName
    $name = $file.Name
    if ((-not (Test-Path -Path "bin/$base.exe")) -or $should_link) {
        echo "Linking $base.exe..."
        clang obj/dep/sqlite3/*.o obj/lib/*.o obj/main/$base.o -o "bin/$base.exe"
    }
}
