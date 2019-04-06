/*
 * Runner.re
 *
 * Entry point to combine the main and sub command logging.
 */
open Cli;

let start = (args, logMsg) => {
  logMsg("Loading config...");
  let config = Config.getConfig(args.configPath^);
  let cmd = String.concat(" ", args.restOfCLI^);
  logMsg("Command to be run is: " ++ cmd);

  let cmds = [cmd, ...config.commandsToRun];
  logMsg("Total command list is: ");
  List.iter(c => logMsg("    " ++ c), cmds);

  logMsg("Staring command runner...");
  let commandOutputs = Command.runMultipleCommand(~config, cmds);
  Logging.makeLogFile(commandOutputs, config, logMsg);

  List.nth(commandOutputs, 0).status
};
