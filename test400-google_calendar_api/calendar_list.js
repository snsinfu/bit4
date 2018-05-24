'use strict';

const fs = require('fs');
const request = require('request');

const API_HOST = 'https://www.googleapis.com/calendar/v3'
const TOKEN_PATH = 'credentials.json';

const {access_token} = JSON.parse(fs.readFileSync(TOKEN_PATH));

const options = {
  url: API_HOST + '/users/me/calendarList',
  auth: {
    bearer: access_token
  }
};

request.get(options, (error, res, body) => {
  console.log('error:', error);
  console.log('body:', body);
});
