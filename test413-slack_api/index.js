'use strict';

const fs = require('fs');
const request = require('request');

const TOKEN_FILE = 'slack-token.json';
const TARGET_CHANNEL = 'botworld';
const MESSAGE = 'I am a bot.';
const NAME = 'botman';

main();

function main() {
  const {access_token} = JSON.parse(fs.readFileSync(TOKEN_FILE));

  const headers = {
    'Authorization': 'Bearer ' + access_token
  };

  const args = {
    channel:  TARGET_CHANNEL,
    text:     MESSAGE,
    as_user:  false,
    username: NAME
  };

  const options = {
    method:  'POST',
    url:     'https://slack.com/api/chat.postMessage',
    json:    true,
    headers: headers,
    body:    args
  };

  request(options, (error, res, body) => {
    console.log(res);
  });
}
