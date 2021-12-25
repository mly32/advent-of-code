.PHONY: build


build:
	@if [ -f "day$(ARGS).cpp" ]; then\
		g++ "day$(ARGS).cpp" -O3 -Wall -Wextra && printf "Compiled to ./a.out\n\n";\
	else\
		printf "Code file does not exist\n";\
	fi

run:
	@/usr/bin/time --format="\n%E real, %U user, %S sys" ./a.out

all: build run
