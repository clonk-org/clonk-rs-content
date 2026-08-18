#!/bin/bash

set -eu

if [ "$#" -ne 1 ]; then
	echo "USAGE: $0 <VERSION>" >&2
	exit 1
fi

VERSION="$1"

# Packs that carry their own upstream version rather than the LegacyClonk one.
#
# These are the directories .gitattributes marks `binary` so that nothing
# rewrites their bytes: they are exact copies of the packs as received.
# Their Version.txt is part of what is redistributed unmodified -- ClonkMars is
# "1.5 [Spirit]", Eke Reloaded is "1.7", Metal & Magic is "3.1b" -- and none of
# them is terminated by a newline, so writing this project's version into them
# would change both the string and the file's length.
THIRD_PARTY_PACKS=(
	ClonkMars.c4d
	ClonkMars.c4f
	EkeReloaded.c4d
	EkeReloaded.c4f
	MetalMagic.c4d
	MetalMagicExtra.c4d
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

echo "Please do not forget to add the latest version to updates/parts.txt if appropriate!"
