/*
 * Runner.re
 *
 * Entry point to combine the main and sub command logging.
 */
open Types.Cli;

/*
 * Always pass over the list, since we don't know if the others are needed until
 * we actually load the config.
 *
 * Could avoid this I guess if I sorted the dependency cycle betwen Config and
 * Command.
 */
let getConfigPaths = args => {
  let rootGitDir =
    Command.runCmd(
      ~storeOutput=true,
      ~runSilently=true,
      "git rev-parse --show-toplevel",
    );

  if (rootGitDir.outputLines != [] && rootGitDir.status == Some(WEXITED(0))) {
    [args.configPath^, List.nth(rootGitDir.outputLines, 0), "."];
  } else {
    [args.configPath^, "."];
  };
};

/* Actually run a command and all its subcommands for logging. */
let start = (~silent=false, args, logMsg) => {
  let configPaths = getConfigPaths(args);

  let config = Config.getConfig(configPaths, logMsg);

  let replacedConfigCommands =
    CommandVariables.formatCommands(config.logCommands, args);
  config.logCommands = replacedConfigCommands;

  let cmd = String.concat(" ", args.restOfCLI^);
  logMsg("Command to be run is: " ++ cmd);

  let cmds = [cmd, ...config.logCommands];
  logMsg("Total command list is: ");
  List.iter(c => logMsg("    " ++ c), cmds);

  logMsg("Staring command runner...");
  let silent = silent || args.silent^;

  /* Store the time, so the Metadata one matches. */
  let time = Unix.gettimeofday();
  let logFile = Logging.getLogFilePath(~time, "cmd", config);

  let commandOutputs = Command.runMultipleCommand(~silent, ~logFile, cmds);

  let enableParsing = args.enableOutputParsing^;

  List.nth(commandOutputs, 0).linesOfInterest = (
    if (enableParsing) {
      Logging.parseCmdOutput(config, logFile);
    } else {
      [];
    }
  );

  Logging.makeLogFile(commandOutputs, config, time, logMsg);

  List.nth(commandOutputs, 0).status;
};

let link = (args, logMsg) => {
  let configPaths = getConfigPaths(args);
  let config = Config.getConfig(configPaths, logMsg);

  let replacedConfigCommands =
    CommandVariables.formatCommands(config.linkCommands, args);
  config.linkCommands = replacedConfigCommands;

  let latestLogFile = Logging.getLastLogFilePath(config.outputPath, logMsg);
  let logFilePath =
    switch (latestLogFile) {
    | Some(path) => path
    | None => ""
    };

  let isMetadataPath =
    Str.string_match(Str.regexp({|.*meta\.log|}), logFilePath, 0);
  let isOutputPath =
    Str.string_match(Str.regexp({|.*output\.log|}), logFilePath, 0);

  if (logFilePath != "" && isMetadataPath) {
    Logging.linkOutputToLogFile(logFilePath, config, args.restOfCLI^, logMsg);
  } else if (isOutputPath && args.force^) {
    let logFilePath =
      Str.replace_first(Str.regexp({|output|}), "meta", logFilePath);
    Logging.linkOutputToLogFile(logFilePath, config, args.restOfCLI^, logMsg);
  } else if (isOutputPath && args.force^ == false) {
    Console.error("An output file for the previous run exists...");
    Console.error("Use the -f flag if you are sure you want to do this.");
  } else {
    Console.error("Could not find a metadata file to link to...");
  };
};
