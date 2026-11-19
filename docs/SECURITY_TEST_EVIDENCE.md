# DukeAI Nexus — Security Test Evidence

**Security milestone:** Approved Filesystem Boundary for Document Ingestion  
**Verification date:** 19 November 2026  
**Test environment:** Android Termux  
**Implementation language:** C++17  
**Project:** DukeAI Nexus

## 1. Purpose

This document records the implementation and verification of a filesystem boundary for document and metadata ingestion in DukeAI Nexus.

The objective is to prevent the `DocumentIngestion` coordinator from reading document or metadata files located outside an explicitly approved data directory.

This is an incremental security improvement to the research prototype. It is not a claim of complete filesystem isolation, a complete sandbox, or production-grade security.

## 2. Security Improvement

The `DocumentIngestion` coordinator receives an approved data directory.

Before either input file is read, the coordinator:

1. Resolves the approved directory to its canonical filesystem path.
2. Confirms that the directory exists and is a directory.
3. Resolves the requested document path.
4. Resolves the requested metadata path.
5. Checks that both resolved paths remain within the approved directory.
6. Confirms that both inputs are regular files.
7. Loads and validates metadata.
8. Loads the document using the validated metadata identity.
9. Passes the validated document and metadata to the retriever.

The implementation is designed to reject:

- Absolute paths outside the approved directory.
- Relative directory traversal, including `../` escapes.
- Symbolic links resolving outside the approved directory.
- Missing or non-regular input files.

Both input paths are checked before either loader reads a file.

## 3. Files Changed

The implementation involves these project files:

| File | Change |
|---|---|
| `include/dukeai/document_ingestion.hpp` | Added approved-root configuration to the ingestion coordinator |
| `src/documents/document_ingestion.cpp` | Added canonical-path resolution and containment validation |
| `src/demo/main.cpp` | Configured the approved demo data directory |
| `tests/test_document_ingestion.cpp` | Added filesystem-boundary tests |

The architecture and testing documentation describe the implementation and its limitations.

## 4. Verification Environment

The tests were executed in the project's Android Termux development environment.

The project uses:

- C++17.
- Clang.
- CMake.
- JsonCpp.
- CTest.

The recorded project directory was:

```text
~/dukeai-nexus
```

The build directory was:

```text
~/dukeai-nexus/build
```

This evidence establishes a successful test run in Android Termux. It does not establish that Linux desktop or other platforms have independently passed the same tests.

## 5. Automated Test Results

### 5.1 Build

Commands:

```bash
cmake -S . -B build
cmake --build build -j2
```

The recorded rebuild completed successfully. All reported project targets were built successfully, including the energy library, test executables, and demonstration executable.

**Result: PASS — Build completed successfully.**

### 5.2 Complete CTest suite

Command:

```bash
ctest --test-dir build --output-on-failure
```

Recorded result on 19 November 2026:

```text
100% tests passed, 0 tests failed out of 8
Total Test time (real) = 0.10 sec
```

The eight registered tests were:

1. `ranker_smoke_test`
2. `text_processing_test`
3. `retrieval_test`
4. `document_loader_test`
5. `document_metadata_test`
6. `document_ingestion_test`
7. `document_metadata_loader_test`
8. `retrieval_demo_test`

**Result: PASS — 8 of 8 registered tests passed.**

### 5.3 Direct document-ingestion security tests

Command:

```bash
./build/dukeai_document_ingestion_tests
```

Recorded output:

```text
PASS: successful ingestion and provenance
PASS: malformed metadata rejection
PASS: missing text file rejection
PASS: duplicate ingestion rejection
PASS: filesystem boundary tests
PASS: all document ingestion tests (29 checks)
```

**Result: PASS — 29 checks passed.**

The filesystem-boundary test group completed successfully. No symlink-skip notice appeared in the recorded output.

The tests cover external document and metadata paths, relative traversal, external symbolic links where supported, index integrity after rejected inputs, and valid in-root ingestion.

### 5.4 Patch quality check

Command:

```bash
git diff --check
```

**Result: PASS — No whitespace errors were reported.**

## 6. Demonstration Regression Check

Command:

```bash
./build/dukeai_demo data/demo
```

The previously recorded demonstration completed successfully after the ingestion changes.

Recorded behaviour:

- Four passages indexed successfully.
- English query: `water conservation`.
- Kiswahili query: `uhifadhi maji`.
- Relevant English and Kiswahili source passages retrieved.
- Source IDs, titles, language information, and passage IDs displayed.

The current retrieval pipeline remains lexical. The ranker is untrained in the demonstration and uses deterministic fallback behaviour.

The system does not generate natural-language answers.

This demonstration is a regression check of the existing retrieval workflow, not an independent language-quality or ranking benchmark.

## 7. Security Limitations

The current implementation uses canonical-path resolution and component-wise directory containment checks.

The following limitations remain:

1. **TOCTOU risk:** A filesystem entry could theoretically change between path validation and file opening.
2. **Standalone loaders:** Direct use of `DocumentLoader` or `DocumentMetadataLoader` does not automatically enforce the coordinator's approved-root boundary.
3. **Platform coverage:** The recorded verification was performed in Android Termux. Independent Linux verification remains necessary.
4. **Security assurance:** These tests do not constitute a complete security audit, penetration test, or certification.

Potential future hardening includes descriptor-relative file access, such as POSIX `openat`, with appropriate protections against symlink and path-replacement attacks.

## 8. Reproduction Instructions

From the repository root:

```bash
cmake -S . -B build

cmake --build build -j2

ctest --test-dir build --output-on-failure

./build/dukeai_document_ingestion_tests

./build/dukeai_demo data/demo

git diff --check
```

Contributors should record the operating system, compiler, build outcome, test outcome, and any skipped tests when reproducing these results.

## 9. Conclusion

The approved filesystem boundary for document ingestion has been implemented and tested in the Android Termux environment.

The recorded verification demonstrates:

- Successful project compilation.
- Eight passing registered automated tests.
- Twenty-nine passing document-ingestion checks.
- Successful English and Kiswahili retrieval demonstration.
- No reported patch whitespace errors.

This milestone provides concrete evidence of an incremental security improvement in DukeAI Nexus.

It does not establish complete filesystem isolation, eliminate all filesystem race conditions, or demonstrate production readiness.

**Evidence status:** Successful Android Termux verification, 19 November 2026.

**Our Languages. Our Heritage. Our Future.**
