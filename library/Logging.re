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

let getLogFile = (job, config) => {
  let outputFolder =
    Util.makeAbsolutePath(Util.join([config.outputPath, getDate()]));

  Util.checkFolderExists(config.outputPath);
  Util.checkFolderExists(outputFolder);

  Util.join([outputFolder, job ++ "_" ++ getFormattedTime() ++ ".log"]);
};

let writeFile = (path, stringList) => {
  let fileOut = open_out(path);

  let rec writeStringListToFile = outputChannel =>
    fun
    | [] => ()
    | [line, ...restOfList] => {
        Printf.fprintf(outputChannel, "%s\n", line);
        writeStringListToFile(outputChannel, restOfList);
      };

  writeStringListToFile(fileOut, stringList);
};

let makeLogFile = (output: list(Command.t), config, logMsg) => {
  let mainLogFile = getLogFile("cmd", config);
  logMsg("Logging command output to " ++ mainLogFile);
  let metadataLogFile = getLogFile("meta", config);
  logMsg("Logging command metadata to " ++ metadataLogFile);

  writeFile(mainLogFile, List.nth(output, 0).outputLines);
};
