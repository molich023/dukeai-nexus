# DukeAI Nexus Research Plan

## 1. Research Overview

DukeAI Nexus is an offline-first African AI research project initially focused on English and Kiswahili civic and scientific learning.

The project investigates how locally accessible knowledge systems can operate on resource-constrained devices while maintaining source traceability, privacy, and responsible data governance.

The current software is an early C++17 prototype.

This research plan distinguishes implemented engineering work from proposed experiments.

## 2. Primary Research Question

Can a compact, locally executable retrieval and ranking system provide useful, traceable access to English and Kiswahili civic and scientific documents on Android and Linux devices?

## 3. Research Objectives

The initial objectives are:

1. Establish a reliable local document-ingestion pipeline.
2. Preserve document metadata and source provenance.
3. Evaluate lexical retrieval for English and Kiswahili material.
4. Investigate compact energy-based ranking.
5. Measure performance on resource-constrained devices.
6. Establish reproducible testing and evaluation procedures.
7. Develop responsible data governance for multilingual resources.

## 4. Current Research Baseline

The current prototype includes:

- C++17 implementation.
- Local plain-text document loading.
- Structured JSON metadata.
- Metadata validation.
- Basic tokenisation and feature extraction.
- Paragraph-based lexical retrieval.
- Deterministic candidate ranking.
- Linear energy-ranker component with pairwise margin-training support.
- Source and provenance preservation.
- English and Kiswahili demonstration material.
- Automated CMake and CTest tests.

The energy-ranker component has been tested using a synthetic fixture. The current demonstration uses an untrained ranker and deterministic fallback.

There is not yet evidence of improved real-world retrieval performance.

## 5. Research Workstreams

### Workstream A: Document Ingestion

Objective:

Improve reliable processing of local documents.

Research activities:

- Review supported document formats.
- Improve validation.
- Test large-document handling.
- Improve metadata consistency.
- Preserve source information.

### Workstream B: Multilingual Text Processing

Objective:

Investigate suitable text processing for English and Kiswahili.

Research activities:

- Unicode-aware processing.
- Language identification.
- Tokenisation evaluation.
- Kiswahili morphology research.
- Language-specific text normalisation.
- Evaluation with human-reviewed examples.

The current tokenizer does not implement full Unicode normalisation or Kiswahili morphological analysis.

### Workstream C: Retrieval

Objective:

Evaluate how effectively local queries retrieve relevant passages.

Research activities:

- Build reviewed evaluation queries.
- Establish relevance judgements.
- Compare lexical retrieval methods.
- Investigate semantic retrieval.
- Evaluate language-specific retrieval behaviour.
- Measure retrieval quality.

### Workstream D: Energy-Based Ranking

Objective:

Investigate whether a compact energy-based model can improve passage ordering.

Research activities:

- Define ranking objectives.
- Develop training and validation fixtures.
- Compare trained and untrained ranking.
- Measure ranking quality.
- Investigate pairwise margin training.
- Evaluate model size and computational cost.

### Workstream E: Android and Linux Performance

Objective:

Measure feasibility on local devices.

Research activities:

- Build reproducibility.
- Memory usage.
- Processing latency.
- Document-size limits.
- CPU utilisation.
- Resource consumption.
- Cross-platform behaviour.

### Workstream F: African Language and Cultural Resources

Objective:

Develop responsible approaches to African language preservation and knowledge access.

Research activities:

- Identify potential language-resource partners.
- Document language and dialect distinctions.
- Review resource permissions.
- Consult speakers and practitioners.
- Develop human-review procedures.
- Establish attribution and consent practices.

## 6. Evaluation Methodology

Future evaluations should use documented datasets, fixed test conditions, and independent held-out examples.

Potential metrics include:

| Area | Proposed measures |
|---|---|
| Retrieval | Precision@k, Recall@k, MRR, nDCG |
| Ranking | Pairwise accuracy, ranking loss |
| Performance | Latency, throughput |
| Memory | Peak memory consumption |
| Portability | Successful builds and tests across platforms |
| Data quality | Metadata validation and provenance completeness |
| Language quality | Human-reviewed relevance and linguistic accuracy |

Evaluation data must be reviewed for rights and permission before use.

## 7. Baselines and Comparisons

Experiments should compare:

- Existing lexical retrieval.
- Alternative lexical scoring methods.
- Trained energy-based ranking.
- Untrained deterministic ranking.
- Future embedding-based retrieval.

Comparisons must use the same evaluation data and clearly documented conditions.

## 8. Reproducibility

Each experiment should record:

- Source-code revision.
- Software dependencies.
- Compiler and build configuration.
- Dataset version.
- Data permissions.
- Evaluation queries.
- Evaluation methodology.
- Hardware and operating system.
- Results and limitations.

## 9. Long-Term Research Directions

The following remain future research directions:

- Local language generation.
- Translation.
- Semantic retrieval.
- African language and dialect support.
- Small-model benchmarking.
- More expressive energy-based models.
- Multimodal processing.
- JEPA and world-model research.
- Android and Linux user interfaces.

These directions must not be represented as existing capabilities until implemented and evaluated.

## 10. Research Integrity

DukeAI Nexus will report results honestly.

Synthetic test results must be distinguished from real-world evaluation.

Prototype demonstrations must be distinguished from production-ready systems.

Research proposals must be distinguished from implemented features.

## 11. Expected Research Contribution

The intended contribution is an openly developed, locally accessible research foundation for traceable multilingual knowledge retrieval and responsible African AI development.

**Our Languages. Our Heritage. Our Future.**
