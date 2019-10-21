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
    | Link
    | Search;

  let checkArg = str => {
    switch (str) {
    | "run" => Some(Run)
    | "genconfig" => Some(GenerateConfig)
    | "search" => Some(Search)
    | "link" => Some(Link)
    | _ => None
    };
  };
};

module Cli = {
  type t = {
    showHelp: ref(bool),
    verbose: ref(bool),
    silent: ref(bool),
    force: ref(bool),
    recurse: ref(bool),
    configPath: ref(string),
    restOfCLI: ref(list(string)),
    mutable command: CommandType.t,
    enableOutputParsing: ref(bool),
    linkToPreviousRun: ref(string),
  };

  let default = {
    showHelp: ref(false),
    verbose: ref(false),
    force: ref(false),
    recurse: ref(false),
    configPath: ref(PathUtil.getConfigLocation()),
    silent: ref(false),
    restOfCLI: ref([]),
    command: CommandType.Run,
    enableOutputParsing: ref(false),
    linkToPreviousRun: ref(""),
  };
};

module Config = {
  [@deriving yojson]
  type t = {
    [@key "outputPath"]
    mutable outputPath: string,
    [@key "logCommands"]
    mutable logCommands: list(string),
    [@key "linkCommands"]
    mutable linkCommands: list(string),
    [@key "loadLocalCommands"]
    loadLocalCommands: bool,
    [@key "valuesToLog"]
    mutable valuesToLog: list(string),
  };

  let default = {
    outputPath: "~/rLogOut",
    logCommands: [],
    linkCommands: [],
    loadLocalCommands: false,
    valuesToLog: ["@LOG@: "],
  };

  let defaultJsonString = {
    {|{
    "outputPath": "~/rLogOut",
    "logCommands": [],
    "linkCommands": [],
    "loadLocalCommands": false,
    "valuesToLog": ["@LOG@: "]
}|};
  };
};
