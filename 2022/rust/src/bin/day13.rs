use std::{cmp::Ordering, fmt::Debug};

use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<(Packet, Packet)>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let packet_pair = |i| ns::separated_pair(Packet::parse, ncc::newline, Packet::parse)(i);

        nc::map(nm::separated_list1(nbc::tag("\n\n"), packet_pair), |data| {
            Self { data }
        })(input)
    }

    fn part1(&mut self) -> Self::Output {
        (self.data.iter().enumerate())
            .filter_map(|(i, (a, b))| if a < b { Some(1 + i) } else { None })
            .sum()
    }

    fn part2(&mut self) -> Self::Output {
        let dividers = [Packet::new(2, 2), Packet::new(6, 2)];

        let mut v = Vec::from_iter(dividers.iter());
        for (a, b) in self.data.iter() {
            v.push(a);
            v.push(b);
        }
        v.sort();

        dividers
            .iter()
            .map(|d| 1 + v.partition_point(|&x| x < d))
            .product()
    }
}

#[derive(PartialEq, Eq, Clone)]
enum Packet {
    List(Vec<Packet>),
    Item(i64),
}

impl Packet {
    fn new(i: i64, depth: usize) -> Self {
        if depth == 0 {
            Self::Item(i)
        } else {
            Self::List(vec![Packet::new(i, depth - 1)])
        }
    }

    fn parse(input: &str) -> IResult<&str, Self> {
        let list = |i| {
            nc::map(
                ns::delimited(
                    nbc::tag("["),
                    nm::separated_list0(nbc::tag(","), Self::parse),
                    nbc::tag("]"),
                ),
                Self::List,
            )(i)
        };
        let item = |i| nc::map(ncc::i64, Self::Item)(i);
        nb::alt((list, item))(input)
    }
}

impl Ord for Packet {
    fn cmp(&self, other: &Self) -> Ordering {
        match (self, other) {
            (Self::List(a), Self::List(b)) => a.cmp(b),
            (Self::Item(a), Self::Item(b)) => a.cmp(b),
            (Self::Item(a), _) => Self::new(*a, 1).cmp(other),
            (_, Self::Item(b)) => self.cmp(&Self::new(*b, 1)),
        }
    }
}

impl PartialOrd for Packet {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Debug for Packet {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::List(l) => f.debug_list().entries(l.iter()).finish(),
            Self::Item(i) => write!(f, "{}", i),
        }
    }
}

fn main() {
    Data::solve();
}
