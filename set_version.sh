#!/bin/bash

set -eu

if [ "$#" -ne 1 ]; then
	echo "USAGE: $0 <VERSION>" >&2
	exit 1
fi

VERSION="$1"

# Packs that carry their own upstream version rather than the LegacyClonk one.
#
# These are imported directories .gitattributes marks `binary` so Git does not
# rewrite their byte conventions. Their upstream Version.txt remains unchanged
# by project release bumps -- ClonkMars is
# "1.5 [Spirit]", Eke Reloaded is "1.7", Metal & Magic is "3.1b", the Ultimate
# Clonk Compilation collection is "2.0", Golems is "4.95.5" and Modern Combat is
# "R1.9" -- and none of them is terminated by a newline, so writing this
# project's version into them would change both the string and the file's
# length.
#
# Only packs with a Version.txt one level down need listing: the loop below
# globs */Version.txt, so approved imports left as single packed files or without
# a top-level Version.txt are skipped without being named here.
THIRD_PARTY_PACKS=(
	ClonkMars.c4d
	ClonkMars.c4f
	Collection.c4f
	EkeReloaded.c4d
	EkeReloaded.c4f
	Golems.c4f
	MetalMagic.c4d
	MetalMagicExtra.c4d
	ModernCombat.c4f
)

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
