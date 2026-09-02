#!/bin/bash

set -eu

if [ "$#" -ne 1 ]; then
	echo "USAGE: $0 <VERSION>" >&2
	exit 1
fi

VERSION="$1"

# Packs that carry their own upstream version rather than the LegacyClonk one.
#
# These are the data-root entries .github/packs.toml marks `bytes = "preserve"`
# and .gitattributes therefore marks `binary`, so Git never rewrites their byte
# conventions. Their upstream Version.txt remains unchanged by project release
# bumps -- ClonkMars is "1.5 [Spirit]", Eke Reloaded is "1.7", Metal & Magic
# is "3.1b", the Ultimate Clonk Compilation collection is "2.0", Golems is
# "4.95.5" and Modern Combat is "R1.9" -- and none of them is terminated by a
# newline, so writing this project's version into them would change both the
# string and the file's length.
#
# Read from the manifest rather than repeated here: the list used to be a copy
# that had to be kept in step by hand, and a pack missing from it was silently
# restamped. The loop below globs */Version.txt, so preserved entries nested
# deeper or left as single packed files come back from the query harmlessly.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
own_version_packs="$("${PYTHON:-python3}" "$SCRIPT_DIR/.github/packs.py" own-version)"
if [ -z "$own_version_packs" ]; then
	echo "packs.toml lists no preserved packs; refusing to stamp anything" >&2
	exit 1
fi
THIRD_PARTY_PACKS=()
while IFS= read -r pack; do
	THIRD_PARTY_PACKS+=("$pack")
done <<<"$own_version_packs"

is_third_party() {
	local candidate="$1" pack
	for pack in "${THIRD_PARTY_PACKS[@]}"; do
		if [ "$pack" = "$candidate" ]; then
			return 0
		fi
	done
	return 1
}

# CRLF, which is what the versioned packs carry and what `* text=auto eol=crlf`
# checks out. `echo` writes LF.
write_version() {
	printf '%s\r\n' "$VERSION" >"$1"
}

for file in */Version.txt; do
	[ -e "$file" ] || continue
	pack="${file%%/*}"
	if is_third_party "$pack"; then
		echo "skipping $pack: third-party pack, keeps its own version"
		continue
	fi
	write_version "$file"
	echo "$pack -> $VERSION"
done

write_version Version.txt
echo "Version.txt -> $VERSION"
