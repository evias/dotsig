# dotsig: digital signatures on-the-fly

The [dotsig][self] software lets you create and verify digital signatures
using RSA cryptography (PKCS1 v1.5), PGP and elliptic-curve cryptography (ECDSA).

You can find the reference documentation [here][docs].

This software is released under the [3-Clause BSD License](./LICENSE).

[![Latest release](https://img.shields.io/github/v/release/evias/dotsig)](#releases)
[![Total downloads](https://img.shields.io/github/downloads/evias/dotsig/total)][download]
[![Reference documentation](https://img.shields.io/badge/Reference%20documentation-blue)][docs]
[![3-Clause BSD License](https://img.shields.io/github/license/evias/dotsig)](./LICENSE)
[![Contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)][issues]

## Releases

The `dotsig` software is currently under active development and frequent changes
to the source code *are to be expected* until the software reaches a stable tag.

The latest release as of May 13th 2024 is: [`v1.1.0-RC.1`][download].

Use `dotsig -v` to find out the version installed on your system.

## Getting started

### Using the pre-built executable

Download the [latest version of `dotsig` here][download] for your system and
install it. Then run one of the commands below to check that your installation
works:

```
dotsig -v
dotsig -h
echo 'Hello, world!' | dotsig
```

### Cross-platform Identities

Identities' private keys are tentatively exported to encrypted DER-encoded files
in the *home folder* of the user running the program, whereas identities' public
keys are tentatively exported to plaintext PEM-encoded files ending with `.pub`.

For Windows users, the *APPDATA* environment variable is used and the files are
stored in `%APPDATA%\.dotsig\`, whereas on Unix systems (including Mac OS), the
identity files are stored in `~/.dotsig`.

:white_check_mark: Note that it is possible to re-use your RSA identity file
(e.g. `-i ~/.ssh/id_rsa`) provided that you are in possession of the passphrase
that unlocks the private key.

### Developers notes

#### Install required dependencies

Following dependencies must be installed on the host system if you want to build
the `dotsig` software from its source code:

- g++-11 or newer (must have support for `-std=c++-20`)
- Python v3.6.10 (`python3 -V`)
- Botan v3.4.0

You will need to install the **Botan** library on your host computer. You can do
this *manually* using the instructions provided in the [Botan build instructions][botan].
The source code repository for Botan can be found on [Github][botan-src].

Note that the current `CMakeLists.txt` *statically links* the `libstdc++` package
and it may be that cmake can't find it due to the restrictive `HINTS` added in the
build instructions.

#### Build the source code

A *CMakeLists.txt* file is provided to overcome the necessity of building the
package. This is currently the preferred method to build the `dotsig` software.

```bash
cd build
cmake ..
cmake --build .
cmake --build . --target install -- DESTDIR=stage
cd stage/usr/local/bin
```

:soon: In the short-to-mid-term future, I plan to enable the installations using
popular package managers including: `apt`, `rpm` and `snap`.

#### Build using Windows

If you are using a Windows operating system, you will need a couple of special
configuration commands after having built the Botan library locally. To start
configuring a Windows build, use:

```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles"
nmake
cmake --build . --target install -- DESTDIR=stage
cd "stage/Program Files (x86)/dotsig/bin"
```

#### Creating installer packages

The `cpack` utility can be used to create installer packages for different OSs.
It is more comfortable for the end-user to receive an installer package rather
than being able to download only the binaries, for Windows it is also necessary
because of the `botan-3.dll` file that is needed to run the software.

To create an installer package *after* having built your source tree with one of
the above commands, and using the corresponding operating system, use:

```bash
# MacOS .sh, .tar.gz, .pkg
cpack -G "STGZ"
cpack -G "TGZ"
cpack -G "productbuild"

# Linux .tar.gz, .deb, .rpm
cpack -G "TGZ"
cpack -G "DEB"
cpack -G "RPM"

# Windows .zip
cpack -G "ZIP"
```

### Installing the user manual

A `man` user manual is available with `docs/dotsig.1`, you can install it on your
system using the `make install-man` command from the root project folder.

You can also directly use this file to open it with man: `man docs/dotsig.1`.

## Examples

By default, this software uses the **ECDSA** standard to sign/verify documents.
You can change the algorithm used between `ecdsa` and `pkcs` using e.g.: `-a pkcs`.

To sign/verify a file with *ECDSA* and your default identity `~/id_ecdsa`, use:

```bash
dotsig path/to/document
dotsig -c path/to/document.sig
```

To sign/verify a *file* with *PKCS* and your default identity `~/id_rsa`, use:
```bash
dotsig path/to/document -a pkcs
dotsig -c path/to/document.sig -a pkcs
```

To sign/verify a *message* with *ECDSA* and your default identity, use:
```bash
echo 'Hello, World!' | dotsig
echo 'Hello, World!' | dotsig -c path/to/signature.sig
```

To sign/verify a *message* with *PKCS* and your default identity, use:
```bash
echo 'Hello, World!' | dotsig -a pkcs
echo 'Hello, World!' | dotsig -c path/to/signature.sig -a pkcs
```

Example of a full-cycle of creation of a digital signature and later
verification of the produced signature file (using STDIN):
```bash
echo 'Hello, World!' | dotsig
echo 'Hello, World!' | dotsig -c stdin.sig
```

## Getting help

Use the following available resources to get help:

- Find the github repository: [evias/dotsig][self].
- Read the reference [documentation][docs].
- Read the manpage: `man dotsig`.
- Read the usage message: `dotsig -h`.
- Find out about changes in the [changelog](./CHANGELOG.md).
- If you found a bug, make sure to [open a new issue][issues].

## License

Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).

Licensed under the [3-Clause BSD License](./LICENSE).

[self]: https://github.com/evias/dotsig
[download]: https://github.com/evias/dotsig/releases/tag/v1.1.0-RC.1
[docs]: https://evias.be/dotsig/v1.1.0/index.html
[issues]: https://github.com/evias/dotsig/issues
[botan-src]: https://github.com/randombit/botan
[botan]: https://botan.randombit.net/handbook/building.html
