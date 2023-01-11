use std::{
    collections::{HashMap, HashSet, VecDeque},
    fmt::Display,
};

use itertools::Itertools;
use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};
use num::complex::Complex;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Vec<Space>>,
    moves: Vec<Dir>,
    face_to_pos: HashMap<usize, Complex<i64>>,
    pos_to_face: HashMap<Complex<i64>, usize>,
    edge: HashMap<(usize, Complex<i64>), (usize, Complex<i64>)>,
    length: usize,
}

impl Day for Data {
    type Output = i64;

    fn parse(input: &str) -> IResult<&str, Self> {
        let grid = |i| {
            nc::map(
                nm::separated_list1(ncc::newline, nm::many1(ncc::one_of(" .#"))),
                |grid| {
                    let (m, n) = (
                        grid.len(),
                        grid.iter().map(|row| row.len()).max().unwrap_or_default(),
                    );
                    let spaces: usize = (grid.iter())
                        .map(|row| row.iter().filter(|&&c| c != ' ').count())
                        .sum();
                    let l = (spaces as f64 / 6.).sqrt() as usize;

                    let mut data = vec![vec![Space::Void; n]; m];
                    for (r, c) in (0..m).cartesian_product(0..n) {
                        if let Some(&ch) = grid[r].get(c) {
                            data[r][c] = if ch == '#' {
                                Space::Wall
                            } else if ch == '.' {
                                Space::Open
                            } else {
                                Space::Void
                            }
                        }
                    }

                    let face_to_pos = (0..m / l)
                        .cartesian_product(0..n / l)
                        .filter_map(|(r, c)| {
                            if grid[r * l].get(c * l).filter(|&&ch| ch != ' ').is_some() {
                                Some(Complex::new(r as i64, c as i64))
                            } else {
                                None
                            }
                        })
                        .enumerate()
                        .collect::<HashMap<_, _>>();
                    let pos_to_face = face_to_pos.iter().map(|(&k, &v)| (v, k)).collect();

                    let edge = Self::find_edges(&pos_to_face)
                        .into_iter()
                        .map(|((kp, kd), (vp, vd))| {
                            ((pos_to_face[&kp], kd), (pos_to_face[&vp], vd))
                        })
                        .collect();

                    (data, face_to_pos, pos_to_face, edge, l)
                },
            )(i)
        };
        let moves = |i| {
            nm::many1(nb::alt((
                nc::map(ncc::u64, |n| Dir::Forward(n as usize)),
                nc::map(nbc::tag("L"), |_| Dir::Left),
                nc::map(nbc::tag("R"), |_| Dir::Right),
            )))(i)
        };

        nc::map(
            ns::separated_pair(grid, nbc::tag("\n\n"), moves),
            |((data, face_to_pos, pos_to_face, edge, length), moves)| Self {
                data,
                moves,
                face_to_pos,
                pos_to_face,
                edge,
                length,
            },
        )(input)
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
enum Space {
    Void,
    Open,
    Wall,
}

#[derive(Debug)]
enum Dir {
    Forward(usize),
    Left,
    Right,
}

#[derive(Debug)]
struct State {
    pos: Complex<i64>,
    dir: Complex<i64>,
}

type Edge = (Complex<i64>, Complex<i64>);

impl Display for Data {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let labels = ['R', 'D', 'L', 'U'];

        writeln!(f, "faces:")?;
        f.debug_map()
            .entries(self.pos_to_face.iter().map(|(k, v)| (k.to_string(), v)))
            .finish()?;

        writeln!(f, "\nedges:")?;
        let edges = (self.edge.iter())
            .map(|((kf, kd), (vf, vd))| ((kf, Self::dir_value()[kd]), (vf, Self::dir_value()[vd])))
            .sorted()
            .map(|((kf, kd), (vf, vd))| {
                format!(
                    "{}{}--{}{}",
                    kf, labels[kd as usize], vf, labels[vd as usize]
                )
            });
        f.debug_list().entries(edges).finish()?;

        Ok(())
    }
}

impl Data {
    fn dir_value() -> HashMap<Complex<i64>, i64> {
        HashMap::from([
            (Complex::new(0, 1), 0),
            (Complex::new(1, 0), 1),
            (Complex::new(0, -1), 2),
            (Complex::new(-1, 0), 3),
        ])
    }

    fn find_edges(pos_to_face: &HashMap<Complex<i64>, usize>) -> HashMap<Edge, Edge> {
        let mut edge = HashMap::new();
        let mut matched = HashSet::new();
        let mut queued = HashMap::new();

        let dirs = Self::dir_value().into_keys().collect_vec();
        let mut queue = pos_to_face
            .keys()
            .cartesian_product(dirs.iter())
            .map(|(&p, &d)| {
                let src = (p, d);
                let tgt = (p + d, -d);
                matched.insert((p, p));
                queued.insert(src, HashSet::from([tgt]));
                (src, tgt)
            })
            .collect::<VecDeque<_>>();

        while let Some((src, tgt)) = queue.pop_front() {
            if edge.contains_key(&src) || edge.contains_key(&tgt) {
                continue;
            }
            if pos_to_face.get(&tgt.0).is_some() {
                if !matched.contains(&(src.0, tgt.0)) && !matched.contains(&(tgt.0, src.0)) {
                    matched.insert((src.0, tgt.0));
                    matched.insert((tgt.0, src.0));
                    edge.insert(src, tgt);
                    edge.insert(tgt, src);
                }
            } else {
                for &d in dirs.iter() {
                    let next_tgt = (tgt.0 + d, -d);
                    if !queued[&src].contains(&next_tgt) {
                        queued.entry(src).and_modify(|e| {
                            e.insert(next_tgt);
                        });
                        queue.push_back((src, next_tgt));
                    }
                }
            }
        }
        edge
    }

    fn face_corner(&self, f: usize) -> Complex<i64> {
        self.face_to_pos[&f] * Complex::new(self.length as i64, 0)
    }

    fn face_at(&self, pos: &Complex<i64>) -> Option<&usize> {
        self.pos_to_face
            .get(&(pos / Complex::new(self.length as i64, 0)))
    }

    fn at(&self, pos: &Complex<i64>) -> &Space {
        &self.data[pos.re as usize][pos.im as usize]
    }

    fn mod_pos(&self, pos: Complex<i64>) -> Complex<i64> {
        Complex::new(
            modulus(pos.re, self.data.len()),
            modulus(pos.im, self.data[0].len()),
        )
    }

    fn flat_move(&self, state: &State) -> State {
        let mut next_pos = self.mod_pos(state.pos + state.dir);
        while *self.at(&next_pos) == Space::Void {
            next_pos = self.mod_pos(next_pos + state.dir);
        }

        State {
            pos: next_pos,
            dir: state.dir,
        }
    }

    fn cube_move(&self, state: &State) -> State {
        let next_pos = self.mod_pos(state.pos + state.dir);

        if self.face_at(&next_pos) != self.face_at(&state.pos) {
            let (face, edge_dir) = self.edge[&(*self.face_at(&state.pos).unwrap(), state.dir)];
            let rel_pos = (next_pos % Complex::new(self.length as i64, 0)) + Complex::new(1, 1);

            let rotated_rel_pos = (rel_pos) * -edge_dir / state.dir;
            let rotated = Complex::new(
                modulus(rotated_rel_pos.re, self.length + 1),
                modulus(rotated_rel_pos.im, self.length + 1),
            );

            State {
                pos: self.face_corner(face) - Complex::new(1, 1) + rotated,
                dir: -edge_dir,
            }
        } else {
            State {
                pos: next_pos,
                dir: state.dir,
            }
        }
    }

    fn common_part(&self, part1: bool) -> <Self as Day>::Output {
        let first_open = self.data[0].iter().find_position(|&&s| s == Space::Open);
        let mut state = State {
            pos: Complex::new(0, first_open.unwrap().0 as i64),
            dir: Complex::new(0, 1),
        };

        for m in self.moves.iter() {
            match m {
                Dir::Forward(n) => {
                    for _ in 0..*n {
                        let next_state = if part1 {
                            self.flat_move(&state)
                        } else {
                            self.cube_move(&state)
                        };
                        if *self.at(&next_state.pos) == Space::Open {
                            state = next_state;
                        } else {
                            break;
                        }
                    }
                }
                Dir::Left => state.dir *= Complex::new(0, 1),
                Dir::Right => state.dir *= Complex::new(0, -1),
            }
        }

        1_000 * (state.pos.re + 1) + 4 * (state.pos.im + 1) + Self::dir_value()[&state.dir]
    }
}

fn main() {
    Data::solve();
}
