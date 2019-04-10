/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */

open Types.Config;
open PathUtil;
open Util;

let getLogFilePath = (job, config) => {
  let outputFolder = makeAbsolutePath(join([config.outputPath, getDate()]));

  checkFolderExists(config.outputPath);
  checkFolderExists(outputFolder);

  join([outputFolder, getFormattedTime() ++ "_" ++ job ++ ".log"]);
};

let formatLinesOfInterest = (command: Types.Command.t) =>
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

let formatSubCommand = (command: Types.Command.t) => {
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
let makeMetaData = (output: list(Types.Command.t)) => {
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

let parseLine = (i, line, linesOfInterest, config) => {
  let line = ref(line);
  let valuesToLog = config.valuesToLog;

  for (j in 0 to List.length(valuesToLog) - 1) {
    let currentRegex = Str.regexp(Str.quote(List.nth(valuesToLog, j)));

    if (Str.string_match(currentRegex, line^, 0)) {
      let subStart = Str.match_end();
      line := String.sub(line^, subStart, String.length(line^) - subStart);
      linesOfInterest := List.append(linesOfInterest^, [(i^ + 1, line^)]);
    };
  };

  i := i^ + 1;
};

let parseCmdOutput = (config, logFilePath) => {
  let fileInput = open_in(logFilePath);
  let i = ref(0);
  let linesOfInterest = ref([]);

  Stream.from(_ =>
    try (Some(input_line(fileInput))) {
    | End_of_file => None
    }
  )
  |> (
    stream =>
      try (
        Stream.iter(
          line => parseLine(i, line, linesOfInterest, config),
          stream,
        )
      ) {
      | _error => close_in(fileInput)
      }
  );

  linesOfInterest^;
};

let makeLogFile = (output: list(Types.Command.t), config, logMsg) => {
  let metadataLogFile = getLogFilePath("meta", config);
  logMsg("Logging command metadata to " ++ metadataLogFile);

  logMsg("Builiding metadata markdown file...");
  let commandMetadata = makeMetaData(output);

  logMsg("Writing metadata log file...");
  PathUtil.writeFile(metadataLogFile, commandMetadata);
};
