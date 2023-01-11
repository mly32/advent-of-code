use std::collections::HashMap;

use itertools::Itertools;
use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};
use num::Integer;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Monkey>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let id = |i| ns::delimited(nbc::tag("Monkey "), ncc::u64, nbc::tag(":"))(i);

        let items = |i| {
            ns::preceded(
                nbc::tag("Starting items: "),
                nm::separated_list1(nbc::tag(", "), ncc::i64),
            )(i)
        };

        let operand = |i| nb::alt((nc::map(nbc::tag("old"), |_| None), nc::map(ncc::i64, Some)))(i);
        let operation = |i| {
            ns::preceded(
                nbc::tag("Operation: new = old "),
                ns::separated_pair(ncc::one_of("*+"), nbc::tag(" "), operand),
            )(i)
        };

        let test = |i| ns::preceded(nbc::tag("Test: divisible by "), ncc::i64)(i);

        let jump_num = |i| ns::preceded(nbc::take_till(|c: char| c.is_ascii_digit()), ncc::u64)(i);

        let monkey = |i| {
            nc::map(
                ns::tuple((
                    whitespace(id),
                    whitespace(items),
                    whitespace(operation),
                    whitespace(test),
                    whitespace(jump_num),
                    whitespace(jump_num),
                )),
                |(id, items, operation, test, jump_true, jump_false)| Monkey {
                    id: id as usize,
                    items,
                    operation,
                    test,
                    jump: (jump_true as usize, jump_false as usize),
                },
            )(i)
        };

        let result = nc::map(
            nm::many0(ns::terminated(monkey, ncc::multispace0)),
            |data| Self { data },
        )(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

#[derive(Debug)]
struct Monkey {
    id: usize,
    items: Vec<i64>,
    operation: (char, Option<i64>),
    test: i64,
    jump: (usize, usize),
}

impl Monkey {
    fn op(&self, x: i64) -> i64 {
        match self.operation {
            ('*', Some(y)) => x * y,
            ('*', None) => x * x,
            ('+', Some(y)) => x + y,
            ('+', None) => x + x,
            _ => panic!("invalid operation"),
        }
    }
}

impl Data {
    fn common_part(&mut self, part1: bool) -> <Self as Day>::Output {
        let rounds = if part1 { 20 } else { 10_000 };
        let modulo = self.data.iter().fold(1, |acc, x| acc.lcm(&x.test));
        let mut inspected = HashMap::<_, usize>::new();

        for (_, id) in (0..rounds).cartesian_product(0..self.data.len()) {
            let m = &self.data[id];
            let next_info = |&x| {
                let v = if part1 { m.op(x) / 3 } else { m.op(x) % modulo };
                let next_id = if v % m.test == 0 { m.jump.0 } else { m.jump.1 };
                (v, next_id)
            };

            *inspected.entry(m.id).or_default() += m.items.len();
            let next = m.items.iter().map(next_info).collect_vec();
            self.data[id].items.clear();
            for (v, next_id) in next {
                self.data[next_id].items.push(v);
            }
        }

        let mut v = inspected.values().collect_vec();
        v.select_nth_unstable_by(1, |a, b| b.cmp(a));
        v[0] * v[1]
    }
}

fn main() {
    Data::solve();
}
