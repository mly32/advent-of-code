use std::collections::HashMap;

use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Monkey>,
    id: HashMap<String, usize>,
    memo: HashMap<usize, i64>,
}

impl Day for Data {
    type Output = i64;

    fn parse(input: &str) -> IResult<&str, Self> {
        let (_, names) = nm::separated_list0(
            ncc::newline,
            ns::terminated(ncc::alpha1, nbc::take_until("\n")),
        )(input)?;
        let id = (names.iter().enumerate())
            .map(|(id, name)| (name.to_string(), id))
            .collect::<HashMap<_, _>>();

        let op = |i| {
            nb::alt((
                nc::map(nbc::tag(" + "), |_| Op::Add),
                nc::map(nbc::tag(" - "), |_| Op::Sub),
                nc::map(nbc::tag(" * "), |_| Op::Mul),
                nc::map(nbc::tag(" / "), |_| Op::Div),
            ))(i)
        };

        let number = |i| {
            nc::map(
                ns::preceded(ns::pair(ncc::alpha1, nbc::tag(": ")), ncc::i64),
                Monkey::Number,
            )(i)
        };
        let math = |i| {
            nc::map(
                ns::preceded(
                    ns::pair(ncc::alpha1, nbc::tag(": ")),
                    ns::tuple((ncc::alpha1, op, ncc::alpha1)),
                ),
                |(a, op, b)| Monkey::Math(id[a], op, id[b]),
            )(i)
        };

        let result = nc::map(
            nm::separated_list0(ncc::newline, nb::alt((number, math))),
            |data| Self {
                data,
                id: id.clone(),
                memo: HashMap::new(),
            },
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Op {
    Add,
    Sub,
    Mul,
    Div,
    Eq,
}

#[derive(Debug, Clone, Copy)]
enum Monkey {
    Number(i64),
    Math(usize, Op, usize),
    X,
}

impl Data {
    fn common_part(&mut self, part1: bool) -> <Self as Day>::Output {
        let root = self.id["root"];
        if !part1 {
            if let Monkey::Math(a, _, b) = self.data[root] {
                self.data[root] = Monkey::Math(a, Op::Eq, b);
            } else {
                panic!("root is not a math operation");
            }
            self.data[self.id["humn"]] = Monkey::X;
        }
        self.dfs(root, None).unwrap()
    }

    fn dfs(&mut self, monkey: usize, known: Option<i64>) -> Option<i64> {
        if self.memo.contains_key(&monkey) {
            return Some(self.memo[&monkey]);
        }
        let result_opt = match self.data[monkey] {
            Monkey::Number(n) => Some(n),
            Monkey::Math(a, op, b) => match (self.dfs(a, None), self.dfs(b, None)) {
                (Some(x), Some(y)) => match op {
                    Op::Add => Some(x + y),
                    Op::Sub => Some(x - y),
                    Op::Mul => Some(x * y),
                    Op::Div => Some(x / y),
                    Op::Eq => panic!("unexpected equal"),
                },
                (Some(x), None) => {
                    if let Some(v) = known {
                        let k = match op {
                            Op::Add => v - x,
                            Op::Sub => x - v,
                            Op::Mul => v / x,
                            Op::Div => x / v,
                            Op::Eq => panic!("unexpected equal"),
                        };
                        self.dfs(b, Some(k))
                    } else if op == Op::Eq {
                        self.dfs(b, Some(x))
                    } else {
                        None
                    }
                }
                (None, Some(y)) => {
                    if let Some(v) = known {
                        let k = match op {
                            Op::Add => v - y,
                            Op::Sub => y + v,
                            Op::Mul => v / y,
                            Op::Div => y * v,
                            Op::Eq => panic!("unexpected equal"),
                        };
                        self.dfs(a, Some(k))
                    } else if op == Op::Eq {
                        self.dfs(a, Some(y))
                    } else {
                        None
                    }
                }
                (None, None) => None,
            },
            Monkey::X => known,
        };
        if let Some(result) = result_opt {
            self.memo.insert(monkey, result);
        }
        result_opt
    }
}

fn main() {
    Data::solve();
}
