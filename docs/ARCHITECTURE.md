# DukeAI Nexus Architecture

## 1. Overview

DukeAI Nexus is an offline-first, open-source African AI research project.

The current implementation is a C++17 research prototype focused on local document ingestion, metadata validation, text processing, and lexical retrieval.

The initial demonstration uses English and Kiswahili civic and scientific learning material.

The architecture is designed to support future research into resource-efficient retrieval, energy-based ranking, multilingual knowledge systems, and locally accessible AI.

## 2. Design Principles

The project follows these architectural principles:

- Offline-first operation.
- Local document processing.
- Minimal external dependencies.
- Source provenance and traceability.
- Explicit document metadata.
- Resource-aware processing.
- Modular C++ implementation.
- Testable and reproducible components.
- Filesystem boundary enforcement at the document ingestion interface.
- Clear separation between implemented functionality and research proposals.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Programming language | C++17 |
| Build system | CMake |
| Compiler | Clang or GCC |
| JSON processing | JsonCpp |
| Testing | CTest |
| Initial platforms | Android Termux and Linux |

## 4. High-Level Architecture

The current processing pipeline is:

```text
Local Documents
      |
      v
Approved Data Directory
      |
      v
Filesystem Boundary Validation
      |
      v
Document Loading
      |
      v
Document Metadata
      |
      v
Metadata Validation
      |
      v
Document Ingestion
      |
      v
Text Processing
      |
      v
Feature Extraction
      |
      v
Lexical Retrieval
      |
      v
Deterministic Ranking
      |
      v
Retrieved Passages
      |
      v
Source and Provenance Information
```

This pipeline retrieves existing source material. It does not generate natural-language answers.

## 5. Implemented Components

### 5.1 Document Loading

The document-loading component reads local plain-text documents.

Current safeguards include:

- Regular-file checks.
- Non-empty document checks.
- Bounded document reading.
- Default maximum document size.
- File I/O error handling.
- Checks for unexpected file growth during reading.

The current loader does not provide comprehensive UTF-8 validation.

### 5.2 Document Metadata

Structured JSON metadata describes documents and their associated information.

Metadata validation checks:

- Required fields.
- Field types.
- Unknown fields.
- Duplicate JSON keys.
- Invalid or conflicting metadata.
- Rights and permission consistency.

The metadata loader uses a bounded input size and rejects unsupported JSON constructs.

### 5.3 Text Processing

The text-processing component provides basic tokenisation and feature extraction.

Current tokenisation behaviour includes:

- Lowercase conversion for ASCII characters.
- Splitting on whitespace and punctuation.
- Basic token processing.
- Preservation of UTF-8 bytes without full Unicode normalisation.

This is not a complete multilingual linguistic processing system.

### 5.4 Document Ingestion and Filesystem Boundary

Document ingestion connects local document loading, metadata processing, validation, and preparation for retrieval.

The ingestion coordinator now requires an approved data directory.

Before either document or metadata content is read, the coordinator:

- Resolves the approved directory to its canonical filesystem path.
- Confirms that the approved path is an existing directory.
- Resolves both requested input paths.
- Checks that both resolved paths remain within the approved directory.
- Rejects paths that resolve outside the approved directory.
- Confirms that both inputs refer to regular files.

The boundary is designed to reject:

- Absolute paths outside the approved directory.
- Relative directory traversal, including `../` escapes.
- Symbolic links that resolve outside the approved directory.

Both paths are checked before either loader reads the corresponding file.

The existing document and metadata loaders retain their standalone interfaces. The filesystem boundary described here is enforced by the `DocumentIngestion` coordinator; direct use of the standalone loaders does not automatically inherit this restriction.

**Security limitation:** canonical-path validation does not eliminate time-of-check/time-of-use (TOCTOU) races. A filesystem entry could theoretically change between path validation and file opening. Further hardening may use descriptor-relative operations such as POSIX `openat` and appropriate symlink protections.

This improvement is a tested filesystem boundary for the ingestion interface, not a claim of complete filesystem isolation or production-grade security certification.

### 5.5 Lexical Retrieval

The retrieval component identifies candidate passages using lexical overlap.

The current implementation considers:

- Query-term overlap.
- Title overlap.
- Exact normalised language-tag matching.
- Candidate filtering.
- Deterministic ordering.

Candidates without query-term overlap are filtered out.

The current retrieval method is lexical. It is not semantic or embedding-based retrieval.

### 5.6 Energy-Based Ranking

The project includes a linear energy-ranker component with pairwise margin-training support.

The ranker has been tested using a synthetic fixture.

The current demonstration uses an untrained ranker with a deterministic fallback.

No claim is made that the current ranker has demonstrated improved real-world retrieval quality.

## 6. Data Flow and Provenance

The system is intended to preserve information about the origin of retrieved content.

A document's metadata and source information should remain associated with its content throughout ingestion and retrieval.

The objective is to make retrieved passages traceable to their underlying source rather than presenting them as independently generated knowledge.

## 7. Current Limitations

The current prototype does not implement:

- Natural-language answer generation.
- A conversational chatbot.
- Translation.
- Semantic or vector retrieval.
- Full African language processing.
- Speech recognition or synthesis.
- Multimodal document understanding.
- JEPA or world-model architectures.
- A production Android or Linux graphical interface.
- Complete filesystem sandboxing.
- Elimination of filesystem TOCTOU races.
- Production-grade security certification.

## 8. Planned Architecture Extensions

Future research may introduce:

1. Unicode-aware text processing.
2. Improved multilingual tokenisation.
3. Embedding-based retrieval.
4. Independent retrieval evaluation.
5. Improved energy-based ranking.
6. Local language generation.
7. Small-model benchmarking.
8. User interfaces for Android and Linux.
9. Community-reviewed language resources.
10. Multimodal knowledge processing.
11. Descriptor-relative filesystem access and stronger protection against filesystem race conditions.
12. Expanded adversarial and security testing.

These are research directions, not current capabilities.

## 9. Testing and Reproducibility

The project uses CMake and CTest.

The standard build and test commands are:

```bash
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
```

The document ingestion security tests can also be executed directly:

```bash
./build/dukeai_document_ingestion_tests
```

The latest recorded verification completed with:

- Successful compilation.
- Eight of eight registered CTest tests passing.
- Twenty-nine document ingestion checks passing.
- A successful English and Kiswahili retrieval demonstration.
- A clean `git diff --check`.

These results describe the tested source revision and environment, not a guarantee of correctness on every platform.

See [TESTING.md](TESTING.md) for testing details.

## 10. Architectural Direction

DukeAI Nexus aims to develop a modular, transparent, offline-first architecture that can be evaluated on resource-constrained devices.

The architecture will evolve through measured experiments rather than assumptions about model performance or system capabilities.

**Our Languages. Our Heritage. Our Future.**
