let foo = () => print_endline("Hello");

let parseLine = (line, lines) => {
    Console.log(line);
    lines := [line, ...lines^]
};

let wrapCommand = (command) => {
    command ++ " 2>&1"
}

let runCmd = (command) => {
  let inChannel = wrapCommand(command) |> Unix.open_process_in;
  let lines = ref([]);

  Stream.from(_ =>
    switch (input_line(inChannel)) {
    | line => Some(line)
    | exception End_of_file => None
    }
  )
  |> (
    stream =>
      try (Stream.iter(line => parseLine(line, lines), stream)) {
      | _error => close_in(inChannel)
      }
  );

  close_in(inChannel);
};
