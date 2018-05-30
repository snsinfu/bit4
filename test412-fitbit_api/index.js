'use strict';

const fs = require('fs');
const http = require('http');
const readline = require('readline');

const bodyParser = require('body-parser');
const ClientOAuth2 = require('client-oauth2');
const express = require('express');
const moment = require('moment');
const request = require('request');


const OAUTH_AUTH_URI      = 'https://www.fitbit.com/oauth2/authorize';
const OAUTH_TOKEN_URI     = 'https://api.fitbit.com/oauth2/token';
const OAUTH_SCOPES        = ['activity', 'heartrate', 'sleep'];
const OAUTH_SECRET_FILE   = 'client_secret.json';
const OAUTH_TOKEN_FILE    = 'token.json';
const OAUTH_REDIRECT_PORT = 3000;
const OAUTH_BRIDGE_HTML   = 'oauth_bridge.html';

const API_BASE = 'https://api.fitbit.com/1.2'

main();

function main() {
  acquireToken().then(token => {
    const date = moment().add(-1, 'day').format('YYYY-MM-DD');
    sleepLog(token, date).then(console.log);
  });
}

function sleepLog(token, date) {
  return new Promise((resolve, reject) => {
    const options = {
      method: 'GET',
      url:    `${API_BASE}/user/-/sleep/date/${date}.json`
    };
    token.sign(options);

    request(options, (error, res, body) => {
      resolve(JSON.parse(body));
    });
  });
}

function acquireToken() {
  const oauth = createAuthenticator();

  // NOTE: Current Fitbit API does not use refresh token. The user can instead
  //       configure a long expiratory time up to a year.

  return acquireTokenData(oauth).then(data => {
    const {accessToken, refreshToken, tokenType, expiresIn} = data;
    const token = oauth.createToken(accessToken, refreshToken, tokenType);
    token.expiresIn(expiresIn)
    return token;
  });
}

function acquireTokenData(oauth) {
  try {
    return Promise.resolve(JSON.parse(fs.readFileSync(OAUTH_TOKEN_FILE)));
  } catch (error) {
    console.log('Open the following URL in the web brower:')
    console.log(oauth.token.getUri());

    return authenticate().then(tokenData => {
      fs.writeFileSync(OAUTH_TOKEN_FILE, JSON.stringify(tokenData));
      return tokenData;
    });
  }
}

function createAuthenticator() {
  const {clientId, clientSecret} = JSON.parse(fs.readFileSync(OAUTH_SECRET_FILE));

  return new ClientOAuth2({
    clientId:         clientId,
    clientSecret:     clientSecret,
    accessTokenUri:   OAUTH_TOKEN_URI,
    authorizationUri: OAUTH_AUTH_URI,
    redirectUri:      'http://localhost:' + OAUTH_REDIRECT_PORT + '/',
    scopes:           OAUTH_SCOPES
  });
}

function authenticate() {
  return new Promise((resolve, reject) => {
    const authApp = express();
    const server = http.createServer(authApp);

    // Fitbit uses URL fragment for passing credentials. We use a bridge HTML
    // to re-POST the credentials to /token.

    authApp.use(bodyParser.urlencoded({
      extended: true
    }));

    authApp.get('/', (req, res) => {
      res.set('Content-Type', 'text/html');
      res.send(fs.readFileSync(OAUTH_BRIDGE_HTML));
    });

    authApp.post('/token', (req, res) => {
      res.send();
      server.close();
      resolve(standardizeTokenData(req.body));
    });

    server.listen(OAUTH_REDIRECT_PORT);
  });
}

function standardizeTokenData(data) {
  return {
    accessToken:  data.access_token,
    refreshToken: data.refresh_token,
    tokenType:    data.token_type,
    expiresIn:    parseInt(data.expires_in)
  };
}
