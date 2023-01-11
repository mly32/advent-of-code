# aoc-2022

[https://adventofcode.com/2022](https://adventofcode.com/2022)

## C++

To compile and run code for day `N`:

```shell
# $N is the day (two digits)
# question 0 for first question, question 1 for second question
# version 0 for practice, version 1 for final
make all DAY=$N Q=0 VER=0
```

## Rust

To compile and run code for day `N`:

```shell
# $N is the day (two digits)
# part 1 for first question, part 2 for second question
# version 1 for practice, version 2 for final
make run DAY=$N PART=1 VER=1
```

To test a range of days from `A` to `B`:

```shell
# start day and end day, inclusive range. requires two digit padding
make verify-all START=$A END=$B
```

An output list of times is included here: [times.txt](./rust/times.txt).
