/*
 * Config.rei
 *
 * Configuration file loading and parsing.
 */

type t = {
  mutable outputPath: string,
  mutable commandsToRun: list(string),
  loadLocalCommands: bool,
  mutable valuesToLog: list(string),
};

let default: t;
let makeDefaultConfig: string => unit;
let getConfig: list(string) => t;
