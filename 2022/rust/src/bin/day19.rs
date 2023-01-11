use std::collections::HashMap;

use nom::{
    bytes::complete as nbc, character::complete as ncc, combinator as nc, multi as nm,
    sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<[Mats; Mat::MAT_COUNT]>,
    ids: Vec<usize>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let id = |i| ns::delimited(nbc::tag("Blueprint "), ncc::u64, nbc::tag(":"))(i);

        let material =
            |i| ns::separated_pair(ncc::u64, nbc::tag(" "), nc::map(ncc::alpha1, Mat::new))(i);
        let robot = |i| {
            nc::map(
                ns::separated_pair(
                    nc::map(ns::preceded(nbc::tag("Each "), ncc::alpha1), Mat::new),
                    nbc::tag(" robot costs "),
                    ns::terminated(
                        nm::separated_list1(nbc::tag(" and "), material),
                        nbc::tag("."),
                    ),
                ),
                |(mat, l)| {
                    let mut costs = [0; Mat::MAT_COUNT];
                    for (count, mat) in l {
                        costs[mat as usize] = count as usize;
                    }
                    (mat, costs)
                },
            )(i)
        };

        let blueprint = |i| {
            nc::map(
                ns::pair(
                    whitespace(id),
                    nm::many_m_n(Mat::MAT_COUNT, Mat::MAT_COUNT, whitespace(robot)),
                ),
                |(id, l)| {
                    let mut robot = [[0; Mat::MAT_COUNT]; Mat::MAT_COUNT];
                    for (mat, costs) in l {
                        robot[mat as usize] = costs;
                    }
                    (id as usize, robot)
                },
            )(i)
        };

        let result = nc::map(nm::many0(blueprint), |info| {
            let (ids, data) = info.into_iter().unzip();
            Self { data, ids }
        })(input);
        result
    }

    fn part1(&mut self) -> Self::Output {
        self.data
            .iter()
            .map(|blueprint| Dfs::new(blueprint, 24).solve())
            .enumerate()
            .map(|(blueprint, max_geodes)| self.ids[blueprint] * max_geodes)
            .sum()
    }

    fn part2(&mut self) -> Self::Output {
        self.data
            .iter()
            .take(3)
            .map(|blueprint| Dfs::new(blueprint, 32).solve())
            .product()
    }
}

#[derive(Debug)]
#[repr(usize)]
enum Mat {
    Ore,
    Clay,
    Obsidian,
    Geode,
}

impl Mat {
    const MAT_COUNT: usize = 4;

    fn new(s: &str) -> Self {
        match s {
            "ore" => Self::Ore,
            "clay" => Self::Clay,
            "obsidian" => Self::Obsidian,
            "geode" => Self::Geode,
            _ => panic!("invalid material"),
        }
    }
}

type Mats = [usize; Mat::MAT_COUNT];

struct Dfs<'a> {
    blueprint: &'a [Mats; Mat::MAT_COUNT],
    max_time: usize,
    max_cost: Mats,
    max_geodes: usize,
    memo: HashMap<(Mats, Mats), usize>,
}
impl<'a> Dfs<'a> {
    fn new(blueprint: &'a [Mats; Mat::MAT_COUNT], max_time: usize) -> Self {
        let mut max_cost = [0; Mat::MAT_COUNT];
        for mat in 0..Mat::MAT_COUNT {
            let max_opt = blueprint.iter().map(|robot| robot[mat]).max();
            max_cost[mat] = if mat == Mat::Geode as usize {
                (max_time - 1) * (max_time) / 2
            } else {
                max_opt.unwrap_or_default()
            }
        }

        Self {
            blueprint,
            max_time,
            max_cost,
            max_geodes: 0,
            memo: HashMap::new(),
        }
    }

    fn solve(&mut self) -> usize {
        let mut robots = [0; Mat::MAT_COUNT];
        robots[Mat::Ore as usize] = 1;
        let resources = [0; Mat::MAT_COUNT];

        self.dfs(self.max_time, robots, resources);
        self.max_geodes
    }

    fn can_add(&self, robot: usize, resources: &Mats) -> bool {
        (0..Mat::MAT_COUNT).all(|mat| resources[mat] >= self.blueprint[robot][mat])
    }

    fn dfs(&mut self, time: usize, robots: Mats, resources: Mats) {
        self.memo.insert((robots, resources), time);
        self.max_geodes = resources[Mat::Geode as usize].max(self.max_geodes);
        if time == 0 {
            return;
        }

        for robot in 0..Mat::MAT_COUNT {
            let mut time_left = time - 1;
            let mut next_resources = resources;
            let mut next_robots = robots;

            while time_left != 0 && !self.can_add(robot, &next_resources) {
                for mat in 0..Mat::MAT_COUNT {
                    next_resources[mat] += robots[mat];
                }
                time_left -= 1;
            }

            let add = self.can_add(robot, &next_resources);
            if add {
                next_robots[robot] = (robots[robot] + 1).min(self.max_cost[robot]);
            }

            for mat in 0..Mat::MAT_COUNT {
                let unmet_count = self.max_cost[mat] - next_robots[mat];
                let cap = self.max_cost[mat] + time_left * unmet_count;
                let cost = if add { self.blueprint[robot][mat] } else { 0 };
                next_resources[mat] = (next_resources[mat] + robots[mat] - cost).min(cap);
            }

            let potential_geodes = next_resources[Mat::Geode as usize]
                + time_left * next_robots[Mat::Geode as usize]
                + (time_left - 1) * (time_left) / 2;
            if potential_geodes > self.max_geodes {
                let better_time = (self.memo.get(&(next_robots, next_resources)))
                    .map(|&t| t >= time_left)
                    .unwrap_or_default();
                if !better_time {
                    self.dfs(time_left, next_robots, next_resources);
                }
            }
        }
    }
}

fn main() {
    Data::solve();
}
