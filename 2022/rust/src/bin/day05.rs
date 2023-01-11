use std::fmt;

use itertools::Itertools;
use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug, Clone)]
struct Data {
    data: Vec<Vec<char>>,
    moves: Vec<Move>,
}

impl Day for Data {
    type Output = Data;

    fn parse(input: &str) -> IResult<&str, Self> {
        let num = |i| whitespace(ncc::i64)(i);
        let none = |i| nc::map(nbc::tag("   "), |_| None)(i);
        let some = |i| {
            nc::map(
                ns::delimited(
                    nbc::tag("["),
                    ncc::satisfy(char::is_alphabetic),
                    nbc::tag("]"),
                ),
                Some,
            )(i)
        };
        let item = |i| nb::alt((none, some))(i);

        let hor_data =
            |i| nm::separated_list1(ncc::newline, nm::separated_list1(nbc::tag(" "), item))(i);
        let numbers = |i| nm::many1(num)(i);
        let data = |i| {
            nc::map(ns::pair(hor_data, numbers), |(d, n)| {
                (0..n.len())
                    .map(|i| {
                        (0..d.len())
                            .rev()
                            .filter_map(|j| d[j].get(i).cloned().flatten())
                            .collect_vec()
                    })
                    .collect_vec()
            })(i)
        };

        let move_line = |i| {
            nc::map(
                ns::tuple((
                    ns::preceded(nbc::tag("move"), num),
                    ns::preceded(nbc::tag("from"), num),
                    ns::preceded(nbc::tag("to"), num),
                )),
                |(a, f, t)| Move {
                    amount: a as usize,
                    from: (f - 1) as usize,
                    to: (t - 1) as usize,
                },
            )(i)
        };
        let moves = |i| nm::many1(move_line)(i);

        let result = nc::map(ns::pair(data, moves), |(data, moves)| Self { data, moves })(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        for &Move { amount, from, to } in self.moves.iter() {
            for _ in 0..amount {
                let c = self.data[from].pop().unwrap();
                self.data[to].push(c)
            }
        }
        self.clone()
    }

    fn part2(&mut self) -> Self::Output {
        for &Move { amount, from, to } in self.moves.iter() {
            let len = self.data[from].len();
            self.data[from]
                .drain(len - amount..)
                .collect_vec()
                .iter()
                .for_each(|c| self.data[to].push(*c));
        }
        self.clone()
    }
}

#[derive(Debug, Clone)]
struct Move {
    amount: usize,
    from: usize,
    to: usize,
}

impl fmt::Display for Data {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let v = (self.data.iter())
            .filter_map(|col| col.last())
            .collect::<String>();
        write!(f, "{}", v)
    }
}

fn main() {
    Data::solve();
}
