/*
 * Config.rei
 *
 * Configuration file loading and parsing.
 */

let makeDefaultConfig: string => unit;
let getConfig: (list(string), string => unit) => Types.Config.t;
