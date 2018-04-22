'use strict';

const hogan = require('hogan.js');

const template = `\
<h1>{{title}}</h1>

<ul>
{{#posts}}
  <li>{{title}}</li>
{{/posts}}
</ul>
`;

const page = hogan.compile(template);

const vars = {
  title: undefined,
  posts: [
    {title: 'Lorem ipsum'},
    {title: 'dolor sit amet'},
    {title: 'consectetur adipiscing elit'}
  ]
};

console.log(page.render(vars));
