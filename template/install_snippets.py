#!/usr/bin/env python3
"""
Install VS Code C++ (cpp) snippets from provided JSON files.

Merges snippets into the user's cpp.json.
Conflict rules:
- Same snippet name: replace existing with incoming (update).
- Prefix-only conflict (no name conflict): skip incoming.

Usage:
  python3 template/install_snippets.py path/to/snippet1.json [snippet2.json ...]

Notes:
  - Input files must be in VS Code snippet JSON format: an object mapping
    snippet names (top-level keys) to objects with at least a "prefix" and "body".
  - Prefix may be a string or an array of strings.
  - The script replaces snippets on name conflicts, and skips only on prefix-only conflicts.
"""

from __future__ import annotations

import argparse
import json
import os
import platform
from pathlib import Path
from typing import Dict, Any, Iterable, Set


def default_user_snippets_dir():
    # Allow environment override for custom VS Code data directories
    env_override = os.environ.get("VSCODE_USER_DATA_DIR")
    if env_override:
        return Path(env_override).expanduser() / "User" / "snippets"

    system = platform.system()
    # macos
    return Path.home() / "Library" / "Application Support" / "Code" / "User" / "snippets"

def resolve_cpp_snippets_path() -> Path:
    return default_user_snippets_dir() / "cpp.json"


def load_json(path) -> Dict[str, Any]:
    if not path.exists():
        return {}
    with path.open("r", encoding="utf-8") as f:
        try:
            data = json.load(f)
        except json.JSONDecodeError as e:
            raise SystemExit(f"Invalid JSON in {path}: {e}")
    if not isinstance(data, dict):
        raise SystemExit(f"Top-level JSON must be an object in {path}")
    return data


def normalize_prefixes(prefix_val: Any) -> Set[str]:
    if isinstance(prefix_val, str):
        return {prefix_val}
    if isinstance(prefix_val, list):
        return {str(p) for p in prefix_val}
    return set()


def collect_existing_prefixes(snippets: Dict[str, Any]) -> Dict[str, Set[str]]:
    by_name: Dict[str, Set[str]] = {}
    for name, entry in snippets.items():
        if isinstance(entry, dict):
            by_name[name] = normalize_prefixes(entry.get("prefix"))
    return by_name


def any_prefix_conflict(existing: Dict[str, Set[str]], incoming_prefixes: Set[str]) -> bool:
    for prefixes in existing.values():
        if prefixes & incoming_prefixes:
            return True
    return False


def merge_snippets(
    target_data: Dict[str, Any], incoming_datas: Iterable[Dict[str, Any]]
) -> tuple[Dict[str, Any], int, int, int]:
    added = 0
    replaced = 0
    skipped = 0
    for incoming in incoming_datas:
        existing_prefix_map = collect_existing_prefixes(target_data)
        for name, entry in incoming.items():
            if not isinstance(entry, dict):
                skipped += 1
                continue
            incoming_prefixes = normalize_prefixes(entry.get("prefix"))
            # Rule 1: same name -> replace existing snippet
            if name in target_data:
                target_data[name] = entry
                # Update prefix map for this name
                existing_prefix_map[name] = incoming_prefixes
                replaced += 1
                continue

            # Rule 2: prefix-only conflict -> skip incoming
            if any_prefix_conflict(existing_prefix_map, incoming_prefixes):
                skipped += 1
                continue

            # Otherwise, add new snippet
            target_data[name] = entry
            existing_prefix_map[name] = incoming_prefixes
            added += 1
    return target_data, added, replaced, skipped


parser = argparse.ArgumentParser(description="Install snippets into VS Code C++ user snippets (cpp.json)")
parser.add_argument("snippets", nargs="+", type=Path, help="Path(s) to snippet JSON files to install")

args = parser.parse_args()

cpp_json_path = resolve_cpp_snippets_path()
cpp_json_path.parent.mkdir(parents=True, exist_ok=True)

# Load target and all incoming
target_data = load_json(cpp_json_path)
incoming_datas = []
for p in args.snippets:
    if not p.exists() or not p.is_file():
        raise SystemExit(f"Snippet file not found: {p}")
    incoming_datas.append(load_json(p))

    merged, added, replaced, skipped = merge_snippets(target_data, incoming_datas)

with cpp_json_path.open("w", encoding="utf-8") as f:
    json.dump(merged, f, indent=2, ensure_ascii=False)
    f.write("\n")

print(f"Updated: {cpp_json_path}")
print(f"Added: {added}; Replaced (name conflicts): {replaced}; Skipped (prefix conflicts/invalid): {skipped}")
