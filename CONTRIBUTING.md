# Contributing to DukeAI Nexus

Thank you for your interest in contributing to DukeAI Nexus.

DukeAI Nexus is an offline-first, open-source African AI research project focused initially on English and Kiswahili civic and scientific learning.

We welcome contributions that improve the software, documentation, testing, accessibility, language resources, and responsible research practices.

## 1. Ways to Contribute

You can contribute through:

- C++ development and code improvements.
- Bug reports and reproducible test cases.
- Documentation and installation instructions.
- Testing on Android (Termux) and Linux.
- English and Kiswahili demonstration materials.
- African language research and linguistic review.
- Retrieval evaluation and benchmarking.
- Accessibility and user-interface research.
- Responsible data governance and security improvements.

## 2. Before Contributing

Please:

1. Read the project README.
2. Review `docs/ARCHITECTURE.md`.
3. Review `docs/DATA_GOVERNANCE.md`.
4. Review `docs/TESTING.md`.
5. Check existing issues and pull requests before starting substantial work.

## 3. Development Principles

Contributions should follow these principles:

- Keep the project offline-first where practical.
- Respect user privacy and local data ownership.
- Avoid unnecessary dependencies.
- Preserve source attribution and provenance.
- Use clear, maintainable and testable code.
- Document known limitations.
- Avoid claiming that planned features are already implemented.

## 4. Testing

Run the project's tests before submitting a contribution:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

For changes affecting the retrieval demonstration, also run:

```bash
./build/dukeai_demo data/demo
```

Include the platform and relevant test results in your pull request.

## 5. Data and Copyright Requirements

Contributors must not submit material unless they have the right to share it.

For external datasets, books, documents, translations, images, recordings, and cultural resources, provide relevant information about:

- Original source.
- Author or creator, where known.
- Copyright holder, where known.
- Licence or permission status.
- Attribution requirements.
- Redistribution permissions.
- Whether public model-training use is permitted.

Do not assume that material available online is free to redistribute or use for model training.

Respect community ownership, cultural protocols, consent, and restrictions on sensitive or sacred knowledge.

## 6. Pull Request Guidelines

A good pull request should:

- Explain the problem being addressed.
- Describe the proposed changes.
- Include relevant tests.
- Identify new dependencies.
- Explain any compatibility implications.
- Document known limitations.
- Include attribution for third-party materials where applicable.

Keep changes focused and avoid unrelated modifications.

## 7. Reporting Bugs

When reporting a bug, include:

- Operating system and environment.
- Build tools and compiler versions.
- Steps to reproduce the problem.
- Expected behaviour.
- Actual behaviour.
- Relevant error messages.
- Test results, where available.

Do not include passwords, access tokens, private documents, or personal information.

## 8. Community Participation

We value respectful collaboration across different backgrounds, languages, disciplines, and communities.

Contributions involving African languages and cultural heritage should prioritise accuracy, attribution, community participation, and responsible representation.

## 9. Code of Conduct

All contributors are expected to follow the project's `CODE_OF_CONDUCT.md`.

## 10. Security Issues

Please follow `SECURITY.md` when reporting security vulnerabilities.

Thank you for helping develop DukeAI Nexus.

**Our Languages. Our Heritage. Our Future.**
