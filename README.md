# osync

a collection of various osu! related tools i've written for managing my beatmap collections

# running

in powershell, running the `make.ps1` script should compile everything correctly. it will use clang, so make sure to have that installed. you can get it through [LLVM](https://releases.llvm.org/). binaries will be put into a `bin/` directory

# dependencies

all of the dependencies are included in the project for simplicity:

- id3lib
- sqlite3
- zlib

id3lib is licensed under the GNU General Public Licence, see `dep/id3lib/COPYING` for details.
sqlite3 is in the public domain, see its source files (`dep/sqlite3/src/sqlite3.c` and `dep/sqlite3/lib/sqlite3.h`) for details.
zlib is licensed under the MIT licence, see `dep/zlib/COPYING` for details.
