# DukeAI Nexus Testing Strategy

**Last verified:** 19 November 2026  
**Verified environment:** Android Termux

## 1. Purpose

This document describes how to configure, build, test, and evaluate DukeAI Nexus.

The project uses CMake for build configuration and CTest for automated testing.

The current C++17 prototype has been built and tested in an Android Termux environment. Other platforms must be tested independently before being described as verified.

## 2. Development Requirements

Required tools:

- C++17-compatible compiler.
- CMake.
- pkg-config.
- JsonCpp.
- Git.

On Android, the development environment is Termux.

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

Build:

```bash
cmake --build build -j2
```

A successful build should complete without compiler or linker errors.

## 5. Run Automated Tests

Run all registered tests:

```bash
ctest --test-dir build --output-on-failure
```

The test suite should be run against the current source revision after relevant code changes.

List registered tests:

```bash
ctest --test-dir build -N
```

## 6. Run the Retrieval Demonstration

After a successful build:

```bash
./build/dukeai_demo data/demo
```

The demonstration uses developer-authored English and Kiswahili water-conservation examples.

It demonstrates local document ingestion, passage retrieval, and source provenance.

It does not generate AI answers or translate documents.

The current demonstration uses lexical candidate selection and an untrained ranker with deterministic fallback behaviour.

The sample language material has not received independent linguistic review. Its metadata records review as pending and rights as unverified.

## 7. Current Automated Test Coverage

The verified CTest suite contains eight registered tests:

1. `ranker_smoke_test`
   - Energy calculation.
   - Pairwise margin-ranking training.
   - Synthetic training fixture.

2. `text_processing_test`
   - ASCII case normalisation.
   - Tokenisation.
   - Repeated terms.
   - Document and title overlap features.
   - Language-tag matching.
   - Empty-input handling.

3. `retrieval_test`
   - Paragraph-based segmentation.
   - Lexical candidate retrieval.
   - Source and passage identification.
   - File path and file-size provenance.
   - Duplicate-document rejection.
   - Metadata-aware ingestion.

4. `document_loader_test`
   - Local text-document loading.
   - Input validation and rejection cases.

5. `document_metadata_test`
   - Structured metadata validation.
   - Required-field and consistency checks.

6. `document_ingestion_test`
   - Successful ingestion.
   - Malformed metadata rejection.
   - Missing text-file rejection.
   - Duplicate-source rejection.
   - Index integrity after rejected ingestion.
   - Filesystem-boundary validation.

7. `document_metadata_loader_test`
   - JSON metadata parsing and validation.

8. `retrieval_demo_test`
   - End-to-end demonstration using two developer-authored sample documents.
   - English and Kiswahili lexical queries.
   - Passage retrieval and source provenance.

## 8. Latest Verified Test Results

**Verification date:** 19 November 2026  
**Environment:** Android Termux

The project was rebuilt and tested against the current local source.

| Verification | Result |
|---|---|
| CMake build | Successful |
| Registered CTest tests | 8 passed |
| CTest failures | 0 |
| Direct ingestion security checks | 29 passed |
| Total CTest duration | 0.10 seconds |
| `git diff --check` | Clean |

The direct security test reported:

```text
PASS: successful ingestion and provenance
PASS: malformed metadata rejection
PASS: missing text file rejection
PASS: duplicate ingestion rejection
PASS: filesystem boundary tests
PASS: all document ingestion tests (29 checks)
```

The complete CTest suite reported:

```text
100% tests passed, 0 tests failed out of 8
Total Test time (real) = 0.10 sec
```

These results describe the tested source revision and Termux environment. They are not independent production benchmarks or a guarantee of correctness on other platforms.

## 9. Filesystem Boundary Security Testing

The `DocumentIngestion` coordinator enforces an approved filesystem directory for document and metadata inputs.

The direct test command is:

```bash
./build/dukeai_document_ingestion_tests
```

The security test group covers:

- Successful ingestion from the approved directory.
- Rejection of absolute document paths outside the approved directory.
- Rejection of absolute metadata paths outside the approved directory.
- Rejection of relative directory traversal for document paths.
- Rejection of relative directory traversal for metadata paths.
- Rejection of symbolic links resolving outside the approved directory, where symlink creation is supported.
- Confirmation that rejected inputs do not add passages to the retriever.
- Successful ingestion of valid files inside the approved directory.

The implementation uses canonical-path resolution and component-wise directory containment checks.

**Important limitations:**

- The boundary is enforced through `DocumentIngestion`, not automatically by every standalone loader call.
- Canonical-path checks do not eliminate TOCTOU races.
- A successful test run is not a complete security audit or certification.
- Filesystem behaviour should be verified independently on each supported operating system.

See [SECURITY_TEST_EVIDENCE.md](SECURITY_TEST_EVIDENCE.md) for the detailed milestone record.

## 10. Demonstration Limitations

The current demonstration uses:

- Two developer-authored sample documents.
- English and Kiswahili language labels.
- Paragraph-based passage segmentation.
- Lexical candidate selection.
- Source and passage provenance.

The ranker is untrained during the demonstration and uses deterministic fallback behaviour.

The demonstration does not establish trained ranking performance, semantic retrieval quality, or language-model capabilities.

## 11. Current Testing Limitations

The current testing record does not establish:

- An independently curated and validated English/Kiswahili corpus.
- Independent held-out retrieval benchmarks.
- Demonstrated semantic or vector retrieval.
- Stemming or morphological analysis.
- Full Unicode normalisation.
- Machine translation quality.
- Natural-language answer-generation quality.
- Production model serialization.
- A production Android or Linux graphical interface.
- Independent Linux build and test results.
- Production-scale memory or latency performance.
- Complete security assurance.

## 12. Future Testing Priorities

Future testing should include:

- Human review of language samples.
- Rights and licence verification for external resources.
- Held-out retrieval evaluation.
- Retrieval precision and recall measurements.
- Ranking evaluation beyond synthetic training fixtures.
- Unicode and multilingual text-processing tests.
- Kiswahili-specific linguistic evaluation.
- Larger document collections.
- Android and Linux cross-platform builds.
- Memory and latency measurements.
- Large-file and long-running stability tests.
- Independent security and privacy review.
- Additional malformed-input and adversarial testing.
- Filesystem race-condition testing.
- Descriptor-relative file access tests.
- Adversarial symlink and path-replacement scenarios.

Future evaluation goals must not be presented as completed capabilities.

## 13. Code Quality and Change Review

Before submitting changes, run:

```bash
git diff --check
```

Review the working tree:

```bash
git status --short
```

Review tracked changes:

```bash
git diff
```

Review untracked files separately because `git diff` does not display their contents.

Do not stage, commit, or submit unrelated files as part of a security change.

## 14. Reporting Test Results

Contributors should report:

- Operating system.
- Device or hardware information, where relevant.
- Compiler version.
- CMake version.
- Build result.
- Test result.
- Any skipped tests.
- Relevant errors.
- Known limitations.

Do not claim a platform has been verified unless the tests have actually been run on that platform.

Distinguish synthetic fixtures from independent datasets, lexical retrieval from semantic retrieval, and source-text retrieval from generated answers.

Preserve document provenance and rights status. Do not claim community validation without evidence.

## 15. Testing Commitment

DukeAI Nexus aims to maintain reproducible, transparent, and independently reviewable testing practices.

A passing test suite demonstrates tested behaviour, not complete correctness or production readiness.

**Our Languages. Our Heritage. Our Future.**
