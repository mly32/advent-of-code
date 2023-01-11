use std::{collections::HashSet, fmt::Display};

use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: HashSet<(i64, i64)>,
}

impl Day for Data {
    type Output = usize;

    fn new(input: &str) -> Self {
        let grid = input.lines().collect_vec();
        let data = grid
            .into_iter()
            .enumerate()
            .flat_map(|(r, row)| {
                row.chars()
                    .enumerate()
                    .filter(|&(_, ch)| ch == '#')
                    .map(move |(c, _)| (r as i64, c as i64))
            })
            .collect();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

#[derive(Clone, Copy)]
#[repr(usize)]
enum Facing {
    N,
    S,
    W,
    E,
    NE,
    NW,
    SE,
    SW,
}

impl Facing {
    const CHECKS: [(Self, [Self; 3]); 4] = [
        (Self::N, [Self::N, Self::NE, Self::NW]),
        (Self::S, [Self::S, Self::SE, Self::SW]),
        (Self::W, [Self::W, Self::NW, Self::SW]),
        (Self::E, [Self::E, Self::NE, Self::SE]),
    ];

    const DIRS: [(i64, i64); 8] = [
        (-1, 0),
        (1, 0),
        (0, -1),
        (0, 1),
        (-1, 1),
        (-1, -1),
        (1, 1),
        (1, -1),
    ];
}

impl Display for Data {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let (a, b) = self.bounds();
        for r in a.0..=b.0 {
            for c in a.1..=b.1 {
                if self.data.contains(&(r, c)) {
                    write!(f, "#")?;
                } else {
                    write!(f, ".")?;
                }
            }
            writeln!(f)?;
        }
        Ok(())
    }
}

impl Data {
    fn bounds(&self) -> ((i64, i64), (i64, i64)) {
        let init = ((i64::MAX, i64::MAX), (i64::MIN, i64::MIN));
        (self.data.iter()).fold(init, |(a, b), v| {
            ((a.0.min(v.0), a.1.min(v.1)), (b.0.max(v.0), b.1.max(v.1)))
        })
    }

    fn add(pos: (i64, i64), d: usize) -> (i64, i64) {
        (pos.0 + Facing::DIRS[d].0, pos.1 + Facing::DIRS[d].1)
    }

    fn move_pos(&self, pos: (i64, i64), t: usize) -> ((i64, i64), Option<Facing>) {
        let is_empty = |d| !self.data.contains(&Self::add(pos, d));
        if (0..Facing::DIRS.len()).all(is_empty) {
            return (pos, None);
        }

        for i in 0..Facing::CHECKS.len() {
            let (d, check) = Facing::CHECKS[(t + i) % Facing::CHECKS.len()];
            if check.iter().all(|&f| is_empty(f as usize)) {
                return (Self::add(pos, d as usize), Some(d));
            }
        }
        (pos, None)
    }

    fn common_part(&mut self, part1: bool) -> <Self as Day>::Output {
        for t in 0.. {
            if part1 && t == 10 {
                break;
            }

            let mut moved = 0;
            let mut next = HashSet::new();

            for &pos in self.data.iter() {
                let (next_pos, f_opt) = self.move_pos(pos, t);
                moved += usize::from(f_opt.is_some());
                if !next.insert(next_pos) {
                    // collisions occur from opposite directions
                    next.remove(&next_pos);
                    next.insert(pos);
                    next.insert(Self::add(next_pos, f_opt.unwrap() as usize));
                    moved -= 2;
                }
            }

            self.data = next;
            if !part1 && moved == 0 {
                return t + 1;
            }
        }

        let (a, b) = self.bounds();
        let area = (b.0 - a.0 + 1) as usize * (b.1 - a.1 + 1) as usize;
        area - self.data.len()
    }
}

fn main() {
    Data::solve();
}
