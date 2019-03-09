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
