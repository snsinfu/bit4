'use strict';

const fs = require('fs');
const request = require('request');

const API_BASE = 'https://reminders-pa.clients6.google.com/v1internalOP';
const TOKEN_PATH = 'credentials.json';

const {access_token} = JSON.parse(fs.readFileSync(TOKEN_PATH));

const options = {
  url: API_BASE + '/reminders/list',
  auth: {
    bearer: access_token
  }
};

request.post(options, (error, res, body) => {
  console.log('error:', error);
  console.log('body:', body);
});
