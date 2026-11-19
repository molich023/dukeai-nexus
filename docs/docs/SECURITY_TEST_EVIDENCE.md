# DukeAI Nexus — Security Test Evidence

**Security milestone:** Approved Filesystem Boundary for Document Ingestion  
**Verification date:** 19 November 2026  
**Test environment:** Android Termux  
**Project:** DukeAI Nexus  
**Implementation language:** C++17

## 1. Purpose

This document records the implementation and verification of a filesystem boundary for document and metadata ingestion in DukeAI Nexus.

The objective is to prevent the ingestion coordinator from reading document or metadata files located outside an explicitly approved data directory.

This is an incremental security improvement to the research prototype, not a claim of complete filesystem isolation or production-grade security.

## 2. Security Improvement

The `DocumentIngestion` coordinator now receives an approved data directory.

Before either input file is read, the coordinator:

1. Resolves the approved directory to its canonical filesystem path.
2. Confirms that the directory exists and is a directory.
3. Resolves the requested document and metadata paths.
4. Checks that both resolved paths remain within the approved directory.
5. Rejects paths resolving outside the approved directory.
6. Confirms that both inputs are regular files.

The implementation is designed to reject:

- Absolute paths outside the approved directory.
- Relative directory traversal, including `../` escapes.
- Symbolic links resolving outside the approved directory.

Both paths are checked before either loader reads the corresponding file.

## 3. Files Changed

The implementation involved the following tracked project files:

| File | Change |
|---|---|
| `include/dukeai/document_ingestion.hpp` | Added approved-root configuration |
| `src/documents/document_ingestion.cpp` | Added canonical-path resolution and containment validation |
| `src/demo/main.cpp` | Configured the approved demo data directory |
| `tests/test_document_ingestion.cpp` | Added filesystem boundary tests |

The architecture and testing documentation were also updated to describe the implementation and its limitations.

## 4. Verification Environment

The tests were executed in the project's Android Termux development environment.

The project uses:

- C++17
- CMake
- Clang
- JsonCpp
- CTest

The recorded build directory was:

`~/dukeai-nexus/build`

This evidence establishes a successful test run in Android Termux. It does not establish that Linux desktop or other platforms have independently passed the same tests.

## 5. Automated Test Results

### 5.1 Build

Command:

```bash
cmake -S . -B build
cmake --build build -j2
```

Result:

**PASS — Configuration and compilation completed successfully.**

The library, test executables, and demonstration executable built successfully.

### 5.2 Complete CTest suite

Command:

```bash
ctest --test-dir build --output-on-failure
```

Recorded result:

```text
100% tests passed, 0 tests failed out of 8

Total Test time (real) = 0.12 sec
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

### 5.3 Direct document ingestion security tests

Command:

```bash
./build/dukeai_document_ingestion_tests
```

Recorded result:

```text
PASS: successful ingestion and provenance
PASS: malformed metadata rejection
PASS: missing text file rejection
PASS: duplicate ingestion rejection
PASS: filesystem boundary tests
PASS: all document ingestion tests (29 checks)
```

**Result: 29 checks passed.**

The filesystem boundary test group completed successfully. No symlink-skip notice appeared in the recorded output.

### 5.4 Patch quality check

Command:

```bash
git diff --check
```

Result:

**PASS — No whitespace errors were reported.**

## 6. Demonstration Regression Check

Command:

```bash
./build/dukeai_demo data/demo
```

Recorded behaviour:

- Four passages indexed successfully.
- English query: `water conservation`.
- Kiswahili query: `uhifadhi maji`.
- Relevant English and Kiswahili source passages retrieved.
- Source IDs, titles, language information, and passage IDs displayed.

The demonstration continued to work after the ingestion security changes.

The current retrieval pipeline remains lexical. The ranker remains untrained in the demonstration and uses a deterministic fallback.

The system does not generate natural-language answers.

## 7. Security Limitations

The current implementation uses canonical-path resolution and component-wise directory containment checks.

The following limitations remain:

1. **TOCTOU risk:** a filesystem entry could theoretically change between path validation and file opening.
2. **Standalone loaders:** direct use of `DocumentLoader` or `DocumentMetadataLoader` does not automatically enforce the coordinator's approved-root boundary.
3. **Platform coverage:** the recorded verification was performed in Android Termux; independent Linux verification remains necessary.
4. **Security assurance:** these tests do not constitute a complete security audit, penetration test, or certification.

Potential future hardening includes descriptor-relative file access, such as POSIX `openat`, with appropriate protections against symlink and path replacement attacks.

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
- Twenty-nine passing document ingestion checks.
- Successful English and Kiswahili retrieval demonstration.
- No reported patch whitespace errors.

This milestone provides concrete evidence of an incremental security improvement in DukeAI Nexus.

It does not establish complete filesystem isolation, eliminate all filesystem race conditions, or demonstrate production readiness.

**Evidence status:** Successful Android Termux verification, 19 November 2026.

**Our Languages. Our Heritage. Our Future.**
