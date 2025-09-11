#!/usr/bin/env python3
"""
Generate a VS Code snippet JSON from a given text/source file.

Usage:
  python3 template/snippet_from_file.py path/to/file \
      [--name NAME] [--prefix PREFIX] [--description DESC] [--output OUT]

By default:
- Snippet name = input file stem (without extension)
- Snippet prefix = snippet name
- Description = "Auto-generated from <input filename>"
- Output file = <input stem>.code-snippets (next to the input file)
  Content is included literally line-by-line.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def build_snippet_body(lines):
    """Convert lines to snippet body literally (no snippet escaping)."""
    return [line.rstrip("\r\n") for line in lines]


def infer_output_path(input_path):
    stem = input_path.stem
    return input_path.with_name(f"{stem}.code-snippets")

parser = argparse.ArgumentParser(description="Create VS Code snippet JSON from a file")
parser.add_argument("input", type=Path, help="Path to the source/text file")
parser.add_argument("--name", "-n", help="Snippet name (defaults to file stem)")
parser.add_argument("--prefix", "-p", help="Snippet prefix (defaults to name)")
parser.add_argument(
    "--description",
    "-d",
    help="Snippet description (defaults to 'Auto-generated from <file>')",
)
parser.add_argument(
    "--output",
    "-o",
    type=Path,
    help="Output .code-snippets JSON path (defaults next to input)",
)

args = parser.parse_args()

input_path: Path = args.input
if not input_path.exists() or not input_path.is_file():
    parser.error(f"Input file not found: {input_path}")

name = args.name or input_path.stem
prefix = args.prefix or name
description = args.description or f"Auto-generated from {input_path.name}"
output_path = args.output or infer_output_path(input_path)
text = input_path.read_text(encoding="utf-8")
lines = text.splitlines(keepends=True)
body = build_snippet_body(lines)

snippet = {
    name: {
        "prefix": prefix,
        "body": body,
        "description": description,
    }
}

output_path.parent.mkdir(parents=True, exist_ok=True)
with output_path.open("w", encoding="utf-8") as f:
    json.dump(snippet, f, indent=2, ensure_ascii=False)
    f.write("\n")

print(str(output_path))
