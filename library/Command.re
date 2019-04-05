/*
 * Command.re
 *
 * Functions related to running a command and saving its output.
 */

let parseLine = (printToScreen, line, lines) => {
  /*
   * Decide if this is needed.
   * Potentially, could use this for some in proc monitoring.
   * That is, add a unique key to commands say [@LOG@]:
   * and those commands are logged differently.
   *
   */
  if (printToScreen) {
    Console.log(line);
  };

  lines := [line, ...lines^];
};

let wrapCommand = command => {
  command ++ " 2>&1";
};

let runCmd =
    (~printToScreen=true, ~logCommand=true, ~config=Config.default, command) => {
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
    (logCommand ? Logging.logCommand(command, config) : wrapCommand(command))
    |> Unix.open_process_in;

  let lines = ref([]);

  Stream.from(_ =>
    switch (input_line(inChannel)) {
    | line => Some(line)
    | exception End_of_file => None
    }
  )
  |> (
    stream =>
      try (
        Stream.iter(line => parseLine(printToScreen, line, lines), stream)
      ) {
      | _error => close_in(inChannel)
      }
  );

  close_in(inChannel);
  lines^;
};
