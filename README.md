# reason-logger


[![CircleCI](https://circleci.com/gh/yourgithubhandle/reason-logger/tree/master.svg?style=svg)](https://circleci.com/gh/yourgithubhandle/reason-logger/tree/master)


**Contains the following libraries and executables:**

```
reason-logger@0.0.0
│
├─test/
│   name:    TestReasonLogger.exe
│   main:    TestReasonLogger
│   require: reason-logger.lib
│
├─library/
│   library name: reason-logger.lib
│   namespace:    ReasonLogger
│   require:
│
└─executable/
    name:    ReasonLoggerApp.exe
    main:    ReasonLoggerApp
    require: reason-logger.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x ReasonLoggerApp.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
