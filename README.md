# osync

a collection of various osu! related tools i've written for managing my beatmap collections

# running

in powershell, running the `make.ps1` script should compile everything correctly. it will use clang, so make sure to have that installed. you can get it through [LLVM](https://releases.llvm.org/). binaries and object files will be put into `bin/` and `obj/` directories respectively

# dependencies

the source files for all dependencies are included in the project for simplicity:

- taglib
- sqlite3
- zlib

all of which are free open source software; view their licence documents or source files in their respective directories (under `dep/`) for details
