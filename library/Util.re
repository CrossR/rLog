/*
 * Util.re
 *
 * Various utilities to help out both logging and running commands.
 */

let leftPadString = (~inputString, ~len, ~padding) => {
  let paddedString = ref(inputString);
  while (String.length(paddedString^) < len) {
    paddedString := padding ++ paddedString^;
  };

  paddedString^;
};

let padDate = str => leftPadString(~inputString=str, ~len=2, ~padding="0");

let getDate = (~time=Unix.gmtime(Unix.gettimeofday()), ()) => {
  string_of_int(time.tm_year + 1900)
  ++ "-"
  ++ padDate(string_of_int(time.tm_mon + 1))
  ++ "-"
  ++ padDate(string_of_int(time.tm_mday));
};

let getTime = (~time=Unix.gmtime(Unix.gettimeofday()), ()) => {
  padDate(string_of_int(time.tm_hour))
  ++ ":"
  ++ padDate(string_of_int(time.tm_min))
  ++ ":"
  ++ padDate(string_of_int(time.tm_sec));
};

let getFormattedTime = (~time=Unix.gettimeofday(), ()) => {
  let time = Unix.gmtime(time);
  getDate(~time, ()) ++ "T" ++ getTime(~time, ()) ++ "Z";
};

let code = str => "`" ++ str ++ "`";

let processStatus = (status: option(Unix.process_status)) => {
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
