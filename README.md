# DukeAI Nexus

**An offline-first, open-source African AI research project.**

DukeAI Nexus is being developed as a local knowledge assistant, initially focused on English and Kiswahili civic and scientific learning.

The project targets Android (Termux) and Linux, with an emphasis on privacy, source traceability, responsible data governance, and resource-aware development.

## Current Status

DukeAI Nexus currently has a working C++17 research prototype.

### Implemented Components

- Linear energy-ranker component with pairwise margin-training support, tested using a synthetic fixture.
- Basic text tokenisation and feature extraction.
- Local plain-text document loading.
- Structured JSON document metadata.
- Metadata validation and source consistency checks.
- Paragraph-based lexical retrieval with deterministic ranking. The current demonstration uses an untrained ranker.
- Source and file provenance preservation.
- Unified document ingestion.
- Standalone English and Kiswahili retrieval demonstration.
- Automated tests using CMake and CTest.

**Current limitations:** DukeAI Nexus is not yet a general-purpose AI assistant. Natural-language generation, translation, semantic retrieval, and JEPA are not implemented.

## Requirements

- C++17 compiler (Clang or GCC).
- CMake.
- pkg-config.
- JsonCpp.

## Installation on Android (Termux)

Update your Termux packages:

```bash
pkg update && pkg upgrade
```

Install the required dependencies:

```bash
pkg install clang cmake pkg-config jsoncpp
```

## Build

From the project root directory:

```bash
cmake -S . -B build
cmake --build build
```

## Run Tests

```bash
ctest --test-dir build --output-on-failure
```

## Run the Retrieval Demonstration

After building the project:

```bash
./build/dukeai_demo data/demo
```

The demonstration retrieves approved English and Kiswahili water-conservation passages. Retrieved passages are source material, not AI-generated answers.

## Project Structure

The main project directories and files include:

```text
dukeai-nexus/
├── docs/                  # Architecture, governance, research and testing
├── include/dukeai/        # Public C++ headers
├── src/                   # Implementation modules
├── tests/                 # Automated tests and fixtures
├── data/demo/             # Demonstration documents
├── CMakeLists.txt         # Build configuration
├── README.md              # Project documentation
└── LICENSE                # Software licence
```

Additional directories and components may be introduced as development progresses.

## Research Principles

DukeAI Nexus follows these principles:

- Offline-first and resource-aware operation.
- Open-source development and transparent limitations.
- Source-labelled and traceable knowledge.
- Copyright and licence awareness.
- Privacy-conscious handling of local data.
- Human review of language and community knowledge.
- Reproducible evaluation and honest reporting.
- Community participation before claiming community impact.

## Planned Research

The following are future research goals, not implemented features:

- English and Kiswahili civic and scientific knowledge packs.
- Semantic retrieval and embedding models.
- Local language generation.
- African language and dialect support.
- Translation and multilingual evaluation.
- Independent held-out retrieval benchmarks.
- More expressive energy-based models.
- Small-model benchmarking on Android and Linux.
- JEPA and world-model research.
- Multimodal capabilities.
- Android and Linux user interfaces.

## Data Governance and Responsible Use

Public availability of this repository does not automatically grant permission to redistribute a document or use it for public model training.

All external research materials must be reviewed for copyright, licensing, attribution, redistribution rights, and permitted use.

Review [`docs/DATA_GOVERNANCE.md`](docs/Data_Governance.md) before adding research materials.

## Contributing

Contributions are welcome.

Please ensure that contributions include:

- Appropriate tests.
- Source attribution where applicable.
- Clear documentation.
- Respect for copyright, licensing and data governance requirements.
- Honest descriptions of implemented capabilities and limitations.

See [`docs/Testing.md`](docs/Testing.md) for testing guidance.

## Platform Support

The project is being developed for:

- Android through Termux.
- Linux environments.

Build and test results should be reported for the actual platform on which they were verified.

## Project Vision

DukeAI Nexus aims to contribute to responsible, locally accessible African AI research, supporting language preservation, civic and scientific learning, and community-centred knowledge development.

**Our Languages. Our Heritage. Our Future.**
