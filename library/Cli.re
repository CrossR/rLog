/*
 * Cli.re
 *
 * Various utilities to help with the CLI of reasonLogger.
 */

type t = {
  showHelp: ref(bool),
  verbose: ref(bool),
  configPath: ref(string),
  restOfCLI: ref(list(string)),
  mutable command: Util.CommandType.t,
};

let default = {
  showHelp: ref(false),
  verbose: ref(false),
  configPath: ref(""),
  restOfCLI: ref([]),
  command: Util.CommandType.Run,
};

let missingCommand = "reasonLogger must be called with -- followed by the command to be ran!";

let helpText = {|reasonLogger 0.0.1
Ryan Cross <r.cross@lancaster.ac.uk>

reasonLogger is a CLI tool for logging the output of commands.
Any command passed to this tool will have its stdOut and stdErr logged
to the location specified in the configuration file.

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
    ("-h", Arg.Set(cliObj.showHelp), " Show this help text."),
    ("-v", Arg.Set(cliObj.verbose), " Enable verbose mode."),
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
      " Stop parsing CLI flags and treat the rest of the command as input for the command to run.",
    ),
  ]);
};

let dealWithAnonArgs = (arg, cliObj) => {
  let findResult = Util.CommandType.checkArg(arg);

  switch (findResult) {
  | Some(command) => cliObj.command = command
  | None => raise(Arg.Bad("Unknown argument: " ++ arg))
  };
};

let isRun = cliObj => cliObj.command == Util.CommandType.Run;
let isConfigGen = cliObj => cliObj.command == Util.CommandType.GenerateConfig;
let isSearch = cliObj => cliObj.command == Util.CommandType.Search;

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
