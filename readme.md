# CodeEval Solutions

This repository houses my personal solutions to [CodeEval][ce]'s public
problems.

CodeEval takes execution speed and memory consumption into account (albeit
incorrectly at the time of this writing) when calculating ranking points.
C and C++ are therefore my languages of choice on this platform as they
generally yield the highest scores.

It is strongly encouraged that you do not view my solutions unless you've
already solved the relevant problems yourself.


## Background

CodeEval adds a new challenge to one of the following categories once a week:

- Easy (35 points)
- Moderate (65 points)
- Hard (100 points)

This typically happens on Fridays during what appears to be Central European
business hours. These base categories are mirrored in the `./src/` directory
and house all my submitted solutions.


## Development

### Basic Setup

```text
$ git clone https://github.com/rdtsc/code-eval-solutions
$ cd code-eval-solutions/
$ sudo npm install grunt-cli -g
$ npm install
```

### Workflow

To keep things consistent, the following [Grunt][grunt] tasks are provided for
automating solution boilerplate creation:

- `update-problem-list`
- `init-solution`

#### Task: `update-problem-list`

The `update-problem-list` task is responsible for caching basic problem
metadata such as problem ID, URL, and brief description. Unfortunately, this
information is stored behind an auth-wall on CodeEval's end and is only
available to registered users*.

The generated metadata cache is stored in JSON format and is located in:
`./tools/storage/problems.json`

You will need to re-run this task once you try to auto-generate a solution
boilerplate for a challenge that's not in the local cache. For example:

```text
$ grunt update-problem-list

Running "update-problem-list" task
CodeEval username: user
user's password: password
>> Saved 200 problem descriptions in: tools/storage/problems.json

Done, without errors.
```

<sup>* CodeEval supports 3rd-party authentication via Facebook and GitHub,
though neither of these authentication methods are currently supported.</sup>

#### Task: `init-solution`

The `init-solution` task is responsible for creating challenge solution
boilerplates which currently include the following actions:

- Solution directory creation in the appropriate difficulty master directory.
- Creation of a basic Markdown readme with a brief description of the problem
  and a link to the official problem statement.
- Creation of the problem's metadata in YAML.
- Scraping of sample input from the problem statement page, if applicable.
- Creation of skeleton solution files in C and C++. Appropriate templates are
  selected based on the presence of sample input in the problem statement.

This task expects a single command-line argument to be passed in the form of
the problem's ID. For example:

```text
$ grunt init-solution:1

Running "init-solution:1" (init-solution) task
>> Created "solutions" directory in: src/easy/fizz-buzz
>> Created meta.yaml in: src/easy/fizz-buzz
>> Created readme.md in: src/easy/fizz-buzz
>> Created input.txt in: src/easy/fizz-buzz
>> Created solution.c in: src/easy/fizz-buzz/solutions/c
>> Created solution.cpp in: src/easy/fizz-buzz/solutions/c++

Done, without errors.
```

Problem IDs can be found in the problem statement page's URL.
For example, the [Fizz Buzz][ce-1] challenge has the following URL:
`https://www.codeeval.com/browse/1/` -- its ID is therefore: `1`.

Alternatively, you may simply reference the problem metadata cache located in:
`./tools/storage/problems.json`

Also note the following:

- Scraping of sample input is done on-demand and therefore requires an active
  internet connection.
- Credentials are *not* required to run this task as the problem statement
  pages are publicly accessible.
- Solution templates can be configured by modifying this task's settings in:
  `./gruntfile.js`.

### Testing

As per CodeEval's setup, use at least GCC 4.8.1

#### C99
```bash
$ gcc -std=c99 -Wall -Wextra -pedantic -O3 -lm -o solution solution.c
```

#### C++11
```bash
$ g++ -std=c++11 -Wall -Wextra -pedantic -O3 -lm -o solution solution.cpp
```


## License and Copyright

All original code is released under the [MIT license][lic-me], unless otherwise
specified.

All referenced product names, trademarks, and logos are property of their
respective owners.


[ce]:     https://www.codeeval.com/
          "CodeEval - Coding Challenges for the World's Best Developers"

[ce-1]:   https://www.codeeval.com/browse/1/
          "CodeEval - Fizz Buzz Challenge"

[grunt]:  http://gruntjs.com/
          "Grunt: The JavaScript Task Runner"

[lic-me]: http://opensource.org/licenses/MIT/
          "The MIT License (MIT)"
