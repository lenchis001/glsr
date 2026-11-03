#!/usr/bin/env python3

"""Bake files from Resources directories into generated C-arrays.

The script scans recursively for directories that contain:
- CMakeLists.txt
- Sources/
- Resources/

For each match it generates:
- Sources/Resources/<Prefix>Resources.h
- Sources/Resources/<Prefix>Resources.cpp

Where <Prefix> is the parent directory of the Resources directory.
"""

import argparse
import hashlib
import os
import re
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple


HEADER_SUFFIX = "Resources.h"
SOURCE_SUFFIX = "Resources.cpp"


class BakedFile(object):
	__slots__ = ("rel_path", "symbol", "member", "data")

	def __init__(self, rel_path, symbol, member, data):
		self.rel_path = rel_path
		self.symbol = symbol
		self.member = member
		self.data = data


def parse_args():
	parser = argparse.ArgumentParser(description="Bake Resources folders into C-arrays.")
	parser.add_argument(
		"root",
		nargs="?",
		default=str(Path(__file__).resolve().parent.parent),
		help="Root directory to scan (defaults to repository root).",
	)
	parser.add_argument(
		"--exclude",
		action="append",
		default=["build", ".git", ".vs", ".vscode", "out"],
		help="Directory name to exclude from recursive scanning. Can be used multiple times.",
	)
	return parser.parse_args()


def iter_targets(root, excluded_names):
	excluded = set(excluded_names)
	for dirpath, dirnames, filenames in os.walk(str(root), topdown=True):
		current = Path(dirpath)
		dirnames[:] = [d for d in dirnames if d not in excluded]

		if "CMakeLists.txt" not in filenames:
			continue

		sources_dir = current / "Sources"
		resources_dir = current / "Resources"
		if sources_dir.is_dir() and resources_dir.is_dir():
			yield current


def to_symbol(rel_path):
	raw = rel_path.replace("\\", "/")
	chars = []
	for ch in raw:
		if ch.isalnum():
			chars.append(ch.lower())
		else:
			chars.append("_")

	symbol = "res_" + "".join(chars).strip("_")
	symbol = "_".join(filter(None, symbol.split("_")))
	if not symbol or symbol == "res":
		symbol = "res_file"
	if symbol[0].isdigit():
		symbol = "res_" + symbol
	return symbol


def to_member_name(rel_path):
	member = to_symbol(rel_path)
	if member.startswith("res_"):
		member = member[4:]
	if not member:
		member = "file"
	if member[0].isdigit():
		member = "file_" + member
	return member


def read_bytes(path):
	with open(str(path), "rb") as handle:
		return handle.read()


def read_text(path):
	with open(str(path), "r", encoding="utf-8") as handle:
		return handle.read()


def sanitize_cpp_identifier(name):
	# Keep generated type names valid if the directory contains non-identifier characters.
	value = re.sub(r"[^0-9A-Za-z_]", "_", name)
	value = re.sub(r"_+", "_", value).strip("_")
	if not value:
		value = "Target"
	if value[0].isdigit():
		value = "_" + value
	return value


def load_files(resources_dir):
	paths = sorted((p for p in resources_dir.rglob("*") if p.is_file()), key=lambda p: p.as_posix().lower())
	used_symbols = {}
	used_members = {}
	baked_files = []

	for full_path in paths:
		rel = full_path.relative_to(resources_dir).as_posix()
		base_symbol = to_symbol(rel)
		base_member = to_member_name(rel)
		symbol = base_symbol
		member = base_member
		if symbol in used_symbols:
			used_symbols[symbol] += 1
			digest = hashlib.sha1(rel.encode("utf-8")).hexdigest()[:8]
			symbol = "{}_{}_{}".format(base_symbol, used_symbols[base_symbol], digest)
		else:
			used_symbols[symbol] = 0

		if member in used_members:
			used_members[member] += 1
			digest = hashlib.sha1(rel.encode("utf-8")).hexdigest()[:8]
			member = "{}_{}_{}".format(base_member, used_members[base_member], digest)
		else:
			used_members[member] = 0

		baked_files.append(BakedFile(rel_path=rel, symbol=symbol, member=member, data=read_bytes(full_path)))

	return baked_files


def format_byte_lines(data, width=12):
	if not data:
		return "    0x00"

	lines = []
	for i in range(0, len(data), width):
		chunk = data[i : i + width]
		lines.append("    " + ", ".join("0x{:02X}".format(byte) for byte in chunk))
	return ",\n".join(lines)


def make_header(class_name):
	template = """#pragma once

#include <cstddef>

namespace BakedResources
{

struct ResourceView
{
	const char* path;
	const unsigned char* data;
	std::size_t size;
};

class {class_name} final
{
public:
	{class_name}() = delete;

    static const ResourceView* GetAll(std::size_t& outCount);
    static const unsigned char* GetContent(const char* fileName, std::size_t& outSize);
	static const ResourceView* FindByName(const char* fileName);

{{resource_members}}
};

} // namespace BakedResources
"""
	return template.replace("{class_name}", class_name)


def make_header_with_files(files, class_name):
	member_lines = ["    static const ResourceView {};".format(baked.member) for baked in files]
	template = make_header(class_name)
	return template.replace("{{resource_members}}", "\n".join(member_lines))


def make_source(files, class_name, header_name):
	lines = [
		'#include "{}"'.format(header_name),
		"",
		"#include <cstring>",
		"",
		"namespace",
		"{",
	]

	for baked in files:
		lines.append("static const unsigned char {}[] =".format(baked.symbol))
		lines.append("{")
		lines.append(format_byte_lines(baked.data))
		lines.append("};")
		lines.append("static const std::size_t {}_size = {};".format(baked.symbol, len(baked.data)))
		lines.append("")

	lines.append("} // namespace")
	lines.append("")
	lines.append("namespace BakedResources")
	lines.append("{")
	lines.append("")

	for baked in files:
		escaped_path = baked.rel_path.replace('"', r'\"')
		lines.append("const ResourceView {}::{} =".format(class_name, baked.member))
		lines.append("{")
		lines.append('    "{}",'.format(escaped_path))
		lines.append("    {},".format(baked.symbol))
		lines.append("    {}_size".format(baked.symbol))
		lines.append("};")
		lines.append("")

	lines.append("namespace")
	lines.append("{")
	lines.append("static const ResourceView kAllResources[] =")
	lines.append("{")
	for baked in files:
		lines.append("    {}::{},".format(class_name, baked.member))
	lines.append("};")
	lines.append("")
	lines.append("static const std::size_t kAllResourcesCount = sizeof(kAllResources) / sizeof(kAllResources[0]);")
	lines.append("")
	lines.append("char NormalizePathSeparator(char ch)")
	lines.append("{")
	lines.append("    return ch == '\\\\' ? '/' : ch;")
	lines.append("}")
	lines.append("")
	lines.append("bool PathsEqual(const char* left, const char* right)")
	lines.append("{")
	lines.append("    if (left == nullptr || right == nullptr)")
	lines.append("    {")
	lines.append("        return left == right;")
	lines.append("    }")
	lines.append("")
	lines.append("    while (*left != '\\0' && *right != '\\0')")
	lines.append("    {")
	lines.append("        if (NormalizePathSeparator(*left) != NormalizePathSeparator(*right))")
	lines.append("        {")
	lines.append("            return false;")
	lines.append("        }")
	lines.append("")
	lines.append("        ++left;")
	lines.append("        ++right;")
	lines.append("    }")
	lines.append("")
	lines.append("    return *left == *right;")
	lines.append("}")
	lines.append("")
	lines.append("const char* FindBaseName(const char* path)")
	lines.append("{")
	lines.append("    if (path == nullptr)")
	lines.append("    {")
	lines.append("        return nullptr;")
	lines.append("    }")
	lines.append("")
	lines.append("    const char* baseName = path;")
	lines.append("    for (const char* cursor = path; *cursor != '\\0'; ++cursor)")
	lines.append("    {")
	lines.append("        if (*cursor == '/' || *cursor == '\\\\')")
	lines.append("        {")
	lines.append("            baseName = cursor + 1;")
	lines.append("        }")
	lines.append("    }")
	lines.append("")
	lines.append("    return baseName;")
	lines.append("}")
	lines.append("} // namespace")
	lines.append("")
	lines.append("const ResourceView* {}::GetAll(std::size_t& outCount)".format(class_name))
	lines.append("{")
	lines.append("    outCount = kAllResourcesCount;")
	lines.append("    return kAllResources;")
	lines.append("}")
	lines.append("")
	lines.append("const ResourceView* {}::FindByName(const char* fileName)".format(class_name))
	lines.append("{")
	lines.append("    if (fileName == nullptr)")
	lines.append("    {")
	lines.append("        return nullptr;")
	lines.append("    }")
	lines.append("")
	lines.append("    for (std::size_t i = 0; i < kAllResourcesCount; ++i)")
	lines.append("    {")
	lines.append("        const char* fullPath = kAllResources[i].path;")
	lines.append("        const char* baseName = FindBaseName(fullPath);")
	lines.append("")
	lines.append("        if (PathsEqual(fullPath, fileName) || PathsEqual(baseName, fileName))")
	lines.append("        {")
	lines.append("            return &kAllResources[i];")
	lines.append("        }")
	lines.append("    }")
	lines.append("")
	lines.append("    return nullptr;")
	lines.append("}")
	lines.append("")
	lines.append("const unsigned char* {}::GetContent(const char* fileName, std::size_t& outSize)".format(class_name))
	lines.append("{")
	lines.append("    const ResourceView* resource = FindByName(fileName);")
	lines.append("    if (resource == nullptr)")
	lines.append("    {")
	lines.append("        outSize = 0;")
	lines.append("        return nullptr;")
	lines.append("    }")
	lines.append("")
	lines.append("    outSize = resource->size;")
	lines.append("    return resource->data;")
	lines.append("}")
	lines.append("")
	lines.append("} // namespace BakedResources")

	return "\n".join(lines) + "\n"


def write_if_changed(path, content):
	if path.exists() and read_text(path) == content:
		return False
	with open(str(path), "w", encoding="utf-8", newline="\n") as f:
		f.write(content)
	return True


def bake_target(target_dir):
	resources_dir = target_dir / "Resources"
	generated_dir = target_dir / "Sources" / "Resources"
	generated_dir.mkdir(parents=True, exist_ok=True)
	prefix = target_dir.name
	class_name = "{}Resources".format(sanitize_cpp_identifier(prefix))
	header_name = "{}{}".format(prefix, HEADER_SUFFIX)
	source_name = "{}{}".format(prefix, SOURCE_SUFFIX)

	files = load_files(resources_dir)
	header_path = generated_dir / header_name
	source_path = generated_dir / source_name

	header_changed = write_if_changed(header_path, make_header_with_files(files, class_name))
	source_changed = write_if_changed(source_path, make_source(files, class_name, header_name))
	return header_changed, source_changed, len(files)


def main():
	args = parse_args()
	root = Path(args.root).resolve()
	if not root.exists() or not root.is_dir():
		print("Root directory does not exist or is not a directory: {}".format(root))
		return 1

	targets = list(iter_targets(root, args.exclude))
	if not targets:
		print("No matching targets found under: {}".format(root))
		return 0

	changed_count = 0
	for target in sorted(targets, key=lambda p: p.as_posix().lower()):
		header_changed, source_changed, file_count = bake_target(target)
		if header_changed or source_changed:
			changed_count += 1

		status = "updated" if (header_changed or source_changed) else "unchanged"
		print("[{}] {} ({} files)".format(status, target.as_posix(), file_count))

	print("Processed {} target(s), changed {}.".format(len(targets), changed_count))
	return 0


if __name__ == "__main__":
	raise SystemExit(main())
