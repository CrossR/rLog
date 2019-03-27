open TestFramework;

describe("Test basic process launching", ({test, _}) => {
  test("Can launch a basic process and get output", ({expect}) => {
    let result = ReasonLoggerLib.Util.runCmd("bash " ++ Unix.getcwd() ++ "/test/assets/test.sh");
    expect.equal(result, []);
  });
});
