# DukeAI Nexus --- Project Vision

**Building Community-Centred, Multilingual and Sovereign AI
Infrastructure for Africa**

**Project identity:** DukeAI Nexus\
**Project status:** Early-stage, individual-led, open-source project\
**Initial focus:** English and Kiswahili civic and science knowledge\
**Current technical foundation:** C++17 document ingestion and lexical
retrieval prototype\
**Guiding principle:** Our Languages. Our Heritage. Our Future.

------------------------------------------------------------------------

## 1. Vision

DukeAI Nexus envisions an open, community-centred knowledge and AI
infrastructure that helps people discover, understand and responsibly
use useful knowledge across African languages and, over time, languages
from around the world.

The project begins with a deliberately modest and testable foundation: a
resource-conscious system that ingests approved documents, validates
their metadata, retrieves relevant passages and presents the source
material for human inspection.

From this foundation, DukeAI Nexus aims to explore how locally relevant
knowledge systems can be developed with African communities, educators,
researchers and language practitioners---not simply for them. The
long-term ambition is to support access to civic, scientific,
educational, historical and cultural knowledge while respecting language
diversity, cultural context, intellectual property, privacy and
community authority.

DukeAI Nexus is not presented as an already completed multilingual AI
platform. It is an evolving open-source project with a working retrieval
prototype and a longer-term research and community-development vision.

## 2. The challenge we seek to address

Access to reliable knowledge is shaped by more than whether information
exists online. People also need to be able to find it, understand its
language, evaluate its source and use it in the context of their lives.

Several connected challenges motivate this work:

-   **Language representation:** Many African languages, dialects and
    forms of local expression have limited representation in mainstream
    digital knowledge and AI systems.
-   **Knowledge discovery:** Useful civic, science and educational
    material can be fragmented across documents, institutions and
    formats.
-   **Source transparency:** Users need ways to inspect where
    information came from instead of relying on unsupported generated
    claims.
-   **Resource constraints:** Systems designed for powerful
    infrastructure may be unsuitable for lower-cost devices,
    intermittent connectivity or limited computing resources.
-   **Data rights and trust:** Digitising or processing knowledge
    without clear rights, consent, attribution and governance can harm
    individuals and communities.
-   **Local relevance:** General-purpose systems may not adequately
    reflect local educational needs, legal and civic contexts,
    terminology or cultural nuance.

These are broad challenges, not claims that one software project can
solve them alone. DukeAI Nexus seeks to contribute a practical, open and
testable part of the response.

## 3. Our mission

Our mission is to develop and evaluate open-source, resource-conscious
knowledge tools that improve access to reliable civic and science
information, beginning with English and Kiswahili, while laying
responsible foundations for wider African-language and
community-governed knowledge work.

The mission has five connected commitments:

1.  **Access:** Make approved, useful knowledge easier to discover.
2.  **Language inclusion:** Treat African languages and dialects as
    important knowledge languages, not as afterthoughts.
3.  **Transparency:** Preserve source provenance and make retrieved
    material inspectable.
4.  **Responsibility:** Respect consent, copyright, privacy, cultural
    context and community decision-making.
5.  **Practical openness:** Build reproducible software, document
    limitations and invite independent review.

## 4. Who the project aims to serve

DukeAI Nexus is being developed with potential use by:

-   Learners, teachers and education organisations seeking accessible
    civic and science resources.
-   Researchers and knowledge practitioners working with approved,
    well-described documents.
-   Kiswahili and other African-language practitioners interested in
    responsible language technology.
-   Community organisations seeking to improve access to locally
    relevant information.
-   Open-source developers and institutions interested in low-resource
    knowledge infrastructure.

These are intended audiences and potential pilot participants. Their
needs must be validated through consultation and supervised evaluation;
the project does not claim that these groups have already adopted or
endorsed the system.

## 5. Initial focus: English and Kiswahili civic and science knowledge

The first use case is intentionally narrow: retrieval of approved civic
and science knowledge in English and Kiswahili.

This focus provides a practical way to test:

-   Whether documents can be ingested and their metadata validated.
-   Whether users can retrieve relevant passages using lexical search.
-   Whether results preserve enough source information for inspection.
-   Whether the system can be built and tested on modest development
    environments.
-   Whether the content governance process is suitable for a responsible
    pilot.

The initial focus is not a claim of complete English--Kiswahili
translation, conversational fluency or equal language performance. Any
future translation, summarisation or generation capability must be
separately implemented, evaluated and reviewed by competent language
practitioners.

## 6. African languages, dialects and cultural preservation

DukeAI Nexus aims to support a future in which African languages and
dialects can participate more fully in digital knowledge infrastructure.

Potential future areas of exploration include additional languages such
as Kikamba and Kikuyu, as well as regional varieties, dialects, oral
histories and community-held knowledge. The selection and prioritisation
of languages should be informed by community interest, available
expertise, practical feasibility and responsible data access---not by
assumptions that a language community has consented to participate.

Cultural preservation requires more than collecting text or audio. It
requires careful attention to:

-   Free, informed and appropriately documented consent.
-   Community authority over whether knowledge may be recorded,
    digitised, shared or used for model development.
-   Attribution, ownership, licensing and benefit-sharing expectations.
-   Sensitive, sacred, restricted or personally identifying knowledge.
-   Dialect variation, spelling conventions, context and culturally
    appropriate interpretation.
-   The ability to correct, withdraw or restrict material where
    agreements and applicable law allow.

The project should not treat public availability as automatic permission
for redistribution or AI training. Where rights or consent are
uncertain, material should be excluded from public datasets and training
use until its status is resolved.

## 7. Knowledge sovereignty and responsible infrastructure

For DukeAI Nexus, knowledge sovereignty means supporting the ability of
people and communities to understand, govern and make informed decisions
about the knowledge resources and technologies that affect them.

This is a guiding direction, not a claim that technical localisation
alone creates sovereignty. Meaningful sovereignty also depends on
governance, skills, institutional partnerships, sustainable maintenance,
accessible infrastructure and community participation.

The project will explore:

-   Open and inspectable software where practical.
-   Local or offline-capable workflows where feasible.
-   Clear documentation of data sources, rights and processing.
-   Interoperable formats that reduce unnecessary lock-in.
-   Resource-conscious designs that can be evaluated on modest devices.
-   Community participation in language priorities, evaluation and
    governance.

## 8. Current implementation: what exists today

The current public prototype is an early C++17 document-ingestion and
lexical-retrieval system.

The implemented foundation includes:

-   Document loading and ingestion components.
-   Metadata parsing and validation.
-   Text processing and lexical feature extraction.
-   Lexical candidate retrieval.
-   A deterministic ranking fallback.
-   Retrieval of source passages for human inspection.
-   A CMake-based build and automated test suite.
-   A demonstration dataset and runnable demonstration.

The prototype is a retrieval system. It does not currently generate
natural-language answers, provide a complete conversational assistant,
or autonomously act on behalf of users.

Its current language focus and demonstration material should not be
confused with a fully evaluated bilingual or multilingual model.

## 9. Long-term technical research direction

Future research will be guided by measured needs and evidence from the
working prototype and pilot activities.

Possible areas of investigation include:

### 9.1 Retrieval quality

Evaluate lexical retrieval, query handling, ranking, metadata quality,
source traceability and relevance using documented test collections and
human assessment.

### 9.2 Small and efficient models

Benchmark suitable small models for defined tasks on target Android and
Linux devices. Assess quality, latency, memory use, energy consumption,
licensing and privacy implications before selecting a model.

### 9.3 Multilingual and translation research

Explore language identification, terminology support, translation and
cross-language retrieval with native-speaker involvement and explicit
evaluation. Do not represent untested output as reliable translation.

### 9.4 Responsible generation

If natural-language generation is introduced, evaluate factual
grounding, citation behaviour, hallucination risks, prompt injection,
harmful outputs and refusal behaviour. Generated responses should remain
distinguishable from source documents.

### 9.5 Energy-based models and world-model research

The project may investigate energy-based model concepts and Joint
Embedding Predictive Architecture (JEPA) or related world-model research
as longer-term research directions. These are not implemented components
of the current prototype and should not be presented as existing product
capabilities.

Research choices will be made based on suitability, reproducibility,
compute requirements, available expertise and demonstrable benefit.

## 10. Security, safety and human control

Trust and safety must be built into the development process as
capabilities expand.

The intended security principles are:

-   Treat user input, retrieved passages, external documents and tool
    outputs as untrusted data.
-   Keep trusted application policy separate from content being
    processed.
-   Use least privilege and read-only defaults for any future
    development agent.
-   Separate reading and analysis from actions that modify files or
    systems.
-   Require explicit human approval for privileged, irreversible or
    externally visible actions.
-   Protect the official repository through branch rules, code review
    and controlled workflows.
-   Validate inputs and outputs and test security controls with
    repeatable cases.
-   Monitor future agent behaviour and maintain an effective emergency
    stop before enabling agent actions.

The current retrieval prototype does not operate an autonomous
development agent. These principles define the security requirements for
future integrations; each control must be implemented and tested before
it is described as operational.

## 11. Data governance and ethical commitments

DukeAI Nexus will use a cautious approach to source selection and data
handling.

For each knowledge source, the project should establish, as applicable:

-   Source identity and provenance.
-   Author, publisher and attribution information.
-   Copyright or licence status.
-   Permission for storage, extraction, redistribution and public
    display.
-   Whether local research use or model training is permitted.
-   Consent and community governance requirements.
-   Review status, quality limitations and correction history.

Permission for one activity must not be assumed to grant permission for
another. For example, permission to read or locally index a document
does not automatically permit redistribution or public model training.

Personal, sensitive, restricted or culturally protected material should
not be included without an appropriate lawful basis, consent and
governance process. Unverified material should remain clearly labelled
and should not be represented as approved for redistribution or
training.

## 12. Development approach and milestones

The project will progress through small, reviewable milestones.

  -----------------------------------------------------------------------
  Stage                   Focus                   Evidence expected
  ----------------------- ----------------------- -----------------------
  Current foundation      Retrieval prototype and Reproducible build,
                          build system            automated tests, demo

  Repository readiness    Documentation,          Independent build
                          clean-clone             instructions, CI
                          verification and        results, repository
                          security baseline       review

  Assessor readiness      Pitch, vision,          Accessible documents,
                          architecture and        working links, deployed
                          website                 static site

  Pilot preparation       Approved content,       Source register,
                          partners and evaluation consent and governance
                          design                  process, pilot protocol

  Pilot evaluation        Supervised use and      Retrieval-quality
                          feedback                results, user feedback,
                                                  language review

  Research expansion      Small models,           Published experiments,
                          multilingual methods    benchmarks and
                          and advanced            documented limitations
                          architectures           
  -----------------------------------------------------------------------

Dates and deliverables should be set against available capacity, partner
commitments and funding. The project should not promise a production
deployment or language coverage before those dependencies are
established.

## 13. How success should be assessed

Success should be measured through evidence rather than broad claims.

Potential measures include:

-   **Reproducibility:** A new developer can build, test and run the
    prototype from the public repository.
-   **Reliability:** Automated tests cover core ingestion, validation
    and retrieval behaviour.
-   **Retrieval quality:** Relevant passages are returned for documented
    test queries, with source information preserved.
-   **Language quality:** Native speakers assess terminology, coverage
    and errors in any future language features.
-   **Governance:** Sources have documented rights, provenance and
    consent status appropriate to their use.
-   **Security:** Controls are tested against defined threats and their
    limitations are recorded.
-   **Usability:** Supervised pilot participants can complete defined
    information-retrieval tasks.
-   **Sustainability:** The codebase, documentation and contribution
    process remain understandable and maintainable.

Targets should be established after baseline measurements and
consultation with prospective users and partners.

## 14. Collaboration and support sought

DukeAI Nexus is an individual-led open-source project. It seeks
constructive engagement and support from:

-   African-language practitioners and community organisations.
-   Civic and science educators.
-   Researchers in information retrieval, language technology and
    responsible AI.
-   Open-source software developers and security reviewers.
-   Institutions able to advise on data governance, rights and
    responsible pilot design.

Priority support needs include independent technical review, language
and cultural consultation, approved knowledge sources, pilot
participants, security assessment and sustainable development capacity.

No partnership, endorsement or community consent should be implied
unless it has been explicitly agreed and documented.

## 15. Sustainability and long-term direction

The project aims to remain open, transparent and maintainable.
Sustainability will require more than publishing source code: it will
require clear governance, responsible content practices, contributor
documentation, realistic infrastructure choices and relationships with
people who can evaluate whether the system is useful.

Potential future paths include community-supported content collections,
educational pilots, institutional collaboration, research partnerships
and contributions from the open-source ecosystem. These are
possibilities to explore, not confirmed commitments or funding
arrangements.

## 16. Closing statement

DukeAI Nexus begins with a practical question: can we build a small,
transparent and reproducible knowledge-retrieval foundation that helps
people find useful information while respecting its sources and the
communities connected to it?

The current prototype is an early step---not the final destination. The
larger vision is to help build responsible, community-centred knowledge
infrastructure in which African languages, local context and human
agency matter.

**Build with communities. Verify with evidence. Preserve with respect.**

**Our Languages. Our Heritage. Our Future.**
