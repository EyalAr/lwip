module.exports = {
    recursive: true,
    reporter: process.env.CI
        ? 'mocha-junit-reporter'
        : 'spec',
    timeout: 2000,
    slow: 200,
    reporterOptions: [
        'testsuitesTitle=true',
        'testCaseSwitchClassnameAndName=true',
        'suiteTitleSeparatedBy="."',
    ]
};
