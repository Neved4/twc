[![C](https://img.shields.io/badge/ISO_C99-A8B9CC?logo=c&logoColor=fff&style=for-the-badge)][C99]
[![POSIX.1](https://img.shields.io/badge/POSIX.1&#8209;2017-6A737D?labelColor=6A737D&style=for-the-badge)][POSIX.1-2017]

# `twc` - simple world clock

`twc` is a fast cli world clock that displays time zone information using
[tz database] to read valid [tz entries].

### Highlights

- 🚀 _**Fast**_ - `twc` executes in only ~ `8 ms` for one entry, (`177 ms`
  for ~`600` entries).
- 🔒 _**Robust**_ - tested to work with all [`tz database`] entries,
  version `2023c`.
- 📦 **Self-contained** - zero dependencies, lighweight, ISO C99,
  (`2676 bytes`, `140 lines`).

## Getting Started

### Setup

Clone the repository:

```sh
git clone https://github.com/Neved4/twc
```

Build the sources:

```console
$ make twc
cc twc.c -o twc
```

Alternatively, if you have [`zig`] installed:
```console
$ zig cc twc.c
```

### Usage

```
twc [-h] [-s FORMAT] [-f FILE | -t ENTRY] ...

Options:
    -h  Prints time in “human-readable” output instead of ISO 8601.
    -f  Specify a file to read entries from.
    -s  Use a different date format to print the time.
    -t  Manually specify a tz database entry.

Files:
    ~/.config/twc/tz.conf
        Stores valid tz database identifiers to be displayed by twc.

See Also:
    time(3), strftime(3)
```

## Compatibility

Runs on _**Linux**_, _**macOS**_ and _**\*BSD**_ systems on [`x86_64`] and
[`arm64`], and compiles with [`zig`], [`clang`], [`gcc`] and any other
compiler that supports [C99] or later.

## Standards

`twc` is compatible with [ISO 9945:2009][POSIX.1-2017], also known as
[POSIX.1-2017] as well as [ISO/IEC 9899][C23] known as [C23], and with
[C99].[^1] [^2]

## Acknowledgments

Special thanks to everybody who helped me with pointers and _measuring
optimal, safe memory allocation_ at every step of the program,
[@K4rakara](https://github.com/K4rakara/) for her code review, and
[@finnoleary](https://github.com/finnoleary), for getting me started.

## License
                 
`twc` is licensed under the terms of the [MIT License].

See the [LICENSE](LICENSE) file for details.

[`arm64`]: https://en.wikipedia.org/wiki/AArch64
[`clang`]: https://clang.llvm.org/
[`gcc`]: https://gcc.gnu.org/
[`x86_64`]: https://en.wikipedia.org/wiki/X86-64
[`zig`]: https://ziglang.org/
[C23]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[C99]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[MIT License]: https://opensource.org/license/mit/
[POSIX.1-2017]: https://pubs.opengroup.org/onlinepubs/9699919799/
[tz database]: https://en.wikipedia.org/wiki/Tz_database
[tz entries]: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

[^1]: _IEEE Std 1003.1-2017: Standard for Information Technology
    — Portable Operating System Interface (POSIX®)_,
    ISO/IEC/IEEE 9945:2009/COR 2:2017.
    URL: https://pubs.opengroup.org/onlinepubs/9699919799/
[^2]: _ISO/IEC 9899:2023: Standard for Information Technology
    — Programming languages — C_, ISO/IEC 9899:2023.
    URL: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf