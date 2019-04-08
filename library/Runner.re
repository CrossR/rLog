/*
 * Runner.re
 *
 * Entry point to combine the main and sub command logging.
 */
open Cli;

/*
 * Always pass over the list, since we don't know if the others are needed until
 * we actually load the config.
 *
 * Could avoid this I guess if I sorted the dependency cycle betwen Config and
 * Command.
 */
let getConfigPaths = args => {
  let rootGitDir =
    Command.runCmd(~runSilently=true, "git rev-parse --show-toplevel");

  if (rootGitDir.outputLines != [] && rootGitDir.status == Some(WEXITED(0))) {
    [args.configPath^, List.nth(rootGitDir.outputLines, 0), "."];
  } else {
    [args.configPath^, "."];
  };
};

let start = (~silent=false, args, logMsg) => {
  let configPaths = getConfigPaths(args);

  let config = Config.getConfig(configPaths, logMsg);

  let cmd = String.concat(" ", args.restOfCLI^);
  logMsg("Command to be run is: " ++ cmd);

  let cmds = [cmd, ...config.commandsToRun];
  logMsg("Total command list is: ");
  List.iter(c => logMsg("    " ++ c), cmds);

  logMsg("Staring command runner...");
  let silent = silent || args.silent^;
  let logFile = Logging.getLogFilePath("cmd", config);

  let commandOutputs =
    Command.runMultipleCommand(~silent, ~logFile, ~config, cmds);

  let enableParsing = args.enableOutputParsing^;

  List.nth(commandOutputs, 0).linesOfInterest = (
    if (enableParsing) {
      Logging.parseCmdOutput(config, logFile);
    } else {
      [];
    }
  );

  Logging.makeLogFile(commandOutputs, config, logMsg);

  List.nth(commandOutputs, 0).status;
};
