import Phaser from "../lib/phaser.js";
import { GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS, GRAPE_EATER_ASSET_KEYS } from "../assets/asset-keys.js";

const UP = 0;
const DOWN = 1;
const LEFT = 2;
const RIGHT = 3;


export class SnakeCharacter {
    
    constructor(scene, x, y, image, size, scale){
        this.scale = scale;
        this.image = image;
        this.snakeSpeed = size;
        this.scene = scene;

        this.alive = true;
        this.speed = 200;
        this.moveTime = 0;

        this.headPosition = new Phaser.Geom.Point(x,y);
        this.body = scene.add.group();

        this.head = this.body.create(x * size * scale, y * size* scale, image);
        this.head.setScale(scale);
        this.head.setOrigin(0);

        this.tail = new Phaser.Geom.Point(x,y);

        this.heading = RIGHT;
        this.direction = RIGHT;

        console.log(size);

    }

    update(time) {
        if (time >= this.moveTime) {
            return this.move(time);
        }
    }

    faceLeft() {
        if (this.direction == UP || this.direction == DOWN) {
            this.heading = LEFT;
        }
    }

    faceRight() {
        if (this.direction == UP || this.direction == DOWN) {
            this.heading = RIGHT;
        }
    }

    faceUp() {
        if (this.direction == LEFT || this.direction == RIGHT) {
            this.heading = UP;
        }
    }

    faceDown() {
        if (this.direction == LEFT || this.direction == RIGHT) {
            this.heading = DOWN;
        }
    }

    move(time) {
        switch(this.heading) {
            case LEFT:
                this.headPosition.x -=1;
                break;

            case RIGHT:
                this.headPosition.x +=1;
                break;

            case UP:
                this.headPosition.y -= 1;
                break;

            case DOWN:
                this.headPosition.y +=1;
                break;
        }

        if (this.headPosition.x < 0 || this.headPosition.x > this.scene.scale.width/(this.snakeSpeed * this.scale) || this.headPosition.y < 0 || this.headPosition.y > this.scene.scale.height/(this.snakeSpeed * this.scale)) {
            console.log("hit wall dead");
            this.alive = false;
        }

        this.direction = this.heading;

        const children = this.body.getChildren();

        // Save last segment position BEFORE shifting
        const last = children[children.length - 1];
        this.tail.x = last.x;
        this.tail.y = last.y;

        Phaser.Actions.ShiftPosition(children, this.headPosition.x * this.snakeSpeed * this.scale, this.headPosition.y * this.snakeSpeed * this.scale, 1);
        
        let hitBody = Phaser.Actions.GetFirst(children, {x: this.head.x, y: this.head.y}, 1);
        if (hitBody) {
            console.log('dead');

            this.alive = false;
            return false
        } else {
            this.moveTime = time + this.speed;
            return true;
        }
    }

    grow() {
        const newPart = this.body.create(this.tail.x, this.tail.y, this.image).setScale(this.scale).setOrigin(0);
    }

    eatFood(food) {
        const xdiff = Phaser.Math.Difference(this.head.x, food.x);
        const ydiff = Phaser.Math.Difference(this.head.y, food.y);
        //console.log(xdiff);
        //console.log(ydiff);
        if (xdiff <= this.snakeSpeed * this.scale && ydiff <= this.snakeSpeed   * this.scale) {
            this.grow();
            food.eat();
            return true;
        } else {
            return false;
        }
    }

    updateGrid(grid) {
        let size= this.snakeSpeed;
        let scale = this.scale;
        this.body.children.each(function (segment) {
            let bx = Math.floor(segment.x / (size * scale));
            let by = Math.floor(segment.y / (size * scale));

            grid[by][bx] = false;
        });
        return grid
    }


}