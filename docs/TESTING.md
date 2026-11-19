# DukeAI Nexus Testing Guide

## 1. Purpose

This document describes how to build, test, and evaluate DukeAI Nexus.

The project uses CMake for build configuration and CTest for automated testing.

The current prototype has been built and tested in an Android Termux environment.

Other platforms should be tested independently before being described as verified.

## 2. Requirements

Required development tools:

- C++17-compatible compiler.
- CMake.
- pkg-config.
- JsonCpp.
- Git.

On Android, use Termux.

## 3. Termux Setup

Update package information:

```bash
pkg update
pkg upgrade
```

Install dependencies:

```bash
pkg install clang cmake pkg-config jsoncpp git
```

Check installed versions:

```bash
clang++ --version
cmake --version
pkg-config --modversion jsoncpp
git --version
```

## 4. Configure and Build

From the project root:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build
```

## 5. Run Automated Tests

Run all registered tests:

```bash
ctest --test-dir build --output-on-failure
```

The test suite should be reviewed whenever code changes are introduced.

## 6. Run the Retrieval Demonstration

After a successful build:

```bash
./build/dukeai_demo data/demo
```

The demonstration uses English and Kiswahili water-conservation examples.

It retrieves existing passages from source documents.

It does not generate AI answers.

## 7. Current Test Coverage

The verified test suite contains eight registered tests:

1. `ranker_smoke_test`
2. `text_processing_test`
3. `retrieval_test`
4. `document_loader_test`
5. `document_metadata_test`
6. `document_ingestion_test`
7. `document_metadata_loader_test`
8. `retrieval_demo_test`

The project has previously completed a run with eight passing tests and zero failures.

Test results may change as the implementation evolves. Always run the tests against the current source revision.
## 8. Security Test Evidence

The filesystem boundary security milestone and its recorded verification results are documented in:

[SECURITY_TEST_EVIDENCE.md](SECURITY_TEST_EVIDENCE.md)

The evidence includes the implementation scope, test environment, automated test results, reproduction commands, and known security limitations.

The evidence applies to the recorded Android Termux test run and should be updated whenever relevant implementation changes are made.
## 9. Component Testing Areas

### 9.1 Text Processing

Test:

- Tokenisation.
- ASCII lowercase handling.
- Whitespace separation.
- Punctuation separation.
- Feature extraction.

Limitations:

- Full Unicode normalisation is not implemented.
- Language-specific morphological processing is not implemented.

### 9.2 Document Loading

Test:

- Valid local files.
- Missing files.
- Empty files.
- File type checks.
- Bounded reads.
- Oversized files.
- File I/O failures.

### 9.3 Metadata Validation

Test:

- Valid metadata.
- Missing required fields.
- Incorrect field types.
- Unknown fields.
- Duplicate keys.
- Invalid rights combinations.
- Conflicting metadata.

### 9.4 Retrieval

Test:

- Query-term overlap.
- Title overlap.
- Language filtering.
- Candidate selection.
- Deterministic ranking.
- Empty and unmatched queries.
- Source and passage identification.

### 9.5 Energy-Based Ranking

Test:

- Ranker construction.
- Synthetic training fixtures.
- Pairwise margin-training behaviour.
- Deterministic fallback behaviour.

Synthetic tests do not establish real-world ranking effectiveness.

## 10. Code Quality Checks

Before submitting changes:

```bash
git diff --check
```

Review the modified files:

```bash
git status --short
```

Review changes:

```bash
git diff
```

## 11. Future Testing Priorities

Future testing should include:

- Unicode and multilingual text processing.
- Kiswahili-specific linguistic evaluation.
- Larger document collections.
- Independent retrieval benchmarks.
- Held-out evaluation data.
- Memory usage.
- Processing latency.
- Large-file behaviour.
- Android and Linux cross-platform builds.
- Long-running stability tests.
- Security and malformed-input testing.

## 12. Reporting Test Results

Contributors should report:

- Operating system.
- Device or hardware information, where relevant.
- Compiler version.
- CMake version.
- Build result.
- Test result.
- Relevant errors.
- Known limitations.

Do not claim a platform has been verified unless the tests have actually been run on that platform.

## 13. Testing Commitment

DukeAI Nexus aims to maintain reproducible, transparent and independently reviewable testing practices.

**A passing test suite demonstrates tested behaviour, not complete correctness or production readiness.**
