var events = (function() {
  function trigger(node, name) {
    node.dispatchEvent(new Event(name));
  }
  return { trigger: trigger };
})();
