const process = require('process')
const util = require('util')

const bodyParser = require('body-parser')
const express = require('express')
const request = require('request')

const { PORT, CLIENT_ID, CLIENT_SECRET } = process.env

const app = express()

app.use(bodyParser.urlencoded({ extended: true }))

app.listen(PORT, function () {
  console.log('Listening on port ' + PORT)
})

app.get('/oauth', function (req, res) {
  if (!req.query.code) {
    res.status(500)
    return
  }

  request({
    url: 'https://slack.com/api/oauth.access',
    method: 'GET',
    qs: { code: req.query.code, client_id: CLIENT_ID, client_secret: CLIENT_SECRET }
  }, function (err, _, body) {
    if (err) {
      console.log(err)
      return
    }
    res.json(body)
  })
})

app.post('/hello', function (req, res) {
  res.send('Hello. Request: ' + util.inspect(req.body))
})
