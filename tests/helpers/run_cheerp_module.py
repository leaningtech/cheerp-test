#!/usr/bin/env python3
"""Run a Cheerp-generated JS module artifact.

This is a tiny helper for LIT tests that compile with -cheerp-make-module=es6
or -cheerp-make-module=commonjs but do not use the create_driver.py +
<test>.testing.js harness.

Why this exists:
- ES6 output is an ES module that exports a default initializer function.
  Running `node file.mjs` does not execute the program.
- CommonJS output typically exports a Promise that resolves after running the
  program. Running `node file.js` does not await it.

Usage:
  run_cheerp_module.py --module=es6 <path/to/file.mjs>
  run_cheerp_module.py --module=commonjs <path/to/file.js>

The script runs node, forwarding stdout/stderr, and exits with node's status.
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path


def _run_es6(*, node: str, module_path: Path) -> int:
    module_path = module_path.resolve()
    workdir = module_path.parent
    base = module_path.name

    # Write a small driver next to the module so relative import works.
    driver_path = workdir / f".{base}.cheerp_driver.mjs"
    driver_src = f"""import mod from './{base}';

(async () => {{
  try {{
    const argv = process.argv.slice(2);
    const env = [];
    await mod({{ relativePath: '', argv, env }});
  }} catch (e) {{
    console.error(e && e.stack ? e.stack : e);
    process.exitCode = 1;
  }}
}})();
"""

    driver_path.write_text(driver_src, encoding="utf-8")

    try:
        p = subprocess.run([node, str(driver_path)], cwd=str(workdir))
        return int(p.returncode)
    finally:
        try:
            driver_path.unlink(missing_ok=True)
        except TypeError:
            # Python < 3.8 compat
            if driver_path.exists():
                driver_path.unlink()


def _run_commonjs(*, node: str, module_path: Path) -> int:
    module_path = module_path.resolve()
    workdir = module_path.parent
    base = module_path.name

    # Use -e to avoid creating a driver file.
    js = (
        "(async () => {\n"
        f"  const p = require('./{base}');\n"
        "  await Promise.resolve(p);\n"
        "})().catch(e => {\n"
        "  console.error(e && e.stack ? e.stack : e);\n"
        "  process.exit(1);\n"
        "});\n"
    )

    p = subprocess.run([node, "-e", js], cwd=str(workdir))
    return int(p.returncode)


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description="Run a Cheerp JS module artifact (es6/commonjs)")
    ap.add_argument("--module", required=True, choices=["es6", "commonjs"], help="Module output kind")
    ap.add_argument("--node", default="node", help="Node executable (default: node)")
    ap.add_argument("module_file", help="Path to compiled module artifact (.mjs/.js)")
    args = ap.parse_args(argv)

    module_path = Path(args.module_file)
    if not module_path.exists():
        print(f"error: module file not found: {module_path}", file=sys.stderr)
        return 2

    if args.module == "es6":
        return _run_es6(node=args.node, module_path=module_path)
    return _run_commonjs(node=args.node, module_path=module_path)


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
