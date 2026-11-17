# DukeAI Nexus Resource Inventory

## 1. Purpose

This document records the status of research materials identified for possible use in DukeAI Nexus.

The inventory distinguishes between materials that have been identified, materials that have been reviewed, and materials approved for redistribution or model training.

Listing a resource here does not mean that it has been ingested, indexed, redistributed, or used for model training.

## 2. Resource Governance

Every external resource must be assessed for:

- Source and provenance.
- Author or creator.
- Copyright holder.
- Publication details.
- Applicable licence.
- Local research-use permissions.
- Redistribution permissions.
- Modification and translation permissions.
- Public model-training permissions.
- Attribution requirements.
- Cultural and community considerations.

Permission for one type of use must not be assumed to grant permission for another.

## 3. Identified Research Resources

The following resources have been identified during project research.

| Resource | Language or subject | Status |
|---|---|---|
| Collins English Dictionary | English language | Copyright review required |
| TUKI English-Swahili Dictionary | English and Kiswahili | Rights unverified |
| Kikamba Self-Instruction Manual | Kikamba | Rights unverified |
| English-Kikuyu Vocabulary by Arthur Wallace McGregor (1904) | English and Kikuyu | Historical work; edition and jurisdiction review required |
| Theory of Computer Science reference document | Computer science | Rights unverified |

## 4. Resource Status

### 4.1 Collins English Dictionary

Identified as a possible English-language reference.

Copyright and licensing restrictions must be respected.

No public redistribution or model-training permission is assumed.

### 4.2 TUKI English-Swahili Dictionary

Identified as a possible English-Kiswahili language resource.

Rights and permitted uses have not been established.

Do not redistribute or use for public model training without appropriate permission.

### 4.3 Kikamba Self-Instruction Manual

Identified as a possible Kikamba language resource.

Rights and permitted uses require verification.

Community and linguistic review may also be appropriate before any future use.

### 4.4 English-Kikuyu Vocabulary by Arthur Wallace McGregor (1904)

Identified as a historical language resource.

Its historical publication date may be relevant to a copyright assessment, but does not independently establish that a particular scan, edition, transcription, or digitisation is free of restrictions.

Rights and edition-specific status must be reviewed.

### 4.5 Theory of Computer Science Reference Document

A local research document has been identified.

Its metadata currently records an UNVERIFIED rights status.

It must not be treated as approved for public redistribution or public model training.

## 5. Demonstration Materials

The project includes developer-authored English and Kiswahili water-conservation demonstration material under:

```text
data/demo/
```

These files support the current local retrieval demonstration.

They are distinct from the external research books and dictionaries listed above.

Before publishing demonstration material, review it to ensure it does not reproduce protected third-party passages.

## 6. Ingestion and Training Status

The presence of an identified resource does not establish that it has been ingested or used.

The inventory must distinguish:

- Identified.
- Acquired.
- Rights reviewed.
- Approved for local use.
- Approved for redistribution.
- Approved for public model training.
- Ingested.
- Evaluated.

No resource should be described as approved for a particular use unless the relevant permission has been documented.

## 7. Repository Handling

Private research materials must not be uploaded to the public repository without appropriate permissions.

In particular, the following local directories should remain excluded unless their contents have been individually reviewed and approved:

```text
data/books/
data/extracted/
```

Metadata may also contain sensitive or rights-related information and should be reviewed before publication.

## 8. Future Inventory Improvements

Future versions of this inventory should include:

- Stable resource identifiers.
- Exact edition and publication information.
- Source URLs where appropriate.
- Rights-holder contact information where appropriate.
- Licence evidence.
- Permission records.
- Review dates.
- Ingestion status.
- Dataset version information.
- Removal and correction history.

## 9. Commitment

DukeAI Nexus aims to develop a responsible research collection in which provenance, copyright, permission, and community considerations are documented.

**No resource should be treated as free to redistribute or use for model training merely because it is accessible online or appears in this inventory.**
