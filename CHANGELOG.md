# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

### Added

- build: add cmake files for cross-platform builds


### Changed

- docs: fix changelog, add reference documentation to readme
- fix: missing includes filesystem and algorithm
- build: removed manual Makefile targets, use cmake instead

## 1.0.0-beta - 2024-05-03

### Added

- Support for ECDSA private- and public keys using PEM encoding.
- Support for PKCS (RSA) private- and public keys using PEM encoding.
- Support for Unix-style command line options with flags and value options.
- Identities abstraction layer (template class) compatible with botan/pk_keys.h.
- Cross-platform terminal (buffered) input management compatible with pipes.
- Cross-platform storage path with Unix' home folders and Windows' APPDATA.
- Support for multiple files inputs, including a mix of .sig and documents.
- Create and verify digital signatures with ECDSA using files and/or stdin.
- Create and verify digital signatures with PKCS using files and/or stdin.

[1.0.0-beta]: https://github.com/evias/dotsig/releases/tag/v1.0.0-beta
