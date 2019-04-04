/*
 * Logging.re
 *
 * Various utilities to logging the output of commands.
 */

let getLogFile = () => {
  "test.out";
};

let logCommand = command => {
  command ++ " 2>&1 | tee " ++ getLogFile();
};
