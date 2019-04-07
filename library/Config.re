/*
 * Config.re
 *
 * Configuration file loading and parsing.
 */

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

  if (!Str.string_match(Str.regexp(".*\\.json"), absPath, 0)) {
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

let loadProjectConfig = path => {
  let currentConfigPath = checkConfigPath(Util.join([path, "config.json"]));

  if (Sys.file_exists(currentConfigPath)) {
    Some(loadConfig(currentConfigPath));
  } else {
    None;
  };
};

let getConfig = configPaths => {
  /* Get the main defualt config */
  let configPath = List.nth(configPaths, 0);
  let mainConfig = loadConfig(configPath);

  if (mainConfig.loadLocalCommands) {
    for (i in 1 to List.length(configPaths) - 1) {
      let configPath = List.nth(configPaths, i);
      let config = loadProjectConfig(configPath);

      let jointList =
        switch (config) {
        | Some(config) =>
          List.append(mainConfig.commandsToRun, config.commandsToRun)
        | None => mainConfig.commandsToRun
        };

      mainConfig.commandsToRun = List.sort_uniq((a, b) => 0, jointList);
    };
  };

  if (mainConfig == default) {
    Console.warn("Using default config...");
  };

  mainConfig;
};
