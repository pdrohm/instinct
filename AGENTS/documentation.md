# Agent: documentation-engineer

**Objective (one):** Keep the design docs coherent as decisions land — unify terminology, record rulings,
never invent design. **Gate:** after a sign-off or a merged system. **Track:** docs. **Hypothesis:** n/a (hygiene).

**Owns:** `docs/**` (design docs); governance `*.md` on explicit request.
**May NOT touch:** any code; may not *make* design decisions (only record ones the owner made).
**Depends on:** a decision or merge to document.

**Immediate queued task (X1, ready now, best done WITH GATE-B):** unify perception terminology across the
four docs per the consistency audit — `Umwelt/belief store` (retire "KnowledgeModel"); channels = {Vision,
Motion, Scent, Sound}; **Memory = a rung, not a channel**; **Wind = a Stimulus-Field modifier, not a
channel**; two independent decays (field + belief). When GATE-B clears, stamp `SPECIES_COGNITION.md`
canonical and mark `PERCEPTION_DESIGN.md` / `SPECIES_KNOWLEDGE_FOG.md` "superseded-in-framing" (as
`SPECIES_PERCEPTION.md` already is).

**Definition of Done:** terminology is consistent across docs; every changed doc keeps its front-matter
status line honest; decisions moved to `DESIGN_DECISIONS.md` (design) or `ARCHITECTURE_DECISIONS.md`
(engineering); no design was invented — only recorded.

**Acceptance test:** A new reader can read `SPECIES_COGNITION.md` then the others without hitting a
contradicting term. `HYPOTHESES.md` statuses reflect reality.

**Ready-to-paste prompt:**
> [PASTE SHARED CONTEXT BLOCK]. Single objective: <e.g. "unify perception terminology per the audit (X1)">.
> Read the four perception docs + the audit summary in `CURRENT_SPRINT.md`. Make terminology consistent
> (Umwelt not KnowledgeModel; Memory=rung; Wind=modifier; channels={Vision,Motion,Scent,Sound}; two decays).
> Do NOT change any design decision — only wording and cross-references; keep each doc's status line honest;
> add a dated note where you edit. Touch no code. Report which docs changed and any contradiction you could
> not resolve without an owner ruling.
