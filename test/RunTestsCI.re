/* CI Specific Tests, that output JUnit test output. */

let jUnitOutputFile = "./" ++ Sys.os_type ++ "_junit.xml";

ReasonLoggerTest.TestFramework.run(
  Rely.RunConfig.withReporters(
    [Default, JUnit(jUnitOutputFile)],
    Rely.RunConfig.initialize(),
  ),
);
