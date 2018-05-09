describe('events.trigger', function() {
  it('triggers an event on specified node', function() {
    var div = document.createElement('div');
    var spyed = {
      handler: function() {}
    };
    var spy = spyOn(spyed, 'handler');

    div.addEventListener('tests:fire', spyed.handler);
    events.fire(div, 'tests:fire');

    expect(spy).toHaveBeenCalled();
  });
});
