<h1 align="center">
  <img src="./dist/citron.svg" width="128" alt=""><br>
  Citron Neo
</h1>

<p align="center">
  An open-source emulator, continuing the Citron codebase.
</p>

<p align="center">
  <a href="LICENSE"><img alt="Licence GPL-3.0-or-later" src="https://img.shields.io/badge/licence-GPL--3.0--or--later-blue"></a>
  <a href="https://discord.gg/7bdSQgQZfu"><img alt="Discord" src="https://img.shields.io/badge/chat-Discord-5865F2?logo=discord&logoColor=white"></a>
</p>

---

## Platforms

| Platform | Frontend | Build notes |
| --- | --- | --- |
| Windows | Qt (`src/citron`) | [docs/BUILDING-CITRON-WINDOWS.md](docs/BUILDING-CITRON-WINDOWS.md) |
| Linux | Qt (`src/citron`), AppImage | [docs/BUILDING-CITRON-LINUX.md](docs/BUILDING-CITRON-LINUX.md) |
| Android (arm64) | `src/android` | Gradle project in `src/android` |
| macOS | Qt (`src/citron`) | — |

## Building

The Linux and Windows guides cover the full pipeline, including the optional
PGO, LTO and BOLT stages. In short:

- **Linux:** `./build-citron-linux.sh setup` once, then
  `./build-citron-linux.sh use --pgo none --lto full` for a build without the
  profiling round. The [Linux guide](docs/BUILDING-CITRON-LINUX.md) covers the
  PGO stages and where each binary lands.
- **Windows:** clang-cl natively, or cross-compiled from Linux with
  llvm-mingw. See the [Windows guide](docs/BUILDING-CITRON-WINDOWS.md).
- **Android:** open `src/android` in Android Studio, or build with Gradle from
  that folder.

Library dependencies are fetched at configure time by CPM, so you do not need
to initialise submodules first.

## Contributing

Issues and pull requests are welcome. Development and maintenance happen on
[Discord](https://discord.gg/7bdSQgQZfu), which is the place to ask before
starting something large.

## Licence

Citron Neo is licensed under the
[GNU General Public License v3.0 or later](LICENSE). Individual files carry
SPDX headers, and the licences of third-party components are in
[`LICENSES/`](LICENSES/).
