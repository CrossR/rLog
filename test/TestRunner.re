open TestFramework;
open TestUtils;

describe("Test main runner", ({test, _}) => {
  test("Runs set of commands correctly", ({expect}) => {
    let args = RLogLib.Types.Cli.default;
    let logMsg = (str: string) => ();

    args.restOfCLI :=
      ["bash", Unix.getcwd() ++ "/test/assets/out_with_log.sh"];
    args.configPath := Unix.getcwd() ++ "/test/assets/config.json";

    let result = RLogLib.Runner.start(~silent=true, args, logMsg);
    expect.equal(result, Some(Unix.WEXITED(0)));
  });

  test("Runs set of long running commands correctly", ({expect}) => {
    let args = RLogLib.Types.Cli.default;
    let logMsg = (str: string) => ();

    args.restOfCLI :=
      ["bash", Unix.getcwd() ++ "/test/assets/out_with_log.sh"];
    args.configPath := Unix.getcwd() ++ "/test/assets/config_sleep.json";

    let startTime = Unix.time();
    let result = RLogLib.Runner.start(~silent=true, args, logMsg);
    let endTime = Unix.time();

    expect.equal(result, Some(Unix.WEXITED(0)));
    expect.equal(endTime -. startTime >= 5.0, true);
  });

  test("Doesn't slow down massively for large output", ({expect}) => {
    let args = RLogLib.Types.Cli.default;
    let logMsg = (str: string) => ();

    args.restOfCLI := ["bash", Unix.getcwd() ++ "/test/assets/spamTerm.sh"];
    args.configPath := Unix.getcwd() ++ "/test/assets/config.json";

    let startTime = Unix.time();
    let result = RLogLib.Runner.start(~silent=true, args, logMsg);
    let endTime = Unix.time();

    expect.equal(result, Some(Unix.WEXITED(0)));
    /*
     * This is pretty generous, to account for differences on CI etc.
     * The original issue was massively slowed (100x slower), so this
     * will still catch things like that easily.
     */
    expect.equal(endTime -. startTime <= 5.0, true);
  });
});
