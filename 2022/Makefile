.PHONY: build

TGT=aoc.out

build:
	@if [ -f "src/day$(DAY).cpp" ]; then\
		g++ "src/day$(DAY).cpp" -o $(TGT) \
			-std=c++20 -O3 -Wall -Wextra && printf "Compiled to ./$(TGT)\n\n";\
	else\
		printf "Code file does not exist\n";\
	fi

run:
	@/usr/bin/time --format="\n%E real, %U user, %S sys" \
		./$(TGT) $(DAY) $(VER) $(Q)

clean:
	rm -f $(TGT)

all: build run
