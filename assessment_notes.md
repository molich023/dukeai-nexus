# DukeAI Nexus — Project Assessment Notes

**Project:** DukeAI Nexus  
**Repository:** https://github.com/molich023/dukeai-nexus  
**Document:** Project Assessment Notes  
**Version:** 1.0  
**Date:** 18 November 2026  
**Project stage:** Early-stage open-source prototype  
**Development status:** Active development and testing  
**Project lead:** Individual developer

---

## 1. Executive Summary

DukeAI Nexus is an early-stage open-source project focused on building a locally accessible knowledge and retrieval system, with an initial emphasis on English and Kiswahili civic and science information.

The project is being developed with an emphasis on accessibility, African knowledge representation, multilingual information access, data governance, privacy, security and responsible AI development.

The current implementation is a C++17-based retrieval prototype. It supports document ingestion, metadata handling, text processing, lexical retrieval and deterministic ranking.

The prototype demonstrates a foundational approach to retrieving relevant source passages from a local knowledge collection.

It is important to distinguish the working prototype from the broader research vision. DukeAI Nexus is not yet a complete conversational AI assistant, autonomous agent or production-ready knowledge platform.

The immediate objective is to establish a reliable, testable and reproducible foundation before introducing more complex AI capabilities.

## 2. Problem and Opportunity

Access to reliable educational, civic and scientific information remains a challenge for many communities, particularly where information is fragmented, difficult to access, available primarily in dominant languages or poorly representative of local knowledge.

African languages and locally relevant knowledge require greater representation in digital information systems.

There is an opportunity to develop open-source infrastructure that supports:

- Access to educational and scientific knowledge.
- English and Kiswahili information retrieval.
- African history and cultural knowledge preservation.
- Access to civic and constitutional information.
- Future support for additional African languages.
- Locally controlled knowledge resources.
- Transparent and responsible AI development.

DukeAI Nexus seeks to contribute to this opportunity through an incremental, locally accessible and open-source development approach.

## 3. Current Working Prototype

The current prototype focuses on retrieval rather than generated answers.

### Implemented capabilities

The project currently includes:

1. A C++17 application structure.
2. CMake-based configuration and compilation.
3. Document ingestion.
4. Metadata parsing and validation.
5. Text processing and lexical feature extraction.
6. Lexical retrieval and candidate selection.
7. Deterministic ranking fallback.
8. Retrieval of source passages for inspection.
9. A demonstration dataset.
10. Automated tests through CTest.

The project has been built and tested in a Termux development environment, and a retrieval demonstration has been executed.

The project also has a GitHub Actions workflow for Ubuntu-based build and test verification.

### Current limitations

The current prototype does not yet provide:

- Natural-language answer generation.
- A fully integrated language model.
- Semantic or vector-based retrieval.
- A complete translation engine.
- A graphical user interface.
- An autonomous development agent.
- Autonomous file or shell execution.
- A production deployment.
- A complete multilingual knowledge platform.

These are potential future development areas, not completed features.

## 4. Technical Architecture

The current implementation uses:

| Component | Technology or approach |
|---|---|
| Primary language | C++17 |
| Build system | CMake |
| JSON handling | JsonCpp |
| Retrieval | Lexical retrieval |
| Ranking | Deterministic ranking fallback |
| Testing | CTest |
| Continuous integration | GitHub Actions |
| CI environment | Ubuntu 24.04 |
| Initial development environments | Linux and Android/Termux |

The architecture is intentionally being developed in manageable stages.

The immediate engineering priority is to make ingestion, metadata handling, retrieval and testing dependable before introducing model inference or agent-based capabilities.

## 5. Continuous Integration Assessment

The project currently uses the GitHub Actions workflow:

`.github/workflows/ci.yml`

The workflow is named:

`DukeAI Nexus CI`

It runs on:

- Pushes to `main`.
- Pull requests targeting `main`.
- Manual execution through `workflow_dispatch`.

### 5.1 Workflow permissions

The workflow declares:

```yaml
permissions:
  contents: read
```

This is a positive security measure because the workflow explicitly limits the repository permission granted to its jobs.

The current build-and-test job does not require repository write permissions or deployment credentials.

### 5.2 Build environment

The workflow runs on:

```yaml
runs-on: ubuntu-24.04
```

This provides a defined Ubuntu runner environment for build and test verification.

The workflow installs the required packages:

- build-essential
- cmake
- pkg-config
- libjsoncpp-dev

### 5.3 Compilation and testing

The workflow configures the project in Release mode:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

It then compiles the project:

```bash
cmake --build build --parallel 2
```

The automated test suite is executed through:

```bash
ctest --test-dir build --output-on-failure
```

Finally, the workflow runs the retrieval demonstration:

```bash
./build/dukeai_demo data/demo
```

This provides a basic verification sequence covering configuration, compilation, automated tests and execution of the demonstration application.

### 5.4 Current CI security strengths

The existing workflow has the following positive characteristics:

1. Explicitly restricted repository permissions.
2. A dedicated Ubuntu build environment.
3. Separate configuration, compilation and testing steps.
4. Automated execution of the CTest suite.
5. Execution of the retrieval demonstration.
6. No deployment credentials declared in the supplied workflow.
7. No repository write permissions declared in the supplied workflow.
8. No automatic deployment step.
9. Support for pull-request testing and manual execution.

These controls provide a useful foundation for maintaining reproducible development checks.

### 5.5 CI limitations and recommended improvements

The workflow is currently a build-and-test pipeline. It should not be presented as a complete security pipeline.

The following improvements should be considered:

| Area | Current position | Recommended improvement |
|---|---|---|
| Repository permissions | `contents: read` | Retain least-privilege permissions. |
| Checkout action | `actions/checkout@v5` | Monitor and review action updates. |
| Dependency installation | Ubuntu packages installed through apt | Review dependency versions and reproducibility. |
| Compiler and build | Release build | Consider additional compiler and warning configurations. |
| Automated tests | CTest | Expand tests as new security and functional requirements are implemented. |
| Static analysis | Not shown in supplied workflow | Consider adding an appropriate C++ static-analysis job. |
| Dependency security | Not shown in supplied workflow | Consider dependency and vulnerability checks. |
| Secret scanning | Not shown in supplied workflow | Enable appropriate repository secret-scanning protections where available. |
| Code security analysis | Not shown in supplied workflow | Evaluate CodeQL or another suitable analysis tool. |
| Deployment | Not included | Keep deployment separate from untrusted pull-request build jobs. |

The workflow currently uses the version tag `actions/checkout@v5`. A future hardening step may pin third-party actions to verified full commit SHAs, with a documented process for updating them.

The workflow also installs packages from the Ubuntu repositories at execution time. This is convenient for development, but dependency versions may change over time. Dependency version control and reproducible-build improvements can be considered as the project matures.

No security scan, static-analysis result or vulnerability assessment should be claimed until the corresponding tool is configured and its results have been reviewed.

## 6. Testing and Verification

Testing is a central part of the current development process.

The current verification sequence is:

```bash
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure
./build/dukeai_demo data/demo
```

This sequence is intended to establish that:

- The project configures successfully.
- The source code compiles.
- Automated tests execute.
- The demonstration application runs.
- Retrieval functionality can be observed using the supplied demonstration data.

Testing is being performed in a development environment that includes Android/Termux, with Ubuntu CI providing an additional Linux verification environment.

Actual test counts, dates, commit identifiers and results should be taken from the relevant test logs and GitHub Actions runs rather than assumed from this assessment document.

## 7. Security and Responsible Development

Security is a foundational project requirement.

The project is developing a security approach based on:

- Least privilege.
- Data validation.
- Controlled document ingestion.
- Source traceability.
- Protection of trusted project configuration.
- Safe handling of untrusted content.
- Human oversight.
- Transparent capability reporting.
- Reproducible testing.
- Privacy-conscious data handling.

The project has separate documentation describing its security requirements and proposed verification activities:

- `docs/SECURITY_GUARDRAILS.md`
- `docs/SECURITY_TEST_PLAN.md`

These documents define current requirements and future controls.

They must not be interpreted as evidence that all proposed controls have already been implemented.

### 7.1 Prompt injection and untrusted content

The current retrieval prototype does not execute retrieved passages as instructions and does not contain an autonomous AI agent.

Prompt-injection resistance, model output validation and agent-specific tool restrictions are therefore future integration requirements.

Before a language model or agent is introduced, the project intends to test:

- Direct and indirect prompt injection.
- Conflicting instructions in retrieved documents.
- Separation of trusted policy from untrusted content.
- Protection of internal configuration and secrets.
- Safe handling of generated commands and code.
- Multilingual safety behaviour.

### 7.2 Future development agent

A local development assistant is a longer-term research and development direction.

If implemented, it must have a separate identity, restricted permissions, isolated execution, explicit human approval for sensitive operations, independent action verification, monitoring and an emergency shutdown mechanism.

No autonomous development-agent capability is claimed in the current prototype.

## 8. Data Governance and Knowledge Integrity

DukeAI Nexus aims to support responsible knowledge management.

The project recognises the importance of:

- Document provenance.
- Source attribution.
- Licensing and usage rights.
- Metadata quality.
- Data integrity.
- Privacy.
- Responsible handling of culturally significant material.
- Clear separation between sourced information and generated interpretations.

African languages and cultural knowledge should not be treated as interchangeable or generic datasets.

Future language and cultural preservation work should involve appropriate source documentation, language expertise, community participation and respect for relevant rights and restrictions.

The project's data governance documentation should evolve alongside the actual ingestion and retrieval implementation.

## 9. Language and Cultural Preservation Vision

The initial focus is English and Kiswahili civic and science information.

Longer-term possibilities include:

- Additional African languages.
- Regional dialects and language varieties.
- African and world history.
- School and educational resources.
- Scientific knowledge.
- African constitutions and legal information.
- Comparative constitutional and civic education.
- Language translation and cultural preservation.

The project recognises that translation quality, cultural context and local terminology require careful evaluation.

A language appearing in a dataset does not, by itself, demonstrate that the system can translate or understand that language.

## 10. Research and Future Technical Directions

The long-term research vision includes investigating:

- Small and efficient local language models.
- Retrieval-augmented generation.
- Semantic and vector retrieval.
- Multilingual information access.
- Energy-based model approaches.
- Joint Embedding Predictive Architecture (JEPA).
- World-model research.
- AI evaluation and benchmarking.
- Responsible AI principles.
- OWASP security guidance.
- NIST AI risk-management guidance.
- Local-first and privacy-conscious AI infrastructure.

These areas are research directions. They are not represented as existing implementation capabilities.

Future research should be introduced only when the current prototype, resources and evaluation capacity support it.

## 11. Development Roadmap

### Phase 1 — Stabilise the retrieval prototype

- Maintain a reliable C++ build.
- Improve ingestion validation.
- Expand automated tests.
- Verify English and Kiswahili retrieval examples.
- Improve source traceability.
- Document limitations.
- Establish repeatable Linux and Android/Termux testing.

### Phase 2 — Improve retrieval and governance

- Strengthen metadata validation.
- Improve source and licensing records.
- Investigate semantic retrieval.
- Expand the demonstration corpus responsibly.
- Improve error handling and resource limits.
- Introduce additional security tests.

### Phase 3 — Evaluate local model integration

- Benchmark suitable small models.
- Evaluate local resource requirements.
- Investigate retrieval-augmented generation.
- Establish model input and output safety requirements.
- Test factual grounding and citation accuracy.
- Measure English and Kiswahili performance.

### Phase 4 — Investigate controlled agent capabilities

- Define an isolated agent identity.
- Implement least-privilege permissions.
- Separate reading, proposing and acting.
- Require human approval for sensitive operations.
- Implement action verification and monitoring.
- Test emergency shutdown controls.

### Phase 5 — Expand knowledge and language coverage

- Evaluate additional African language resources.
- Develop appropriate cultural and linguistic evaluation methods.
- Expand educational and civic knowledge collections.
- Explore partnerships and pilot-user feedback.

The roadmap is conditional on technical feasibility, resources, security review and evaluation results.

## 12. Project Leadership and Capacity

DukeAI Nexus is currently led by an individual developer.

The project is being developed incrementally, with practical testing and documentation used to validate progress.

The current development approach prioritises:

- A functional and inspectable prototype.
- Open-source development.
- Transparent technical documentation.
- Reproducible testing.
- Clear separation of implemented and planned functionality.
- Responsible use of limited development resources.

Additional support would be valuable in:

- C++ engineering and architecture review.
- Information retrieval and machine learning.
- Kiswahili and African language evaluation.
- Data governance and licensing.
- Application security.
- User testing and community engagement.
- Open-source project sustainability.

## 13. Expected Impact

If successfully developed and evaluated, DukeAI Nexus could contribute to:

1. Improved access to educational and scientific information.
2. Better representation of African knowledge resources.
3. More accessible English and Kiswahili information retrieval.
4. Open-source local-first knowledge infrastructure.
5. Research into responsible multilingual AI.
6. Greater awareness of data provenance and knowledge governance.
7. Practical learning opportunities for developers and contributors.

These are intended outcomes. Their achievement will depend on implementation, testing, partnerships and adoption.

## 14. Key Risks and Mitigation

| Risk | Potential impact | Mitigation |
|---|---|---|
| Limited development capacity | Slower progress | Prioritise a small working prototype and staged milestones. |
| Limited computing resources | Constraints on model selection | Start with retrieval and benchmark efficient local models. |
| Poor-quality source data | Inaccurate retrieval | Validate metadata and document provenance. |
| Licensing uncertainty | Restrictions on knowledge distribution | Review source rights before including material. |
| Security vulnerabilities | Data or system compromise | Apply least privilege, validation and repeatable security tests. |
| Prompt injection in future models | Unsafe model behaviour | Test untrusted content boundaries before model integration. |
| Agent misuse in future development | Unauthorised system changes | Use isolation, approval controls and a kill switch. |
| Limited language evaluation resources | Poor multilingual performance | Seek language experts and pilot-user feedback. |
| Scope expansion | Failure to complete core milestones | Keep the initial prototype and measurable deliverables as priorities. |

## 15. Support Requested

The project would benefit from support in the following areas:

- Technical mentorship and architecture review.
- Security and privacy review.
- Retrieval and model evaluation.
- African language and cultural knowledge expertise.
- Data governance and licensing guidance.
- Pilot-user access and usability feedback.
- Open-source collaboration.
- Infrastructure and computing support where necessary.

The immediate objective is not to build every proposed capability at once. It is to strengthen a working foundation and demonstrate measurable progress.

## 16. Assessment Position and Honest Capability Statement

DukeAI Nexus should currently be assessed as an early-stage open-source retrieval prototype with a broader multilingual and responsible-AI research vision.

Its strongest present evidence is the existence of a C++17 implementation, a working retrieval demonstration, automated tests and a GitHub Actions build-and-test workflow.

Its principal development challenge is moving from a foundational retrieval system toward a more capable, well-evaluated and secure local knowledge assistant.

The project does not claim to have completed the following:

- A production-ready AI assistant.
- Full multilingual translation.
- Autonomous AI-agent functionality.
- JEPA or world-model implementation.
- Full OWASP or NIST compliance.
- Independent security certification.
- Production-scale deployment.
- Proven community impact.

Future claims should be supported by working code, test results, documented evaluations or independently verifiable evidence.

## 17. Evidence Checklist for Reviewers

The following evidence should be maintained as the project develops.

| Evidence | Location or source | Assessment |
|---|---|---|
| Source code | GitHub repository | Available for inspection |
| Build configuration | `CMakeLists.txt` | Available for inspection |
| Automated tests | `tests/` | Available for inspection |
| CI workflow | `.github/workflows/ci.yml` | Available for inspection |
| CI execution results | GitHub Actions | Verify the relevant run |
| Retrieval demonstration | `data/demo/` and application output | Verify against current source revision |
| Architecture documentation | `docs/Architecture.md` | Available for inspection |
| Data governance documentation | `docs/Data_Governance.md` | Available for inspection |
| Testing documentation | `docs/Testing.md` | Available for inspection |
| Security guardrails | `docs/SECURITY_GUARDRAILS.md` | Verify implementation against requirements |
| Security test plan | `docs/SECURITY_TEST_PLAN.md` | Record actual execution results |
| Research plan | `docs/Research_plan.md` | Available for inspection |
| Language preservation vision | `docs/African_languages_cultural_preservation_vision.md` | Available for inspection |
| Assessor presentation | `docs/PITCH_DECK.md` and presentation file | Verify files are committed |
| Project vision | `docs/PROJECT_VISION.md` | Verify file is committed |

The evidence checklist must be updated when files are added, renamed or changed.

## 18. Closing Statement

DukeAI Nexus is an attempt to build an open-source foundation for more accessible, locally relevant and responsibly governed knowledge systems.

The project begins with a deliberately limited retrieval prototype rather than claiming to have already solved the wider challenges of multilingual AI.

Its development philosophy is based on practical implementation, transparent limitations, security-conscious engineering, measurable testing and a commitment to representing African knowledge and languages with care.

The immediate goal is to make the existing prototype dependable, independently testable and useful as a foundation for future research and collaboration.

**DukeAI Nexus — building a responsible, locally accessible knowledge foundation, one verified capability at a time.**
