/*
 * Util.re
 *
 * Various utilities to help out both logging and running commands.
 */

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
