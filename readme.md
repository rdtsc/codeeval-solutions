# CodeEval (Defunct) Solutions

**As of 2018-02-12, [CodeEval][ce] has ceased operations. This repository houses
an archive of relevant helper tools and my ancient solutions to all 239
interactive challenges that once lived on the platform.**

A mirror of the platform's entire public problem set (sans solutions/tools) is
available in [this repository][ce-mirror].


## Background

It is strongly encouraged that you do not view my solutions unless you've
already solved the relevant problems yourself.

CodeEval takes execution speed and memory consumption into account (albeit
incorrectly at the time of this writing) when calculating ranking points.
C and C++ are therefore my languages of choice on this platform as they
generally yield the highest scores. Additionally, JavaScript, PHP, and Python
are used here and there, mainly for practice.

CodeEval adds a new challenge to one of the following categories once a week:

- Easy (35 points)
- Moderate (65 points)
- Hard (100 points)

This typically happens on Fridays during what appears to be Central European
business hours. These base categories are mirrored in the `./src/` directory
and house all my submitted solutions.

In practice, achieving a perfect score was not possible because a solution would
have to consume no memory and have an infinite speed of execution. Therefore,
the provided solutions in C and C++ almost always favor speed/leanness over
readability in the interest of accumulating maximum points. An attainable
"perfect" solution would yield the following scores by CodeEval's judge:

- **34.998** points for challenges in the *easy* category
- **64.996** points for challenges in the *moderate* category
- **99.994** points for challenges in the *hard* category

The maximum attainable account score, therefore, would have been
`(34.998 * 82) + (64.996 * 81) + (99.994 * 76)` = **15,734.056** points at the
time of this writing.

Various patches were applied (pre-flight) to the C and C++ solutions which
resulted in the accumulation of an almost perfect account score of
**15,734.013** points across all 239 challenges.

Some had to do with implementing fast challenge-specific I/O and aggressive
memory management, while others exploited discovered bugs in the instrumentation
framework and the platform's test harness which tricked the judge into awarding
nearly maximum points, even for solutions with sub-optimal time/space
complexity.


## Development

### Basic Setup

```text
$ git clone https://github.com/rdtsc/codeeval-solutions
$ cd codeeval-solutions
$ sudo npm install grunt-cli -g
$ npm install
```

### Workflow

To keep things consistent, the following [Grunt][grunt] tasks are provided for
automating solution testing and boilerplate creation.

- `update-problem-list`
- `init-solution`
- `watch`

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
$ grunt init-solution:1

Running "init-solution:1" (init-solution) task
Fatal error: Problem 1 not found in local list.
Try running `grunt update-problem-list` first!
```

```text
$ grunt update-problem-list

Running "update-problem-list" task
CodeEval username: user
user's password: password
>> Saved 200 problem descriptions in: tools/storage/problems.json

Done, without errors.
```

<sup>* CodeEval supports 3rd-party authentication via Facebook and GitHub,
though neither of these authentication methods are currently supported by this
task.</sup>

#### Task: `init-solution`

The `init-solution` task is responsible for creating challenge solution
boilerplates which currently include the following actions:

- Solution directory creation in the appropriate difficulty master directory.
- Creation of a basic Markdown readme with a brief description of the problem
  and a link to the official problem statement.
- Creation of the problem's metadata in YAML.
- Scraping of sample input from the problem statement page, if applicable.
- Creation of skeleton solution files in C99, C++11, JavaScript, PHP, and
  Python 3. Appropriate templates are selected based on the presence of sample
  input in the problem statement.

This task requires a single command-line argument to be passed in the form of
the problem's ID. For example:

```text
$ grunt init-solution:1

Running "init-solution:1" (init-solution) task
>> Created "solutions" directory in: src/easy/fizz-buzz
>> Created meta.yaml in: src/easy/fizz-buzz
>> Created readme.md in: src/easy/fizz-buzz
>> Created readme.pdf in: src/easy/fizz-buzz
>> Created input.txt in: src/easy/fizz-buzz
>> Created solution.c in: src/easy/fizz-buzz/solutions/c
>> Created solution.cpp in: src/easy/fizz-buzz/solutions/c++
>> Created solution.js in: src/easy/fizz-buzz/solutions/javascript
>> Created solution.php in: src/easy/fizz-buzz/solutions/php
>> Created solution.py in: src/easy/fizz-buzz/solutions/python

Done, without errors.
```

In addition, an optional language key argument may be passed to the task which
will only create a solution template in the specified language. Existing
solutions for the specified problem in other languages are left untouched. For
example:

```text
$ grunt init-solution:1:c++

Running "init-solution:1:c++" (init-solution) task
>> Created "solutions" directory in: src/easy/fizz-buzz
>> Created meta.yaml in: src/easy/fizz-buzz
>> Created readme.md in: src/easy/fizz-buzz
>> Created readme.pdf in: src/easy/fizz-buzz
>> Created input.txt in: src/easy/fizz-buzz
>> Created solution.cpp in: src/easy/fizz-buzz/solutions/c++

Done, without errors.
```

A list of valid solution language keys may be found in this task's settings,
located in: `./gruntfile.js`, under the `init-solution` key.

Problem IDs may be found in the problem statement page's URL.
For example, the [Fizz Buzz][ce-1] challenge has the following URL:
`https://www.codeeval.com/browse/1/` -- its ID is therefore: `1`.

Alternatively, you may simply reference the problem metadata cache located in:
`./tools/storage/problems.json`.

Also note the following:

- Scraping of sample input is done on-demand and therefore requires an active
  Internet connection.
- Credentials are *not* required to run this task as the problem statement
  pages are publicly accessible.
- Solution templates may be configured by modifying this task's settings in:
  `./gruntfile.js`.

#### Task: `watch`

The `watch` task is able to watch a specific solution file for changes and
automatically perform the following series of actions when a change is
detected*:

- Compile the solution, if applicable.
- Display any compilation or interpreter errors and bail, or;
- Execute the generated binary, if any, against the problem's sample input.
- Report execution time.
- Remove the generated binary, if any.

This task expects two command-line arguments to be passed in the form of the
problem's ID and target language as displayed in the target's `solutions`
directory. For example:

```text
$ grunt watch:1:c++

Running "watch:1:c++" (watch) task
# Make changes to ./src/easy/fizz-buzz/solutions/c++/solution.cpp...

* Compiling...
* Executing...
--------------------------------------------------------------------------------
1 2 F 4 B F 7 8 F B
1 F 3 F 5 F B F 9 F 11 F 13 FB 15
--------------------------------------------------------------------------------
>> real 0m0.002s
>> user 0m0.001s
>> sys  0m0.001s

# The task polls for changes and replays the steps above as appropriate.
# Send a SIGINT to terminate the task, as it runs forever by default.
```

Any compilation/interpreter errors and/or warnings will appear above the first
fold.

C99, C++11, JavaScript, PHP, and Python 3 are supported out of the box. Support
for additional languages may be added by modifying this task's settings in:
`./gruntfile.js`.

<sup>* This task is currently not cross-platform friendly.</sup>

### Testing

As per CodeEval's setup, use at least GCC 4.8.4, Node.js 5.5.0, PHP 5.6.19,
and Python 3.4.3, in a Linux environment.

#### C99
```bash
$ gcc -std=c99 -Wall -Wextra -pedantic -O1 -o solution solution.c -lm
$ [ ! -f input.txt ] && ./solution || ./solution input.txt
```

#### C++11
```bash
$ g++ -std=c++11 -Wall -Wextra -pedantic -O1 -o solution solution.cpp -lm
$ [ ! -f input.txt ] && ./solution || ./solution input.txt
```

#### JavaScript
```bash
$ chmod u+x ./solution.js
$ [ ! -f input.txt ] && ./solution.js || ./solution.js input.txt
```

#### PHP
```bash
$ chmod u+x ./solution.php
$ [ ! -f input.txt ] && ./solution.php || ./solution.php input.txt
```

#### Python 3
```bash
$ chmod u+x ./solution.py
$ [ ! -f input.txt ] && ./solution.py || ./solution.py input.txt
```


## License and Copyright

All original code is released under the [MIT license][lic-me], unless otherwise
specified.

All referenced product names, trademarks, and logos are property of their
respective owners.


[ce]:        https://www.codeeval.com/
             "CodeEval - Coding Challenges for the World's Best Developers"

[ce-mirror]: https://github.com/rdtsc/codeeval-problem-statements/
             "CodeEval Problem Statement Archive"

[ce-1]:      src/easy/fizz-buzz/
             "CodeEval - Fizz Buzz Challenge"

[grunt]:     http://gruntjs.com/
             "Grunt: The JavaScript Task Runner"

[lic-me]:    http://opensource.org/licenses/MIT/
             "The MIT License (MIT)"
