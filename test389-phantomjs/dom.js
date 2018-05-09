function element(tag, options) {
  var element = document.createElement(tag);

  if (options && options.text) {
    element.appendChild(document.createTextNode(options.text));
  }
  if (options && options.className) {
    element.className = options.className;
  }

  return element;
}
