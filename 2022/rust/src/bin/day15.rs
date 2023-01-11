use itertools::Itertools;
use nalgebra::{Matrix2, RowVector2, Vector2};
use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Sensor>,
    beacons: Vec<Vector2<i64>>,
    y: i64,
    frequency: i64,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let till_num = |i| nbc::take_till(|c: char| c == '-' || c.is_ascii_digit())(i);
        let num = |i| ns::preceded(till_num, ncc::i64)(i);
        let vec2 = |i| nc::map(ns::pair(num, num), |(x, y)| Vector2::new(x, y))(i);
        let sensors_and_beacons = |i| {
            nc::map(
                nm::separated_list1(ncc::newline, ns::pair(vec2, vec2)),
                |v| v.into_iter().map(|(s, b)| (Sensor::new(s, b), b)).unzip(),
            )(i)
        };

        let result = nc::map(
            ns::separated_pair(sensors_and_beacons, nbc::tag("\n\n"), ns::pair(num, num)),
            |((data, beacons), (y, frequency))| Self {
                data,
                beacons,
                y,
                frequency,
            },
        )(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        let mut bounds = Vec::new();
        for s in self.data.iter() {
            let dx = s.dist - (self.y - s.sensor.y).abs();
            if dx >= 0 {
                bounds.push((s.sensor.x - dx, false));
                bounds.push((s.sensor.x + dx, true));
            }
        }
        bounds.sort();
        let mut merged_bounds = Vec::new();

        let (mut depth, mut start) = (0, None);
        for (x, end) in bounds {
            depth += if end { -1 } else { 1 };
            if start.is_none() && !end {
                start = Some(x);
            }
            if depth == 0 && end {
                merged_bounds.push((start.unwrap_or_default(), x));
                start = None;
            }
        }
        let overlap = merged_bounds
            .iter()
            .map(|(start, end)| (end - start + 1) as usize)
            .sum::<usize>();
        let beacons_in_y = self
            .beacons
            .iter()
            .filter(|b| b.y == self.y)
            .unique()
            .count();
        overlap - beacons_in_y
    }

    fn part2(&mut self) -> Self::Output {
        let slopes = (self.data.iter().flat_map(Sensor::outer_slopes))
            .counts()
            .iter()
            .filter_map(|(&k, &v)| if v > 1 { Some(k) } else { None })
            .collect_vec();

        let points = (slopes.iter().cartesian_product(slopes.iter()))
            .filter_map(|(s1, s2)| {
                let a = Matrix2::from_rows(&[s1.0, s2.0]).cast::<f64>();
                let b = Vector2::new(s1.1, s2.1).cast::<f64>();
                let decomp = a.lu();

                if let Some(res) = decomp.solve(&b) {
                    if res.x.fract().abs() < f64::EPSILON && res.y.fract().abs() < f64::EPSILON {
                        let (x, y) = (res.x as i64, res.y as i64);
                        if 0 <= x && x <= self.frequency && 0 <= y && y <= self.frequency {
                            return Some(Vector2::new(x, y));
                        }
                    }
                }
                None
            })
            .collect_vec();

        for v in points {
            let ok = (self.data.iter()).all(|s| Sensor::dist(&s.sensor, &v) > s.dist);
            if ok {
                return (v.x * 4_000_000 + v.y) as usize;
            }
        }
        0
    }
}

#[derive(Debug)]
struct Sensor {
    sensor: Vector2<i64>,
    dist: i64,
}

impl Sensor {
    fn new(sensor: Vector2<i64>, beacon: Vector2<i64>) -> Self {
        let dist = Self::dist(&sensor, &beacon);
        Self { sensor, dist }
    }

    fn dist(a: &Vector2<i64>, b: &Vector2<i64>) -> i64 {
        (a - b).abs().sum()
    }

    // return ax + by = c where
    // (x1, y1) = (s.x, s.y + d * (dist + 1))
    // x + my = x1 + my1 <-> ax + by = c
    fn outer_slopes(&self) -> Vec<(RowVector2<i64>, i64)> {
        let signs = [-1, 1];
        (signs.iter().cartesian_product(signs.iter()))
            .map(|(m, d)| {
                let (x1, y1) = (self.sensor.x, self.sensor.y + d * (self.dist + 1));
                (RowVector2::new(1, *m), x1 + m * y1)
            })
            .collect_vec()
    }
}

fn main() {
    Data::solve();
}
