use itertools::Itertools;

use aoc_2022::*;

#[derive(Debug)]
struct Data {
    data: Vec<(usize, usize)>,
}

impl Data {
    const SCORE: [i64; 3] = [1, 2, 3];
    const RESULT: [i64; 3] = [0, 3, 6];
}

impl Day for Data {
    type Output = i64;

    fn new(input: &str) -> Self {
        let data = (input.lines())
            .map(|l| {
                let chars = l.split(' ').collect_vec();
                (
                    (chars[0].as_bytes()[0] - b'A') as usize,
                    (chars[1].as_bytes()[0] - b'X') as usize,
                )
            })
            .collect_vec();
        Self { data }
    }

    fn part1(&mut self) -> Self::Output {
        self.data
            .iter()
            .map(|(opp, cur)| {
                let outcome = match *cur {
                    x if x == (opp + 1) % 3 => 6,
                    x if x == *opp => 3,
                    _ => 0,
                };
                outcome + Data::SCORE[*cur]
            })
            .sum()
    }

    fn part2(&mut self) -> Self::Output {
        self.data
            .iter()
            .map(|(opp, cur)| {
                let shape = match *cur {
                    2 => opp + 1,
                    1 => *opp,
                    0 => Data::SCORE.len() + opp - 1,
                    _ => panic!("cannot happen"),
                };
                Data::SCORE[shape % 3] + Data::RESULT[*cur]
            })
            .sum()
    }
}

fn main() {
    Data::solve();
}
