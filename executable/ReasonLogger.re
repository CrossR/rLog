/*
 * ReasonLogger.re
 *
 * Main entry point for the CLI app.
 */

let logAndRun = () => {
  let args = ReasonLoggerLib.Cli.getArgs();
  let logMsg = message => args.verbose^ ? Console.log(message) : ();

  logMsg("Full passed arguments are: ");
  logMsg(args);

  let finished = ref(false);

  if (args.showHelp^) {
    finished := true;
  };

  if (! finished^) {
    logMsg("Loading config...");
    let config = ReasonLoggerLib.Config.getConfig(args.configPath^);
    let cmd = String.concat(" ", args.restOfCLI^);
    logMsg("Command to be run is: " ++ cmd);

    logMsg("Staring command runner...");
    let _ = ReasonLoggerLib.Command.runCmd(~config, cmd);
    ();
  };

  ();
};

logAndRun();
