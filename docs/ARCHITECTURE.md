# DukeAI Nexus Architecture

**Project status:** Working C++17 local retrieval and energy-ranking research prototype  
**Verified development environment:** Android Termux  
**Initial demonstration languages:** English and Kiswahili

## 1. Overview

DukeAI Nexus is an offline-first, open-source African AI research project. Its initial use case is a local civic and scientific knowledge assistant for Android and Linux.

The current implementation focuses on local document ingestion, metadata validation, text processing, lexical retrieval, and an experimental energy-based ranking component.

The prototype retrieves existing passages from local source documents. It does not currently generate natural-language answers, translate documents, or operate as a general-purpose conversational assistant.

The longer-term architecture is intended to support resource-efficient retrieval, multilingual knowledge systems, locally accessible AI, and community-centred African language research.

## 2. Design Principles

The project follows these principles:

- Offline-first operation.
- Local document processing.
- Minimal external dependencies.
- Source provenance and traceability.
- Explicit document metadata.
- Resource-aware processing.
- Modular C++ implementation.
- Testable and reproducible components.
- Filesystem boundary enforcement at the document-ingestion interface.
- Clear separation between implemented functionality and planned research.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Programming language | C++17 |
| Build system | CMake |
| Compiler | Clang or GCC |
| JSON processing | JsonCpp |
| Testing | CTest |
| Initial development environment | Android Termux |
| Intended platforms | Android and Linux |

Linux remains a target platform, but it must be independently tested before being described as verified.

## 4. High-Level Architecture

The current processing pipeline is:

```text
Local Documents and Metadata
             |
             v
Approved Data Directory
             |
             v
Filesystem Boundary Validation
             |
             v
Document and Metadata Loading
             |
             v
Metadata Validation
             |
             v
Unified Document Ingestion
             |
             v
Text Processing and Segmentation
             |
             v
Lexical Candidate Retrieval
             |
             v
Energy Scoring and Deterministic Ordering
             |
             v
Retrieved Passages
             |
             v
Source and Provenance Information
```

The pipeline retrieves existing source material. It does not generate natural-language answers.

## 5. Implemented Components

### 5.1 Text Processing

The text-processing component provides basic tokenisation and feature extraction.

Current behaviour includes:

- ASCII lowercase normalisation.
- Tokenisation using whitespace and punctuation boundaries.
- Preservation of non-ASCII UTF-8 bytes.
- Query-term overlap with passage text.
- Query-term overlap with passage titles.
- Exact normalised language-tag matching.

Limitations:

- No stemming or morphological analysis.
- No full Unicode normalisation.
- No comprehensive language-specific tokenisation.
- No semantic embeddings.

This is basic lexical processing, not a complete multilingual linguistic system.

### 5.2 Energy-Based Ranking

The project includes a linear energy-based candidate ranker.

Current implementation includes:

- Three input features.
- Energy calculation.
- Pairwise margin-ranking training support.
- A synthetic training fixture used by the ranker smoke test.

Lower energy represents a preferred candidate.

The synthetic training fixture does not establish performance on independent or real-world datasets.

The standalone retrieval demonstration uses an untrained ranker with deterministic fallback behaviour. Its observed ordering must not be presented as evidence of learned ranking effectiveness.

### 5.3 Local Document Loading

The document loader reads local plain-text documents.

Implemented safeguards include:

- Regular-file checks.
- Empty-document checks.
- Bounded document reading.
- Configurable maximum file size.
- File extension and file-type checks.
- File I/O error handling.
- Checks for unexpected file growth during reading.
- File path and byte-size provenance.

The current C++ loader supports plain text. PDF extraction is not implemented.

The loader does not provide comprehensive UTF-8 validation.

### 5.4 Document Metadata

Structured JSON metadata describes documents and their associated information.

Metadata processing uses JsonCpp and includes:

- Required-field validation.
- Field-type checks.
- Rejection of unknown fields.
- Duplicate JSON-key detection.
- Invalid or conflicting metadata rejection.
- Basic rights-status validation.
- Source ID, title, and language consistency checks.
- Bounded metadata input.

Rights and permission metadata must not be interpreted as proof that a source's ownership or licence has been independently verified.

### 5.5 Document Ingestion and Filesystem Boundary

The `DocumentIngestion` coordinator connects document loading, metadata loading, validation, and indexing.

The coordinator requires an approved data directory. The approved directory must already exist and refer to a directory.

Before either loader reads its input, the coordinator:

1. Resolves the approved directory to its canonical filesystem path.
2. Resolves the requested document path.
3. Resolves the requested metadata path.
4. Checks that both resolved paths remain within the approved directory.
5. Confirms that both inputs are regular files.
6. Loads and validates metadata.
7. Loads the document using the validated metadata identity.
8. Passes the validated document and metadata to the retriever.

The boundary is designed to reject:

- Absolute paths outside the approved directory.
- Relative directory traversal, including `../` escapes.
- Symbolic links that resolve outside the approved directory.
- Missing or non-regular input files.

Both input paths are checked before either loader reads a file.

The filesystem boundary is enforced through `DocumentIngestion`. Direct use of the standalone `DocumentLoader` or `DocumentMetadataLoader` does not automatically inherit the coordinator's approved-root restriction.

**Security limitation:** canonical-path validation does not eliminate time-of-check/time-of-use (TOCTOU) races. A filesystem entry could theoretically change between path validation and file opening.

Further hardening may use descriptor-relative operations such as POSIX `openat`, with appropriate symlink protections and platform-specific testing.

This is a tested filesystem boundary for the ingestion interface, not complete filesystem isolation, a complete sandbox, or production-grade security certification.

### 5.6 Retrieval

The retrieval component:

- Segments documents into passages.
- Preserves source and passage identifiers.
- Performs lexical candidate filtering.
- Calculates retrieval features.
- Uses energy scores for candidate ordering.
- Applies deterministic tie-breaking.
- Supports configurable result limits.
- Preserves document provenance.

Candidates without query-term overlap are filtered out.

The current method is lexical retrieval. It is not semantic or vector retrieval.

### 5.7 Unified Indexing and Provenance

The ingestion process associates document content with validated metadata.

The system preserves source information, including:

- Source ID.
- Document title.
- Language.
- Passage ID.
- Original file path.
- File size.

Invalid or duplicate ingestion is rejected without partially modifying the retrieval index.

Retrieved documents remain untrusted data. They must not be treated as executable instructions or trusted system directives.

## 6. Current Demonstration

The current demonstration uses developer-authored English and Kiswahili water-conservation examples.

It demonstrates:

- Local document and metadata loading.
- Passage indexing.
- English and Kiswahili lexical queries.
- Source and passage identification.
- Retrieval provenance.

The demonstration explicitly reports lexical candidate selection, an untrained ranker with deterministic fallback, and no answer generation.

The sample language material has not received independent linguistic review. Its metadata records review as pending and rights as unverified.

## 7. Current Limitations

The current prototype does not implement:

- Natural-language answer generation.
- A conversational chatbot.
- General-purpose machine translation.
- Semantic or vector retrieval.
- Full African language and dialect processing.
- Speech recognition or speech synthesis.
- Multimodal document understanding.
- JEPA or learned world-model architectures.
- A production Android or Linux graphical interface.
- Complete filesystem sandboxing.
- Elimination of filesystem TOCTOU races.
- Production-grade security certification.
- Independent production-scale performance evaluation.

## 8. Planned Architecture Extensions

Future research may introduce:

1. Unicode-aware text processing.
2. Improved multilingual tokenisation.
3. Kiswahili-specific linguistic evaluation.
4. Embedding-based retrieval.
5. Independent retrieval benchmarks.
6. Improved energy-based ranking.
7. Local language generation.
8. Small-model benchmarking.
9. Community-reviewed language resources.
10. Android and Linux interfaces.
11. Multimodal knowledge processing.
12. Descriptor-relative filesystem access and stronger protection against filesystem race conditions.
13. Expanded adversarial and security testing.

These are research directions, not current capabilities.

## 9. Research Direction

The longer-term research direction includes energy-based learning, retrieval-augmented knowledge, resource-aware local models, and carefully evaluated language support.

Relevant research references include:

- LeCun et al., *A Tutorial on Energy-Based Learning* (2006).
- Sermanet et al., *EBLearn: Open-Source Energy-Based Learning in C++* (2009).

These references inform the research direction; they do not mean their methods have all been implemented in this project.

## 10. Build and Reproducibility

Required development tools:

- C++17-compatible compiler.
- CMake.
- pkg-config.
- JsonCpp.

Configure and build:

```bash
cmake -S . -B build
cmake --build build -j2
```

Run the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

Run the demonstration:

```bash
./build/dukeai_demo data/demo
```

Run the direct ingestion security tests:

```bash
./build/dukeai_document_ingestion_tests
```

The 19 November 2026 Android Termux verification recorded:

- Successful build.
- Eight of eight registered CTest tests passing.
- Twenty-nine direct document-ingestion checks passing.
- Successful English and Kiswahili retrieval demonstration.
- Clean `git diff --check`.

These results describe the tested source revision and environment. They do not guarantee correctness on every platform.

See [TESTING.md](TESTING.md) for testing procedures and [SECURITY_TEST_EVIDENCE.md](SECURITY_TEST_EVIDENCE.md) for the filesystem-boundary verification record.

## 11. Architectural Direction

DukeAI Nexus aims to develop a modular, transparent, offline-first architecture that can be evaluated on resource-constrained devices.

The architecture will evolve through measured experiments, reproducible tests, community review, and clear reporting rather than assumptions about model performance or system capabilities.

**Our Languages. Our Heritage. Our Future.**
