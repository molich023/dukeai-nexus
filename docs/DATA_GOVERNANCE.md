# DukeAI Nexus Data Governance

## 1. Purpose

This document defines the principles and procedures for handling documents, datasets, language resources, cultural materials, and other information used in DukeAI Nexus.

The project aims to support responsible African AI research while respecting copyright, privacy, community ownership, and the rights of original creators.

## 2. Core Principles

DukeAI Nexus follows these principles:

- Respect for copyright and intellectual property.
- Clear source attribution.
- Documented licensing and permission status.
- Privacy-conscious local processing.
- Responsible handling of personal information.
- Community participation in language-resource development.
- Respect for cultural protocols and restricted knowledge.
- Transparency about data limitations.
- No assumption that publicly accessible information is freely reusable.

## 3. Data Categories

Project materials should be classified into appropriate categories.

### 3.1 Developer-Created Materials

These include original demonstration documents, test fixtures, and project documentation created specifically for DukeAI Nexus.

Their publication is subject to the applicable project licence and any third-party content incorporated into them.

### 3.2 Publicly Available External Materials

Public availability does not automatically grant permission to:

- Copy or redistribute a document.
- Modify or translate it.
- Include it in a dataset.
- Use it for model training.
- Publish extracted text.
- Commercially reuse it.

Every external resource must undergo rights review.

### 3.3 Personal and Sensitive Information

Personal information should not be collected unless there is a clear, legitimate and documented purpose.

Sensitive information must not be placed in public repositories without an appropriate legal basis and necessary safeguards.

### 3.4 Community and Cultural Knowledge

Language and cultural resources may be associated with individual creators, communities, families, institutions, or traditional knowledge holders.

Such resources must not automatically be treated as unrestricted public data.

## 4. Document Metadata

Documents should have structured metadata where appropriate.

Recommended metadata includes:

- Document identifier.
- Title.
- Author or creator.
- Original source.
- Publication date, where known.
- Language.
- Subject.
- Copyright holder, where known.
- Licence.
- Rights status.
- Redistribution permission.
- Public model-training permission.
- Attribution requirements.
- Review status.

Metadata must not claim permissions that have not been verified.

## 5. Rights and Permission States

The project should distinguish between the following states:

| Status | Meaning |
|---|---|
| VERIFIED | Rights and permissions have been reviewed and documented |
| UNVERIFIED | Permission status has not been established |
| RESTRICTED | Use is limited by licence, permission or other conditions |
| PROHIBITED | The proposed use is not authorised |

A document marked UNVERIFIED must not be treated as approved for public redistribution or public model training.

## 6. Separate Permission Decisions

Permission to access a document does not necessarily grant permission to redistribute it.

Permission to redistribute does not necessarily grant permission to use it for model training.

The following decisions must be considered separately:

1. Local research access.
2. Local document processing.
3. Public redistribution.
4. Modification or translation.
5. Commercial use.
6. Public model-training use.

## 7. Copyright and Licensing

Before adding an external resource to the repository, contributors must establish:

- Who created or owns the material.
- Whether the material is protected by copyright.
- Which licence applies.
- Whether redistribution is permitted.
- Whether modification is permitted.
- Whether training use is permitted.
- Whether attribution is required.
- Whether additional permissions are necessary.

Historical publication dates alone are not sufficient proof that a modern edition, scan, transcription, or translation is free of copyright restrictions.

The MIT licence for DukeAI Nexus software does not automatically apply to external documents or datasets.

## 8. African Languages and Cultural Knowledge

DukeAI Nexus recognises that African languages and cultural knowledge require responsible stewardship.

Contributors should:

- Identify language varieties accurately.
- Consult knowledgeable speakers and language practitioners.
- Respect dialectal and regional differences.
- Avoid treating one language variety as universally representative.
- Obtain appropriate permission for recordings and contributed materials.
- Provide attribution where appropriate.
- Respect community restrictions and cultural protocols.
- Avoid publishing sacred, restricted or sensitive knowledge without authorisation.

Community participation should be meaningful rather than merely symbolic.

## 9. Privacy and Offline Processing

The project prioritises local document processing.

The intended approach is to minimise unnecessary transmission of user documents to external services.

Future network-connected features must document:

- What information is transmitted.
- Why transmission is necessary.
- Which services receive the information.
- How information is retained.
- How users can control or disable the feature.

The current prototype should not be described as independently certified for privacy or security.

## 10. Data Storage and Repository Rules

The public repository should contain only materials approved for publication.

Private books, extracted research documents, credentials, personal data, and unverified external resources must remain outside the public repository.

The project's `.gitignore` uses default-deny rules for local datasets, with an explicit allowance for reviewed demonstration materials.

## 11. Review and Approval

Before adding new materials:

1. Identify the source.
2. Determine the copyright holder, where possible.
3. Review the applicable licence.
4. Determine the permitted uses.
5. Record attribution requirements.
6. Assess privacy and cultural considerations.
7. Record the decision in metadata.
8. Review the proposed public files before committing.

Unresolved rights questions must remain documented as unresolved.

## 12. Corrections and Removal Requests

Creators, rights holders, contributors, and affected communities may request correction, attribution updates, or removal of material.

Requests should be reviewed respectfully and promptly.

Where a material rights or privacy concern is identified, public distribution should be suspended while the issue is investigated.

## 13. Future Governance Work

Future work may include:

- A documented dataset review process.
- Community consultation procedures.
- Contributor consent records.
- Language-resource quality standards.
- Dataset versioning and provenance.
- Independent governance review.
- Clear procedures for removal and correction requests.

## 14. Commitment

DukeAI Nexus aims to develop technology that respects the people, languages, creators, and communities whose knowledge makes the research possible.

Responsible data governance is a core project requirement, not an optional addition.

**Our Languages. Our Heritage. Our Future.**
