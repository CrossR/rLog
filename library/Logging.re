/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */
open Config;
open Util;

let padDate = str => leftPadString(~inputString=str, ~len=2, ~padding="0");

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
  let outputFolder = makeAbsolutePath(join([config.outputPath, getDate()]));

  checkFolderExists(config.outputPath);
  checkFolderExists(outputFolder);

  join([outputFolder, job ++ "_" ++ getFormattedTime() ++ ".log"]);
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

let formatLinesOfInterest = (command: Command.t) =>
  if (command.linesOfInterest == []) {
    [];
  } else {
    let formattedLinesOfInterest =
      List.map(
        a => "    - Line " ++ string_of_int(fst(a)) ++ ": " ++ snd(a),
        command.linesOfInterest,
      );
    combineLists([["Logged Output:", ""], formattedLinesOfInterest, [""]]);
  };

let formatSubCommand = (command: Command.t) => {
  combineLists([
    [
      "### " ++ code(command.command),
      "",
      processStatus(command.status),
      "Time Taken: " ++ string_of_float(command.runningTime),
      "",
    ],
    codeBlock(command.outputLines),
    [""],
  ]);
};

/*
 * Metadata should be markdown-esque, which makes it
 * easier to read in a bunch of places.
 *
 * Layout is broadly as follows:
 *
 * # fullMainCmd
 *
 * ## Result
 *
 * Return Code: X
 * Time taken: X
 *
 * Logged output:
 *  Line X: Y
 *  Line Z: Y
 *
 * ## Metadata
 *
 * ### cmd1
 *
 * Return Code: X
 * Time taken: X
 *
 * Output for cmd1
 *
 * ### cmd2
 *
 * Return Code: X
 * Time taken: X
 *
 * Output for cmd2
 */
let makeMetaData = (output: list(Command.t)) => {
  let mainCommand = List.nth(output, 0);

  let header = (n, str) => String.make(n, '#') ++ " " ++ str;
  let mainCommandOutput = [
    header(1, code(mainCommand.command)),
    "",
    header(2, "Result"),
    "",
    processStatus(mainCommand.status),
    "Time Taken: " ++ string_of_float(mainCommand.runningTime),
    "",
    ...formatLinesOfInterest(mainCommand),
  ];

  let subCommands = ref([]);

  for (i in 1 to List.length(output) - 1) {
    let currentSubCommand = List.nth(output, i);
    let metadata = formatSubCommand(currentSubCommand);

    subCommands := List.append(subCommands^, metadata);
  };

  if (List.length(subCommands^) == 0) {
    mainCommandOutput;
  } else {
    List.append(mainCommandOutput, subCommands^);
  };
};

let makeLogFile = (output: list(Command.t), config, logMsg) => {
  let mainLogFile = getLogFile("cmd", config);
  logMsg("Logging command output to " ++ mainLogFile);
  let metadataLogFile = getLogFile("meta", config);
  logMsg("Logging command metadata to " ++ metadataLogFile);

  logMsg("Writing main log file...");
  writeFile(mainLogFile, List.nth(output, 0).outputLines);

  logMsg("Builiding metadata markdown file...");
  let commandMetadata = makeMetaData(output);

  logMsg("Writing metadata log file...");
  writeFile(metadataLogFile, commandMetadata);
};
