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
};

let default = {
  showHelp: ref(false),
  verbose: ref(false),
  configPath: ref(""),
  restOfCLI: ref([]),
};

let helpText = {|
reasonLogger 0.0.1
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
  [
    ("-h", Arg.Set(cliObj.showHelp), "Show this help text."),
    ("-v", Arg.Set(cliObj.verbose), "Enable verbose mode."),
    (
      "--config",
      Arg.Set_string(cliObj.configPath),
      "Provide a custom configuration file location.",
    ),
    (
      "--",
      Arg.Rest(arg => cliObj.restOfCLI := List.append(cliObj.restOfCLI^, [arg])),
      "Stop parsing CLI flags and treat the rest of the command as input for the command to run.",
    ),
  ];
};

let getArgs = () => {
  let cliObj = default;
  Arg.parse(
    argList(cliObj),
    x => raise(Arg.Bad("Unknown argument: " ++ x)),
    helpText,
  );

  if (cliObj.showHelp^) {
    Arg.usage(argList(cliObj), helpText);
  };

  cliObj;
};
