# DukeAI Nexus — Security Guardrails

**Document type:** Security architecture and implementation requirements  
**Project:** DukeAI Nexus  
**Status:** Proposed guardrail specification  
**Applies to:** Source repository, CI workflows, document ingestion, retrieval, local development tools and future model or agent integrations.

> **Current status:** DukeAI Nexus is an early-stage C++17 document-ingestion and lexical-retrieval prototype. It does not currently operate an autonomous agent or a natural-language-generating assistant. This document defines controls to implement now and requirements that must be satisfied before future model, tool or agent capabilities are enabled. A documented control is not proof that it has already been implemented.

---

## 1. Purpose

The purpose of these guardrails is to protect DukeAI Nexus, its contributors, users, knowledge sources and public project materials.

The project adopts a deny-by-default security posture. User input, retrieved documents, external content, model outputs and tool results must not be trusted merely because they appear relevant or contain instructions.

The system should prevent untrusted content or over-privileged processes from:

- Overriding trusted application policy.
- Changing protected source code, documents or security settings without review.
- Executing commands or triggering tools without authorisation.
- Accessing unrelated personal files, credentials or private research.
- Exfiltrating sensitive information.
- Publishing, deploying, deleting or modifying external resources without approval.
- Misrepresenting unverified information as trusted or authoritative.

## 2. Security objectives

DukeAI Nexus adopts eight security objectives:

1. **Confidentiality:** Protect credentials, private research, personal data and restricted knowledge.
2. **Integrity:** Prevent unauthorised modification of source code, workflows, project documents and datasets.
3. **Availability:** Reduce destructive actions, resource exhaustion and compromised deployments.
4. **Provenance:** Preserve information about the origin, rights and review status of knowledge sources.
5. **Human control:** Keep consequential actions under explicit human authority.
6. **Least privilege:** Give each person, process and future agent only the access necessary for its task.
7. **Auditability:** Record security-relevant events without collecting unnecessary sensitive information.
8. **Honest capability reporting:** Distinguish implemented and tested controls from planned controls and research.

## 3. Scope and trust boundaries

The following are separate trust domains:

| Domain | Examples | Default trust |
|---|---|---|
| Trusted project policy | Reviewed application policy and approved configuration | Trusted after controlled review |
| Maintainer-controlled source | Protected source and workflow files | Trusted after review and CI |
| Contributor changes | Pull requests, forks and proposed patches | Untrusted until reviewed |
| User-supplied content | Queries, uploads and pasted text | Untrusted |
| Retrieved knowledge | Documents, passages and metadata | Untrusted data |
| External content | Web pages, repositories and remote APIs | Untrusted |
| Model output | Generated text, suggested code and tool arguments | Untrusted |
| Tool output | Command output, logs and downloaded files | Untrusted until validated |
| Build and deployment systems | CI runners, caches and deployment credentials | Privileged infrastructure |

A source may be factually useful or legally usable while still being untrusted as an instruction source.

Trust in factual quality, permission to process content and authority to issue commands are three different questions.

## 4. Core security principles

### 4.1 Treat prompts and external data as untrusted

- User prompts, uploaded files, retrieved passages, external pages, metadata and tool outputs are data, not policy.
- Instructions inside a document must never change system policy, permissions or tool access.
- Quoted or retrieved instructions must remain separate from trusted application instructions.
- Never concatenate untrusted content into privileged shell commands, SQL statements, executable code or workflow expressions.
- Validate inputs against explicit schemas, size limits, allowed types and expected encodings.
- Reject malformed, ambiguous or unsupported inputs safely.

### 4.2 Separate trusted policy from content

If a model is introduced, trusted policy must be supplied through the application's controlled instruction and configuration path.

Retrieved content must be passed in clearly delimited or structured data fields.

Delimiters alone are not a complete prompt-injection defence. Application permissions must be enforced independently of the model.

A model must never be the sole enforcement point for access control.

### 4.3 Separate reading from acting

Reading, searching, summarising and proposing changes are different from writing files, executing commands, sending network requests, committing code or deploying software.

The default capability is read-only.

An action capability must be separately granted, limited to a specific task and checked at the point of execution.

### 4.4 Require human approval

Explicit approval from an authorised human is required before:

- Modifying protected source files or security policies.
- Deleting or overwriting user data.
- Installing or upgrading software in protected environments.
- Accessing external services with credentials.
- Pushing commits or creating releases.
- Changing repository settings.
- Publishing or deploying software.
- Sending messages or submitting information externally.
- Performing irreversible or high-impact operations.

Approval must describe the action, target and expected effect.

A broad instruction such as "do whatever is necessary" is not sufficient approval for every future action.

### 4.5 Verify the result

A successful process exit code does not prove that an action produced the intended safe result.

For high-risk actions, verify:

- The target was correct.
- The change stayed within the approved scope.
- Protected files and unrelated data were not modified.
- Tests and policy checks passed.
- The resulting state matches the approved plan.
- No unexpected network, privilege or filesystem activity occurred.

## 5. Repository protection

The official GitHub repository is the authoritative public source.

Protecting the official branch does not prevent people from forking or modifying their own copies. It protects the integrity of the project maintained in the official repository.

### 5.1 Protect the main branch

Configure GitHub repository rules or branch protection for `main`:

- Require pull requests before merging.
- Require approval from an authorised reviewer where the repository structure permits.
- Require relevant CI checks to pass.
- Require approval of the most recent reviewable push where supported.
- Dismiss stale approvals when new commits materially change reviewed code.
- Require code-owner review for protected paths where supported.
- Block force pushes and branch deletion.
- Restrict direct pushes to authorised maintainers.
- Apply equivalent protection to release branches and tags where used.

A solo maintainer should not create a false impression of independent review. Where independent review is unavailable, record that limitation and use automated checks plus deliberate maintainer review.

### 5.2 Protect critical files with CODEOWNERS

Create `.github/CODEOWNERS` and assign an actual maintainer account or team that has repository access.

Suggested protected paths:

```text
/.github/CODEOWNERS
/.github/workflows/          @YOUR_GITHUB_USERNAME
/CMakeLists.txt              @YOUR_GITHUB_USERNAME
/include/                    @YOUR_GITHUB_USERNAME
/src/                        @YOUR_GITHUB_USERNAME
/tests/                      @YOUR_GITHUB_USERNAME
/docs/SECURITY_GUARDRAILS.md @YOUR_GITHUB_USERNAME
/docs/SECURITY_TEST_PLAN.md  @YOUR_GITHUB_USERNAME
```

Replace `@YOUR_GITHUB_USERNAME` with the real authorised GitHub username.

CODEOWNERS only creates an effective review requirement when repository rules require code-owner approval.

### 5.3 Protect project documents and data

- Treat the project vision, pitch, security policy, governance documents and assessment evidence as maintainer-controlled content.
- Require review before changing these documents on the official branch.
- Keep private research, unlicensed books, extracted copyrighted text, secrets, personal records and local model files out of the public repository unless publication is explicitly authorised.
- Maintain a default-deny data policy.
- Allowlist only intended public demonstration data.
- Review additions to `data/`, `docs/` and release assets for rights, provenance and sensitive content.
- Do not assume that a file is safe to publish because it exists in a local working directory.

## 6. GitHub Actions and workflow security

CI workflows are privileged automation and must be treated as part of the software supply chain.

### 6.1 Minimum permissions

- Set workflow-level permissions to `contents: read` when only checkout and build/test are required.
- Do not grant `write-all`.
- Grant additional permissions only to the specific job that needs them.
- Do not expose deployment credentials to build-and-test jobs.
- Do not use repository secrets in workflows that execute untrusted pull-request code.
- Avoid `pull_request_target` for workflows that check out or execute code from untrusted pull requests.
- Prefer the ordinary `pull_request` event for contribution validation.

### 6.2 Untrusted pull requests

Treat pull-request titles, descriptions, branch names, commit messages, changed files, artifacts and source code as untrusted.

- Never evaluate pull-request text as shell code.
- Do not interpolate untrusted values directly into `run:` shell commands.
- Pass values through safely quoted environment variables and validate them.
- Do not check out untrusted code in privileged workflows with write permissions or secrets.
- Do not let pull requests control workflows that receive privileged credentials.
- Review artifact and cache handling to prevent untrusted jobs from poisoning outputs later consumed by privileged jobs.
- Separate untrusted test jobs from release and deployment jobs.

### 6.3 Pin and review third-party actions

- Pin third-party GitHub Actions to verified full-length commit SHAs.
- Use version tags only when a deliberate, documented risk decision permits it.
- Review action source, maintainer, permissions and release history.
- Update pinned versions through reviewed changes.
- Minimise third-party actions.

### 6.4 Build artifacts, caches and dependencies

- Do not execute downloaded artifacts without validation.
- Do not allow untrusted pull-request jobs to populate caches later used by privileged release jobs.
- Use narrow cache keys and isolate trust domains.
- Pin or constrain dependencies where practical.
- Use lockfiles when supported.
- Generate and retain build/test evidence for releases.
- Consider a software bill of materials (SBOM) and dependency vulnerability checks as the release process matures.

### 6.5 Deployment separation

Website deployment must be separate from ordinary build-and-test CI.

- Keep Cloudflare deployment credentials out of ordinary test jobs.
- Use narrowly scoped deployment credentials.
- Use protected environment approvals where available.
- Deploy only from the protected production branch.
- Use preview deployments for review where supported.
- Verify the deployed commit and published files.
- Maintain rollback instructions and the ability to disable or revert a deployment.

## 7. Document ingestion and retrieval security

The current prototype processes documents and metadata. The following controls apply directly to this boundary.

### 7.1 Input validation

- Enforce documented file-size limits.
- Validate file types and encoding.
- Reject malformed JSON, duplicate keys, unexpected fields and invalid field types.
- Reject path traversal attempts and unsafe file paths.
- Do not trust filenames or extensions as proof of content type.
- Use safe temporary-file handling and clean up temporary files.
- Avoid following unexpected symbolic links when processing untrusted paths.
- Fail safely with useful errors that do not disclose secrets or private filesystem details.

### 7.2 Source provenance and rights

Each source should have enough metadata to identify its origin and review state.

As applicable, record:

- Source identifier, title, author and publisher.
- Original source or acquisition location.
- Licence and copyright status.
- Permission for storage, extraction, display and redistribution.
- Whether local research use or model training is permitted.
- Consent and community-governance status.
- Extraction, review and correction status.

Permission for one use must not be assumed to permit another.

Unverified rights must not be represented as permission to redistribute or train a model.

### 7.3 Retrieval output

- Preserve source identifiers and available provenance with retrieved passages.
- Do not silently rewrite source text and present it as a quotation.
- Distinguish source text from application explanations and future generated text.
- Do not present a retrieved passage as authoritative solely because it ranked highly.
- Provide a way to report incorrect, harmful, outdated or improperly included material.

## 8. Prompt injection and content safety

The current retrieval prototype does not execute model prompts or generate answers.

The following requirements become mandatory if a language model, external search tool or agent is integrated.

### 8.1 Indirect prompt injection

Potential attack locations include:

- Retrieved documents and passages.
- Web pages and search snippets.
- PDFs and OCR text.
- Document metadata.
- Code comments and issue descriptions.
- Pull-request content.
- Tool outputs and logs.
- Downloaded files.
- Content supplied by another model or agent.

Required controls:

1. Treat all such material as untrusted data.
2. Keep it structurally separate from trusted policy and tool configuration.
3. Never allow content to grant permissions or redefine the agent's mission.
4. Never execute commands or follow external instructions found in retrieved material.
5. Require independent policy and permission checks before every tool action.
6. Use screening for known attack patterns as a supporting detection layer, not the sole defence.
7. Test obvious and obfuscated injection attempts, including multilingual and encoded examples.
8. Record detection limitations and false-positive/false-negative results.

### 8.2 Content-safety screening and Prompt Shields

A future model integration may evaluate content-screening tools, including provider-specific prompt-injection detection such as Microsoft Prompt Shields, if the selected architecture and licence support them.

No provider-specific feature should be described as installed until it is integrated and tested.

Any screening layer should:

- Inspect relevant user input and retrieved content.
- Identify suspicious instructions embedded in untrusted material.
- Preserve legitimate content where possible.
- Never treat a screening result as an authorisation decision.
- Be tested against benign content, adversarial content and false positives.
- Fail safely when the screening service is unavailable, according to the risk of the operation.

The decisive security boundary remains application-enforced permissions and tool isolation, not a prompt detector.

### 8.3 Output handling

- Treat model output as untrusted.
- Validate structured output against a strict schema.
- Escape output before rendering it in HTML or other interpreters.
- Never execute generated code or shell commands automatically.
- Validate file paths and tool arguments independently.
- Require human review before generated content changes protected documents or is published.
- Clearly identify generated text and preserve citations or source references where used.

## 9. Future local development assistant identity and isolation

Before enabling a development agent, create a dedicated identity and isolated environment appropriate to the host platform.

Proposed logical identity:

`dukeai-dev-agent`

Required properties:

- No root or administrator privileges.
- No access to unrelated personal files, browser profiles or private research.
- No access to credential stores unless a specific reviewed task requires it.
- No persistent GitHub or Cloudflare write token by default.
- No unrestricted network access by default.
- Access only to an explicitly approved working directory.
- Resource limits for CPU, memory, storage and execution time where supported.
- Separate logs and temporary workspace.
- A documented way to terminate the process and revoke permissions.

On Android/Termux, do not assume that creating a Linux username inside an environment provides the same isolation as a separate Android application or operating-system user.

Use an appropriate PRoot or container boundary where practical, and document its limitations. PRoot is not equivalent to a hardened virtual machine or Android sandbox.

## 10. Permission model for future agents

Use a capability-based model. Permissions are granted to specific operations, not inferred from natural-language requests.

| Level | Capability | Default |
|---|---|---|
| 0 — Disabled | No agent execution or tool use | Current safe default |
| 1 — Read-only | Read approved project files and knowledge sources | First capability to evaluate |
| 2 — Propose | Prepare patches, commands or plans without executing them | Human reviews proposal |
| 3 — Approved write | Apply a narrowly scoped, explicitly approved change | Approval per task |
| 4 — External/privileged | Push, deploy, delete, install, send or change external state | Disabled unless separately authorised |

Enforcement requirements:

- Deny all capabilities not explicitly allowlisted.
- Check permissions at the tool boundary, not only in the prompt.
- Scope each grant to the task, resource and duration.
- Expire temporary permissions where feasible.
- Log grants, denials, approvals and outcomes.
- Do not allow an agent to approve its own request.
- Do not allow retrieved content or model output to expand permissions.
- Revoke permissions after the task completes or when trust is reduced.

## 11. High-risk action approval and verification

For every high-risk action, use a controlled sequence:

1. **Declare:** Identify the requested action, target, reason and expected effect.
2. **Validate:** Check the target, input, scope and applicable policy.
3. **Authorise:** Obtain explicit approval from an authorised human.
4. **Execute:** Perform only the approved action using minimum required permission.
5. **Verify:** Inspect the resulting state and confirm the change stayed within scope.
6. **Record:** Preserve an audit event and relevant test evidence.
7. **Recover:** Roll back or stop if the result differs from the approved plan.

Actions affecting external systems, protected repository content, credentials, publication or irreversible data should receive the strongest review.

## 12. Monitoring and audit

Before agent deployment, define monitoring for:

- Repeated denied actions and permission-boundary violations.
- Unexpected filesystem reads or writes.
- Attempts to access secrets or unrelated directories.
- Unexpected network destinations or data transfer.
- Privilege escalation attempts.
- Repeated malformed or suspicious inputs.
- Unexpected tool invocation patterns.
- Changes to protected source, workflow or policy files.
- Deployment and release events.

Audit records should capture the minimum information necessary to reconstruct security-relevant events:

- Timestamp.
- Actor or process identity.
- Action and target.
- Permission decision.
- Approval reference.
- Outcome.
- Verification result.

Do not store passwords, access tokens, private keys or unnecessary sensitive user content in logs.

Define retention and access controls for audit records.

Monitoring is not a substitute for prevention. Alerts must have a defined response process.

## 13. Kill switch and incident response

A kill switch must exist and be tested before any autonomous or tool-using agent is deployed.

Suggested default configuration:

```text
DUKEAI_AGENT_ENABLED=false
DUKEAI_AGENT_ACTIONS_ENABLED=false
DUKEAI_EXTERNAL_NETWORK_ENABLED=false
DUKEAI_PRIVILEGED_ACTIONS_ENABLED=false
```

The application must enforce these settings outside the model.

If agent execution is disabled, a model response must not be accepted as permission to execute an action.

The kill switch should:

- Stop new agent tasks.
- Block new tool invocations.
- Prevent privileged and external actions.
- Revoke or disable temporary credentials where feasible.
- Preserve security logs needed for investigation.
- Clearly report that the agent is disabled.
- Require authorised human action to re-enable capabilities.

A configuration flag is not a complete kill switch unless every relevant execution path checks it and tests prove that actions are blocked.

Incident response should include:

1. Stop affected tasks.
2. Revoke credentials.
3. Isolate the affected environment.
4. Preserve relevant evidence.
5. Assess data exposure and repository changes.
6. Restore a known-good state.
7. Notify affected parties where required.
8. Document corrective actions.

## 14. Implementation priorities

### Implement or verify now

- Keep CI permissions minimal.
- Use safe pull-request triggers.
- Avoid privileged execution of untrusted contributions.
- Protect `main`, critical workflow files and security documents.
- Review third-party workflow actions and pin verified SHAs.
- Keep secrets and deployment credentials out of test jobs.
- Maintain the default-deny policy for private data.
- Maintain document metadata validation and malformed-input tests.
- Document provenance and rights requirements.
- Record what is actually implemented and tested.

### Required before model integration

- Explicit trusted-policy and untrusted-content separation.
- Prompt-injection and content-safety evaluation.
- Strict output and tool-argument validation.
- Application-enforced permission checks.
- Human approval for consequential actions.
- Logging, monitoring and incident-response procedures.
- A tested kill switch.

### Required before autonomous agent deployment

- Dedicated isolated identity and workspace.
- Least-privilege capability enforcement.
- Read-only default and separate write capability.
- No default access to credentials or external network.
- Approval and verification for high-risk actions.
- Tested emergency stop and credential revocation.
- Security review and documented residual risks.

## 15. Security acceptance criteria

A control should be marked implemented only when there is evidence that it exists.

A control should be marked tested only when a repeatable test has passed.

Minimum acceptance criteria before enabling a tool-using agent:

- An untrusted document cannot change application policy or grant tool permissions.
- A read-only agent cannot write, delete, install, push or deploy.
- A denied operation is blocked at the application/tool boundary.
- No untrusted pull-request workflow can access deployment credentials.
- Protected repository changes require configured review and CI controls.
- High-risk actions cannot proceed without explicit human approval.
- Action results are checked against the approved target and scope.
- The kill switch blocks new tasks and tool actions.
- Logs do not expose secrets.
- Known limitations and residual risks are documented.

## 16. Limitations and residual risks

No single prompt filter, test suite, branch rule or model can eliminate all security risk.

Potential residual risks include:

- Compromised maintainer accounts.
- Malicious dependencies.
- Vulnerabilities in the operating system or build environment.
- Novel prompt-injection techniques.
- Incorrect rights assessments.
- Supply-chain compromise.
- Human approval errors.
- Failures in third-party services.

DukeAI Nexus should use defence in depth, independent review where possible, timely updates, clear incident procedures and honest reporting of limitations.

## 17. References and alignment

This document is informed by established security engineering concepts and should be reviewed against current versions of relevant guidance, including:

- OWASP Top 10 for Large Language Model Applications.
- OWASP guidance on prompt injection and excessive agency.
- NIST AI Risk Management Framework.
- NIST Secure Software Development Framework (SSDF).
- GitHub documentation on Actions security hardening, repository rulesets and CODEOWNERS.
- Cloudflare documentation on Pages deployment and credential management.

These references are guidance sources, not a certification or claim of compliance.

Any formal compliance statement requires a scoped assessment and supporting evidence.

## 18. Change control

Changes to this security document should be reviewed alongside relevant source code, workflow or architecture changes.

For each material change, record:

- The threat or requirement being addressed.
- The files and components affected.
- The implementation owner.
- The tests and evidence.
- Known limitations and residual risk.
- The approval and review date.

---

**Security principle:** DukeAI Nexus should earn trust through constrained permissions, verifiable behaviour, transparent governance and repeatable evidence—not through claims that an AI system is inherently trustworthy.

**Our Languages. Our Heritage. Our Future.**
