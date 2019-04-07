/*
 * Command.rei
 *
 * Functions related to running a command and saving its output.
 */

type t = {
  command: string,
  mutable outputLines: list(string),
  mutable linesOfInterest: list((int, string)),
  mutable status: option(Unix.process_status),
};

let default: (string) => t
let runCmd: (~runSilently: bool=?, ~config: Config.t=?, string) => t
let runMultipleCommand: (~silent: bool=?, ~config: Config.t, list(string)) => list(t)
