/*
 * CommandVariables.re
 *
 * Command variable replacements for rLog.
 */

let replaceCWD: list(string) => list(string);
let replaceGitRoot: list(string) => list(string);
let replaceOutputFiles: (list(string), list(string)) => list(string);
let replaceOutputFile: (list(string), list(string)) => list(string);
let replaceCommand: (list(string), list(string)) => list(string);

let formatCommands:
  (list(string), Types.Cli.t, string => unit) => list(string);
