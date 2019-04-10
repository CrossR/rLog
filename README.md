# rLog

<p align="center">
<a href="https://dev.azure.com/CrossR-1/rLog/_build/latest?definitionId=5&branchName=master"><img alt="Build Status" src="https://dev.azure.com/CrossR-1/rLog/_apis/build/status/CrossR.rLog?branchName=master"></a>
</p>

rLog is a very WIP tool for logging the output of commands for scientific
workflows.

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

Configuration is achieved with a `config.json` in `~/.config/rLog/`:

```json
{
    "outputPath": "~/rLogOut",
    "logCommands": [],
    "linkCommands": [],
    "loadLocalCommands": false,
    "valuesToLog": ["@LOG@: "]
}
```

Where:

 - `outputPath` is the folder log files will be stored in.
 - `logCommands` is the additional logging commands to run.
 - `linkCommands` is the commands to run when calling `rLog link`.
 - `localLocalCommands` enables the ability to use a per project config file.
    This will cause `rLog` to load a local `config.json`, if there is
    one in the current directory. It will also attempt to get one from the `git`
    root, if one exists. Any values in the `logCommands` and `valuesToLog`
    will be combined, but the `outputPath` will be overridden. The override
    order is Current directory > Project > Global, so the current folder config
    has the highest priority.
 - `valuesToLog` is a list of strings that appear at the start of output
    that is especially important to log. For example `echo "@LOG@: Cosmics
    disabled"` will cause `Cosmics disabled` to appear in the metadata log
    file separately, so it is more easily found. These values can be printed
    in anyway, as long as they appear in the terminal `stdout`/`stderr`. This
    must be explicitly enabled by passing `-p` or `--parse` to the CLI, since
    it can slow down commands considerably.

## Usage

Usage should just be `rLog -- commandToRun`, to run `commandToRun` and have
it logged to the location outlined in the config file, with the `logCommands`
ran to produce metadata for that command.

`rLog link -- output1.txt` can be used to link to the most recent run. Link
in this context will create a symlink to a new `output` file, which will run
any of the `linkCommands`, as well as giving an easy way of accessing the
command output and metadata output.

`rLog genconfig` will generate a default config file in
`~/.config/rLog` (or in a custom location if `--config-path` is
given.)

## ToDo

Stuff to do:

 - [X] Make a release, so I have a binary.
 - [X] Return the ran commands status code.
 - [X] Implement the project specific config.
 - [ ] Add a search command (to dump out all commands, for use with FZF etc).
 - [X] Add command to get a link to the most recent config (so then I can do
    `do_simulation.sh` and then `rLog link` and have a link to the
    log file in the data location too).
 - [ ] Add a variable syntax to the commands (so I can define dynamic
    commands, that depend on the env/command ran, not just static ones).
 - [ ] Check if I can get this running on Windows (not a priority since none
    of my workflow that benefits from this is on Windows...).

## Thanks

Thanks to both

 - @Schniz : https://github.com/Schniz/fnm
 - @ulrikstrid : https://github.com/ulrikstrid/reenv

For their existing CLI apps that I used heavily to help fix things I couldn't
get working! Especially around making a static binary.