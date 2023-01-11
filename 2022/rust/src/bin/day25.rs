use std::collections::HashMap;

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<String>,
}

impl Day for Data {
    type Output = String;

    fn new(input: &str) -> Self {
        let data = input.lines().map(str::to_string).collect_vec();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        let to_dec = HashMap::from([('2', 2), ('1', 1), ('0', 0), ('-', -1), ('=', -2)]);
        let to_snafu = (to_dec.iter().map(|(k, v)| (v, k))).collect::<HashMap<_, _>>();
        let mut sum: i64 = self
            .data
            .iter()
            .map(|s| {
                (s.chars().rev())
                    .fold((0, 1), |(v, place), c| {
                        let dv = place as i64 * to_dec[&c];
                        (v + dv, place * to_dec.len())
                    })
                    .0
            })
            .sum();

        let mut snafu_sum = String::new();
        while sum != 0 {
            let mut dig = sum % to_dec.len() as i64;
            sum /= to_dec.len() as i64;
            if dig > 2 {
                dig -= to_dec.len() as i64;
                sum += 1;
            }
            snafu_sum.push(*to_snafu[&dig]);
        }
        if snafu_sum.is_empty() {
            snafu_sum.push('0');
        }
        snafu_sum.chars().rev().collect()
    }

    fn part2(&mut self) -> Self::Output {
        String::new()
    }
}

fn main() {
    Data::solve();
}
