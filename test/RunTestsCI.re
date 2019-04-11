/* CI Specific Tests, that output JUnit test output. */
include TestLogger;
include TestCommand;
include TestRunner;
include TestCommandVariables;

let jUnitOutputFile = "./" ++ Sys.os_type ++ "_junit.xml";

TestFramework.run(
  Rely.RunConfig.withReporters(
    [Default, JUnit(jUnitOutputFile)],
    Rely.RunConfig.initialize(),
  ),
);
