open TestFramework;
open TestUtils;

describe("Test library utilities", ({test, _}) =>
  test("Formats execution time correctly", ({expect}) => {
    let inputTime = 73947.38492;
    let expectedResult = "20:32:27.38491999999";

    let result = RLogLib.Util.formatExecutionTime(inputTime);
    expect.equal(expectedResult, result);
  })
);
