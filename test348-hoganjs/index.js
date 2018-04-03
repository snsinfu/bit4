'use strict';

const fs = require('fs');
const hogan = require('hogan.js');

const template = hogan.compile(fs.readFileSync('template.html', 'utf8'));

console.log(template.render({
  recentPosts: [
    {title: '1 Lorem ipsum'},
    {title: '2 Lorem ipsum'}
  ],

  olderPosts: [
    {
      day: "31 March",
      posts: [
        {title: '3 Lorem ipsum'},
        {title: '4 Lorem ipsum'}
      ]
    },
    {
      day: "30 March",
      posts: [
        {title: '5 Lorem ipsum'},
        {title: '6 Lorem ipsum'},
        {title: '7 Lorem ipsum'}
      ]
    }
  ]
}));
