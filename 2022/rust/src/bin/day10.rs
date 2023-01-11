use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<(i64, i64)>,
}

impl Day for Data {
    type Output = String;

    fn parse(input: &str) -> IResult<&str, Self> {
        let line = |i| {
            nb::alt((
                nc::map(ns::preceded(nbc::tag("addx "), ncc::i64), |x| (2, x)),
                nc::map(nbc::tag("noop"), |_| (1, 0)),
            ))(i)
        };
        nc::map(nm::separated_list0(ncc::newline, line), |data| Self {
            data,
        })(input)
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

impl Data {
    fn common_part(&self, part1: bool) -> <Self as Day>::Output {
        let (mut res, mut t, mut x) = (0, 0, 1);
        let mut screen = String::new();
        for &(dt, dx) in self.data.iter() {
            for _ in 0..dt {
                t += 1;
                if t % 40 == 20 {
                    res += t * x;
                }
                let pos = (t - 1) % 40;
                let pix = x - 1 <= pos && pos <= x + 1;

                if screen.len() % 41 == 40 {
                    screen.push('\n')
                }
                screen.push(if pix { '#' } else { '.' });
            }
            x += dx;
        }
        if part1 {
            res.to_string()
        } else {
            screen
        }
    }
}

fn main() {
    Data::solve();
}
