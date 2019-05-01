/*
 * CommandVariables.re
 *
 * Command variable replacements for rLog.
 */

let cwdRegex = Str.regexp({|\${CWD}|});

let gitRootRegex = Str.regexp({|\${GITROOT}|});
let gitRootReplaceRegex = Str.regexp({|.*\${GITROOT}.*|});

let outputFileRegex = Str.regexp({|\${OUTPUTFILE}|});

let outputFilesRegex = Str.regexp({|\${OUTPUTFILES}|});

let commandRegex = Str.regexp({|\${COMMAND}|});

let replaceCWD = commands =>
  List.map(c => Str.global_replace(cwdRegex, Sys.getcwd(), c), commands);
let replaceOutputFiles = (commands, outputFiles) =>
  List.map(
    c =>
      Str.global_replace(
        outputFilesRegex,
        String.concat(" ", outputFiles),
        c,
      ),
    commands,
  );
let replaceCommand = (commands, argCommands) =>
  List.map(
    c =>
      Str.global_replace(commandRegex, String.concat(" ", argCommands), c),
    commands,
  );

let replaceGitRoot = commands => {
  let rootGitDir =
    Command.runCmd(
      ~storeOutput=true,
      ~runSilently=true,
      "git rev-parse --show-toplevel",
    );

  /*
   * If we match, replace the variable, otherwise, remove the commands.
   */
  if (rootGitDir.outputLines != [] && rootGitDir.status == Some(WEXITED(0))) {
    let absGitPath =
      PathUtil.makeAbsolutePath(List.nth(rootGitDir.outputLines, 0));
    List.map(c => Str.global_replace(gitRootRegex, absGitPath, c), commands);
  } else {
    List.filter(c => !Str.string_match(gitRootReplaceRegex, c, 0), commands);
  };
};

let replaceOutputFile = (commands, outputFiles) => {
  let mapCommandToOutputs = (command, outputFiles) => {
    let matchPos =
      try (Str.search_forward(outputFileRegex, command, 0)) {
      | Not_found => (-1)
      };

    matchPos != (-1)
      ? List.map(
          o => Str.global_replace(outputFileRegex, o, command),
          outputFiles,
        )
      : [command];
  };

  List.map(c => mapCommandToOutputs(c, outputFiles), commands)
  |> List.flatten;
};

let formatCommands = (commands, args, logMsg) => {
  let finalCommands = ref([]);

  logMsg("Replacing CWD and GITROOT...");
  finalCommands := replaceCWD(commands);
  finalCommands := replaceGitRoot(finalCommands^);

  if (Cli.isRun(args)) {
    logMsg("Replacing COMMAND...");
    finalCommands := replaceCommand(finalCommands^, args.restOfCLI^);
  } else if (Cli.isLink(args)) {
    logMsg("Replacing OUTPUTFILE and OUTPUTFILES...");
    logMsg("Replacement is recursive: " ++ string_of_bool(args.recurse^));
    finalCommands := replaceOutputFile(finalCommands^, args.restOfCLI^);
    finalCommands := replaceOutputFiles(finalCommands^, args.restOfCLI^);
  };

  finalCommands^;
};
