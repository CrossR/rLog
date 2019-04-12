open TestFramework;
open TestUtils;
open RLogLib.Command;

describe("Test basic process launching", ({test, _}) => {
  test("Can launch a basic process and get output", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh";
    let result = runCmd(~storeOutput=true, ~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);
    expect.equal(result.status, Some(Unix.WEXITED(0)));
    expect.equal(result.command, cmd);
  });

  test(
    "Can launch a basic process and get both stdout and stderr", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_err.sh";
    let result = runCmd(~storeOutput=true, ~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);
    expect.equal(result.status, Some(Unix.WEXITED(0)));
    expect.equal(result.command, cmd);
  });

  test("Can launch a basic process and get an error code", ({expect}) => {
    let cmd = "bash " ++ Unix.getcwd() ++ "/test/assets/std_out_then_error.sh";
    let result = runCmd(~storeOutput=true, ~runSilently=true, cmd);

    let expectedResult = range(1, 50);

    expect.equal(
      List.length(result.outputLines),
      List.length(expectedResult),
    );
    expect.equal(result.outputLines, expectedResult);
    expect.equal(result.status, Some(Unix.WEXITED(123)));
    expect.equal(result.command, cmd);
  });

  test("Runs multiple commands correctly", ({expect}) => {
    let cmds = [
      "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh",
      "bash " ++ Unix.getcwd() ++ "/test/assets/std_err.sh",
      "bash " ++ Unix.getcwd() ++ "/test/assets/std_out.sh",
      "thisCommandWillError",
    ];

    let expectedResults = [[], range(1, 50), range(1, 50), []];
    let results = runMultipleCommand(~silent=true, cmds);

    for (i in 0 to List.length(results) - 1) {
      let result = List.nth(results, i);
      let cmd = List.nth(cmds, i);
      let expected = List.nth(expectedResults, i);

      if (i != 3) {
        expect.equal(result.outputLines, expected);
        expect.equal(result.status, Some(Unix.WEXITED(0)));
        expect.equal(result.command, cmd);
      } else {
        expect.equal(result.status, Some(Unix.WEXITED(127)));
      };
    };
  });
});
