let foo = () => print_endline("Hello");

let parseLine = (printToScreen, line, lines) => {
  if (printToScreen) {
    Console.log(line);
  };

  lines := [line, ...lines^];
};

let wrapCommand = command => {
  command
};

let runCmd = (~printToScreen=true, command) => {
  let (pOut, pIn, pErr) = Unix.open_process_full(wrapCommand(command), [||]);

  let outLines = ref([]);

  Stream.from(_ =>
    switch (input_line(pOut)) {
    | line => Some(line)
    | exception End_of_file => None
    }
  )
  |> (
    stream =>
      try (
        Stream.iter(line => parseLine(printToScreen, line, outLines), stream)
      ) {
      | _error => close_in(pOut)
      }
  );

  Stream.from(_ =>
    switch (input_line(pErr)) {
    | line => Some(line)
    | exception End_of_file => None
    }
  )
  |> (
    stream =>
      try (
        Stream.iter(line => parseLine(printToScreen, line, outLines), stream)
      ) {
      | _error => close_in(pErr)
      }
  );

  close_in(pOut);
  close_in(pErr);
  outLines^;
};
