describe('delay', function() {
  var clock;

  beforeEach(function() {
    clock = sinon.useFakeTimers();
  });

  afterEach(function() {
    clock.restore();
  });

  it('delays execution of a callback', function() {
    var callback = sinon.fake();
    delay(50).then(callback);

    clock.tick(49);
    expect(callback.notCalled).toBe(true);

    clock.tick(1);
    expect(callback.calledOnce).toBe(true);

    clock.tick(1000);
    expect(callback.calledOnce).toBe(true);
  });
});
