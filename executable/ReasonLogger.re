
let logAndRun = () => {
    let cmd = "/home/ryan/test.sh";
    let args = ReasonLoggerLib.Cli.getArgs();
    let finished = ref(false);

    if (args.showHelp^) {
        finished := true;
    }

    if (!finished^) {
        let _ = ReasonLoggerLib.Command.runCmd(~printToScreen=true, ~logCommand=true, cmd);
    };

    ()
}

logAndRun()
