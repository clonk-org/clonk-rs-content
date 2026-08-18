# Packs the unpacked group directories in this repository into C4Groups,
# and zips them into packed/lc_content.zip.
#
# Nothing automated calls this. clonk-rs ships game data as the
# content-addressed content.zip that .github/workflows/publish-content.yml
# builds, and no workflow in either repository refers to packed/. This is
# here for producing packed groups by hand: differential work against the
# C++ oracle, which reads packed groups, and anything aimed at a peer
# running a classic engine.
#
# C4GROUP is satisfied by the clonk-c4group crate, which clonk-org/clonk-rs
# builds and installs as `c4group`; no external tool is needed.

C4GROUP:=c4group
ZIP=zip
PACK_CMD:=cp -r SRC TARGET && $(C4GROUP) TARGET -p
ZIP_CMD:=$(ZIP) -n c4f:c4g:c4d:c4s -j TARGET SRCS

PACKS:=$(wildcard *.c4?)
PACKED_PACKS:=$(patsubst %,packed/%,$(PACKS))
ARCHIVE:=packed/lc_content.zip

.PHONY: all clean

all: $(PACKED_PACKS) $(ARCHIVE)

packed/.exists: Makefile
	@test -f packed/.exists || (mkdir -p packed && touch packed/.exists)

packed/%: % packed/.exists
	$(RM) -r $@
	$(subst TARGET,$@,$(subst SRC,$<,$(PACK_CMD)))

$(ARCHIVE): $(PACKED_PACKS)
	$(RM) $@
	$(subst TARGET,$@,$(subst SRCS,$^,$(ZIP_CMD)))

clean:
	$(RM) -r packed
