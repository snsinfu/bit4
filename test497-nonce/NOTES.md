Pre-shared key authentication of client and server with random nonce.

    C -> S:  cnonce || message
    S -> C:  snonce || server-challenge
    C -> S:  HMAC(cnonce || snonce || server-challenge) || client-challenge
    S -> C:  HMAC(cnonce || snonce || client-challenge)

Example:

    C -> S:  {nonce: "308d8751", message: "..."}
    S -> C:  {nonce: "40b64b00", challenge: "9f05e7ac"}
    C -> S:  {mac: HMAC("308d8751" + "40b64b00" + "9f05e7ac"), challenge: "8f68dfbf"}
    S -> C:  {mac: HMAC("308d8751" + "40b64b00" + "8f68dfbf")}

Replay attack won't work as long as nonce and challenge values are unique.

[1]: https://security.stackexchange.com/a/76603
