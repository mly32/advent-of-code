use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<i64>,
}

impl Day for Data {
    type Output = i64;

    fn new(input: &str) -> Self {
        let data = (input.lines())
            .map(|n| n.parse::<i64>().unwrap())
            .collect_vec();
        Self { data }
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
        let scale = if part1 { 1 } else { 811_589_153 };
        let values = self.data.iter().map(|x| scale * x).collect_vec();

        let keys = self.data.len();
        let mut pos = (0..keys).collect_vec();
        let mut list = (0..keys).collect_vec();
        let zero_key = self.data.iter().find_position(|&&x| x == 0).unwrap().0;

        let repeat = if part1 { 1 } else { 10 };
        for _ in 0..repeat {
            for key in 0..keys {
                let p = pos[key];
                let delta = modulus(values[key], keys - 1);

                for i in p..(p + delta as usize) {
                    let (from, to) = (i % keys, (i + 1) % keys);
                    pos.swap(list[from], list[to]);
                    list.swap(from, to);
                }
            }
        }

        let zero_pos = pos[zero_key];
        ([1_000, 2_000, 3_000].iter())
            .map(|delta| {
                let key = list[(zero_pos + delta) % keys];
                values[key]
            })
            .sum()
    }
}

fn main() {
    Data::solve();
}
