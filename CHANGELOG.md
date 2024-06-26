# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## v1.1.0-RC.1 - 2024-05-13

### Added

- feat: add openpgp-compatible RSA signatures
- feat: add OpenPGP::Identity template and support for RSA, DSA, ECDSA and EdDSA
- feat: add get_option implementation with default value
- feat: enables openpgp, openpgp:dsa, openpgp:ecdsa and openpgp:eddsa
- options: accepts -a openpgp:{type} with values: rsa, dsa, ecdsa and eddsa

### Changed

- core: algorithm "pkcs" now uses PKCS1v15(SHA-256) signature padding scheme
- build: more package metadata and set install folder permissions
- docs: add correct identity filenames
- fix: update docs and filenames for openpgp
- docs: fix license text appearing in docs, add missing variable docs

## v1.0.0-beta.4 - 2024-05-09

### Added

- build: add installers config (cpack) and fix windows builds
- docs: add Windows build instructions
- feat: use std::filesystem::path for storage path building

### Changed

- fix: add binary-mode switch for DataSource_Stream imports
- fix: use std::filesystem for get_storage_path() (windows fix)

## v1.0.0-beta.3 - 2024-05-08 

### Added

- build: add cmake files for cross-platform builds
- docs: update building instructions

### Changed

- docs: fix changelog, add reference documentation to readme
- fix: missing includes filesystem and algorithm
- build: removed manual Makefile targets, use cmake instead
- build: use statically linked botan and libstdc++ (temporary)

## v1.0.0-beta - 2024-05-03

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


## All Versions

- v1.0.0-beta: https://github.com/evias/dotsig/releases/tag/v1.0.0-beta
- v1.0.0-beta.3: https://github.com/evias/dotsig/releases/tag/v1.0.0-beta.3
- v1.0.0-beta.4: https://github.com/evias/dotsig/releases/tag/v1.0.0-beta.4
- v1.1.0-RC.1: https://github.com/evias/dotsig/releases/tag/v1.1.0-RC.1
