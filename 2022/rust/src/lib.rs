use std::{
    collections::HashMap,
    env,
    fmt::{Debug, Display},
    fs,
    path::PathBuf,
    time::Instant,
};

use itertools::Itertools;
use nom::{character::complete as ncc, combinator as nc, sequence as ns, IResult};
#[macro_use]
extern crate lazy_static;

lazy_static! {
    pub static ref DIRS: HashMap<char, (i64, i64)> =
        HashMap::from([('U', (-1, 0)), ('D', (1, 0)), ('L', (0, -1)), ('R', (0, 1)),]);
}

pub fn whitespace<'a, F, O>(inner: F) -> impl FnMut(&'a str) -> IResult<&'a str, O>
where
    F: FnMut(&'a str) -> IResult<&'a str, O>,
{
    ns::delimited(ncc::multispace0, inner, ncc::multispace0)
}

pub fn modulus(n: i64, m: usize) -> i64 {
    let m = m as i64;
    (n % m + m) % m
}

pub trait Day: Sized + Debug {
    type Output: Display;

    fn parse(input: &str) -> IResult<&str, Self> {
        nc::fail(input)
    }

    fn new(input: &str) -> Self {
        match Self::parse(input) {
            Ok((_, data)) => data,
            Err(e) => panic!("cannot parse: {:?}", e),
        }
    }

    fn part1(&mut self) -> Self::Output;

    fn part2(&mut self) -> Self::Output;

    fn solve() {
        let args = CliArgs::new();
        let input_str = fs::read_to_string(args.input)
            .unwrap_or_else(|error| panic!("cannot read input file: {}", error));

        let now = Instant::now();

        let mut day = Self::new(input_str.as_str());
        let result = match args.part {
            1 => day.part1(),
            2 => day.part2(),
            _ => panic!("invalid part argument"),
        };

        let elapsed = now.elapsed();

        match args.output {
            None => {
                println!("Time: {:?}\n\nResult:\n{}", elapsed, result);
            }
            Some(output) => {
                let result_str = format!("{}", result).trim().to_string();
                let output_str = fs::read_to_string(output)
                    .unwrap_or_else(|error| panic!("cannot read output file: {}", error))
                    .trim()
                    .to_string();
                if result_str.eq(&output_str) {
                    println!("Ok. Time: {:?}", elapsed);
                } else {
                    println!(
                        "Fail. Expected: {:?}. Result: {:?}.",
                        result_str, output_str
                    );
                }
            }
        }
    }
}

struct CliArgs {
    part: i64,
    input: PathBuf,
    output: Option<PathBuf>,
}

impl CliArgs {
    pub fn new() -> Self {
        let args = env::args().collect_vec();

        let part_str = match &args.get(1) {
            Some(x) => x.as_str(),
            None => panic!("missing part argument"),
        };
        let part = match part_str {
            "1" => 1,
            "2" => 2,
            _ => panic!("invalid part argument"),
        };

        let input_str = match &args.get(2) {
            Some(x) => x.as_str(),
            None => panic!("missing input file path argument"),
        };
        let input = PathBuf::from(input_str);
        if !input.is_file() {
            panic!("invalid input file path")
        }

        let verify = match &args.get(3) {
            Some(x) => x.as_str() == "true",
            None => false,
        };

        let output = if verify {
            let output_str = match &args.get(4) {
                Some(x) => x.as_str(),
                None => panic!("missing output file path argument"),
            };
            let output = PathBuf::from(output_str);
            if !input.is_file() {
                panic!("invalid output file path")
            }
            Some(output)
        } else {
            None
        };

        Self {
            part,
            input,
            output,
        }
    }
}
