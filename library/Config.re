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
  [@key "valuesToLog"]
  valuesToLog: list(string),
};

let default = {
  outputPath: "~/reasonLoggerOut",
  commandsToRun: [],
  loadLocalCommands: false,
  valuesToLog: ["@LOG@: "],
};

let defaultJsonString = {
  {|{
    "outputPath": "~/reasonLoggerOut",
    "commandsToRun": [],
    "loadLocalCommands": false,
    "valuesToLog": ["@LOG@: "]
}|};
};

let getConfigLocation = () =>
  switch (Sys.os_type) {
  | "Win32" => Util.join([Util.getHome(), "reasonLogger", "config.json"])
  | _ => Util.join([Util.getHome(), ".config", "reasonLogger", "config.json"])
  };

let checkConfigPath = path => {
  let absPath = Util.makeAbsolutePath(path);

  if (!Str.string_match(Str.regexp("config.json"), absPath, 0)) {
    Util.join([absPath, "config.json"]);
  } else {
    absPath;
  };
};

let saveConfig = (configPath, config) =>
  Yojson.Safe.to_file(configPath, config);

let makeDefaultConfig = configPath => {
  /*
   * Should perhaps have this have some option of the config type,
   * if I end up having the project specific config be different.
   *
   * Most likely, the project specific one will have an additional option for
   * "override" or "add" to combine the commandsToRun or only use the project
   * specific ones.
   */
  let configPath =
    configPath != "" ? checkConfigPath(configPath) : getConfigLocation();

  if (!Sys.file_exists(configPath)) {
    let defaultJson = Yojson.Safe.from_string(defaultJsonString);
    Yojson.Safe.to_file(configPath, defaultJson);
  };
};

let loadConfig = configPath => {
  let configPath = configPath != "" ? configPath : getConfigLocation();

  /* Make the default config if its needed. */
  makeDefaultConfig(configPath);

  switch (Yojson.Safe.from_file(configPath) |> of_yojson) {
  | Ok(config) => config
  | Error(loc) =>
    Console.log("Error Loc " ++ loc);
    default;
  };
};

let loadProjectConfig = () => {
  /*
   * Steps here:
   *
   * Get the current working directory.
   * Then:
   *   - Get the config from there.
   *   - Get the config from the project root (if Git)
   *
   * Return that config.
   */
};

let getConfig = configPath => {
  let config = loadConfig(configPath);

  if (config == default) {
    Console.warn("Using default config...");
  };

  config;
};
