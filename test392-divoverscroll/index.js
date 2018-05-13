(function() {
  'use strict';

  var main;
  var list;

  document.addEventListener('DOMContentLoaded', function() {
    main = document.getElementById('main');
    list = document.getElementById('list');

    appendContents(20);

    list.addEventListener('scroll', handleScroll);
    list.addEventListener('app:bottom', handleBottom);
  });

  function appendContents(itemCount) {
    var fragment = document.createDocumentFragment();

    for (var i = 0; i < itemCount; i++) {
      var li = document.createElement('li');
      li.appendChild(document.createTextNode('Lorem ipsum'));
      fragment.appendChild(li);
    }

    list.appendChild(fragment);
  }

  function handleScroll(event) {
    var scrollBottom = list.scrollHeight - (list.clientHeight + list.scrollTop);
    if (scrollBottom < 1) {
      list.dispatchEvent(new Event('app:bottom'));
    }
  }

  function handleBottom(event) {
    var toast = element('span', 'toast spinner', 'Loading...');
    main.appendChild(toast);

    window.setTimeout(function() {
      main.removeChild(toast);
      appendContents(10);
    }, 1000);
  }

  function element(tag, className, text) {
    var element = document.createElement(tag);
    element.className = className;
    element.appendChild(document.createTextNode(text))
    return element;
  }
})();
