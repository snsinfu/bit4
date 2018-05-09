describe('element', function() {
  it('creates a DOM node', function() {
    var span = element('span');
    expect(span.tagName).toBe('SPAN');
  });

  it('creates a DOM node with specified content', function() {
    var span = element('span', {
      text: 'Lorem ipsum',
      className: 'small'
    });
    expect(span.tagName).toBe('SPAN');
    expect(span.innerHTML).toBe('Lorem ipsum');
    expect(span.className).toBe('small');
  });
});
