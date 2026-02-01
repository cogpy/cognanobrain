# Phase 3: Platform Extensions Walkthrough

## Summary

Implemented Python bindings and WebAssembly support for NanoBrain, expanding accessibility beyond native C++.

---

## 3.1 Python Bindings ✅

| File | Purpose |
|------|---------|
| [bindings.cpp](file:///e:/antg/nanob/cognanobrain/python/bindings.cpp) | pybind11 C++ bindings |
| [setup.py](file:///e:/antg/nanob/cognanobrain/python/setup.py) | pip-installable setup |
| [pyproject.toml](file:///e:/antg/nanob/cognanobrain/python/pyproject.toml) | Modern Python packaging |
| [nanobrain/__init__.py](file:///e:/antg/nanob/cognanobrain/python/nanobrain/__init__.py) | Package entrypoint |
| [tests/test_nanobrain.py](file:///e:/antg/nanob/cognanobrain/python/tests/test_nanobrain.py) | pytest suite |
| [examples/quickstart.py](file:///e:/antg/nanob/cognanobrain/python/examples/quickstart.py) | Usage demo |

__Build:__ `cd python && pip install -e .`

---

## 3.2 WebAssembly ✅

| File | Purpose |
|------|---------|
| [CMakeLists.wasm.txt](file:///e:/antg/nanob/cognanobrain/src/cpp/CMakeLists.wasm.txt) | Emscripten config |
| [wasm_bindings.cpp](file:///e:/antg/nanob/cognanobrain/src/cpp/wasm_bindings.cpp) | embind bindings |
| [wasm/index.html](file:///e:/antg/nanob/cognanobrain/src/cpp/wasm/index.html) | Browser demo |
| [wasm/README.md](file:///e:/antg/nanob/cognanobrain/src/cpp/wasm/README.md) | Build instructions |

__Build:__

```bash
emcmake cmake -B build-wasm
cmake --build build-wasm
```

__Exposed Classes:__ `UnifiedKernel`, `TimeCrystalKernel`, `TCTransform`

---

## Next: GPU Acceleration

- CUDA backend for NVIDIA
- Metal backend for macOS
