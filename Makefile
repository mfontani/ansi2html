.ONESHELL:
SHELL = /bin/bash
.SHELLFLAGS = -e -c

CC = gcc

DEFINES = -DITERM2_COLOR_SCHEMES

C_FLAGS = -O2 -D_GNU_SOURCE -flto -ggdb3 -march=native -Wall -Wextra -Werror -Wformat-security -Wshadow -Wredundant-decls
ifeq (,$(findstring ++,$(CC)))
	C_FLAGS += -std=c2x -Wstrict-prototypes -Wold-style-definition -Wnested-externs
else
	C_FLAGS += -std=c++2x -Wno-vla-cxx-extension
    ifneq (,$(findstring clang++,$(CC)))
		# clang++: error: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Werror,-Wdeprecated]
		C_FLAGS += -Wno-deprecated
    endif
endif
ifeq (gcc,$(findstring gcc,$(CC)))
	C_FLAGS += -Wlogical-op
endif

L_FLAGS =

ADD_L_FLAGS =
ADD_C_FLAGS = -fsanitize=address -fsanitize=undefined -fsanitize=pointer-compare -fsanitize-address-use-after-scope -fstack-protector-all
ifneq (11,$(shell cat /etc/debian_version 2>/dev/null | cut -d . -f 1))
	ADD_C_FLAGS += -fsanitize=pointer-subtract -fno-sanitize-recover=pointer-subtract
endif
ifeq (11,$(shell cat /etc/debian_version | cut -d . -f 1))
    ifeq (gcc,$(findstring gcc,$(CC)))
		ADD_C_FLAGS += -Wl,--push-state,--no-as-needed -lcrypt -Wl,--pop-state
    endif
endif
ADD_C_FLAGS += -fsanitize-recover=undefined,float-cast-overflow,float-divide-by-zero,pointer-compare
ifeq (gcc,$(findstring gcc,$(CC)))
	ADD_C_FLAGS += -Werror=maybe-uninitialized -fno-sanitize-recover=bounds-strict
endif
ADD_L_FLAGS += $(ADD_C_FLAGS)
ifeq (gcc,$(findstring gcc,$(CC)))
	ADD_L_FLAGS += -static-libasan
endif
# for CodeChecker
ifneq (,$(CC_LOGGER_GCC_LIKE))
	ADD_C_FLAGS =
	ADD_L_FLAGS =
endif
# NO_ASAN=1 make ... to not get asan
ifneq (,$(NO_ASAN))
	ADD_C_FLAGS =
	ADD_L_FLAGS =
endif

C_FILES = \
src/ansi2html.c src/main.c src/structs/ansi_color_palette.c src/iterm2_color_schemes/ansi_color_palette.c src/structs/ansi_color_type.c src/structs/ansi_fg_or_bg.c
H_FILES = \
src/ansi2html.h src/structs/ansi_color.h src/structs/ansi_color_palette.h src/iterm2_color_schemes/ansi_color_palette.h src/iterm2_color_schemes/named_palettes.h src/structs/ansi_color_type.h src/structs/ansi_fg_or_bg.h src/structs/ansi_rgb.h src/structs/ansi_style.h src/structs/ansi_style_properties.h

# This comes first so one can just "make" and just make the executable.
# I'll just go do "make all" instead.
ansi2html: Makefile $(C_FILES) $(H_FILES)
	@rm -f $@
	@start=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	$(CC) $(DEFINES) $(C_FLAGS) $(L_FLAGS) $(ADD_L_FLAGS) -o $@ $(C_FILES)
	@end=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	@perl -E'say sprintf "Build  time: %.4f seconds", $$ARGV[1] - $$ARGV[0]' "$$start" "$$end"

.PHONY: all
all: format tags ansi2html

.PHONY: format
format: scripts/format.sh Makefile $(C_FILES) $(H_FILES)
	@start=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	@./scripts/format.sh clang-format -i $(C_FILES) $(H_FILES)
	@end=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	@perl -E'say sprintf "Format time: %.4f seconds", $$ARGV[1] - $$ARGV[0]' "$$start" "$$end"

tags: Makefile $(C_FILES) $(H_FILES)
	@rm -f tags
	@start=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	@ctags --extra=+f $(C_FILES) $(H_FILES)
	@end=$$(perl -MTime::HiRes=gettimeofday -E'say scalar gettimeofday')
	@perl -E'say sprintf "Tags   time: %.4f seconds", $$ARGV[1] - $$ARGV[0]' "$$start" "$$end"

.PHONY: clean
clean:
	rm -f ansi2html tags

.PHONY: run-tests
run-tests:
	@prove -v tests/*.sh

.PHONY: prove
prove:
	@prove tests/*.sh
