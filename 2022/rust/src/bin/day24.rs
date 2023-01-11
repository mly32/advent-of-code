use std::collections::{HashMap, HashSet};

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vec<Vec<char>>>,
    walls: Vec<Vec<bool>>,
}

impl Day for Data {
    type Output = usize;

    fn new(input: &str) -> Self {
        let data = (input.lines())
            .map(|l| {
                l.chars()
                    .map(|c| match c {
                        '.' => vec![],
                        _ => vec![c],
                    })
                    .collect_vec()
            })
            .collect_vec();
        let walls = (input.lines())
            .map(|l| l.chars().map(|c| c == '#').collect_vec())
            .collect_vec();
        Self { data, walls }
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

impl Data {
    fn ok_pos(&self, (r, c): (usize, usize)) -> bool {
        !self.walls[r][c] && self.data[r][c].is_empty()
    }

    fn common_part(&mut self, part1: bool) -> <Self as Day>::Output {
        let (m, n) = (
            self.data.len(),
            self.data.get(0).map(|x| x.len()).unwrap_or_default(),
        );
        let dirs = HashMap::from([
            ('^', (-1, 0)),
            ('v', (1, 0)),
            ('<', (0, -1)),
            ('>', (0, 1)),
            ('_', (0, 0)),
        ]);
        let move_dir = |(r, c), d| {
            let (dr, dc) = dirs[&d];
            (
                modulus(r as i64 + dr, m) as usize,
                modulus(c as i64 + dc, n) as usize,
            )
        };

        let start_y = self.walls[0].iter().find_position(|&s| !s);
        let start = (0, start_y.unwrap().0);
        let end_y = (self.walls[m - 1].iter()).find_position(|&s| !s);
        let end = (m - 1, end_y.unwrap().0);

        let goals = if part1 {
            vec![end]
        } else {
            vec![end, start, end]
        };

        let last_goal = (*goals.last().unwrap(), goals.len());
        let mut dp = HashSet::from([((start), 0)]);
        let mut t = 0;

        while !dp.contains(&last_goal) {
            let mut grid = vec![vec![vec![]; n]; m];
            for (r, c) in (0..m).cartesian_product(0..n) {
                if self.walls[r][c] {
                    continue;
                }
                for &d in self.data[r][c].iter() {
                    let (mut next_r, mut next_c) = move_dir((r, c), d);
                    if self.walls[next_r][next_c] {
                        for _ in 0..2 {
                            (next_r, next_c) = move_dir((next_r, next_c), d);
                        }
                    }
                    grid[next_r][next_c].push(d);
                }
            }
            self.data = grid;

            let mut next = HashSet::new();
            for (pos, g) in dp {
                for (&d, &(dr, dc)) in dirs.iter() {
                    if pos == start && d == '^' || pos == end && d == 'v' {
                        continue;
                    }
                    let (r, c) = (pos.0 + dr as usize, pos.1 + dc as usize);
                    if self.ok_pos((r, c)) {
                        let i = if (r, c) == goals[g] { g + 1 } else { g };
                        next.insert(((r, c), i));
                    }
                }
            }
            dp = next;
            t += 1;
        }
        t
    }
}

fn main() {
    Data::solve();
}
