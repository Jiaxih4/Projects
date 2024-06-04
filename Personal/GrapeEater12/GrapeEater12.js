let board;
let boardWidth = 1500 - 100
let boardHeight = 860 -160
let context;

let mouthWidth = 150;
let mouthHeight = 100;

let grapeRadius = 30;
let grapeDefaultX = boardWidth - 200;
let grapeDefaultY = boardHeight - 600;

let offset = [0,0];
let isDown = false;
let open = false;
let frame = 0;

let titleArr = [];

let eaten = 0;



let mouth = {
    width : mouthWidth,
    height : mouthHeight,
    x : boardWidth/2 - mouthWidth/2 + 5,
    y : boardHeight/2 - mouthHeight/2 + 145
}

let grape = {
    radius : grapeRadius,
    x : boardWidth - 50,
    y : boardHeight - 600

}

window.onload = function(){
    board = document.getElementById("board");
    board.height = boardHeight
    board.width = boardWidth
    context = board.getContext("2d");

    grapes = new Image();
    grapes.src = 'file.png';
    grapes.onload = function() {
        context.drawImage(grapes, grapeDefaultX-120, grapeDefaultY-150, 300, 500);
    }
    

    requestAnimationFrame(update);
}

function update() {
    requestAnimationFrame(update);

    if (Math.floor(frame +0.05) > 6) {
        frame = 0;
    } else {
        frame = frame +0.1;
    }

    document.addEventListener('mousedown', setDownTrue)
    document.addEventListener('mouseup', setDownFalse)
    document.addEventListener('mousemove', calPos)

    context.fillStyle="purple";
    context.font = "50px Arial";
    context.fillText("Grapes Eaten: " +eaten ,10,350);


    dzhongopen = new Image();
    dzhongopen.src = 'dzhongshut.png';
    dzhongshut = new Image();
    dzhongshut.src = 'dzhongopen.png';
    if (!open) {
        dzhongopen.onload = function() {
            context.clearRect(0,0, boardWidth, boardHeight);
            context.drawImage(dzhongopen, 325, 120, 800, 800);
        }
    } else {
        dzhongshut.onload = function() {
            context.clearRect(0,0, boardWidth, boardHeight);
            context.drawImage(dzhongshut, 325, 120, 800, 800);
        }
    }

    context.fillStyle = "black"
    context.fillRect(mouth.x + 20, mouth.y, mouth.width, mouth.height)

    context.fillStyle = "green"
    context.fillRect(grapeDefaultX-120, grapeDefaultY-50, 300, 350)

    if (isDown) {
        context.fillStyle = "purple";
        context.beginPath();
        context.arc(grape.x, grape.y, grape.radius, 0, 2 * Math.PI);
        context.fill();
    }

        context.drawImage(grapes, grapeDefaultX-120, grapeDefaultY-150, 300, 500);

    titleImage = new Image();
    titleImage.src = 'frame'+Math.floor(frame)+'.png';
    titleImage.onload = function() {
        context.drawImage(titleImage,420,0);
    }
}

function setDownTrue(event){
    if (onGrape(event)){
        isDown = true;
        offset = [event.clientX, event.clientY]
        grape.x = offset[0]
        grape.y = offset[1]
    }
}

function setDownFalse() {
    isDown = false;
    open = false;
    if (grape.x > mouth.x -30 && grape.x < mouth.x + mouthWidth - 30 && grape.y > mouth.y && grape.y < mouth.y + mouthHeight) {
        let num = getRandomInt(3);
        if (num == 0) {
            let newSound = new Audio('Dzhong grap.mp4');
            newSound.play()
        } else if (num == 1) {
            let newSound = new Audio('Dzhong laugh.mp4');
            newSound.play()
        } else if (num == 2) {
            let newSound = new Audio('Dzhong grape x2.mp4');
            newSound.play()
        }
        eaten += 1;
    }
    grape.x = grapeDefaultX
    grape.y = grapeDefaultY
}

function calPos(event) {
    if (isDown) {
        offset = [event.clientX, event.clientY]

        grape.x = offset[0] -250
        grape.y = offset[1]
        if (grape.x > mouth.x && grape.x < mouth.x + mouthWidth && grape.y > mouth.y && grape.y < mouth.y + mouthHeight) {
            open = true;
        } else {
            open = false;
        }
    }
}

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
  }

function onGrape(event) {
    return event.clientX > grapeDefaultX-120  && event.clientX < grapeDefaultX + 300 && event.clientY > grapeDefaultY -50&& event.clientY < grapeDefaultY -50 + 350
}

(grapeDefaultX-120, grapeDefaultY-50, 300, 350)