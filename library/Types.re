/*
 * Types.re
 *
 * The types used for rLog.
 */

module Command = {
  type t = {
    command: string,
    mutable outputLines: list(string),
    mutable linesOfInterest: list((int, string)),
    mutable status: option(Unix.process_status),
    mutable runningTime: float,
  };

  let default = command => {
    command,
    outputLines: [],
    linesOfInterest: [],
    status: None,
    runningTime: 0.0,
  };
};
module CommandType = {
  type t =
    | Run
    | GenerateConfig
    | Search;

  let checkArg = str => {
    switch (str) {
    | "run" => Some(Run)
    | "genconfig" => Some(GenerateConfig)
    | "search" => Some(Search)
    | _ => None
    };
  };
};

module Cli = {
  type t = {
    showHelp: ref(bool),
    verbose: ref(bool),
    silent: ref(bool),
    configPath: ref(string),
    restOfCLI: ref(list(string)),
    mutable command: CommandType.t,
    enableOutputParsing: ref(bool),
  };

  let default = {
    showHelp: ref(false),
    verbose: ref(false),
    configPath: ref(PathUtil.getConfigLocation()),
    silent: ref(false),
    restOfCLI: ref([]),
    command: CommandType.Run,
    enableOutputParsing: ref(false),
  };
};

module Config = {
  [@deriving yojson]
  type t = {
    [@key "outputPath"]
    mutable outputPath: string,
    [@key "commandsToRun"]
    mutable commandsToRun: list(string),
    [@key "loadLocalCommands"]
    loadLocalCommands: bool,
    [@key "valuesToLog"]
    mutable valuesToLog: list(string),
  };

  let default = {
    outputPath: "~/rLogOut",
    commandsToRun: [],
    loadLocalCommands: false,
    valuesToLog: ["@LOG@: "],
  };

  let defaultJsonString = {
    {|{
    "outputPath": "~/rLogOut",
    "commandsToRun": [],
    "loadLocalCommands": false,
    "valuesToLog": ["@LOG@: "]
}|};
  };
};
