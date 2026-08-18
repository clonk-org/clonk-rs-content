#!/bin/bash
# Tests set_version.sh against a fixture tree.
#
# Lives under .github/ so it does not ship in content.zip: the packaging tool
# excludes that directory by path segment.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SET_VERSION="$REPO_ROOT/set_version.sh"

failures=0

fail() {
	echo "  FAIL: $*" >&2
	failures=$((failures + 1))
}

pass() {
	echo "  ok: $*"
}

# The packs the LegacyClonk version describes carry a CRLF-terminated
# Version.txt. The third-party packs carry their own version string with no
# trailing newline at all, and .gitattributes marks their directories `binary`
# so that nothing rewrites their bytes.
build_fixture() {
	local root="$1"
	local base third_party

	for base in Objects.c4d Worlds.c4f Material.c4g; do
		mkdir -p "$root/$base"
		printf '4.9.11.1\r\n' >"$root/$base/Version.txt"
	done

	mkdir -p "$root/ClonkMars.c4d" "$root/EkeReloaded.c4d" "$root/MetalMagic.c4d"
	printf '1.5 [Spirit]' >"$root/ClonkMars.c4d/Version.txt"
	printf '1.7' >"$root/EkeReloaded.c4d/Version.txt"
	printf '3.1b' >"$root/MetalMagic.c4d/Version.txt"

	# MetalMagicExtra ships without a Version.txt at all.
	mkdir -p "$root/MetalMagicExtra.c4d"

	# A third-party scenario nested inside a base pack.
	mkdir -p "$root/Melees.c4f/Queron3.c4s"
	printf '4.9.11.1\r\n' >"$root/Melees.c4f/Version.txt"
	printf '3.41' >"$root/Melees.c4f/Queron3.c4s/Version.txt"

	printf '4.9.11.1\r\n' >"$root/Version.txt"
}

# `want` is a printf format, so a trailing \r\n survives: command substitution
# strips trailing newlines from a value, which would silently weaken the very
# assertion this test exists to make.
expect_bytes() {
	local label="$1" path="$2" want="$3"
	local got
	got="$(od -An -c <"$path" | tr -s ' ')"
	want="$(printf '%b' "$want" | od -An -c | tr -s ' ')"
	if [ "$got" = "$want" ]; then
		pass "$label"
	else
		fail "$label: got [$got] want [$want]"
	fi
}

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
build_fixture "$tmp"

( cd "$tmp" && bash "$SET_VERSION" 4.9.12.0 ) >/dev/null 2>&1

echo "base packs take the new version, CRLF preserved:"
expect_bytes "Objects.c4d" "$tmp/Objects.c4d/Version.txt" '4.9.12.0\r\n'
expect_bytes "Worlds.c4f" "$tmp/Worlds.c4f/Version.txt" '4.9.12.0\r\n'
expect_bytes "Material.c4g" "$tmp/Material.c4g/Version.txt" '4.9.12.0\r\n'
expect_bytes "Melees.c4f" "$tmp/Melees.c4f/Version.txt" '4.9.12.0\r\n'
expect_bytes "root Version.txt" "$tmp/Version.txt" '4.9.12.0\r\n'

echo "third-party packs keep their own version, byte for byte:"
expect_bytes "ClonkMars.c4d" "$tmp/ClonkMars.c4d/Version.txt" '1.5 [Spirit]'
expect_bytes "EkeReloaded.c4d" "$tmp/EkeReloaded.c4d/Version.txt" '1.7'
expect_bytes "MetalMagic.c4d" "$tmp/MetalMagic.c4d/Version.txt" '3.1b'
expect_bytes "Queron3.c4s" "$tmp/Melees.c4f/Queron3.c4s/Version.txt" '3.41'

echo "a pack with no Version.txt is not given one:"
if [ -e "$tmp/MetalMagicExtra.c4d/Version.txt" ]; then
	fail "MetalMagicExtra.c4d gained a Version.txt"
else
	pass "MetalMagicExtra.c4d"
fi

echo "the script rejects a missing argument:"
if ( cd "$tmp" && bash "$SET_VERSION" ) >/dev/null 2>&1; then
	fail "exited 0 with no version argument"
else
	pass "non-zero exit with no argument"
fi

if [ "$failures" -ne 0 ]; then
	echo "$failures check(s) failed" >&2
	exit 1
fi
echo "all checks passed"
