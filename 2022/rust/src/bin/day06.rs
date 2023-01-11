use std::{collections::HashMap, fmt};

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<String>,
}

impl Day for Data {
    type Output = Output;

    fn new(input: &str) -> Self {
        let data = input.lines().map(|s| s.to_string()).collect_vec();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(4)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(14)
    }
}

impl Data {
    fn common_part(&self, window: usize) -> Output {
        Output(
            self.data
                .iter()
                .map(|s| Data::single_solve(s, window))
                .collect_vec(),
        )
    }

    fn single_solve(s: &String, window: usize) -> usize {
        let mut freq = HashMap::<_, usize>::new();
        for (i, &c) in s.as_bytes().iter().enumerate() {
            *freq.entry(c).or_default() += 1;
            if i >= window {
                let d = s.as_bytes()[i - window];
                if *freq.entry(d).and_modify(|e| *e -= 1).or_default() == 0 {
                    freq.remove(&d);
                }
            }
            if freq.keys().len() == window {
                return i + 1;
            }
        }
        0
    }
}
struct Output(Vec<usize>);

impl fmt::Display for Output {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.0.iter().enumerate().fold(Ok(()), |result, (i, v)| {
            result.and_then(|_| {
                if i != 0 {
                    writeln!(f)?;
                }
                write!(f, "{}", v)
            })
        })
    }
}

fn main() {
    Data::solve();
}
