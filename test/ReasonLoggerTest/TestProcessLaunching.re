open TestFramework;

let rec range = (i, j) =>
  if (i > j) {
    [];
  } else {
    [string_of_int(i), ...range(i - 1, j)];
  };

describe("Test basic process launching", ({test, _}) => {
  test("Can launch a basic process and get output", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh";
    let result = ReasonLoggerLib.Util.runCmd(~printToScreen=false, cmd);

    let expectedResult = List.rev(range(1, 50));

    expect.equal(List.length(result), List.length(expectedResult));
    expect.equal(result, expectedResult);
  });

  test("Can launch a basic process and get both stdout and stderr", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_err.sh";
    let result = ReasonLoggerLib.Util.runCmd(~printToScreen=false, cmd);

    let expectedResult = List.rev(range(1, 50));

    expect.equal(List.length(result), List.length(expectedResult));
    expect.equal(result, expectedResult);
  });
});
