open TestFramework;
open TestUtils;
open RLogLib.CommandVariables;

describe("Test command variable replacement", ({test, _}) => {
  test("Can replace CWD variable.", ({expect}) => {
    Unix.chdir("test");
    let command = "echo '${CWD}'";

    let expectedResult = Str.regexp({|.*test|});
    let result = replaceCWD([command]);

    expect.int(List.length(result)).toBe(1);

    let test = Str.string_match(expectedResult, List.nth(result, 0), 0);
    expect.bool(test).toBe(true);
    Unix.chdir("..");
  });

  test("Can replace GITROOT variable.", ({expect}) => {
    Unix.chdir("test");
    let command = "echo '${GITROOT}'";

    /* Fix for CI */
    let expectedResult = Str.regexp({|.*\(rLog\|app\|s\)|});
    let result = replaceGitRoot([command]);

    expect.int(List.length(result)).toBe(1);

    let test = Str.string_match(expectedResult, List.nth(result, 0), 0);
    expect.bool(test).toBe(true);
    Unix.chdir("..");

    let cwd = Sys.getcwd();
    Unix.chdir("/");

    let expectedResult = [];
    let result = replaceGitRoot([command]);

    expect.int(List.length(result)).toBe(0);
    expect.list(result).toEqual(expectedResult);
    Unix.chdir(cwd);
  });

  test("Can replace OUTPUTFILE variable.", ({expect}) => {
    let command = ["echo '${OUTPUTFILE}'", "rm '${OUTPUTFILE}'"];
    let outputFiles = ["testFile1.txt", "output.root", "sim.pndr"];

    let expectedResult = [
      "echo 'testFile1.txt'",
      "echo 'output.root'",
      "echo 'sim.pndr'",
      "rm 'testFile1.txt'",
      "rm 'output.root'",
      "rm 'sim.pndr'",
    ];
    let result = replaceOutputFile(command, outputFiles);

    expect.int(List.length(result)).toBe(6);
    expect.list(result).toEqual(expectedResult);
  });

  test("Can replace OUTPUTFILES variable.", ({expect}) => {
    let command = "echo '${OUTPUTFILES}'";
    let outputFiles = ["testFile1.txt", "output.root", "sim.pndr"];

    let expectedResult = ["echo 'testFile1.txt output.root sim.pndr'"];
    let result = replaceOutputFiles([command], outputFiles);

    expect.int(List.length(result)).toBe(1);
    expect.list(result).toEqual(expectedResult);
  });

  test("Can replace COMMAND variable.", ({expect}) => {
    let command = "echo '${COMMAND}'";
    let commandFromArgs = ["bash test1.sh", "bash test2.sh", "bash test3.sh"];

    let expectedResult = ["echo 'bash test1.sh bash test2.sh bash test3.sh'"];
    let result = replaceCommand([command], commandFromArgs);

    expect.int(List.length(result)).toBe(1);
    expect.list(result).toEqual(expectedResult);
  });

  test("Full format works", ({expect}) => {
    let command = [
      "echo 'Bob'",
      "cat '${OUTPUTFILE}'",
      "echo 'Bill'",
      "print '${OUTPUTFILES}'",
    ];
    let args = RLogLib.Types.Cli.default;
    args.restOfCLI := ["test1.sh", "test2.sh"];
    args.command = RLogLib.Types.CommandType.Link;

    let expectedResult = [
      "echo 'Bob'",
      "cat 'test1.sh'",
      "cat 'test2.sh'",
      "echo 'Bill'",
      "print 'test1.sh test2.sh'",
    ];
    let result = formatCommands(command, args, str => ());

    expect.int(List.length(result)).toBe(5);
    expect.list(result).toEqual(expectedResult);

    let command = ["echo 'Bob'", "echo '${COMMAND}'", "echo 'Bill'"];
    let args = RLogLib.Types.Cli.default;
    args.restOfCLI := ["bash test1.sh", "bash test2.sh"];
    args.command = RLogLib.Types.CommandType.Run;

    let expectedResult = [
      "echo 'Bob'",
      "echo 'bash test1.sh bash test2.sh'",
      "echo 'Bill'",
    ];
    let result = formatCommands(command, args, str => ());

    expect.int(List.length(result)).toBe(3);
    expect.list(result).toEqual(expectedResult);
  });
});
