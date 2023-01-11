use std::collections::HashMap;

use nom::{
    branch as nb, bytes::complete as nbc, character::complete as ncc, combinator as nc,
    multi as nm, sequence as ns, IResult,
};

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<Instr>,
}

impl Day for Data {
    type Output = usize;

    fn parse(input: &str) -> IResult<&str, Self> {
        let word = |i| nbc::is_not(" \t\r\n")(i);

        let cd = |i| {
            nc::map(
                ns::preceded(nbc::tag("$ cd "), nbc::take_until("\n")),
                |s: &str| Instr::Cd(s.to_string()),
            )(i)
        };

        let dir = nc::map(ns::preceded(nbc::tag("dir "), word), |s: &str| {
            Entry::Dir(s.to_string())
        });
        let file = nc::map(
            ns::separated_pair(ncc::u64, nbc::tag(" "), word),
            |(n, s)| Entry::File(s.to_string(), n as usize),
        );
        let ls = nc::map(
            ns::preceded(
                nbc::tag("$ ls\n"),
                nm::separated_list0(ncc::newline, nb::alt((dir, file))),
            ),
            Instr::Ls,
        );

        nc::map(
            nm::separated_list0(ncc::newline, nb::alt((cd, ls))),
            |data| Self { data },
        )(input)
    }

    fn part1(&mut self) -> Self::Output {
        let sizes = self.common_part();
        sizes.iter().filter(|&&v| v <= 100_000).sum()
    }

    fn part2(&mut self) -> Self::Output {
        let mut sizes = self.common_part();
        sizes.sort();
        const TOTAL_SPACE: usize = 70_000_000;
        const FREE_REQ: usize = 30_000_000;

        let unused = TOTAL_SPACE - sizes.last().cloned().unwrap_or_default();
        let i = sizes.partition_point(|&x| x < FREE_REQ - unused);
        sizes.get(i).cloned().unwrap_or_default()
    }
}

#[derive(Debug)]
enum Entry {
    Dir(String),
    File(String, usize),
}

#[derive(Debug)]
enum Instr {
    Cd(String),
    Ls(Vec<Entry>),
}

impl Data {
    fn common_part(&self) -> Vec<usize> {
        let mut path = Vec::new();

        let mut entries_map = HashMap::new();
        for instr in self.data.iter() {
            match instr {
                Instr::Cd(p) => match p.as_str() {
                    "/" => path.clear(),
                    ".." => {
                        (path.pop());
                    }
                    _ => path.push(p.as_str()),
                },
                Instr::Ls(l) => {
                    entries_map.insert(path.clone(), l);
                }
            };
        }

        let mut sizes = Vec::new();
        path.clear();

        Data::dfs(&entries_map, &mut sizes, &mut path);
        sizes
    }

    fn dfs<'a>(
        entries_map: &HashMap<Vec<&str>, &'a Vec<Entry>>,
        sizes: &mut Vec<usize>,
        path: &mut Vec<&'a str>,
    ) -> usize {
        let size = entries_map
            .get(path)
            .unwrap()
            .iter()
            .map(|e| match e {
                Entry::Dir(d) => {
                    path.push(d.as_str());
                    let s = Self::dfs(entries_map, sizes, path);
                    path.pop();
                    s
                }
                Entry::File(_, s) => *s,
            })
            .sum();
        sizes.push(size);
        size
    }
}

fn main() {
    Data::solve();
}
