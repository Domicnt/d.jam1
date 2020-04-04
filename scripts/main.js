//-------Server Stuff-------//

//set up socket
//var socket = io.connect('http://localhost/');//connects to localhost
var socket = io.connect('https://djam1.herokuapp.com');//connects to external server

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

    //draw players
    for (let i = 0; i < world.players.length; i++) {
        context.fillStyle = "#" + world.players[i].color;
        for (let j = world.players[i].trailX.length; j >= 0; j--) {
            context.globalAlpha = map(world.players[i].trailX.length - j, world.players[i].trailX.length, 1);
            context.beginPath();
            context.arc(map(world.players[i].trailX[j], world.width, width), map(world.players[i].trailY[j], world.height, height), map(world.players[i].r, world.width, width), 0, 2 * Math.PI);
            context.fill();
        }
        context.fillStyle = "#FFF";
        context.font = "30px Arial";
        context.fillText(world.players[i].score, map(world.players[i].x, world.width, width), map(world.players[i].y, world.height, height) - map(world.players[i].r, world.width, width) - 20);
    }

    //draw goals
    context.fillStyle = "#F55";//red
    context.fillRect(0, height / 2 - map(world.goalHeight, world.height, height) / 2, 3, map(world.goalHeight, world.height, height));
    context.fillRect(width - 3, height / 2 - map(world.goalHeight, world.height, height) / 2, 3, map(world.goalHeight, world.height, height));
}

//communication with server

//send inputs to server
let up = false;
let left = false;
let down = false;
let right = false;

window.addEventListener('keydown', event => {
    if (event.key == 'w' || event.keyCode == 38) up = true;
    if (event.key == 'a' || event.keyCode == 42) left = true;
    if (event.key == 's' || event.keyCode == 40) down = true;
    if (event.key == 'd' || event.keyCode == 44) right = true;
}, false);

window.addEventListener('keyup', event => {
    if (event.key == 'w' || event.keyCode == 38) up = false;
    if (event.key == 'a' || event.keyCode == 42) left = false;
    if (event.key == 's' || event.keyCode == 40) down = false;
    if (event.key == 'd' || event.keyCode == 44) right = false;
}, false);

setInterval(function () {
    if (up) socket.emit('up');
    if (left) socket.emit('left');
    if (down) socket.emit('down');
    if (right) socket.emit('right');
}, 40);

//draw on recieving update from server
socket.on('world', function (data) {
    draw(data);
});