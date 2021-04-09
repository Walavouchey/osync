$should_link = 0;

mkdir obj -erroraction ignore >$null 2>&1;
mkdir obj/dep -erroraction ignore >$null 2>&1;

# ZLIB

$zlib_source_files = ls dep/zlib/src/*.c;
mkdir obj/dep/zlib -erroraction ignore >$null 2>&1;
ForEach ($file in $zlib_source_files) {
    $base = $file.BaseName;
    $name = $file.Name;
    if (-not (Test-Path -Path obj/dep/zlib/$base.o)) {
        echo "Compiling $name...";
        clang $file -I dep/zlib/include -Wno-deprecated-declarations -O3 -c -o "obj/dep/zlib/$base.o"
        if ($LASTEXITCODE -eq 0) { $should_link = 1; }
    }
}

# TAGLIB

$taglib_include = (ls .\dep\taglib\taglib\ -directory -recurse | foreach {$s = @()} { $s += "-I"; $s += $_ | resolve-path -relative } {$s}) + "-I" + "dep/taglib/3rdparty" + "-I" + "dep/taglib" + "-I" + "dep/taglib/taglib";

$source_files = ls dep/taglib/taglib/*.cpp -recurse;
mkdir obj/dep/taglib -erroraction ignore >$null 2>&1;
ForEach ($file in $source_files) {
    $base = $file.BaseName
    $name = $file.Name
    if (-not (Test-Path -Path obj/dep/taglib/$base.o)) {
        echo "Compiling $name..."
        clang $file -std=c++14 -I "dep/zlib/include" @taglib_include -D TAGLIB_STATIC -D NDEBUG -D HAVE_ZLIB -Wno-microsoft-unqualified-friend -Wno-deprecated-declarations -Wno-delete-abstract-non-virtual-dtor -Wno-duplicate-decl-specifier  -O3 -c -o "obj/dep/taglib/$base.o";
        if ($LASTEXITCODE -eq 0) { $should_link = 1; }
    }
}

# SQLITE3

if (-not (Test-Path -Path obj/dep/sqlite3/sqlite3.o)) {
    echo "Compiling sqlite3.c...";
    mkdir obj/dep -erroraction ignore >$null 2>&1;
    mkdir obj/dep/sqlite3 -erroraction ignore >$null 2>&1;
    clang dep/sqlite3/src/sqlite3.c -I lib -Wno-deprecated-declarations -O3 -c -o obj/dep/sqlite3/sqlite3.o;
    if ($LASTEXITCODE -eq 0) { $should_link = 1; }
}

# SOURCE

$source_files = ls src/*.cpp;
mkdir obj/lib -erroraction ignore >$null 2>&1;
ForEach ($file in $source_files) {
    $base = $file.BaseName;
    $name = $file.Name;
    if (-not (Test-Path -Path obj/lib/$base.o)) {
        echo "Compiling $name...";
        clang $file -std=c++17 -I lib -I dep/sqlite3/lib -I dep/taglib -D TAGLIB_STATIC -O3 -c -o "obj/lib/$base.o";
        if ($LASTEXITCODE -eq 0) { $should_link = 1; }
    }
}

# MAIN

$main_files = ls *.cpp;
$compiled_files = @(0) * $main_files.count;
$index = 0;
mkdir obj/main -erroraction ignore >$null 2>&1;
ForEach ($file in $main_files) {
    $base = $file.BaseName;
    $name = $file.Name;
    if (-not (Test-Path -Path obj/main/$base.o)) {
        echo "Compiling $name...";
        clang $file -std=c++17 -I lib -I dep/sqlite3/lib @taglib_include -D TAGLIB_STATIC -O3 -c -o "obj/main/$base.o";
        if ($LASTEXITCODE -eq 0) { $compiled_files[$index] = 1; }
    }
    $index++;
}

# LINK

mkdir bin -erroraction ignore >$null 2>&1;
$index = 0;
ForEach ($file in $main_files) {
    $base = $file.BaseName;
    $name = $file.Name;
    if ((-not (Test-Path -Path "bin/$base.exe")) -or $should_link -or $compiled_files[$index]) {
        echo "Linking $base.exe...";
        clang obj/dep/zlib/*.o obj/dep/taglib/*.o obj/dep/sqlite3/*.o obj/lib/*.o obj/main/$base.o -o "bin/$base.exe";
    }
    $index++;
}
