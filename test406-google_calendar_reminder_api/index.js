'use strict';

const crypto = require('crypto');
const fs = require('fs');
const https = require('https');

// Set values in your request cookies sent to reminders-pa.client6.google.com
const credentials = {
  HSID:    '_________________',
  SSID:    '_________________',
  APISID:  '__________________________________',
  SAPISID: '__________________________________',
  SID:     '_______________________________________________________________________'
};
try {
  Object.assign(credentials, JSON.parse(fs.readFileSync('credentials.json')));
} catch {
}

const hostname = 'reminders-pa.clients6.google.com';
const path = '/v1internalOP/reminders/list';
const xorigin = 'https://calendar.google.com';
const nonce = new Date().getTime().toString();

let hash = crypto.createHash('sha1');
hash.update(nonce)
    .update(' ')
    .update(credentials.SAPISID)
    .update(' ')
    .update(xorigin);
const sapisidHash = nonce + '_' + hash.digest('hex');

const cookie = Object.getOwnPropertyNames(credentials)
                     .map(key => key + '=' + credentials[key])
                     .join('; ');

const options = {
  method: 'POST',
  hostname: hostname,
  path: path,
  headers: {
    'Authorization': 'SAPISIDHASH ' + sapisidHash,
    'Cookie': cookie,
    'X-Origin': xorigin
  }
};

let payload = null;
// options.headers['Content-Type'] = 'application/json+protobuf';
// payload = '{"5": 0}';
// payload = '{"5": 1}';

request(options, payload)
  .then(body => {
    console.info(JSON.stringify(JSON.parse(body)));
  })
  .catch(error => {
    console.error(error);
  });

function request(options, payload) {
  return new Promise((resolve, reject) => {
    const req = https.request(options, res => {
      let body = '';

      res.on('data', chunk => {
        body += chunk;
      });

      res.on('end', () => {
        resolve(body, res);
      });
    });

    if (payload) {
      req.write(payload);
    }

    req.on('error', error => {
      reject(new Error(`request failed: ${error.message}`));
    });

    req.end();
  });
}
