/*
 * Command.re
 *
 * Functions related to running a command and saving its output.
 */

type t = {
  mutable outputLines: list(string),
  mutable linesOfInterest: list(string),
};

let default = () => {outputLines: [], linesOfInterest: []};

let parseLine = (runSilently, line, commandOutput) => {
  /*
   * Parse a line for printing and storing.
   */

  if (!runSilently) {
    Console.log(line);
  };

  commandOutput.outputLines = List.append(commandOutput.outputLines, [line]);
};

let checkForLinesOfInterest = (commandOutput, valuesToLog) => {
  /*
   * Parse all the lines of the command to find any of special interest.
   */

  let linesOfInterest = ref([]);
  let formattedLines = ref([]);

  for (i in 0 to List.length(commandOutput.outputLines) - 1) {
    let line = ref(List.nth(commandOutput.outputLines, i));

    for (j in 0 to List.length(valuesToLog) - 1) {
      let currentRegex = Str.regexp(Str.quote(List.nth(valuesToLog, j)));

      if (Str.string_match(currentRegex, line^, 0)) {
        let subStart = Str.match_end();
        line := String.sub(line^, subStart, String.length(line^) - subStart);
        linesOfInterest := List.append(linesOfInterest^, [line^]);
      };
    };

    formattedLines := List.append(formattedLines^, [line^]);
  };

  commandOutput.outputLines = formattedLines^;
  commandOutput.linesOfInterest = linesOfInterest^;

  commandOutput;
};

let wrapCommand = command => {
  command ++ " 2>&1";
};

let runCmd = (~runSilently=false, ~config=Config.default, command) => {
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
  let inChannel =
    (
      !runSilently
        ? Logging.logCommand(command, config) : wrapCommand(command)
    )
    |> Unix.open_process_in;

  let commandOutput = default();

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
          line => parseLine(runSilently, line, commandOutput),
          stream,
        )
      ) {
      | _error => close_in(inChannel)
      }
  );

  close_in(inChannel);

  checkForLinesOfInterest(commandOutput, Config.(config.valuesToLog));
};
