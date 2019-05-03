/*
 * PathUtil.re
 *
 * Path related utility helpers.
 */

/* Given a list of paths, combine them. */
let join = paths => {
  let sep = Filename.dir_sep;
  let (head, rest) =
    switch (paths) {
    | [] => ("", [])
    | [head, ...rest] => (head, rest)
    };
  List.fold_left((accum, p) => accum ++ sep ++ p, head, rest);
};

/* Get the appropriate Home directory for an OS. */
let getHome = () => {
  switch (Sys.getenv_opt("HOME"), Sys.os_type) {
  | (Some(dir), _) => dir
  | (None, "Win32") => Sys.getenv("LOCALAPPDATA")
  | (None, _) => ""
  };
};

/* Get the appropriate config directory for an OS. */
let getConfigLocation = () =>
  switch (Sys.os_type) {
  | "Win32" => join([getHome(), "rLog", "config.json"])
  | _ => join([getHome(), ".config", "rLog", "config.json"])
  };

/* Make a path absolute, by expanding ~ or adding the missing path. */
let makeAbsolutePath = path =>
  if (path.[0] == '~') {
    Str.replace_first(Str.regexp("~"), getHome(), path);
  } else if (path.[0] != '/') {
    join([Unix.getcwd(), path]);
  } else {
    path;
  };

/* Create a folder with some sensible permissions. */
let makeFolder = path => {
  Unix.mkdir(path, 0o755);
};

/* Check that a given folder exists. */
let checkFolderExists = path => {
  let absPath = makeAbsolutePath(path);

  let folderStat =
    try (Some(Unix.stat(absPath))) {
    | Unix.Unix_error(_, _, _) => None
    };

  switch (folderStat) {
  | None => makeFolder(absPath)
  | _ => ()
  };
};

/* Recursively check all folders in a path exists */
let checkPathExists = path => {
  let absPath = makeAbsolutePath(path);

  let fileSep = Str.regexp(Filename.dir_sep);
  let splitPath = Str.split(fileSep, absPath);

  let firstPathSep = Str.search_forward(fileSep, absPath, 1);
  let path = ref(String.sub(absPath, 0, firstPathSep));

  for (i in 1 to List.length(splitPath) - 1) {
    checkFolderExists(path^);
    path := join([path^, List.nth(splitPath, i)]);
  };
};

let writeFile = (path, stringList) => {
  let fileOut = open_out(path);

  let rec writeStringListToFile = outputChannel =>
    fun
    | [] => ()
    | [line, ...restOfList] => {
        Printf.fprintf(outputChannel, "%s\n", line);
        writeStringListToFile(outputChannel, restOfList);
      };

  writeStringListToFile(fileOut, stringList);
};

let getMostRecentFromFolder = (path, shouldBeDirectory) => {
  let absPath = makeAbsolutePath(path);
  let contents = ref(Array.to_list(Sys.readdir(absPath)));

  if (contents^ == []) {
    None;
  } else {
    if (shouldBeDirectory) {
      contents :=
        List.filter(c => Sys.is_directory(join([absPath, c])), contents^);
    };
    contents := List.sort((a, b) => compare(a, b), contents^);
    let mostRecent = List.nth(contents^, List.length(contents^) - 1);
    let result = join([absPath, mostRecent]);
    Some(result);
  };
};

let makeLink = (path, fileName) =>
  try (Unix.symlink(path, fileName)) {
  | Unix.Unix_error(Unix.EEXIST, command, _) =>
    Console.error("Failed to " ++ command ++ " due to file already existing!")
  | Unix.Unix_error(err, command, _) =>
    Console.error(
      "Failed to " ++ command ++ " with error " ++ Unix.error_message(err),
    )
  };

let getFilesFromPathRecursively = dir => {
  let rec walk = acc => {
    fun
    | [] => acc
    | [dir, ...tail] => {
        let contents = Array.to_list(Sys.readdir(dir));
        let contents = List.rev_map(Filename.concat(dir), contents);
        let (dirs, files) =
          List.fold_left(
            ((dirs, files), f) =>
              switch (Unix.(stat(f).st_kind)) {
              | Unix.S_REG => (dirs, [f, ...files]) /* Regular file */
              | Unix.S_DIR => ([f, ...dirs], files) /* Directory */
              | _ => (dirs, files)
              },
            ([], []),
            contents,
          );
        walk(files @ acc, dirs @ tail);
      };
  };
  walk([], [dir]);
};

let getAllFiles = paths => {
  let totalFiles = ref([]);
  for (i in 0 to List.length(paths) - 1) {
    let currentLinkObject = makeAbsolutePath(List.nth(paths, i));

    let isDirectory =
      switch (Unix.(stat(currentLinkObject).st_kind)) {
      | Unix.S_REG => false
      | Unix.S_DIR => true
      | _ => false
      };

    if (isDirectory) {
      let files = getFilesFromPathRecursively(currentLinkObject);
      totalFiles := List.append(totalFiles^, files);
    } else {
      totalFiles := List.append(totalFiles^, [currentLinkObject]);
    };
  };

  totalFiles^;
};
