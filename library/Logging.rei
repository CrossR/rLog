/*
 * Logging.rei
 *
 * Various utilities to logging the output of commands.
 */

let getDate: (~time: Unix.tm=?, unit) => string;
let getTime: (~time: Unix.tm=?, unit) => string;
let getFormattedTime: (~time: float=?, unit) => string;

let getLogFilePath: (string, Config.t) => string;
let makeLogFile: (list(Command.t), Config.t, string => unit) => unit;
let parseCmdOutput: (Config.t, string) => list((int, string));
