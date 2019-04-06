/*
 * Runner.re
 *
 * Entry point to combine the main and sub command logging.
 */
open Cli;

let start = (args, logMsg) => {
  /*
   * Write a top level runner, that will launch this function, as well as all
   * the result of the stuff that is discussed here. Once that is done, I
   * should pass this a config object, rather than a path.
   *
   * Just before we start the command, we should load the config in, and then
   * start up and commands that are supposed to be run.
   *
   * This should be done in a thread to not block, and return back a list of strings?
   * Maybe a list of commandoutput types?
   *
   * This can then be combined with the other output to produce a final output file.
   *
   * I see two output files:
   *  - Command output file, direct from tee.
   *  - The metadata one, with the command ran, error code, logger commands from
   *    the config file, any extra commands logged as in the parseLine func.
   */
    logMsg("Loading config...");
    let config = Config.getConfig(args.configPath^);
    let cmd = String.concat(" ", args.restOfCLI^);
    logMsg("Command to be run is: " ++ cmd);

    let cmds = [cmd, ...config.commandsToRun]
    logMsg("Total command list is: ")
    List.iter((c) => logMsg("    " ++ c), cmds);

    logMsg("Staring command runner...");
    let _ = Command.runMultipleCommand(~config, cmds);
    ();
};