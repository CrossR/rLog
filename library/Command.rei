/*
 * Command.rei
 *
 * Functions related to running a command and saving its output.
 */

let runCmd:
  (~storeOutput: bool=?, ~runSilently: bool=?, ~logFile: string=?, string) =>
  Types.Command.t;
let runMultipleCommand:
  (~storeFirst: bool=?, ~silent: bool=?, ~logFile: string=?, list(string)) =>
  list(Types.Command.t);
