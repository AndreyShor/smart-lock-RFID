const express = require('express')
const bodyParser = require('body-parser');


const app = express()
const port = 8080;

const routes = require('./routes/routes')

app.use(bodyParser.urlencoded({ extended: false }))

app.use('', routes)

app.listen(port, () => {
    console.log(`App listening on port ${port}`)
})