[![C](https://img.shields.io/badge/ISO_C99-A8B9CC?logo=c&logoColor=fff)][C99]
![IEEE](https://img.shields.io/badge/POSIX.1&#8209;2017-00629B?logo=ieee&logoColor=fff)
[![CodeQL](https://github.com/github/docs/actions/workflows/codeql.yml/badge.svg)](https://github.com/Neved4/twc/actions/workflows/codeql.yml)

# `twc` - Tiny world clock in C! 🛠️

Fast, CLI world clock that displays time zone information using
[tz database] to read valid [tz entries].

```console
$ twc -h -f tools/samples/tz-small.conf
America/New York     2023-12-10  16:39:43  -0500
America/Los Angeles  2023-12-10  13:39:43  -0800
UTC                  2023-12-10  21:39:43  +0000
Europe/London        2023-12-10  21:39:43  +0000
Europe/Paris         2023-12-10  22:39:43  +0100
Asia/Tokyo           2023-12-11  06:39:43  +0900
Australia/Sydney     2023-12-11  08:39:43  +1100
```

### Highlights

- 🚀 _**Fast**_ - 5X times faster than `date`.[^1] Only ≈ `8 ms` for one
  entry, and ≈ `177 ms` for ≈ 600 entries.
- 🔒 _**Robust**_ - tested to work with all [tz database] entries,
  [`version 2023c`].
- 📦 _**Self-contained**_ - zero dependencies, ISO C99,
  lightweight (`132 lines`, `2659 bytes`).

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
$ zig cc twc.c -t twc
```

### Usage

```
twc [-h] [-f path] [-s format] [-t timezone] ...

Options:
  -f path
      Read config from path (default "$HOME/.config/twc/tz.conf")
  -h
      Print in human-readable format
  -s format
      Set desired time format (e.g. "%Y-%m-%d")
  -t timezone
      specific timezone (e.g. "Asia/Tokyo")

Examples:
  Print Tokyo's date in a human-readable format with YY-MM-DD format:

    $ twc -h -s %Y-%m-%d -t Asia/Tokyo
    2024-01-02
  
  Print date in Los Angeles using an environment variable for the timezone:
  
    $ TZ=America/Los_Angeles twc
    2024-01-02T15:04:05-0800

Environment:
  TZ
    Timezone to use when displaying dates.
  XDG_CONFIG_HOME
    When defined, it will be used to store the tz.conf file.

Files:
  ~/.config/twc/tz.conf
    Stores tz database entries to be displayed.

See also:
    time(3), strftime(3), environ(7)
```

### Docker

To compile the binary inside a [Docker] image, run:
```sh
docker build .
```

## Compatibility

Runs on _**Linux**_, _**macOS**_ and _**\*BSD**_ systems on [`x86_64`] and
[`arm64`], and compiles with [`zig`], [`clang`], [`gcc`], [`tcc`] and any other
compiler that supports [C99] or later.

## Standards

`twc` is compatible with [POSIX.1-2017][][^2] as well as [C23][][^3], and
it outputs [ISO 8601][][^4] format.

## Acknowledgments

Special thanks to everybody who helped me with pointers and memory
allocation, [@enigmatico](https://gitlab.com/enigmatico) and
[@K4rakara](https://github.com/K4rakara/) for her code review, and
[@finnoleary](https://github.com/finnoleary), for getting me started.

## License
                 
`twc` is licensed under the terms of the [MIT License].

See the [LICENSE](LICENSE) file for details.

[`hyperfine`]: https://github.com/sharkdp/hyperfine
[`arm64`]: https://en.wikipedia.org/wiki/AArch64
[`x86_64`]: https://en.wikipedia.org/wiki/X86-64
[`clang`]: https://clang.llvm.org/
[`gcc`]: https://gcc.gnu.org/
[`tcc`]: https://bellard.org/tcc/
[`zig`]: https://ziglang.org/
[MIT License]: https://opensource.org/license/mit/
[C23]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[C99]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[POSIX.1-2017]: https://pubs.opengroup.org/onlinepubs/9699919799/
[ISO 8601]: https://www.iso.org/obp/ui/#iso:std:iso:8601:-2:ed-1:v1:en
[tz database]: https://en.wikipedia.org/wiki/Tz_database
[tz entries]: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
[`version 2023c`]: https://www.iana.org/time-zones
[Docker]: https://www.docker.com/

[^1]: _cfr._ `date` command takes ≈ `931 ms` when iterating over ≈ 600
    entries. Measured with [`hyperfine`].
[^2]: _IEEE Std 1003.1-2017: Standard for Information Technology
    — Portable Operating System Interface (POSIX®)_, \
    ISO/IEC/IEEE 9945:2009/COR 2:2017. URL: https://pubs.opengroup.org/onlinepubs/9699919799/
[^3]: _ISO/IEC 9899: Standard for Information Technology
    — Programming languages — C_, ISO/IEC 9899:2023. \
    URL: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
[^4]: _ISO 8601: Date and time — Representations for information interchange_, ISO 8601-1:2019. \
    URL: https://www.iso.org/obp/ui/#iso:std:iso:8601:-2:ed-1:v1:en
