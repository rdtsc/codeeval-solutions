# CodeEval Solutions

This repository houses my personal solutions to [CodeEval][ce]'s public
problems.

CodeEval takes execution speed and memory consumption into account (albeit
incorrectly at the time of this writing) when calculating ranking points.
C and C++ are therefore my languages of choice on this platform as they
generally yield the highest scores. Python is used here and there, mainly
for practice.

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
- Creation of skeleton solution files in C99, C++11, and Python 3. Appropriate
  templates are selected based on the presence of sample input in the problem
  statement.

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
>> Created solution.py in: src/easy/fizz-buzz/solutions/python

Done, without errors.
```

Problem IDs may be found in the problem statement page's URL.
For example, the [Fizz Buzz][ce-1] challenge has the following URL:
`https://www.codeeval.com/browse/1/` -- its ID is therefore: `1`.

Alternatively, you may simply reference the problem metadata cache located in:
`./tools/storage/problems.json`

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
- Remove the generated binary, if applicable.

This task expects two command-line arguments to be passed in the form of the
problem's ID and target language as displayed in the target's "solutions"
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

Any compilation or interpreter errors and/or warnings will appear above the
first fold.

C99, C++11, and Python 3 are supported out of the box. Support for additional
languages may be added by modifying this task's settings in: `./gruntfile.js`.

<sup>* This task is currently not cross-platform friendly.</sup>

### Testing

As per CodeEval's setup, use at least GCC 4.8.1 and Python 3.2.3.

#### C99
```bash
$ gcc -std=c99 -Wall -Wextra -pedantic -O1 -o solution solution.c -lm
$ [ ! -f input.txt ] && ./solution || ./solution input.txt < input.txt
```

#### C++11
```bash
$ g++ -std=c++11 -Wall -Wextra -pedantic -O1 -o solution solution.cpp -lm
$ [ ! -f input.txt ] && ./solution || ./solution input.txt < input.txt
```

#### Python 3
```bash
$ chmod u+x ./solution.py
$ [ ! -f input.txt ] && ./solution.py || ./solution.py input.txt < input.txt
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
