I monitored network requests from https://calendar.google.com/calendar/r/week
and got a private API endpoint for the Reminaders service:

https://reminders-pa.clients6.google.com/v1internalOP/reminders/list?key=AIz...

The query parameter key= seems to be not necessary for viewing my reminder
tasks. These HTTP headers are used:

    Authorization: SAPISIDHASH 1527338753_6bb...
    X-Origin: https://calendar.google.com
    Cookie: ...

According to a reverse enginnering effort by a user on SO [1], the SAPISIDHASH
value is derived as follows:

    SAPISIDHASH = time + "_" + hash
    hash = sha1(time + " " + SAPISID + " " + xorigin)

And this worked. The time part could be any nonce (random value worked). In
addition to this, the following five cookie entries were necessary:

- HSID
- SSID
- APISID
- SAPISID
- SID

These values are not contained in OAuth2 response.

[1]: https://stackoverflow.com/a/32065323

Make request content type "application/json+protobuf" and send {"5":1} as the
request body to get the list of completed tasks. The response then is in
protobuf and field names are all mangled though.
