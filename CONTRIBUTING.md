# How to contribute

Any kind of contribution / feedback is appreciated! One of the greatest
motivations to make an open source project is to see people use it and get
involved (in any level).

How to:

0. [Suggest features / improvements](#suggest-features--improvements)
0. [Report bugs](#report-bugs)
0. [Contribute code](#contribute-code)

## Suggest features / improvements

Got an improvement you think can make lwip better? Great!

Open an issue with a descriptive title. Describe in a clear language the feature
you need. If possible, write a code sample of how you think this feature should
work.

## Report bugs

Open an issue with a descriptive title. __Write instructions__ on how to
reproduce the problem. __Code samples__ which demonstrate the problem can be
very useful. Also include error logs and information about your system (OS,
node version, npm version, lwip version, etc.).

## Contribute code

Code contributions are happily welcomed. In order to make the process managable
and pleasant, please do not blindly open PRs without discussing it first.

A PR should address an already opened issue. If your PR fixes a bug, there
should already be an issue reporting this bug. If your PR implements a new
feature, there should be an issue requesting this feature.

0. Disscuss your intention to work on an issue in the issue itself.
0. Fork lwip.
0. You may want to read development notes in `DEVELOPMENT.md`.
0. Your code should be based on the latest `version/x.x.x` branch, or `master`
   if no `version/x.x.x` branch is available. Ask to be sure.
   Before releasing a new version, a `version/x.x.x` branch will be opened, into
   which all feature branches for this version will be merged. Eventually this
   version branch will be merged into `master`.
0. Your commits should describe your changes. Try not to make too many unrelated
   changes in one commit. Split them. Write descriptive commit logs.
0. Include tests for your changes / new feature.
0. Make sure existing tests still pass.
0. If your PR changes the public API, update the README accordingly.
0. There are no written code conventions (yet), but try to get a sense of the
   coding style and keep similar style (no tabs, please).
0. Don't change git history (no force push).
0. Spell check.

## Copyrights

This project is published under the MIT lisence. All contributions are included
under the same lisence.
