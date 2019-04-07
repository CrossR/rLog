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

  if (args.showHelp^ || List.length(args.restOfCLI^) == 0) {
    finished := true;
  };

  if (! finished^) {
    let _ = ReasonLoggerLib.Runner.start(args, logMsg);
    ();
  };
  /* I should return the main command error code here */
};

/*
 * Still need to add a few more things around this:
 * - Get the latest log file command (so I can easily link to files)
 * - A search command (just throw out command names and dates to the screen)
 * - Project based config (ie find git root and append all commands in there to
 * the normal commands to run)
 */
logAndRun();
