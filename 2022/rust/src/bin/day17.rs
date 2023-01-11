use std::collections::HashMap;

use itertools::Itertools;
use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Dir>,
    rocks: Vec<Rock>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let data = |i| nm::many1(nc::map(ncc::one_of("<>"), Dir::new))(i);
        let rock = |i| {
            nc::map(
                nm::separated_list1(ncc::newline, nm::many1(ncc::one_of("#."))),
                |rows| {
                    let len = rows.get(0).map(|x| x.len()).unwrap_or_default();
                    let shape = (rows.into_iter().rev())
                        .map(|s| {
                            (s.iter().rev()).fold(0, |acc, &c| (acc << 1) + u8::from(c == '#'))
                        })
                        .collect_vec();
                    Rock::new(len, shape)
                },
            )(i)
        };

        nc::map(
            ns::separated_pair(
                data,
                nbc::tag("\n\n"),
                nm::separated_list0(nbc::tag("\n\n"), rock),
            ),
            |(data, rocks)| Self { data, rocks },
        )(input)
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(2022)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(1_000_000_000_000)
    }
}

#[derive(Debug, Clone, Copy)]
#[repr(i32)]
enum Dir {
    Left = -1,
    Right = 1,
}

impl Dir {
    fn new(c: char) -> Self {
        if c == '<' {
            Self::Left
        } else {
            Self::Right
        }
    }
}

#[derive(Debug)]
struct Rock {
    rows: usize,
    len: usize,
    shapes: Vec<Vec<u8>>,
    heights: Vec<usize>,
}

impl Rock {
    fn new(len: usize, shape: Vec<u8>) -> Self {
        let rows = shape.len();
        let shapes = (0..Data::WIDTH)
            .map(|shift| (shape.iter()).map(|row| row << shift).collect_vec())
            .collect_vec();
        let heights = (0..len)
            .map(|c| {
                (shape.iter().enumerate())
                    .filter(|(_, &row)| row as usize & (1 << c) != 0)
                    .map(|(r, _)| r + 1)
                    .max()
                    .unwrap_or_default()
            })
            .collect_vec();
        Self {
            rows,
            len,
            shapes,
            heights,
        }
    }
}

impl Data {
    const WIDTH: usize = 7;
    const FLOOR: u8 = (1 << Self::WIDTH) - 1;
    const START: (usize, usize) = (3, 2);

    fn common_part(&self, total_rocks: usize) -> <Self as Day>::Output {
        let mut rocks = self.rocks.iter().enumerate().cycle();
        let mut wind = self.data.iter().enumerate().cycle().peekable();

        let mut chamber = vec![Self::FLOOR];
        let mut heights: Vec<Option<usize>> = vec![None; Self::WIDTH];
        let mut memo = HashMap::new();
        let (mut rock_count, mut skipped) = (0, None);

        while rock_count < total_rocks {
            let (rock_key, rock) = rocks.next().unwrap();

            if skipped.is_none() {
                let wind_key = wind.peek().unwrap().0;
                let surface = heights
                    .iter()
                    .map(|&o| o.map(|h| chamber.len() - h))
                    .collect_vec();
                let key = (rock_key, wind_key, surface);
                if let Some((old_rock_count, old_max_height)) = memo.get(&key) {
                    let rock_diff = rock_count - old_rock_count;
                    let height_diff = chamber.len() - old_max_height;
                    let cycles = (total_rocks - 1 - rock_count) / rock_diff;
                    skipped = Some(cycles * height_diff);
                    rock_count += cycles * rock_diff;
                } else {
                    memo.insert(key, (rock_count, chamber.len()));
                }
            }

            let mut pos = (chamber.len() + Self::START.0, Self::START.1);
            for move_down in [false, true].into_iter().cycle() {
                let (r, c) = if move_down {
                    (pos.0 - 1, pos.1)
                } else {
                    let c = pos.1 as i32 + (*wind.next().unwrap().1 as i32);
                    if c < 0 || (c as usize + rock.len - 1) >= Self::WIDTH {
                        continue;
                    }
                    (pos.0, c as usize)
                };

                let collided = (0..rock.rows).any(|dr| {
                    let chamber_row = chamber.get(r + dr).cloned().unwrap_or_default();
                    chamber_row & rock.shapes[c][dr] != 0
                });
                if !collided {
                    pos = (r, c);
                } else if move_down {
                    break;
                }
            }

            let (r, c) = pos;
            while chamber.len() < r + rock.rows {
                chamber.push(0);
            }
            for dr in 0..rock.rows {
                chamber[r + dr] |= rock.shapes[c][dr];
            }
            for (dc, h) in rock.heights.iter().enumerate() {
                let height = heights[c + dc].get_or_insert(0);
                *height = (r + h).max(*height);
            }
            rock_count += 1;
        }
        skipped.unwrap_or_default() + chamber.len() - 1
    }
}

fn main() {
    Data::solve();
}
