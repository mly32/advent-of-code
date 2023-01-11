use std::collections::{HashSet, VecDeque};

use itertools::Itertools;
use nalgebra::Vector3;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vector3<i64>>,
    bounds: Option<(Vector3<i64>, Vector3<i64>)>,
}

impl Day for Data {
    type Output = usize;

    fn new(input: &str) -> Self {
        let data = (input.lines())
            .map(|l| {
                let v = (l.split(',').map(|n| n.parse::<i64>().unwrap())).collect_vec();
                Vector3::new(v[0], v[1], v[2])
            })
            .collect_vec();
        let bounds = data.iter().fold(None, |acc_opt, &v| {
            Some(acc_opt.map_or((v, v), |(mn, mx)| (v.inf(&mn), v.sup(&mx))))
        });
        Self { data, bounds }
    }

    fn part1(&mut self) -> Self::Output {
        let counts = self.data.iter().counts();
        (self.data.iter().cartesian_product(Self::DIRS.iter()))
            .filter(|&(v, dv)| !counts.contains_key(&(v + dv)))
            .count()
    }

    fn part2(&mut self) -> Self::Output {
        if self.bounds.is_none() {
            return 0;
        }
        let (mn, mx) = self.bounds.unwrap();
        let (mn, mx) = (mn - Vector3::from_element(1), mx + Vector3::from_element(1));
        let counts = self.data.iter().counts();
        let mut seen = HashSet::from([mn]);
        let mut queue = VecDeque::from([mn]);
        let mut result = 0;
        while let Some(v) = queue.pop_front() {
            for dv in Self::DIRS {
                let u = v + dv;
                if counts.contains_key(&u) {
                    result += 1;
                } else if !seen.contains(&u) && u >= mn && u <= mx {
                    seen.insert(u);
                    queue.push_back(u);
                }
            }
        }
        result
    }
}

impl Data {
    const DIRS: [Vector3<i64>; 6] = [
        Vector3::new(1, 0, 0),
        Vector3::new(0, 1, 0),
        Vector3::new(0, 0, 1),
        Vector3::new(-1, 0, 0),
        Vector3::new(0, -1, 0),
        Vector3::new(0, 0, -1),
    ];
}

fn main() {
    Data::solve();
}
