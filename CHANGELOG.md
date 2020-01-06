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
