function Delayed() {
}

Delayed.prototype.resolve = function() {
};

Delayed.prototype.then = function(onResolved) {
  this.resolve = onResolved;
};

function delay(timeout) {
  var delayed = new Delayed();
  window.setTimeout(function() { delayed.resolve(); }, timeout);
  return delayed;
}
