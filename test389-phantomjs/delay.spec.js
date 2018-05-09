describe('delay', function() {
  it('delays execution of a callback', function(done) {
    var startTime = Date.now();
    delay(50).then(function() {
      var elapsedTime = Date.now() - startTime;
      expect(elapsedTime).toBeGreaterThanOrEqual(50);
      done();
    });
  });
});
