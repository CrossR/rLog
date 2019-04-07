open TestFramework;
open TestUtils;

describe("Test logging utilities", ({test, _}) => {
  test("Returns correctly formatted date", ({expect}) => {
    let time = Unix.gmtime(1548922088.0);
    let result = RLogLib.Logging.getDate(~time, ());
    let expectedResult = "2019-01-31";

    expect.equal(result, expectedResult);
  });

  test("Returns correctly formatted time", ({expect}) => {
    let time = Unix.gmtime(1548922088.0);
    let result = RLogLib.Logging.getTime(~time, ());
    let expectedResult = "08:08:08";

    expect.equal(result, expectedResult);
  });

  test("Returns correctly formatted datetime", ({expect}) => {
    let time = 1548922088.0;
    let result = RLogLib.Logging.getFormattedTime(~time, ());
    let expectedResult = "2019-01-31T08:08:08Z";

    expect.equal(result, expectedResult);
  });
});
