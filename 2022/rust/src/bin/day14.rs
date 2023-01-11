use std::collections::HashMap;

use itertools::Itertools;
use nalgebra::Vector2;
use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: HashMap<Vector2<i64>, Space>,
    last: HashMap<i64, i64>,
    floor: i64,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let line = |i| {
            nm::separated_list0(
                nbc::tag(" -> "),
                nc::map(
                    ns::separated_pair(ncc::i64, nbc::tag(","), ncc::i64),
                    |(a, b)| Vector2::new(a, b),
                ),
            )(i)
        };
        let from_paths = |paths: Vec<Vec<Vector2<i64>>>| {
            let mut data = HashMap::new();
            for path in paths {
                for (a, b) in path.iter().tuple_windows() {
                    let delta = (b - a).map(|x| x.clamp(-1, 1));
                    let mut v = *a;
                    while !v.eq(b) {
                        data.insert(v, Space::Rock);
                        v += delta;
                    }
                }
                path.last().and_then(|v| data.insert(*v, Space::Rock));
            }

            let mut last = HashMap::new();
            for v in data.keys() {
                last.entry(v.x)
                    .and_modify(|e| *e = v.y.max(*e))
                    .or_insert(v.y);
            }
            let floor = last.values().max().cloned().unwrap_or_default() + 2;
            Self { data, last, floor }
        };

        nc::map(nm::separated_list0(ncc::newline, line), from_paths)(input)
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

#[derive(Debug)]
enum Space {
    Rock,
    Sand,
}

impl Data {
    fn common_part(&mut self, part1: bool) -> <Self as Day>::Output {
        let mut result = 0;
        while self.fall(!part1) {
            result += 1;
        }
        result
    }

    fn fall(&mut self, has_floor: bool) -> bool {
        let mut v = Vector2::new(500, 0);
        if self.data.contains_key(&v) {
            return false;
        }
        while self.last.get(&v.x).map(|&y| y > v.y).unwrap_or(false) {
            v.y += 1;
            if !self.data.contains_key(&v) {
                continue;
            }
            v.x -= 1;
            if !self.data.contains_key(&v) {
                continue;
            }
            v.x += 2;
            if !self.data.contains_key(&v) {
                continue;
            }

            v -= Vector2::new(1, 1);
            self.data.insert(v, Space::Sand);
            return true;
        }

        if !has_floor {
            return false;
        }
        while !self.data.contains_key(&v) && v.y < self.floor {
            v.y += 1;
        }
        v.y -= 1;

        self.data.insert(v, Space::Sand);
        self.last.insert(v.x, self.floor);
        true
    }
}

fn main() {
    Data::solve();
}
