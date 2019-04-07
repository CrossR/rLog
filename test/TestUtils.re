/*
 * TestUtils.re
 *
 * Helpers for the tests.
 */

let rec range = (i, j) =>
  if (i > j) {
    [];
  } else {
    [string_of_int(i), ...range(i + 1, j)];
  };
