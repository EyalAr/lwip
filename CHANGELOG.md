## [2.0.1](https://github.com/randytarampi/lwip/compare/v2.0.0...v2.0.1) (2020-05-01)


### Bug Fixes

* package.json, package-lock.json & .snyk to reduce vulnerabilities ([a5be9d6](https://github.com/randytarampi/lwip/commit/a5be9d6b8c6422bff10d42fa1a382529c97fda94))
* package.json, package-lock.json & .snyk to reduce vulnerabilities ([662dceb](https://github.com/randytarampi/lwip/commit/662dceb1b7056074a4f9b7f413e0d19ce527679e))
* package.json, package-lock.json & .snyk to reduce vulnerabilities ([468a0e1](https://github.com/randytarampi/lwip/commit/468a0e15d112c32d883581d264978859f0709cdb))

# [2.0.0](https://github.com/randytarampi/lwip/compare/v1.3.1...v2.0.0) (2020-04-29)


### Features

* Support node@14. ([8e74b81](https://github.com/randytarampi/lwip/commit/8e74b815882499b69610c924391af522c80d4206)), closes [/github.com/randytarampi/lwip/pull/34#issuecomment-620724886](https://github.com//github.com/randytarampi/lwip/pull/34/issues/issuecomment-620724886)


* Merge pull request #36 from randytarampi/feat/support-node-14 ([abe243f](https://github.com/randytarampi/lwip/commit/abe243f39366d8184ee3e8b6686445db7d32560d)), closes [#36](https://github.com/randytarampi/lwip/issues/36)


### BREAKING CHANGES

* Drop support for node<8.

I mean, technically we haven't supported node<8 since  https://github.com/randytarampi/lwip/commit/76066757ec41c21e2e25520025a92dac3c9e7166 per  https://github.com/randytarampi/lwip/commit/b1883ebeb49e39aa2e6443a59fd72b9ff017688c, but by removing the tests I'm saying that it's **done**. Besides, node@8 is EoL already anyways right?
* Drop support for node<8.

## [1.3.1](https://github.com/randytarampi/lwip/compare/v1.3.0...v1.3.1) (2020-04-28)


### Bug Fixes

* **blur:** Just use `CImg.blur` to blur images ([4cf9aaf](https://github.com/randytarampi/lwip/commit/4cf9aaf4b76c6a26fb6b45ffd830da405f38e0cc))

# [1.3.0](https://github.com/randytarampi/lwip/compare/v1.2.0...v1.3.0) (2020-04-28)


### Features

* **Image:** Fix conflicts per https://github.com/randytarampi/lwip/pull/29. ([0745d4f](https://github.com/randytarampi/lwip/commit/0745d4f966d4ba5220309f132f83efe9beeae98e)), closes [/github.com/randytarampi/lwip/pull/29/files#diff-42195936cee3445e27c1695ab97c3cc8R563](https://github.com//github.com/randytarampi/lwip/pull/29/files/issues/diff-42195936cee3445e27c1695ab97c3cc8R563)

# [1.2.0](https://github.com/randytarampi/lwip/compare/v1.1.0...v1.2.0) (2020-04-28)


### Features

* **Image:** Fix conflicts per https://github.com/randytarampi/lwip/pull/28. ([7cd33d5](https://github.com/randytarampi/lwip/commit/7cd33d5e6b39ae9f383acd89381dd811abf9ee00))

# [1.1.0](https://github.com/randytarampi/lwip/compare/v1.0.1...v1.1.0) (2020-01-06)


### Features

* **package:** Blindly upgrade our dependencies for the new year. ([111fefc](https://github.com/randytarampi/lwip/commit/111fefc01aaaf4e7b0ebfe14500c7f22cacb2140))

## [1.0.1](https://github.com/randytarampi/lwip/compare/v1.0.0...v1.0.1) (2020-01-05)


### Bug Fixes

* Set our minimum node version to node@8, test against node@12, and allow failures against node@6. ([7606675](https://github.com/randytarampi/lwip/commit/7606675))

# [1.0.0](https://github.com/randytarampi/lwip/compare/v0.0.19...v1.0.0) (2020-01-05)


### Features

* Support node@12. ([b1883eb](https://github.com/randytarampi/lwip/commit/b1883eb)), closes [#20](https://github.com/randytarampi/lwip/issues/20)


### BREAKING CHANGES

* Drop support for node@6, but add support for node@12.

## [0.0.19](https://github.com/randytarampi/lwip/compare/v0.0.18...v0.0.19) (2019-03-10)


### Bug Fixes

* **travis:** Don't bother caching dependencies. ([81c0c4e](https://github.com/randytarampi/lwip/commit/81c0c4e))


### Reverts

* chore(package): Apparently we support node^4? ([5c860bf](https://github.com/randytarampi/lwip/commit/5c860bf))

## [0.0.18](https://github.com/randytarampi/lwip/compare/v0.0.17...v0.0.18) (2019-03-10)


### Bug Fixes

* **build:** Always build _everything_ from source. ([#5](https://github.com/randytarampi/lwip/issues/5)) ([f3a8775](https://github.com/randytarampi/lwip/commit/f3a8775))


### Reverts

* chore(build): use system libs on non-windows, speed up build ([#1](https://github.com/randytarampi/lwip/issues/1)) ([e61204b](https://github.com/randytarampi/lwip/commit/e61204b))

## [0.0.17](https://github.com/randytarampi/lwip/compare/v0.0.16...v0.0.17) (2019-03-09)


### Bug Fixes

* **build:** Build against the included `zlib`. ([#4](https://github.com/randytarampi/lwip/issues/4)) ([ee4352a](https://github.com/randytarampi/lwip/commit/ee4352a))


### Reverts

* Conditionally use ZLIB version, based on version of NodeJS ([53a9ccb](https://github.com/randytarampi/lwip/commit/53a9ccb))

## [0.0.16](https://github.com/randytarampi/lwip/compare/v0.0.15...v0.0.16) (2019-03-09)


### Bug Fixes

* **appveyor:** Actually `npm install -g npm`. ([d08cdad](https://github.com/randytarampi/lwip/commit/d08cdad))
* **appveyor:** Allow failures on node@6. ([4104ae6](https://github.com/randytarampi/lwip/commit/4104ae6))
* **appveyor:** Don't bother passing `--msvs_version` to `npm install`. ([be700c0](https://github.com/randytarampi/lwip/commit/be700c0))
* **appveyor:** Install the latest `npm`. ([ccf1e39](https://github.com/randytarampi/lwip/commit/ccf1e39))
* **appveyor:** Just run commands in shell mode. ([7841147](https://github.com/randytarampi/lwip/commit/7841147))

## [0.0.15](https://github.com/randytarampi/lwip/compare/v0.0.14...v0.0.15) (2019-03-08)


### Bug Fixes

* `new Buffer` -> `Buffer.allocUnsafe`. ([9b4b113](https://github.com/randytarampi/lwip/commit/9b4b113))

## [0.0.14](https://github.com/randytarampi/lwip/compare/v0.0.13...v0.0.14) (2019-03-08)


### Bug Fixes

* **travis:** `allow_failures` for all Windows builds per my note in cfb1e2e21d53eee3f9b6b39180347ff250af2020. ([56b6711](https://github.com/randytarampi/lwip/commit/56b6711))


### Reverts

* chore: Stop running CI on Appveyor. ([cfb1e2e](https://github.com/randytarampi/lwip/commit/cfb1e2e))

## [0.0.13](https://github.com/randytarampi/lwip/compare/v0.0.12...v0.0.13) (2019-03-08)


### Performance Improvements

* **package:** Just hardcode `node-gyp rebuild -j 8`. ([f07bf13](https://github.com/randytarampi/lwip/commit/f07bf13))

## [0.0.12](https://github.com/randytarampi/lwip/compare/v0.0.11...v0.0.12) (2019-03-08)


### Bug Fixes

* **semantic-release:** Use `@semantic-release/git` properly ([9cb6c1e](https://github.com/randytarampi/lwip/commit/9cb6c1e))
