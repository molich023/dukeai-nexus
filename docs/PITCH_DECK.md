# DukeAI Nexus — Assessor Pitch

**Our Languages. Our Heritage. Our Future.**

> Project status: early, individual-led, open-source C++17 retrieval prototype. This document distinguishes implemented functionality from proposed research and future capabilities.

## 1. The problem

Useful civic and science knowledge can be difficult to discover, verify and access. African languages and dialects remain underrepresented in many digital knowledge systems. Connectivity, device capability, data costs and control over information affect who benefits. Knowledge systems must also respect provenance, consent, ownership and cultural context.

## 2. The opportunity

DukeAI Nexus proposes to build practical foundations before promising a large model: a small, reproducible knowledge-retrieval system; visible source passages; resource-conscious engineering; and staged collaboration with educators, researchers and language communities.

## 3. What exists today

The current prototype is a C++17 document-ingestion and lexical-retrieval system. Its current capabilities include:

- Document ingestion and metadata handling.
- Metadata and input validation.
- Text processing and lexical feature extraction.
- Lexical candidate selection and deterministic ranking fallback.
- Returning source passages for human inspection.
- CMake build, automated tests and a demonstration dataset.

The system retrieves source passages. It does not currently generate natural-language answers.

## 4. How it works

1. Load approved documents and metadata.
2. Validate metadata and document inputs.
3. Process text and extract lexical features.
4. Retrieve and rank candidate passages.
5. Return source-grounded passages for a person to review.

## 5. Language and cultural vision

The initial engineering focus is English and Kiswahili civic and science knowledge. Longer-term exploration may include Kikamba, Kikuyu and additional African languages and dialects. Oral, cultural and creative knowledge should only be included with appropriate consent, attribution and governance. Native-speaker evaluation, dialect representation and community authority are essential.

Language expansion is a roadmap objective, not a claim that the current prototype is a trained multilingual model.

## 6. Trust, safety and governance

Security and human control are design requirements:

- Treat user input, retrieved documents and external content as untrusted data.
- Keep reading separate from acting; default future agents to read-only access.
- Apply least privilege and minimal workflow permissions.
- Protect important branches and require review for sensitive changes.
- Require human approval for high-impact or irreversible actions.
- Test controls, monitor behaviour and provide a kill switch before enabling agent actions.
- Respect copyright, attribution, privacy, consent and redistribution restrictions.

These are design objectives and staged controls. The project must report which controls are implemented and tested rather than imply that all agent safeguards are already operational.

## 7. Roadmap

**Current priority:** clean-clone reproducibility, automated test evidence, documentation, repository security and an assessor-facing website.

**Next:** supervised pilot design, approved knowledge sources and evaluation with educators, researchers and language practitioners.

**Later research:** retrieval-quality improvements, semantic retrieval, small-model benchmarking and carefully evaluated language generation.

**Longer-term research:** energy-based approaches and JEPA/world-model concepts. These are not implemented capabilities of the current prototype.

## 8. Impact and evaluation

Progress should be measured through:

- Technical reproducibility, build reliability, tests, retrieval relevance and source traceability.
- User task completion, comprehension and usefulness in supervised pilot studies.
- Native-speaker assessment, dialect coverage and error analysis.
- Rights verification, consent records, privacy review and correction processes.
- Maintainability, contributor participation and resource-conscious operation.

## 9. Leadership and support sought

DukeAI Nexus is an individual-led open-source project under active development and testing. Support sought includes technical review, language-community partnerships, and feedback from educators and researchers. Potential pilot participants include civic and science educators, language practitioners and researchers.

The immediate request is to review the prototype, validate practical use cases and help shape a responsible pilot.

## 10. What DukeAI Nexus is—and is not

**It is:** an early C++17 document-ingestion and lexical-retrieval prototype with source-grounded outputs and an evolving test suite.

**It is not yet:** a trained multilingual foundation model, full conversational assistant, semantic/vector retrieval system, verified translation system, deployed autonomous agent, or implemented JEPA/world model.

Future capabilities will be reported only after implementation and evaluation.

## 11. Closing

DukeAI Nexus aims to make useful knowledge more accessible while respecting language, people and provenance.

**Build with communities. Verify with evidence. Preserve with respect.**

*Our Languages. Our Heritage. Our Future.*
