"use strict";

import * as https from "https";


export function requestHTTPS(options) {
  return new Promise((resolve, reject) => {
    const req = https.request(options, res => {
      const { statusCode } = res;
      if (!isSuccess(statusCode)) {
        res.resume();
        reject(new Error(`request failed with status ${statusCode}`));
      }

      // Assume text document.
      let body = "";
      res.on("data", chunk => {
        body += chunk;
      });
      res.on("end", () => {
        resolve(body, res.headers, statusCode);
      });
    });

    req.on("error", e => {
      reject(new Error(`request failed: ${e.message}`));
    });

    req.end();
  });
}

function isSuccess(statusCode) {
  return 200 <= statusCode && statusCode < 300;
}
