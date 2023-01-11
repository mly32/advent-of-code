use std::{cmp::Ordering, collections::BinaryHeap};

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vec<i64>>,
    start: (usize, usize),
    end: (usize, usize),
}

impl Day for Data {
    type Output = usize;

    fn new(input: &str) -> Self {
        let chars = input.lines().map(|s| s.as_bytes()).collect_vec();
        let (mut start, mut end) = ((0, 0), (0, 0));
        let data = (chars.iter().enumerate())
            .map(|(r, row)| {
                (row.iter().enumerate())
                    .map(|(c, &x)| match x {
                        b'S' => {
                            start = (r, c);
                            0
                        }
                        b'E' => {
                            end = (r, c);
                            25
                        }
                        _ => (x - b'a') as i64,
                    })
                    .collect_vec()
            })
            .collect_vec();

        Self { data, start, end }
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

#[derive(PartialEq, Eq)]
struct State {
    cost: usize,
    pos: (usize, usize),
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
        (other.cost)
            .cmp(&self.cost)
            .then_with(|| self.pos.cmp(&other.pos))
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Data {
    fn grid_neighbors(pos: (usize, usize), (m, n): (usize, usize)) -> Vec<(usize, usize)> {
        DIRS.values()
            .filter_map(|&(dr, dc)| {
                let (r, c) = (pos.0 as i64 + dr, pos.1 as i64 + dc);
                if 0 <= r && (r as usize) < m && 0 <= c && (c as usize) < n {
                    Some((r as usize, c as usize))
                } else {
                    None
                }
            })
            .collect_vec()
    }

    fn common_part(&self, part1: bool) -> <Self as Day>::Output {
        let (m, n) = (
            self.data.len(),
            self.data.get(0).map(|x| x.len()).unwrap_or_default(),
        );
        let mut dist = (0..m)
            .map(|_| (0..n).map(|_| usize::MAX).collect_vec())
            .collect_vec();
        let mut min_heap = BinaryHeap::new();

        let start = if part1 { self.start } else { self.end };
        dist[start.0][start.1] = 0;
        min_heap.push(State {
            cost: 0,
            pos: start,
        });

        while let Some(State { cost, pos }) = min_heap.pop() {
            if part1 && pos == self.end {
                return cost;
            }
            if cost > dist[pos.0][pos.1] {
                continue;
            }

            Self::grid_neighbors(pos, (m, n))
                .iter()
                .filter(|u| {
                    let diff = self.data[u.0][u.1] - self.data[pos.0][pos.1];
                    if part1 {
                        diff <= 1
                    } else {
                        -1 <= diff
                    }
                })
                .for_each(|&u| {
                    let next = State {
                        cost: dist[pos.0][pos.1] + 1,
                        pos: u,
                    };

                    if next.cost < dist[next.pos.0][next.pos.1] {
                        dist[next.pos.0][next.pos.1] = next.cost;
                        min_heap.push(next);
                    }
                })
        }

        if part1 {
            0
        } else {
            ((0..m).cartesian_product(0..n))
                .filter(|&(r, c)| self.data[r][c] == 0)
                .map(|(r, c)| dist[r][c])
                .min()
                .unwrap_or_default()
        }
    }
}

fn main() {
    Data::solve();
}
