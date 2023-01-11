use std::collections::HashSet;

use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<(char, usize)>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let line = |i| {
            nc::map(
                ns::separated_pair(ncc::satisfy(char::is_alphabetic), nbc::tag(" "), ncc::u64),
                |(c, n)| (c, n as usize),
            )(i)
        };
        nc::map(nm::separated_list0(ncc::newline, line), |data| Self {
            data,
        })(input)
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(2)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(10)
    }
}

impl Data {
    fn common_part(&self, knots: usize) -> <Self as Day>::Output {
        let mut rope = vec![(0, 0); knots];
        let mut seen = HashSet::new();
        for &(c, n) in self.data.iter() {
            for _ in 0..n {
                rope[0].0 += DIRS[&c].0;
                rope[0].1 += DIRS[&c].1;
                for i in 1..knots {
                    let diff = (rope[i - 1].0 - rope[i].0, rope[i - 1].1 - rope[i].1);
                    if diff.0.abs() > 1 || diff.1.abs() > 1 {
                        rope[i].0 += diff.0.clamp(-1, 1);
                        rope[i].1 += diff.1.clamp(-1, 1);
                    }
                }
                seen.insert(rope[knots - 1]);
            }
        }
        seen.len()
    }
}

fn main() {
    Data::solve();
}
