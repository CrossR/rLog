/*
 * Command.re
 *
 * Functions related to running a command and saving its output.
 */

type t = {
  command: string,
  mutable outputLines: list(string),
  mutable linesOfInterest: list((int, string)),
  mutable status: option(Unix.process_status),
  mutable runningTime: float,
};

let default = command => {
  command,
  outputLines: [],
  linesOfInterest: [],
  status: None,
  runningTime: 0.0,
};

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

let wrapCommand = (command, logFile) => {
  logFile == "" ? command ++ " 2>&1" : command ++ " 2>&1 | tee -a " ++ logFile;
};

let runCmd =
    (
      ~storeOutput=false,
      ~runSilently=false,
      ~config=Config.default,
      ~logFile="",
      command,
    ) => {
  let startTime = Unix.gettimeofday();

  let inChannel = wrapCommand(command, logFile) |> Unix.open_process_in;
  let commandOutput = default(command);

  Stream.from(_ =>
    switch (input_line(inChannel)) {
    | line => Some(line)
    | exception End_of_file => None
    }
  )
  |> (
    stream =>
      try (
        Stream.iter(
          line => parseLine(storeOutput, runSilently, line, commandOutput),
          stream,
        )
      ) {
      | _error => close_in(inChannel)
      }
  );

  commandOutput.status = Some(Unix.close_process_in(inChannel));

  let endTime = Unix.gettimeofday();
  commandOutput.runningTime = endTime -. startTime;

  /* Fix the backwards output array. */
  if (commandOutput.outputLines != []) {
    commandOutput.outputLines = List.rev(commandOutput.outputLines);
  };

  commandOutput;
};

let runMultipleCommand =
    (~silent=false, ~logFile="", ~config, listOfCommands: list(string)) => {
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
        ~config,
        c,
      ),
    parMapList,
  );
};
