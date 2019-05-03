/*
 * Cli.re
 *
 * Various utilities to help with the CLI of rLog.
 */

open Types.Cli;

let missingCommand = "rLog must be called with -- followed by the command to be ran!";

let helpText = {|rLog 1.0.1
Ryan Cross <r.cross@lancaster.ac.uk>

rLog is a CLI tool for logging the output of commands. Any command passed to
this tool will have its stdOut and stdErr logged to the location specified in
the configuration file.

To get the most of this tool, add additional logging commands to the
configuration file, such that the full state of your environment is logged
when running a command.

This is especially useful for any Data Science applications, to log any
model parameters, as well as using `git` commands and more as additional
logging, to store the state of the code for a given data output.

Flags:
|};

let argList = cliObj => {
  /* Space at the start of every string so the Arg.align works */
  Arg.align([
    (
      "genconfig",
      Arg.Unit(() => ()),
      " Generate a config file in the default or given location.",
    ),
    (
      "link",
      Arg.Unit(() => ()),
      " Link the passed data file(s) to the previous run.",
    ),
    ("-h", Arg.Set(cliObj.showHelp), " Show this help text."),
    ("-v", Arg.Set(cliObj.verbose), " Enable verbose mode."),
    ("-R", Arg.Set(cliObj.recurse), " Recursively link files from folders."),
    (
      "-f",
      Arg.Set(cliObj.force),
      " Enable forcing, which will skip warnings of duplicated link files.",
    ),
    ("--parse", Arg.Set(cliObj.enableOutputParsing), " "),
    (
      "-p",
      Arg.Set(cliObj.enableOutputParsing),
      " Enable line parsing, to look for lines of interest.",
    ),
    (
      "-s",
      Arg.Set(cliObj.silent),
      " Enable silent mode, no output is printed for any command.",
    ),
    (
      "--config-path",
      Arg.Set_string(cliObj.configPath),
      " Provide a custom configuration file location.",
    ),
    (
      "--",
      Arg.Rest(
        arg => cliObj.restOfCLI := List.append(cliObj.restOfCLI^, [arg]),
      ),
      " Stop parsing CLI flags and treat the rest of the command as input for the given function (run or link).",
    ),
  ]);
};

/* Anonymous args are all the ones after the -- */
let dealWithAnonArgs = (arg, cliObj) => {
  let findResult = Types.CommandType.checkArg(arg);

  switch (findResult) {
  | Some(command) => cliObj.command = command
  | None => raise(Arg.Bad("Unknown argument: " ++ arg))
  };
};

let isRun = cliObj => cliObj.command == Types.CommandType.Run;
let isConfigGen = cliObj => cliObj.command == Types.CommandType.GenerateConfig;
let isSearch = cliObj => cliObj.command == Types.CommandType.Search;
let isLink = cliObj => cliObj.command == Types.CommandType.Link;

let getArgs = () => {
  let cliObj = default;
  Arg.parse(argList(cliObj), x => dealWithAnonArgs(x, cliObj), helpText);

  if (cliObj.showHelp^) {
    Arg.usage(argList(cliObj), helpText);
  };

  if (isRun(cliObj) && List.length(cliObj.restOfCLI^) == 0) {
    Console.error(missingCommand);
  };

  cliObj;
};
