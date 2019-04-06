open TestFramework;
open TestUtils;
open ReasonLoggerLib.Command;

describe("Test basic process launching", ({test, _}) => {
  test("Can launch a basic process and get output", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh";
    let result = runCmd(~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);
  });

  test(
    "Can launch a basic process and get both stdout and stderr", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_err.sh";
    let result = runCmd(~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);
  });

  test("Logs lines of interest as expected", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/out_with_log.sh";
    let result = runCmd(~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);

    let linesOfInterest = [(1, "1"), (50, "50")];
    expect.equal(
      List.length(result.linesOfInterest),
      List.length(linesOfInterest),
    );
    expect.equal(result.linesOfInterest, linesOfInterest);
  });
});
