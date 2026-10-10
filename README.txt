PVM OOT experiment evidence, published 2026-10-10

This independent evidence branch contains the reviewed public artifacts for
the host nopku, guest PKRU, guest MPK configuration, and DEBUGCTL reports.
The ZIP bundles are byte-for-byte the reviewed preparation artifacts.
manifest.json finalizes publication metadata and lists their SHA-256 hashes
and every enclosed file hash. Each ZIP also includes README.txt and SHA256SUMS.

host-nopku.zip       Host RDPKRU invalid-opcode evidence and OSPKE guard controls.
guest-pkru.zip       Four native/guest pkey rounds and standalone reproducer.
guest-mpk-config.zip Guest XCR0/XSETBV controls and inspected example config.
debugctl.zip        DEBUGCTL A/B timings, 36 complete decoded traces, controls.

pkru-ospke.patch, debugctl-backend-scope.patch, and pkey-smoke.c are also
provided as separate small files for reading and downloading.

Measurements use historical PR #29 source commit
2fc0196a2ebd67261c9cf4f2c3e504516bb5e0bc, not the reorganized current PR head.
The bundles include source/build provenance and the recorded reproduction
procedures with their harness sources. These describe the measured lab;
the complete decoded perf text is included, while native perf.data is
retained in the original lab archive. Vendor backend validation and the
guest PKRU scheduling root cause remain limited as stated in the reports.

For immutable downloads, use raw.githubusercontent.com/hky1999/linux/
followed by this evidence commit SHA and the filename. The evidence commit
has only artifact files and does not change the PR #29 compatibility branch.
