.PHONY: build run verify verify-all

MAKEFLAGS += --no-print-directory --silent

DAY?=01
PART?=1
VER?=1
VERIFY?=false

VER_NAME=$(if $(filter 1,$(VER)),practice,final)
INPUT?=../input/$(VER_NAME)/day$(DAY).in
OUTPUT?=../output/part$(PART)/$(VER_NAME)/day$(DAY).ans

START?=01
END?=01
DAYS=$(shell seq -f "%02g" $(START) $(END))

build:
	cargo build --quiet --release

run:
	cargo run --quiet --release --bin "day$(DAY)" \
		-- "$(PART)" "$(INPUT)" "$(VERIFY)" "$(OUTPUT)"; \
	$(VERIFY) || ( \
		grep -q . "$(OUTPUT)" && printf "\nExpected:\n" && cat "$(OUTPUT)" || : \
	)

verify:
	for VER in 1 2; do \
		VER_NAME=$$([ $${VER} = 1 ] && echo "practice: " || echo "test:     "); \
		printf "day %s, part %s, %s" "$(DAY)" "$(PART)" "$${VER_NAME}"; \
		$(MAKE) run DAY=$(DAY) PART=$(PART) VER=$${VER} VERIFY=true; \
	done

verify-all:
	OK=0; \
	for D in $(DAYS); do \
		for PART in 1 2; do \
			RESULT=$$($(MAKE) verify DAY=$${D} PART=$${PART}); \
			printf "%s\n" "$${RESULT}"; \
			PASSED=$$(echo $${RESULT} | grep -o "Ok\." | wc -l); \
			OK=$$((OK + PASSED)); \
		done; \
	done; \
	printf "\nOk: %s/%s\n" "$${OK}" "$$((($(END) - $(START) + 1) * 4))"
