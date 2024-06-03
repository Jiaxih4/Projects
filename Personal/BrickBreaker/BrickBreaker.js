//board
let board;
let boardWidth = 500;
let boardHeight = 500;
let context;

let playerWidth = 120;
let playerHeight = 10;
let player_vel = 10;

let ballradius = 10;
let ball_vel_x = 1;
let ball_vel_y = 2;

let blockArray = [];
let blockWidth = 60;
let blockHeight = 20;
let blockColumns = 8;
let blockRows = 3;
let blockMaxRows = 10;
let blockCount = 0;

let blockX = 10;
let blockY = 30;

let GameOver= false;
let cleared = false;


let ball = {
    x : boardWidth/2,
    y : boardHeight/2,
    radius : ballradius,
    velocity_x : ball_vel_x,
    velocity_y : ball_vel_y
}

let player = {
    x : boardWidth/2 - playerWidth/2,
    y : boardHeight - playerHeight - 5,
    width : playerWidth,
    height : playerHeight,
    velocity : player_vel
}

window.onload = function(){
    board = document.getElementById("board");
    board.height = boardHeight
    board.width = boardWidth
    context = board.getContext("2d");

    //player
    context.fillStyle = "skyblue";
    context.fillRect(player.x, player.y, player.width, player.height);

    requestAnimationFrame(update);
    createBlocks();
}

function update(){
    requestAnimationFrame(update)
    if (GameOver) {
        return;
    }
    if (blockCount == 0) {
        context.font = "20px sans-serif";
        context.fillText("Level Clear: Press 'Up' For Next Level", 80, 400);
        clear = true;
        return;
    }
    document.addEventListener("keydown", movePlayer);
    context.clearRect(0,0, boardWidth, boardHeight);
    //player
    context.fillStyle = "skyblue";
    context.fillRect(player.x, player.y, player.width, player.height);

    //ball
    context.fillStyle = "white";
    ball.x += ball.velocity_x;
    ball.y += ball.velocity_y;
    context.beginPath();
    context.arc(ball.x, ball.y, ball.radius, 0, 2 * Math.PI);
    context.fill();

    //hit wall
    if (hitBoarderRight() || hitBoarderleft() ) {
        ball.velocity_x *= -1;
    }
    if (hitBoarderTop()) {
        ball.velocity_y *=  -1;
    }
    if (hitBoarderBottom()) {
        context.font = "20px sans-serif";
        context.fillText("Game Over: Press 'Space' to Restart", 80, 400);
        GameOver = true;
    }
    //hit paddle
    if (hitPaddleRight(player) || hitPaddleLeft(player)) {
        ball.velocity_x = Math.min(ball.velocity_x * -1 + (Math.random() - 0.5) * 3, 3);
        ball.velocity_y = (Math.random() - 0.5) * 3;
    } 
    else if (hitPaddleTop(player) || hitPaddleBottom(player)) {
        ball.velocity_y = Math.min(ball.velocity_y * -1 + (Math.random() - 0.5) * 3, 3);
        ball.velocity_x = (Math.random() - 0.5) *3 ;
    }

    context.fillStyle = "skyblue";
    for (let i = 0; i < blockArray.length; i++) {
        let block = blockArray[i];
        if (block.special <= 2) {
            context.fillStyle = "red";
        } else {
            context.fillStyle = "skyblue";
        }
        if (!block.break) {
            if (hitPaddleTop(block) || hitPaddleBottom(block)) {
                block.break = true;
                ball.velocity_y *= -1;
                blockCount -= 1;
            }
            else if (hitPaddleRight(block) || hitPaddleLeft(block)) {
                block.break = true;
                ball.velocity_x *= -1;
                blockCount -= 1;
            }
            context.fillRect(block.x, block.y, block.width, block.height);
        }
    }

}

function movePlayer(k) {
    if (k.code == "Space") {
        resetGame();
    }
    if (k.code == "ArrowUp" && clear) {
        nextlevel();
    }
    if (k.code == "ArrowLeft" && (player.x - player.velocity + 10) > 0 ) {
        player.x -= player.velocity;
    } else if (k.code == "ArrowRight" && (player.x + player.velocity + player.width - 10) < boardWidth ) {
        player.x += player.velocity;
    }
}

function hitBoarderRight() {
    return (ball.x + ball.radius) > board.width;
}

function hitBoarderleft() {
    return (ball.x - ball.radius) < 0;
}

function hitBoarderTop() {
    return (ball.y - ball.radius) < 0;
}

function hitBoarderBottom() {
    return (ball.y + ball.radius) > board.height;
}

function noCollision(a) {
    return (a.x < ball.x + ball.radius) && //doesn't touch right
           (a.x + a.width > ball.x) && //doesn't touch left
           (a.y < ball.y + ball.radius) && //doesn't touch top
           (a.y + a.height > ball.y);   //doesn't touch bottom
}

function hitPaddleTop(a) {
    return (noCollision(a) && (a.y <= (ball.y + ball.radius)));
}

function hitPaddleBottom(a) {
    return (noCollision(a) && (a.y + a.height <= ball.y));
}

function hitPaddleRight(a) {
    return (noCollision(a) && (a.x <= (ball.x + ball.radius)));
}

function hitPaddleLeft(a) {
    return (noCollision(a) && (a.x + a.width <= ball.x));
}

function createBlocks() {
    blockArray = [];
    for (let c = 0; c < blockColumns; c++) {
        for (let r = 0; r < blockRows; r++) {
            let block = {
                x : blockX + c*blockWidth + c * 10,
                y : blockY + r * blockHeight + r*10,
                width : blockWidth,
                height : blockHeight,
                break : false,
                special : Math.floor(Math.random() * 10)
                
            }
            blockArray.push(block);
        }
    }
    blockCount = blockArray.length;
}

function resetGame() {
    GameOver = false;
    let ball = {
        x : boardWidth/2,
        y : boardHeight/2,
        radius : ballradius,
        velocity_x : ball_vel_x,
        velocity_y : ball_vel_y
    }
    
    let player = {
        x : boardWidth/2 - playerWidth/2,
        y : boardHeight - playerHeight - 5,
        width : playerWidth,
        height : playerHeight,
        velocity : player_vel
    }
    blockArray = [];
    createBlocks();

}

function nextlevel(){
    clear = false;
    let ball = {
        x : boardWidth/2,
        y : boardHeight/2,
        radius : ballradius,
        velocity_x : ball_vel_x,
        velocity_y : ball_vel_y
    }
    
    let player = {
        x : boardWidth/2 - playerWidth/2,
        y : boardHeight - playerHeight - 5,
        width : playerWidth,
        height : playerHeight,
        velocity : player_vel
    }
    blockRows += 1;
    blockArray = [];
    createBlocks();
}