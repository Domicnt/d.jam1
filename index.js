const express = require('express');
const app = express();
const server = require('http').Server(app)
server.listen(process.env.PORT || 80);
app.use('', express.static(__dirname));
const io = require('socket.io')(server);

const addon = require('./build/Release/testaddon.node');

//connections' IDs
let connections = [];
let connectionIDs = [];

function newID() {
    var result = '';
    var characters = 'abcdefghijklmnopqrstuvwxyz0123456789';
    for (var i = 0; i < 10; i++) {
        result += characters.charAt(Math.floor(Math.random() * characters.length));
    }
    return result;
}

io.on('connection', function (socket) {
    let ID = newID();
    connections.push(socket);
    connectionIDs.push(ID);
    addon.add(ID);
    socket.emit('ID', ID);

    //when recieving inputs from this connection
    socket.on('up', function () {
        addon.move(0, -.25, ID);
    });
    socket.on('left', function () {
        addon.move(-.25, 0, ID);
    });
    socket.on('down', function () {
        addon.move(0, .25, ID);
    });
    socket.on('right', function () {
        addon.move(.25, 0, ID);
    });

    //when this connection pings server
    socket.on('ping', function () {
        socket.emit('pong');
    });

    //when this connection disconnects
    socket.on('disconnect', function () {
        let disconnected = 0;
        for (let i = 0; i < connectionIDs.length; i++) {
            if (ID == connectionIDs[i]) {
                disconnected = i;
                connections.splice(disconnected, 1);
                connectionIDs.splice(disconnected, 1);
                break;
            }
        }
        for (let i = 0; i < connectionIDs.length; i++) {
            if (i < 2) {
                connections[i].emit('player', i + 1);
            }
            socket.emit('queue', i + 1);
        }
        addon.remove(ID);
    });
});

//ping client every minute
setInterval(function () {
    io.sockets.emit('ping');
}, 60000);

//perform one game step and update clients every 1/50th of a second
setInterval(function () {
    io.sockets.emit('world', addon.step());
}, 20)

module.exports = addon;