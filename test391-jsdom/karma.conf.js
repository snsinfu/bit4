module.exports = function(config) {
  config.set({
    basePath: '.',
    files: [
      'dom.js',
      'delay.js',
      'dom.spec.js',
      'delay.spec.js'
    ],
    frameworks: ['jasmine', 'sinon'],
    browsers: ['jsdom'],
    reporters: ['dots'],
    logLevel: config.LOG_WARN
  });
};
