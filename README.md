# dotsig: digital signatures on-the-fly

The [dotsig][self] software lets you create and verify digital signatures
using RSA cryptography (PKCS1 v1.5), PGP and elliptic-curve cryptography (ECDSA).

This software is released under the [3-Clause BSD License](./LICENSE).

[![Latest release](https://img.shields.io/github/v/release/evias/dotsig)](#releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)][issues]
[![3-Clause BSD License](https://img.shields.io/github/license/evias/dotsig)](./LICENSE)

## Releases

The `dotsig` software is currently under active development and frequent changes
to the source code *are to be expected* until the software reaches a stable tag.

The latest release as of May 2nd 2024 is: `v1.0.0-beta`.

Use `dotsig -v` to find out the version installed on your system.

## Getting started

### Build and install the software

A *Makefile* is provided to overcome the necessity of building the package. This
is currently the preferred method to install the `dotsig` software.

```bash
make
make install
```

:soon: In the short-to-mid-term future, I plan to enable the installation using popular
package managers including: `apt`, `rpm` and `snap`.

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

## Examples

By default, this software uses the **ECDSA** standard to sign/verify documents.

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
[docs]: https://dotsig.evi.as
[issues]: https://github.com/evias/dotsig/issues