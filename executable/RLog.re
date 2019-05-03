/*
 * RLog.re
 *
 * Main entry point for the CLI app.
 */
open RLogLib;

let logAndRun = (args, logMsg) => {
  let processStatus = Runner.start(args, logMsg);

  /* For anything but an actual exit, return 1 */
  switch (processStatus) {
  | Some(WEXITED(int)) => int
  | Some(WSIGNALED(int)) => 1
  | Some(WSTOPPED(int)) => 1
  | None => 1
  };
};

/*
 * Still need to add a few more things around this:
 * - Get the latest log file command (so I can easily link to files)
 * - A search command (just throw out command names and dates to the screen)
 * - Project based config (i.e. find git root and append all commands in there to
 * the normal commands to run)
 */
let getArgsAndRun = () => {
  let args = Cli.getArgs();
  let logMsg = message => args.verbose^ ? Console.log(message) : ();

  logMsg("Full passed arguments are: ");
  logMsg(args);

  let finished = ref(false);
  let exitCode = ref(0);

  if (args.showHelp^) {
    finished := true;
  };

  let shouldRun = Cli.isRun(args) && List.length(args.restOfCLI^) > 0;

  if (shouldRun && ! finished^) {
    exitCode := logAndRun(args, logMsg);
  } else if (Cli.isConfigGen(args)) {
    Config.makeDefaultConfig(args.configPath^);
  } else if (Cli.isLink(args)) {
    Runner.link(args, logMsg);
  };

  exit(exitCode^);
};

/* Start the main app */
getArgsAndRun();
