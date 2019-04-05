/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;

let getDate = (currentTime: Unix.tm) => {
  string_of_int(currentTime.tm_year)
  ++ "-"
  ++ string_of_int(currentTime.tm_mon + 1)
  ++ "-"
  ++ string_of_int(currentTime.tm_mday);
};

let getTime = (currentTime: Unix.tm) => {
  string_of_int(currentTime.tm_hour)
  ++ ":"
  ++ string_of_int(currentTime.tm_min)
  ++ ":"
  ++ string_of_int(currentTime.tm_sec);
};

let getTimeStep = () => {
  let currentTime = Unix.gmtime(Unix.time());
  getDate(currentTime) ++ "T" ++ getTime(currentTime) ++ "Z";
};

let getLogFile = configPath => {
  /*
   * TODO here:
   *  - Decide on folder structure + make it
   */
  let config = Config.getConfig(configPath);

  Util.checkFolderExists(config.outputPath);

  Util.join([config.outputPath, getTimeStep() ++ ".log"]);
};

let logCommand = (command, configPath) => {
  let logFilePath = getLogFile(configPath);

  command ++ " 2>&1 | tee " ++ logFilePath;
};
