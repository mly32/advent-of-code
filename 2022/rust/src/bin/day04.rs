use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<((i64, i64), (i64, i64))>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let range = |i| ns::separated_pair(ncc::i64, nbc::tag("-"), ncc::i64)(i);
        let range_pair = |i| ns::separated_pair(range, nbc::tag(","), range)(i);

        let result = nc::map(nm::separated_list1(ncc::newline, range_pair), |data| Self {
            data,
        })(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        self.data
            .iter()
            .filter(|((a1, a2), (b1, b2))| (a1 <= b1 && b2 <= a2) || (b1 <= a1 && a2 <= b2))
            .count()
    }

    fn part2(&mut self) -> Self::Output {
        self.data
            .iter()
            .filter(|((a1, a2), (b1, b2))| a1 <= b2 && b1 <= a2)
            .count()
    }
}

fn main() {
    Data::solve();
}
