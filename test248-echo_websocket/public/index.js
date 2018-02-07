(function() {
  'use strict';

  const TOPIC = 'technology';
  const LOG_LIST_ID = 'response';

  document.addEventListener('DOMContentLoaded', () => {
    const uri = 'ws://' + window.location.host + '/topics/' + TOPIC;
    const socket = new WebSocket(uri);

    socket.addEventListener('open', e => log('Connection opened'));
    socket.addEventListener('message', e => log('Message: ' + e.data));
    socket.addEventListener('close', e => log('Connection closed'));
  });

  function log(message) {
    const list = document.getElementById(LOG_LIST_ID);
    const timestamp = new Date().toISOString();
    const entry = ' [' + timestamp + '] ' + message;
    list.appendChild(createElement('li', entry));
  }

  function createElement(tag, text) {
    const element = document.createElement(tag);
    element.appendChild(document.createTextNode(text));
    return element;
  }
})();
