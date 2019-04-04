/*
 * Command.re
 *
 * Functions related to running a command and saving its output.
 */

let parseLine = (printToScreen, line, lines) => {
  if (printToScreen) {
    Console.log(line);
  };

  lines := [line, ...lines^];
};

let wrapCommand = command => {
  command ++ " 2>&1";
};

let runCmd = (~printToScreen=true, ~logCommand=true, ~configPath="", command) => {
  let inChannel =
    (
      logCommand
        ? Logging.logCommand(command, configPath) : wrapCommand(command)
    )
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
