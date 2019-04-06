open TestFramework;
open TestUtils;

describe("Test basic process launching", ({test, _}) => {
  test("Can launch a basic process and get output", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh";
    let result = ReasonLoggerLib.Command.runCmd(~runSilently=true, cmd);

    let expectedResult = List.rev(range(1, 50));

    expect.equal(List.length(result), List.length(expectedResult));
    expect.equal(result, expectedResult);
  });

  test(
    "Can launch a basic process and get both stdout and stderr", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_err.sh";
    let result = ReasonLoggerLib.Command.runCmd(~runSilently=true, cmd);

    let expectedResult = List.rev(range(1, 50));

    expect.equal(List.length(result), List.length(expectedResult));
    expect.equal(result, expectedResult);
  });
});
