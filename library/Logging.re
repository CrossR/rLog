/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;

let getLogFile = configPath => {
  let config = Config.getConfig(configPath);

  /* Add timestamp etc */
  Util.join([config.outputPath, "out.log"]);
};

let logCommand = (command, configPath) => {
  let logFilePath = getLogFile(configPath);

  command ++ " 2>&1 | tee " ++ logFilePath;
};
