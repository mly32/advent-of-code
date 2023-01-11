use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vec<u32>>,
}

impl Day for Data {
    type Output = usize;

    fn new(input: &str) -> Self {
        let data = (input.lines())
            .map(|s| s.chars().map(|c| c.to_digit(10).unwrap()).collect_vec())
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
        let m = self.data.len();
        let n = self.data.get(0).map(|x| x.len()).unwrap_or_default();
        let res = ((0..m).cartesian_product(0..n)).map(|(r, c)| self.is_vis((r as i64, c as i64)));
        if part1 {
            res.filter(|x| x.iter().any(|&x| x.0)).count()
        } else {
            res.map(|x| x.iter().map(|&x| x.1).product())
                .max()
                .unwrap_or_default()
        }
    }

    fn is_vis(&self, (r, c): (i64, i64)) -> Vec<(bool, usize)> {
        let &orig = self
            .data
            .get(r as usize)
            .and_then(|x| x.get(c as usize))
            .unwrap();
        DIRS.values()
            .map(|&(dr, dc)| {
                let (mut i, mut j) = (r + dr, c + dc);
                while let Some(&next) = self.data.get(i as usize).and_then(|x| x.get(j as usize)) {
                    if orig <= next {
                        return (false, ((r - i).abs() + (c - j).abs()) as usize);
                    }
                    i += dr;
                    j += dc;
                }
                (true, ((i - r - dr).abs() + (j - c - dc).abs()) as usize)
            })
            .collect_vec()
    }
}

fn main() {
    Data::solve();
}
