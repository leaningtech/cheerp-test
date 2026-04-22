#!/usr/bin/env python3
"""Determinism check for Cheerp lit tests.

Compares two already-populated Outputs/ trees and reports whether the
per-test artifact directories match byte-for-byte. On mismatch, both trees'
copies are saved under determinism_failures_compile_only/ for inspection.

This script does NOT run the compiler or lit itself. Orchestration of the
two lit passes lives in run_lit_tests.py; the role of this script is
purely to diff the produced artifacts.
"""

from __future__ import annotations

import argparse
import hashlib
import re
import shutil
import sys
from pathlib import Path


def _md5_file(path: Path) -> str:
    h = hashlib.md5()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def _build_manifest(tmp_dir: Path) -> dict[str, str]:
    manifest: dict[str, str] = {}
    for p in sorted(tmp_dir.rglob("*")):
        if not p.is_file():
            continue
        rel = str(p.relative_to(tmp_dir))
        manifest[rel] = _md5_file(p)
    return manifest


def _summarize_diff(a: dict[str, str], b: dict[str, str], limit: int = 10) -> str:
    ka, kb = set(a), set(b)
    missing = sorted(ka - kb)
    extra = sorted(kb - ka)
    changed = sorted(k for k in (ka & kb) if a[k] != b[k])
    parts: list[str] = []
    if missing:
        parts.append("missing in B: " + ", ".join(missing[:limit]))
    if extra:
        parts.append("extra in B: " + ", ".join(extra[:limit]))
    if changed:
        parts.append("changed: " + ", ".join(changed[:limit]))
    return " | ".join(parts) if parts else "no diff"


def artifact_dir_for(tree_root: Path, source_root: Path, test_file: Path) -> Path:
    """Return the lit %t directory for a test under the given output tree."""
    rel = test_file.resolve().relative_to(source_root.resolve())
    return tree_root / rel.parent / "Output" / f"{rel.name}.tmp"


def _safe_filename(s: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]+", "_", s)


def compare_test(
    test_file: Path,
    tree_a: Path,
    tree_b: Path,
    source_root: Path,
    failures_dir: Path,
) -> tuple[str, str]:
    """Returns (status, message). status is one of 'pass', 'fail', 'skip'."""
    dir_a = artifact_dir_for(tree_a, source_root, test_file)
    dir_b = artifact_dir_for(tree_b, source_root, test_file)

    a_exists = dir_a.is_dir()
    b_exists = dir_b.is_dir()
    if not a_exists or not b_exists:
        which = []
        if not a_exists:
            which.append(str(dir_a))
        if not b_exists:
            which.append(str(dir_b))
        return "skip", f"missing artifact dir(s): {', '.join(which)}"

    manifest_a = _build_manifest(dir_a)
    manifest_b = _build_manifest(dir_b)

    if not manifest_a and not manifest_b:
        return "skip", "both dirs empty (test produced no artifacts)"

    if manifest_a == manifest_b:
        return "pass", ""

    failures_dir.mkdir(parents=True, exist_ok=True)
    slug = _safe_filename(str(test_file.resolve().relative_to(source_root.resolve())))
    dest = failures_dir / slug
    if dest.exists():
        shutil.rmtree(dest)
    shutil.copytree(dir_a, dest / "a")
    shutil.copytree(dir_b, dest / "b")

    return "fail", f"{_summarize_diff(manifest_a, manifest_b)}\n  saved: {dest}"


def main() -> int:
    ap = argparse.ArgumentParser(description="Cheerp determinism comparator")
    ap.add_argument("test_files", nargs="+", type=Path, help="Test source files to compare (already sampled)")
    ap.add_argument("--tree-a", required=True, type=Path, help="First Outputs tree")
    ap.add_argument("--tree-b", required=True, type=Path, help="Second Outputs tree")
    ap.add_argument("--source-root", required=True, type=Path, help="Lit test_source_root (e.g. <repo>/tests)")
    ap.add_argument("--failures-dir", required=True, type=Path, help="Where to copy mismatched output dirs")
    args = ap.parse_args()

    totals = {"pass": 0, "fail": 0, "skip": 0}
    for test_file in args.test_files:
        status, msg = compare_test(
            test_file=test_file,
            tree_a=args.tree_a,
            tree_b=args.tree_b,
            source_root=args.source_root,
            failures_dir=args.failures_dir,
        )
        totals[status] += 1
        rel = test_file
        try:
            rel = test_file.resolve().relative_to(args.source_root.resolve())
        except ValueError:
            pass
        prefix = {"pass": "[pass]", "fail": "[fail]", "skip": "[skip]"}[status]
        if msg:
            print(f"{prefix} {rel}: {msg}")
        else:
            print(f"{prefix} {rel}")

    print("=" * 60)
    print("Determinism summary")
    print(f"  total:  {sum(totals.values())}")
    print(f"  pass:   {totals['pass']}")
    print(f"  fail:   {totals['fail']}")
    print(f"  skip:   {totals['skip']}")

    return 1 if totals["fail"] > 0 else 0


if __name__ == "__main__":
    sys.exit(main())
