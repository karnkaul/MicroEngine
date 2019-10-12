## Untitled Game
***Powered by `MicroEngine`***

### Building
1. Run CMake on the root directory and generate a project ("out" is ignored in source control)
    1. This will download SFML once
1. Open project in IDE (if applicable) and build
    1. This will build SFML once (until project is cleaned): that throws a few warnings; please ignore
1. Debug/Run the built executable(s) in `Build`
    1. For IDEs like Visual Studio, set `MicroEngine` as the debug target / startup project, but always build `ALL_BUILD` in order to auto-generate CMake when its cache is out-of-date.

For more details please refer to [Build Setup](https://github.com/karnkaul/MicroEngine/wiki/Build-Setup)
