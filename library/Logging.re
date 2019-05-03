/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */

open PathUtil;
open Util;

open Types.Config;
open Types.Command;

let getLogFilePath = (~time=Unix.gettimeofday(), job, config) => {
  let outputFolder =
    makeAbsolutePath(join([config.outputPath, getDateStr()]));

  checkFolderExists(config.outputPath);
  checkFolderExists(outputFolder);

  join([
    outputFolder,
    getFormattedDateTimeStr(~time, ()) ++ "_" ++ job ++ ".log",
  ]);
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
    combineLists([
      ["### Logged Output", ""],
      formattedLinesOfInterest,
      [""],
    ]);
  };

let formatOutputFiles = outputFiles =>
  if (outputFiles == []) {
    [];
  } else {
    let formattedLinesOfInterest =
      List.map(a => "    - " ++ makeAbsolutePath(a), outputFiles);
    combineLists([["## Output Files", ""], formattedLinesOfInterest, [""]]);
  };

let formatSubCommand = (command: Types.Command.t) => {
  combineLists([
    [
      "### " ++ code(command.command),
      "",
      processStatus(command.status),
      "Time Taken: " ++ Util.formatExecutionTime(command.runningTime),
      "",
    ],
    codeBlock(command.outputLines),
    [""],
  ]);
};

let formatSubCommands = (output, start) => {
  let subCommands = ref([]);

  for (i in start to List.length(output) - 1) {
    let currentSubCommand = List.nth(output, i);
    let metadata = formatSubCommand(currentSubCommand);

    subCommands := List.append(subCommands^, metadata);
  };

  subCommands^;
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
    "Time Taken: " ++ Util.formatExecutionTime(mainCommand.runningTime),
    "",
    ...formatLinesOfInterest(mainCommand),
  ];

  let subCommands = formatSubCommands(output, 1);

  if (List.length(subCommands) == 0) {
    mainCommandOutput;
  } else {
    List.append(mainCommandOutput, subCommands);
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

let makeLogFile = (output: list(Types.Command.t), config, time, logMsg) => {
  let metadataLogFile = getLogFilePath(~time, "meta", config);
  logMsg("Logging command metadata to " ++ metadataLogFile);

  logMsg("Building metadata markdown file...");
  let commandMetadata = makeMetaData(output);

  logMsg("Writing metadata log file...");
  writeFile(metadataLogFile, commandMetadata);
};

let getLastLogFilePath = (logOutputPath, logMsg) => {
  let absPath = makeAbsolutePath(logOutputPath);
  logMsg("Checking " ++ absPath);
  let mostRecentDateFolder = getMostRecentFromFolder(absPath, true);

  let folderToCheck =
    switch (mostRecentDateFolder) {
    | Some(path) => path
    | None => ""
    };

  if (folderToCheck == "") {
    None;
  } else {
    let mostRecentLogFile = getMostRecentFromFolder(folderToCheck, false);
    switch (mostRecentLogFile) {
    | Some(path) => Some(path)
    | None => None
    };
  };
};

/* I.e. every line up to lines of interest. */
let logFileHeaderLen = 7;

let getCurrentHeader = (path, config, logMsg) => {
  let currentLogFileLines = open_in(makeAbsolutePath(path));

  let i = ref(0);
  let headerLines = Array.init(logFileHeaderLen, i => "");
  let getLines = (i, line, headerLines) => {
    headerLines[i^] = line;
    i := i^ + 1;

    if (i^ > logFileHeaderLen) {
      close_in(currentLogFileLines);
    };
  };

  Stream.from(_ =>
    try (Some(input_line(currentLogFileLines))) {
    | End_of_file => None
    }
  )
  |> (
    stream =>
      try (Stream.iter(line => getLines(i, line, headerLines), stream)) {
      | _error => close_in(currentLogFileLines)
      }
  );

  headerLines;
};

let metadataExtensionLen = 8;
let pathWithoutType = path =>
  String.sub(path, 0, String.length(path) - metadataExtensionLen);

let linkOutputToLogFile = (metadataPath, config, outputs, logMsg) => {
  let currentHeader = getCurrentHeader(metadataPath, config, logMsg);
  let linkLogFilePath = pathWithoutType(metadataPath) ++ "output.log";
  let commandOutputPath = pathWithoutType(metadataPath) ++ "cmd.log";
  logMsg("Output link file will be " ++ linkLogFilePath);

  logMsg("Running the link commands...");
  let commandOutput =
    Command.runMultipleCommand(
      ~storeFirst=true,
      ~silent=true,
      config.linkCommands,
    );
  let profileOutput =
    List.append(
      formatOutputFiles(outputs),
      ["## Link Commands", "", ...formatSubCommands(commandOutput, 0)],
    );

  let linkMetadata =
    List.append(Array.to_list(currentHeader), profileOutput);

  logMsg("Writing link log file...");
  writeFile(linkLogFilePath, linkMetadata);

  PathUtil.makeLink(linkLogFilePath, "outputLogFile.log");
  PathUtil.makeLink(metadataPath, "metadataLogFile.log");
  PathUtil.makeLink(commandOutputPath, "cmdOutputLogFile.log");
};
