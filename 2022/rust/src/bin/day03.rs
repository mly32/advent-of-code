use std::collections::HashSet;

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<String>,
}

impl Day for Data {
    type Output = i64;

    fn new(input: &str) -> Self {
        let data = input.lines().map(|l| l.to_string()).collect();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        self.data
            .iter()
            .map(|l| Data::chunk_sum(vec![&l[..l.len() / 2], &l[l.len() / 2..]]))
            .sum()
    }

    fn part2(&mut self) -> Self::Output {
        self.data
            .iter()
            .map(|s| s.as_str())
            .chunks(3)
            .into_iter()
            .map(|chunk| Data::chunk_sum(chunk.collect()))
            .sum()
    }
}

impl Data {
    fn chunk_sum(chunk: Vec<&str>) -> i64 {
        (chunk.iter())
            .map(|s| HashSet::from_iter(s.as_bytes().iter()))
            .fold(None, |acc, set2: HashSet<_>| {
                Some(if let Some(set1) = acc {
                    &set1 & &set2
                } else {
                    set2
                })
            })
            .unwrap_or_default()
            .iter()
            .map(|c| {
                (if c.is_ascii_lowercase() {
                    *c - b'a' + 1
                } else {
                    *c - b'A' + 27
                }) as i64
            })
            .sum()
    }
}

fn main() {
    Data::solve();
}
