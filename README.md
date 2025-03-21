[![C](https://img.shields.io/badge/ISO_C17-A8B9CC?logo=c&logoColor=fff)][C17]
[![IEEE](https://img.shields.io/badge/POSIX.1&#8209;2024-00629B?logo=ieee&logoColor=fff)][POSIX.1-2024]
[![CodeQL](https://github.com/github/docs/actions/workflows/codeql.yml/badge.svg)](https://github.com/Neved4/twc/actions/workflows/codeql.yml)
[![Homebrew](https://img.shields.io/badge/Homebrew-tap-2AAB47?logo=homebrew&logoColor=959DA5&labelColor=2B3137)](https://github.com/Neved4/homebrew-tap/blob/main/Formula/gotwc.rb)

# `twc` - Tiny world clock in C! 🛠️

Fast, CLI world clock that displays time zone information using
[tz database] to read valid [tz entries].

It works similar to [zdump(8)], but it lets you format the output more
precisely:

```console
$ twc -h -f tools/samples/tz-small.conf
America/New York     2024-12-10  16:39:43  -0500
America/Los Angeles  2024-12-10  13:39:43  -0800
UTC                  2024-12-10  21:39:43  +0000
Europe/London        2024-12-10  21:39:43  +0000
Europe/Paris         2024-12-10  22:39:43  +0100
Asia/Tokyo           2024-12-11  06:39:43  +0900
Australia/Sydney     2024-12-11  08:39:43  +1100
```

### Highlights

- 🚀 _**Fast**_ - 5X times faster than `date`.[^1] Only ≈ `8 ms` for one
  entry, and ≈ `177 ms` for ≈ 600 entries.
- 🔒 _**Robust**_ - tested to work with all [tz database] entries,
  [`version 2024b`].
- 📦 _**Self-contained**_ - zero dependencies, ISO C17,
  lightweight (`175 lines`, `3235 bytes`).

## Getting Started

### Prerequisites

If you are building [`twc`], you'll need:

- A [C] compiler that supports [C99] or later.
- A system with a working `asprintf` implementation.
- If `asprintf` is not present, you can use [Neved4/asprintf].

### Installing

If you have [Homebrew] installed, just run:
```shell
brew install Neved4/homebrew-tap/twc
```

### Building

Clone the repository:

```shell
git clone https://github.com/Neved4/twc
```

Build the sources:

```shell
make twc
```

Alternatively, if you have [`zig`] installed:
```shell
zig cc twc.c -o twc
```

### Usage

These are the options available:

```text
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
  Print Tokyo date in a human-readable format with YY-MM-DD format:

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
```shell
docker build .
```

## Compatibility

Runs on _**Linux**_, _**macOS**_ and _**\*BSD**_ systems on [`x86_64`] and
[`arm64`], and compiles with [`zig`], [`clang`], [`gcc`], [`tcc`] and any other
compiler that supports [C17] or later.

Also builds on any [C99] compiler on systems that supports and have
`asprintf()`.

## Standards

`twc` is compatible with [POSIX.1-2024][][^2] as well as [C23][][^3], and
it outputs [ISO 8601][][^4] format.

## See Also

Various [`zdump`][zdump(8)] implementations:

- [glibc - zdump.c](https://github.com/apple-oss-distributions/system_cmds/blob/main/zdump/zdump.c)
- [apple - zdump.c](https://github.com/apple-oss-distributions/ICU/blob/main/icu/icu4c/source/tools/tzcode/zdump.c#L94)
- [FreeBSD - zdump.c](https://github.com/freebsd/freebsd-src/blob/main/contrib/tzcode/zdump.c)
- [OpenBSD - zdump.c](https://github.com/openbsd/src/blob/master/usr.sbin/zdump/zdump.c)
- [Boruch Baum - zdump3](https://github.com/Boruch-Baum/zdump-3-/blob/master/zdump3.c)

## Acknowledgments

Special thanks to everybody who helped me with pointers and memory allocation,
[@navi](https://git.vlhl.dev/navi), [@enigmatico](https://gitlab.com/enigmatico)
and [@K4rakara](https://github.com/K4rakara/) for their code reviews, and thx to
[@lexiwitch](https://github.com/lexiwitch), for getting me started.

## License

`twc` is licensed under the terms of the [MIT License].

See the [LICENSE](LICENSE) file for details.

[Neved4/asprintf]: https://github.com/Neved4/asprintf
[`twc`]: https://github.com/Neved4/twc
[`hyperfine`]: https://github.com/sharkdp/hyperfine
[`arm64`]: https://en.wikipedia.org/wiki/AArch64
[`x86_64`]: https://en.wikipedia.org/wiki/X86-64
[`clang`]: https://clang.llvm.org/
[`gcc`]: https://gcc.gnu.org/
[`tcc`]: https://bellard.org/tcc/
[`zig`]: https://ziglang.org/
[MIT License]: https://opensource.org/license/mit/
[C]: https://en.wikipedia.org/wiki/C_(programming_language)
[C99]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
[C17]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf
[C23]: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
[POSIX.1-2024]: https://pubs.opengroup.org/onlinepubs/9799919799/
[ISO 8601]: https://www.iso.org/obp/ui/#iso:std:iso:8601:-2:ed-1:v1:en
[tz database]: https://en.wikipedia.org/wiki/Tz_database
[tz entries]: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
[`version 2024b`]: https://www.iana.org/time-zones
[Docker]: https://www.docker.com/
[Homebrew]: https://brew.sh/
[zdump(8)]: https://man.freebsd.org/cgi/man.cgi?query=zdump&apropos=0&sektion=0&manpath=FreeBSD+7.0-RELEASE&format=html

[^1]: _cfr._ `date` command takes ≈ `931 ms` when iterating over ≈ 600
    entries. Measured with [`hyperfine`].
[^2]: _IEEE Std 1003.1-2024: Standard for Information Technology
    — Portable Operating System Interface (POSIX®)_, \
    ISO/IEC DIS 9945. URL: https://pubs.opengroup.org/onlinepubs/9799919799/
[^3]: _ISO/IEC 9899: Standard for Information Technology
    — Programming languages — C_, ISO/IEC 9899:2023. \
    URL: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
[^4]: _ISO 8601: Date and time — Representations for information interchange_, ISO 8601-1:2019. \
    URL: https://www.iso.org/obp/ui/#iso:std:iso:8601:-2:ed-1:v1:en
