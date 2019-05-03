/*
 * Config.re
 *
 * Configuration file loading and parsing.
 */

open Types.Config;
open Types.Command;

let checkConfigPath = path => {
  let absPath = PathUtil.makeAbsolutePath(path);

  if (!Str.string_match(Str.regexp(".*\\.json"), absPath, 0)) {
    PathUtil.join([absPath, "config.json"]);
  } else {
    absPath;
  };
};

let saveConfig = (configPath, config) =>
  Yojson.Safe.to_file(configPath, config);

let makeDefaultConfig = configPath => {
  let configPath =
    configPath != ""
      ? checkConfigPath(configPath) : PathUtil.getConfigLocation();
  PathUtil.checkPathExists(configPath);

  if (!Sys.file_exists(configPath)) {
    let defaultJson = Yojson.Safe.from_string(defaultJsonString);
    Yojson.Safe.to_file(configPath, defaultJson);
  };
};

let loadConfig = configPath => {
  let configPath =
    configPath != "" ? configPath : PathUtil.getConfigLocation();

  /* Make the default config if its needed. */
  makeDefaultConfig(configPath);

  switch (Yojson.Safe.from_file(configPath) |> of_yojson) {
  | Ok(config) => config
  | Error(loc) =>
    Console.log("Line of code with error: " ++ loc);
    Types.Config.default;
  };
};

let loadProjectConfig = path => {
  let currentConfigPath =
    checkConfigPath(PathUtil.join([path, "config.json"]));

  if (Sys.file_exists(currentConfigPath)) {
    Some(loadConfig(currentConfigPath));
  } else {
    None;
  };
};

let combineProjectConfig = (main, project) => {
  /* Combine the commands and logger values */
  let jointCommands = List.append(main.logCommands, project.logCommands);
  main.logCommands = List.sort_uniq(compare, jointCommands);

  let jointLogging = List.append(main.valuesToLog, project.valuesToLog);
  main.valuesToLog = List.sort_uniq(compare, jointLogging);

  let jointLink = List.append(main.linkCommands, project.linkCommands);
  main.linkCommands = List.sort_uniq(compare, jointLink);

  /* Override the output path to a project specific one, if set */
  if (project.outputPath != "") {
    main.outputPath = project.outputPath;
  };

  main;
};

let getConfig = (configPaths, logMsg) => {
  /* Get the main default config */
  logMsg("Loading config from:");
  let configPath = List.nth(configPaths, 0);
  PathUtil.checkPathExists(configPath);

  let mainConfig = ref(loadConfig(configPath));
  logMsg("    " ++ configPath);

  if (mainConfig^.loadLocalCommands) {
    for (i in 1 to List.length(configPaths) - 1) {
      let configPath = List.nth(configPaths, i);
      logMsg("    " ++ configPath);
      let config = loadProjectConfig(configPath);

      mainConfig :=
        (
          switch (config) {
          | Some(projectConfig) =>
            combineProjectConfig(mainConfig^, projectConfig)
          | None => mainConfig^
          }
        );
    };
  };

  if (mainConfig^ == Types.Config.default) {
    Console.warn("Using default config...");
  };

  mainConfig^;
};
