'use strict' // para usar las nuevas utilidades de javascript
const express = require('express') // importa la libreria de express
const bodyParser = require('body-parser') // body parser sirve para formatear las peticiones y manejarlas mas facil

const app = express()
const port = process.env.PORT || 3000

app.use(bodyParser.urlencoded({ extended:false }))
app.use(bodyParser.json())

app.get('/sensors', (req, res) => { //para agregar un parametro se usan los :
    res.send(200, {Sensors: []})
})

app.post('/sensors', (request, res) => {
    console.log(request.body)
    res.status(200).send({message: 'la prueba post ha sido exitosa.'})
})

app.put('/sensors/led/:led-status', (req, res) => { //para agregar un parametro se usan los :
       
})

app.listen(port, () => {
    console.log(`app corriendo en express escuchando en http://localhost:${port}`)
})