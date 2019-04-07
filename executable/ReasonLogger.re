/*
 * ReasonLogger.re
 *
 * Main entry point for the CLI app.
 */
open ReasonLoggerLib;

let logAndRun = () => {
  let args = Cli.getArgs();
  let logMsg = message => args.verbose^ ? Console.log(message) : ();

  logMsg("Full passed arguments are: ");
  logMsg(args);

  let finished = ref(false);

  if (args.showHelp^) {
    finished := true;
  };

  let shouldRun = Cli.isRun(args) && List.length(args.restOfCLI^) > 0;

  if (shouldRun && ! finished^) {
    let _ = Runner.start(args, logMsg);
    ();
  } else if (args.command == Util.CommandType.GenerateConfig) {
    Config.makeDefaultConfig(args.configPath^);
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
