use std::collections::{HashMap, HashSet};

use itertools::Itertools;
use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<usize>,
    flow: Vec<usize>,
    name: Vec<String>,
    ith_bit: Vec<u128>,
    dist: HashMap<(usize, usize), usize>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let valve = |i| ns::preceded(nbc::tag("Valve "), ncc::alpha1)(i);
        let rate = |i| ns::preceded(nbc::tag(" has flow rate="), ncc::u64)(i);
        let prec = |i| {
            nb::alt((
                nbc::tag("; tunnels lead to valves "),
                nbc::tag("; tunnel leads to valve "),
            ))(i)
        };
        let neighbors = |i| ns::preceded(prec, nm::separated_list1(nbc::tag(", "), ncc::alpha1))(i);
        let line = |i| ns::tuple((valve, rate, neighbors))(i);

        let result = nc::map(nm::separated_list0(ncc::newline, line), |lines| {
            let id_map: HashMap<&&str, usize> = (lines.iter().enumerate())
                .map(|(i, (v, _, _))| (v, i))
                .collect();
            let data = (0..id_map.len())
                .filter(|v| lines[*v].0 == Data::ROOT || lines[*v].1 > 0)
                .collect_vec();

            let flow = lines.iter().map(|(_, r, _)| *r as usize).collect();
            let name = lines.iter().map(|(v, _, _)| v.to_string()).collect();
            let ith_bit = (0..id_map.len()).map(|i| 1 << i).collect_vec();

            let graph = (lines.iter())
                .map(|(_, _, n)| n.iter().map(|v| id_map[v]).collect::<HashSet<_>>())
                .collect_vec();
            let mut dist = HashMap::new();
            for (v, u) in (0..id_map.len()).cartesian_product(0..id_map.len()) {
                if v == u {
                    dist.insert((v, u), 0);
                } else if graph[v].contains(&u) {
                    dist.insert((v, u), 1);
                } else {
                    dist.insert((v, u), Data::INF);
                }
            }
            for l in (0..id_map.len()).permutations(3) {
                let (k, i, j) = (l[0], l[1], l[2]);
                let d = dist[&(i, k)] + dist[&(k, j)];
                dist.entry((i, j)).and_modify(|e| *e = d.min(*e));
            }

            Self {
                data,
                flow,
                name,
                ith_bit,
                dist,
            }
        })(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        self.common_part(true)
    }

    fn part2(&mut self) -> Self::Output {
        self.common_part(false)
    }
}

impl Data {
    const ROOT: &str = "AA";
    const INF: usize = 1e9 as usize;

    fn common_part(&self, part1: bool) -> <Self as Day>::Output {
        let aa = (self.data.iter())
            .find(|&&i| self.name[i] == Data::ROOT)
            .cloned()
            .unwrap_or_default();

        let mut dp = HashMap::<u128, usize>::new();

        if part1 {
            self.visit(&mut dp, aa, 0, 30, 0);
            dp.values().max().cloned().unwrap_or_default()
        } else {
            self.visit(&mut dp, aa, 0, 26, 0);
            (dp.iter().cartesian_product(dp.iter()))
                .filter(|(a, b)| a.0 & b.0 == 0)
                .map(|(a, b)| a.1 + b.1)
                .max()
                .unwrap_or_default()
        }
    }

    fn visit(
        &self,
        dp: &mut HashMap<u128, usize>,
        v: usize,
        bitmask: u128,
        time: i64,
        pressure: usize,
    ) {
        let best_pressure = dp.entry(bitmask).or_default();
        *best_pressure = pressure.max(*best_pressure);
        for &u in self.data.iter() {
            let time_left = time - self.dist[&(v, u)] as i64 - 1;
            if (bitmask & self.ith_bit[u] == 0) && time_left > 0 {
                let next_pressure = pressure + self.flow[u] * (time_left as usize);
                self.visit(dp, u, bitmask | self.ith_bit[u], time_left, next_pressure);
            }
        }
    }
}

fn main() {
    Data::solve();
}
