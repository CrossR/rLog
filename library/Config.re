/*
 * Config.re
 *
 * Configuration file loading and parsing.
 */

[@deriving yojson]
type t = {
  [@key "outputPath"]
  outputPath: string,
  [@key "commandsToRun"]
  commandsToRun: list(string),
  [@key "loadLocalCommands"]
  loadLocalCommands: bool,
};

let default = {
  outputPath: "~/reasonLoggerOut",
  commandsToRun: [],
  loadLocalCommands: false,
};

let defaultJsonString = {
  {|{
    "outputPath": "~/reasonLoggerOut",
    "commandsToRun": [],
    "loadLocalCommands": false
}|};
};

let getConfigLocation = () =>
  switch (Sys.os_type) {
  | "Win32" => Util.join([Util.getHome(), "reasonLogger", "config.json"])
  | _ => Util.join([Util.getHome(), ".config", "reasonLogger", "config.json"])
  };

let saveConfig = (configPath, config) =>
  Yojson.Safe.to_file(configPath, config);

let loadConfig = configPath => {
  let configPath = configPath != "" ? configPath : getConfigLocation();

  if (configPath != "" && !Sys.file_exists(configPath)) {
    let defaultJson = Yojson.Safe.from_string(defaultJsonString);
    Yojson.Safe.to_file(configPath, defaultJson);
  };

  switch (Yojson.Safe.from_file(configPath) |> of_yojson) {
  | Ok(config) => config
  | Error(loc) =>
    Console.log("Error Loc " ++ loc);
    default;
  };
};

let getConfig = configPath => {
  let config = loadConfig(configPath);

  if (config == default) {
    Console.warn("Using default config...");
  };

  config;
};
