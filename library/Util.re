/*
 * Util.re
 *
 * Various utilities to help out both logging and running commands.
 */

open Unix;

let join = paths => {
  let sep = Filename.dir_sep;
  let (head, rest) =
    switch (paths) {
    | [] => ("", [])
    | [head, ...rest] => (head, rest)
    };
  List.fold_left((accum, p) => accum ++ sep ++ p, head, rest);
};

let getHome = () => {
  switch (Sys.getenv_opt("HOME"), Sys.os_type) {
  | (Some(dir), _) => dir
  | (None, "Win32") => Sys.getenv("LOCALAPPDATA")
  | (None, _) => ""
  };
};

let makeAbsolutePath = path => {
    if (String.get(path, 0) == '~') {
        Str.replace_first(Str.regexp("~"), getHome(), path)
    } else {
        path
    };
};


let makeFolder = path => {
  Unix.mkdir(path, 0o755);
};

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
