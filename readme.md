# exqudens-cpp-usb

##### how-to-build

```
conan remove --force "$(basename "$(pwd)")"
git clean -xdf
cmake --preset <name>
cmake --build --preset <name>
```

##### how-to-export

```
conan remove --force "$(basename "$(pwd)")"
git clean -xdf
cmake --preset <name>
cmake --build --preset <name> --target conan-export
```

##### how-to-export-all

```
conan remove --force "$(basename "$(pwd)")"
git clean -xdf
cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"
cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target conan-export || exit 255"
```
