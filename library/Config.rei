/*
 * Config.rei
 *
 * Configuration file loading and parsing.
 */

type t = {
  outputPath: string,
  commandsToRun: list(string),
  loadLocalCommands: bool,
  valuesToLog: list(string),
};

let default: t
let makeDefaultConfig: (string) => unit
let getConfig: (string) => t
