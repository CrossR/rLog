/*
 * Runner.rei
 *
 * Entry point to combine the main and sub command logging.
 */

let start:
  (~silent: bool=?, Cli.t, string => unit) => option(Unix.process_status);
