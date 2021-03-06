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

let getLocalDateTime = utcTime => Unix.localtime(utcTime);

let padDate = str => leftPadString(~inputString=str, ~len=2, ~padding="0");

let getDateStr = (~time=getLocalDateTime(Unix.gettimeofday()), ()) => {
  string_of_int(time.tm_year + 1900)
  ++ "-"
  ++ padDate(string_of_int(time.tm_mon + 1))
  ++ "-"
  ++ padDate(string_of_int(time.tm_mday));
};

let getTimeStr = (~time=getLocalDateTime(Unix.gettimeofday()), ()) => {
  padDate(string_of_int(time.tm_hour))
  ++ ":"
  ++ padDate(string_of_int(time.tm_min))
  ++ ":"
  ++ padDate(string_of_int(time.tm_sec));
};

let getFormattedDateTimeStr = (~time=Unix.gettimeofday(), ()) => {
  let time = getLocalDateTime(time);
  getDateStr(~time, ()) ++ "T" ++ getTimeStr(~time, ()) ++ "Z";
};

let floatStr = (~start=0, inputStr) => {
  let str = string_of_float(inputStr);
  String.sub(str, start, String.length(str) - 1 - start);
};

let formatExecutionTime = executionTime => {
  let secondsInHour = 3600.0;
  let secondsInMin = 60.0;

  let hours = floor(executionTime /. secondsInHour);
  let remainingTime = executionTime -. hours *. secondsInHour;

  let minutes = floor(remainingTime /. secondsInMin);
  let remainingTime = remainingTime -. minutes *. secondsInMin;

  let seconds = floor(remainingTime);
  let remainingTime = remainingTime -. seconds;

  leftPadString(floatStr(hours), 2, "0")
  ++ ":"
  ++ leftPadString(floatStr(minutes), 2, "0")
  ++ ":"
  ++ leftPadString(floatStr(seconds), 2, "0")
  ++ "."
  ++ floatStr(~start=2, remainingTime);
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

let genRandomStr = length => {
  Random.self_init();
  let gen = () =>
    switch (Random.int(26 + 26 + 10)) {
    | n when n < 26 => int_of_char('a') + n
    | n when n < 26 + 26 => int_of_char('A') + n - 26
    | n => int_of_char('0') + n - 26 - 26
    };
  let gen = _ => String.make(1, char_of_int(gen()));
  String.concat("", Array.to_list(Array.init(length, gen)));
};

let stripFirstNChar = (str, ~n=1) =>
  if (String.length(str) < n) {
    str;
  } else {
    String.sub(str, n, String.length(str) - n);
  };

let stripLastNChar = (str, ~n=1) =>
  if (String.length(str) < n) {
    str;
  } else {
    String.sub(str, 0, String.length(str) - n);
  };

let stripFirstAndLastFromString = (str, first, last) =>
  stripFirstNChar(str, ~n=first) |> stripLastNChar(~n=last);
