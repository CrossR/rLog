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
    ReasonLoggerLib.Runner.start(args, logMsg);
  };
  ();
};

logAndRun();
