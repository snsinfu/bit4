module.exports = function(config) {
  config.set({
    basePath: '.',
    files: [
      'dom.js',
      'delay.js',
      'dom.spec.js',
      'delay.spec.js'
    ],
    frameworks: ['jasmine'],
    browsers: ['PhantomJS'],
    logLevel: config.LOG_WARN
  });
};
