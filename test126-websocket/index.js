#!/usr/bin/env node

const util = require('util');
const WebSocketClient = require('websocket').client;

const client = new WebSocketClient();

const WS_URL = process.env.WS_URL
const SUB_CHANNEL = process.env.SUB_CHANNEL

client.on('connectFailed', function(error) {
  console.log('Connection failed: ' + error.toString());
});

client.on('connect', function(connection) {
  connection.on('error', function(error) {
    console.log("Connection failed: " + error.toString());
  });

  connection.on('close', function() {
    console.log('Connection closed');
  });

  connection.on('message', function(message) {
    if (message.type !== 'utf8') {
      console.log('Unrecognized message');
    }

    const data = message.utf8Data;
    process.stdout.write(util.format("%d\t%s\n", Date.now(), data))
  });

  connection.sendUTF(JSON.stringify({type: "subscribe", channel: SUB_CHANNEL}));
});

client.connect(WS_URL);
