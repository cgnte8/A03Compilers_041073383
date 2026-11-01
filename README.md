# Assignment 1 [Cell] — Student Model Folder

Matches **Week 2-3** in the course's Lab Instructions (`AC-262S-Compilers-LABsOutline.pdf`):
the Cell stage encodes/decodes a source file with a Vigenère-style cipher,
so every later stage only ever reads *encoded* input.

## What's in this folder

| File | Purpose |
|---|---|
| `Biology.h` / `Biology.c` | Shared types/constants + the dispatcher/menu, trimmed to option **[1] Cell** only |
| `Step1Cell.h` / `Step1Cell.c` | The Cell implementation (encode/decode) |
| `Main1Cell.c` | Cell's command-line entry point |
| `InputExample1.txt` | The Hello World source program (the only input this folder needs to handle) |
| `logo.txt` | ASCII-art banner `printLogo()` displays |
| `run_a1.bat` / `run_a1.sh` | Builds `biology` and demos encode → decode → byte-for-byte verification |

## What's deliberately left out

- **Tissue/Organ/System/Body** (`Step2Tissue.c` onward) — added in later assignments' folders.
- **Self-test suites** (`Test1Cell.c`, menu options 6-0) — not required to demo Cell.
- **`Util.c` / Cdoc**, **`ErrorInput*.txt`**, and **`InputExample2.txt`-`InputExample6.txt`** —
  this folder is scoped to the one demo case (`InputExample1.txt`, "Hello World").

## Build & run

```
run_a1.bat        (Windows -- requires gcc on PATH, e.g. via MSYS2 or MinGW-w64)
./run_a1.sh       (Linux/macOS/WSL)
```

Or manually:
```
gcc -std=gnu99 -fcommon -w -o biology Biology.c Main1Cell.c Step1Cell.c -lm
./biology 1 1 InputExample1.txt CODED.TXT     # encode
./biology 1 0 CODED.TXT DECODED.TXT           # decode
```
`DECODED.TXT` should be byte-for-byte identical to `InputExample1.txt`. Running
`biology` with no arguments opens the interactive menu (option `1`, then the
same three prompts).

## Sample demo (per the Lab Instructions, Week 3)

```
biology 1 1 InputExample1.txt CODED.TXT
```
prints the BioCompiler banner, `Operation '1' completed successfully.`, and
the encoded (unreadable) contents of `CODED.TXT`.
