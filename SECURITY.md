# Security Policy

## DukeAI Nexus Security

DukeAI Nexus takes software security, privacy, data integrity, and responsible development seriously.

The project is under active development. Its security controls and capabilities should not be assumed to be production-ready.

## Supported Versions

At this early research stage, security fixes are primarily considered for the latest development version on the `main` branch.

Other versions may not receive security updates.

## Reporting a Vulnerability

Please do not publish sensitive vulnerability details in a public issue before coordinating with the maintainers.

Use GitHub's private vulnerability reporting feature if it is enabled for this repository.

If private vulnerability reporting is unavailable, contact the project maintainers privately through an available GitHub contact channel.

Please include:

- A description of the vulnerability.
- Affected component or file.
- Steps to reproduce the issue.
- Potential security impact.
- Relevant logs or proof-of-concept material, where safe.
- Suggested mitigation, if known.

Do not include real passwords, access tokens, private keys, personal data, or confidential documents in a report.

## Security Development Principles

DukeAI Nexus aims to follow these principles:

- Offline-first operation where practical.
- Minimal external dependencies.
- Secure handling of local files.
- Input validation and bounded resource consumption.
- Clear source provenance and metadata validation.
- Protection against accidental disclosure of private data.
- Transparent security limitations.
- Reproducible testing and responsible vulnerability handling.

## Data Protection

Contributors must not commit:

- API keys or access tokens.
- Passwords or authentication credentials.
- Private encryption keys.
- Personal or confidential information.
- Unauthorised copyrighted research materials.
- Private datasets or documents without permission.

External documents and datasets must be reviewed under `docs/DATA_GOVERNANCE.md`.

## Security Limitations

DukeAI Nexus is a research prototype.

The current implementation should not be treated as:

- A certified secure system.
- A production-ready AI service.
- A replacement for professional security review.
- A guarantee that retrieved documents are accurate or trustworthy.
- A guarantee that all local data is protected against unauthorised access.

Security testing and independent review will be expanded as the project develops.

## Responsible Disclosure

We appreciate responsible security research and coordinated disclosure.

Please allow maintainers reasonable time to investigate and address a vulnerability before public disclosure.

## Security Updates

Security-related changes and fixes will be documented through relevant GitHub commits, releases, or security advisories.

Thank you for helping keep DukeAI Nexus and its users safe.
