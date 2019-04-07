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

let makeAbsolutePath = path =>
  if (path.[0] == '~') {
    Str.replace_first(Str.regexp("~"), getHome(), path);
  } else {
    path;
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

let leftPadString = (~inputString, ~len, ~padding) => {
  let paddedString = ref(inputString);
  while (String.length(paddedString^) < len) {
    paddedString := padding ++ paddedString^;
  };

  paddedString^;
};

let code = str => "`" ++ str ++ "`";

let processMessage = (status: option(Unix.process_status)) => {
  switch (status) {
  | Some(WEXITED(code)) => "Exit Code: " ++ string_of_int(code)
  | Some(WSIGNALED(code)) =>
    "Exit (via signal) Code: " ++ string_of_int(code)
  | Some(WSTOPPED(code)) => "Stop Code: " ++ string_of_int(code)
  | None => "NO ERROR CODE"
  };
};

let rec combineLists = listOfLists =>
  switch (listOfLists) {
  | [] => []
  | [l, ...rest] => List.append(l, combineLists(rest))
  };

let codeBlock = strList => combineLists([["```sh"], strList, ["```"]]);

module CommandType = {
  type t =
    | Run
    | GenerateConfig
    | Search;

  let checkArg = str => {
    switch (str) {
    | "run" => Some(Run)
    | "genconfig" => Some(GenerateConfig)
    | "search" => Some(Search)
    | _ => None
    };
  };
};
