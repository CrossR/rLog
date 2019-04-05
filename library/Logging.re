/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;

let padDate = str =>
  Util.leftPadString(~inputString=str, ~len=2, ~padding="0");

let getDate = (~currentTime=Unix.gmtime(Unix.time()), ()) => {
  string_of_int(currentTime.tm_year + 1900)
  ++ "-"
  ++ padDate(string_of_int(currentTime.tm_mon + 1))
  ++ "-"
  ++ padDate(string_of_int(currentTime.tm_mday + 1));
};

let getTime = (~currentTime=Unix.gmtime(Unix.time()), ()) => {
  padDate(string_of_int(currentTime.tm_hour))
  ++ ":"
  ++ padDate(string_of_int(currentTime.tm_min))
  ++ ":"
  ++ padDate(string_of_int(currentTime.tm_sec));
};

let getFormattedTime = (~time=Unix.time(), ()) => {
  let currentTime = Unix.gmtime(time);
  getDate(~currentTime, ()) ++ "T" ++ getTime(~currentTime, ()) ++ "Z";
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
