/*
 * Command.re
 *
 * Functions related to running a command and saving its output.
 */

open Types.Command;

/*
 * Print the results of the command, and store when needed. Storing is only for
 * the subcommands here, to save performance.
 */
let parseLine = (storeOutput, runSilently, line, commandOutput) => {
  /*
   * Parse a line for printing, if needed
   */
  if (!runSilently) {
    Console.log(line);
  };

  if (storeOutput) {
    commandOutput.outputLines = [line, ...commandOutput.outputLines];
  };
};

let getTeeCommand = (command, logFile) => {
  let pipeName = "/tmp/rLog" ++ Util.genRandomStr(10);
  (
    Printf.sprintf(
      "mkfifo %s; tee %s < %s & %s > %s",
      pipeName,
      logFile,
      pipeName,
      command,
      pipeName,
    ),
    pipeName,
  );
};

/*
 * For the main command, directly tee its results, else stream them.
 */
let wrapCommand = (command, logFile) => {
  logFile == "" ? (command ++ " 2>&1", "") : getTeeCommand(command, logFile);
};

/*
 * Run a given command, storing its output or printing if asked.
 */
let runCmd = (~storeOutput=false, ~runSilently=false, ~logFile="", command) => {
  let startTime = Unix.gettimeofday();

  let commandOutput = default(command);

  let processOutput = command => {
    let inChannel = Unix.open_process_in(command);

    let rec process_otl_aux = () => {
      let line = input_line(inChannel);
      parseLine(storeOutput, runSilently, line, commandOutput);
      process_otl_aux();
    };

    try (process_otl_aux()) {
    | End_of_file =>
      let stat = Unix.close_process_in(inChannel);
      (commandOutput, stat);
    };
  };

  let (wrappedCommand, pipeName) = wrapCommand(command, logFile);
  let result = processOutput(wrappedCommand);
  commandOutput.status = Some(snd(result));

  let endTime = Unix.gettimeofday();
  commandOutput.runningTime = endTime -. startTime;

  /* Fix the backwards output array. */
  if (commandOutput.outputLines != []) {
    commandOutput.outputLines = List.rev(commandOutput.outputLines);
  };

  /* Tidy the pipe up if needs be. */
  if (pipeName != "") {
    Sys.remove(pipeName);
  };

  commandOutput;
};

/*
 * Run a list of commands in parallel.
 *
 * The main command isn't stored, but the rest are.
 */
let runMultipleCommand =
    (~silent=false, ~logFile="", listOfCommands: list(string)) => {
  let parMapList = Parmap.L(listOfCommands);

  /* Run all but the main command silently, unless explicity silent. */
  let runSilently = i => silent ? true : i != 0;

  /* Only give a log file for the first command. */
  let getLogFilePath = i => i == 0 ? logFile : "";

  /* Store output for all commands except the first, to help with performance. */
  let storeOutput = i => i == 0 ? false : true;

  Parmap.parmapi(
    (i, c) =>
      runCmd(
        ~storeOutput=storeOutput(i),
        ~runSilently=runSilently(i),
        ~logFile=getLogFilePath(i),
        c,
      ),
    parMapList,
  );
};
