use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vec<i64>>,
}

impl Day for Data {
    type Output = i64;

    fn new(input: &str) -> Self {
        let data = input
            .split("\n\n")
            .map(|s| s.lines().map(|n| n.parse::<i64>().unwrap()).collect_vec())
            .collect_vec();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        self.data.iter().map(|l| l.iter().sum()).max().unwrap_or(0)
    }

    fn part2(&mut self) -> Self::Output {
        let sums = self
            .data
            .iter()
            .map(|l| l.iter().sum::<i64>())
            .sorted_by(|a, b| b.cmp(a))
            .collect_vec();
        sums[..3].iter().sum()
    }
}

fn main() {
    Data::solve();
}
