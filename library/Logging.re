/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;

let padDate = str =>
  Util.leftPadString(~inputString=str, ~len=2, ~padding="0");

let getDate = (~time=Unix.gmtime(Unix.time()), ()) => {
  string_of_int(time.tm_year + 1900)
  ++ "-"
  ++ padDate(string_of_int(time.tm_mon + 1))
  ++ "-"
  ++ padDate(string_of_int(time.tm_mday));
};

let getTime = (~time=Unix.gmtime(Unix.time()), ()) => {
  padDate(string_of_int(time.tm_hour))
  ++ ":"
  ++ padDate(string_of_int(time.tm_min))
  ++ ":"
  ++ padDate(string_of_int(time.tm_sec));
};

let getFormattedTime = (~time=Unix.time(), ()) => {
  let time = Unix.gmtime(time);
  getDate(~time, ()) ++ "T" ++ getTime(~time, ()) ++ "Z";
};

let getLogFile = config => {
  let outputFolder = Util.join([config.outputPath, getDate()]);
  Util.checkFolderExists(config.outputPath);
  Util.checkFolderExists(outputFolder);

  Util.join([outputFolder, getFormattedTime() ++ ".log"]);
};

let logCommand = (command, configPath) => {
  let logFilePath = getLogFile(configPath);

  command ++ " 2>&1 | tee " ++ logFilePath;
};
