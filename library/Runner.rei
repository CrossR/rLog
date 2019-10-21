/*
 * Runner.rei
 *
 * Entry point to combine the main and sub command logging.
 */

let start:
  (~silent: bool=?, Types.Cli.t, string => unit) =>
  option(Unix.process_status);
let link: (Types.Cli.t, string => unit) => unit;
let printPreviousRuns: (Types.Cli.t, string => unit) => unit;
