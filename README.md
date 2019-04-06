# reasonLogger

[![Build Status](https://dev.azure.com/CrossR-1/reasonLogger/_apis/build/status/CrossR.reasonLogger?branchName=master)](https://dev.azure.com/CrossR-1/reasonLogger/_build/latest?definitionId=4&branchName=master)

reasonLogger is a very WIP tool for logging the output of commands for
scientific workflows.

I find myself very often running some long-running command to produce
simulation output or similar, where the output depends very heavily on the
current state of my environment.

Having the automated ability to run a set of commands, and store their output
means that I can log out not only the result of a command, but the state of
the environment at the time of the simulation.

For example:

    - `echo` out environment variables to check library versions.
    - `git` commands to get the current commit and `diff` of the repository.
    - Dumping out the current contents of a configuration file.
    - Status of input files.

## Configuration

Configuration is achieved with a `config.json` in `~/.config/reasonLogger/`:

```json
{
    "outputPath": "~/reasonLoggerOut",
    "commandsToRun": [],
    "loadLocalCommands": false,
    "valuesToLog": ["@LOG@: "]
}
```

Where:

    - `outputPath` is the folder log files will be stored in.
    - `commandsToRun` is the additional logging commands to run.
    - `localLocalCommands` enables the ability to use a per project config file. **TODO**.
    - `valuesToLog` is a list of strings that appear at the start of output
    that is especially important to log. For example `echo "@LOG@: Cosmics
    disabled"` will cause `Cosmics disabled` to appear in the metadata log
    file separately, so it is more easily found. These values can be printed
    in anyway, as long as they appear in the terminal `stdout`/`stderr`.

## Usage

Usage should just be `reasonLogger -- commandToRun` (**Once I've made a binary release**).

## ToDo

Stuff to do:

    - Make a release, so I have a binary.
    - Return the ran commands status code.
    - Implement the project specific config.
    - Add a search command (to dump out all commands, for use with FZF etc).
    - Add command to get a link to the most recent config (so then I can do
    `do_simulation.sh` and then `reasonLogger link` and have a link to the
    log file in the data location too).
    - Add a variable syntax to the commands (so I can define dynamic
    commands, that depend on the env/command ran, not just static ones).
    - Check if I can get this running on Windows (not a priority since none
    of my workflow that benefits from this is on Windows...).