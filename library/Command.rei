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
  mutable runningTime: float,
};

let default: string => t;
let runCmd:
  (
    ~storeOutput: bool=?,
    ~runSilently: bool=?,
    ~config: Config.t=?,
    ~logFile: string=?,
    string
  ) =>
  t;
let runMultipleCommand:
  (~silent: bool=?, ~logFile: string=?, ~config: Config.t, list(string)) =>
  list(t);
