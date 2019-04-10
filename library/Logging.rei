/*
 * Logging.rei
 *
 * Various utilities to logging the output of commands.
 */

let getLogFilePath: (string, Types.Config.t) => string;
let makeLogFile:
  (list(Types.Command.t), Types.Config.t, string => unit) => unit;
let parseCmdOutput: (Types.Config.t, string) => list((int, string));
