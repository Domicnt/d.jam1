//-------Server Stuff-------//

//set up socket
var socket = io.connect('http://localhost/');//connects to localhost
//var socket = io.connect('https://.herokuapp.com');//connects to external server

//client's ID
let ID = 0;

//update ID when sent from server
socket.on('ID', function (data) {
    ID = data;
});

//return something when pinged by server
socket.on('ping', function (data) {
    socket.emit('pong');
});

//-------Game Stuff-------//

//set up canvas
let canvas = document.getElementById('canvas');
let context = canvas.getContext('2d');
let width = window.innerWidth;
let height = window.innerHeight;
context.canvas.width = width;
context.canvas.height = height;

//drawing stuff
function map(value, high1, high2) {
    return high2 * value / high1;
}

function draw(world) {
    //clear screen
    context.fillStyle = "#000";
    context.fillRect(0, 0, width, height);
    context.fillStyle = "#FFF";

    //draw players
    for (let i = 0; i < world.players.length; i++) {
        context.beginPath();
        context.arc(map(world.players[i].x, world.width, width), map(world.players[i].y, world.height, height), map(world.players[i].r, world.width, width), 0, 2 * Math.PI);
        context.fill();
    }

    //border
    context.beginPath();
    context.rect(0, 0, width, height);
    context.stroke();
}

//communication with server

//send inputs to server
window.addEventListener('keydown', event => {
    if (event.key == 'w' || event.keyCode == 38) socket.emit('up');
    if (event.key == 'a' || event.keyCode == 39) socket.emit('left');
    if (event.key == 's' || event.keyCode == 40) socket.emit('down');
    if (event.key == 'd' || event.keyCode == 41) socket.emit('right');
}, false);

//draw on recieving update from server
socket.on('world', function (data) {
    draw(data);
});