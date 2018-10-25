const express = require('express')
const socketIO = require('socket.io')
const http = require('http')

const app = express()
const server = http.createServer(app)
const io = socketIO.listen(server)

const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const parser = new Readline()

io.on('connection', (socket) => {
    console.log('New socket connected')
})

app.get('/', (req, res, next) => {
    res.sendfile(__dirname + '/index.html')
})

const mySerial = new SerialPort('/dev/cu.usbmodem14101', {
    baudRate: 115200
})

mySerial.on('open', () => {
    console.log('Opened serial port')
})

mySerial.on('data', (data) => {
    console.log(data.toString())
    io.emit('arduino:data', {
        value: data.toString()
    })
})

mySerial.on('err', (err) => {
    console.log(err.message)
})

server.listen(3000, () => {
    console.log('Server on port 3000')
})