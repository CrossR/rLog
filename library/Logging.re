/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;

let getLogFile = configPath => {
  /*
   * TODO here:
   *  - Add timestamp
   *  - Decide on folder structure + make it
   */
  let config = Config.getConfig(configPath);

  Util.checkFolderExists(config.outputPath);

  Util.join([config.outputPath, "out.log"]);
};

let logCommand = (command, configPath) => {
  let logFilePath = getLogFile(configPath);

  command ++ " 2>&1 | tee " ++ logFilePath;
};
