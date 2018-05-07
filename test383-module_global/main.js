const { JSDOM } = require('jsdom');
const { window } = new JSDOM('<h1>jsdom</h1>');

global.window = window;
global.document = window.document;
global.getComputedStyle = window.getComputedStyle;

require('./web-code.js');

console.log(document.body.getElementsByTagName('p'));
