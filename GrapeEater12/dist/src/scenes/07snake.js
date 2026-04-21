import { D12_AUDIO_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS } from "../assets/asset-keys.js";
import { SnakeCharacter } from "../Character/snakeCharacter.js";
import { SnakeFoodCharacter } from "../Character/snakeFoodCharacter.js";
import Phaser from "../lib/phaser.js";
import { DragGrapesScene } from "./01drag_grapes.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Snake extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.SNAKE_SCENE,
        })
        var UP = 0;
        var DOWN = 1;
        var LEFT = 2;
        var RIGHT = 3;
    }

    create() {

        //Set Level
        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        this.physics.world.setBoundsCollision(true, true, true, false);

        //Add Grape Catcher
        const textureCatcher = this.textures.get(GRAPE_CATCHER_ASSET_KEYS.GRAPE_CATCHER);
        const sourceImageCatcher = textureCatcher.source[0].image;
        this.snakeWidth = sourceImageCatcher.width;
        this.snakeHeight = sourceImageCatcher.height;
        this.snake = new SnakeCharacter(this, 8, 8, GRAPE_CATCHER_ASSET_KEYS.GRAPE_CATCHER, this.snakeWidth, 0.1);
        this.cursors = this.input.keyboard.createCursorKeys(); 

        //Add Grape
        const textureGrape = this.textures.get(GRAPE_ASSET_KEYS.GRAPE);
        const sourceImageGrape = textureGrape.source[0].image;
        this.widthGrape = sourceImageGrape.width;
        this.food = new SnakeFoodCharacter(this, 4, 4, GRAPE_ASSET_KEYS.GRAPE, this.widthGrape, 2);
    }

    update(time) {
        if (!this.snake.alive) {
            this.scene.start(SCENE_KEYS.END_SCENE);
            return;
        }
        if (this.cursors.left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
            this.snake.faceLeft();
        } else if (this.cursors.right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
            this.snake.faceRight();
        } else if (this.cursors.up.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W).isDown) {
            this.snake.faceUp();
        } else if (this.cursors.down.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.S).isDown) {
            this.snake.faceDown();
        }
        
        if (this.snake.update(time)){
            if (this.snake.eatFood(this.food)) {
                this.repositionFood();
                if (this.food.total == 4) {
                    this.sound.play(D12_AUDIO_ASSET_KEYS.CHICKENJOCKEY);
                    this.registry.set('level', this.registry.get('level') + 1);
                    this.scene.start(SCENE_KEYS.FROGGER_SCENE)
                }
            }
        }


    }

    repositionFood() {
        const testGrid = [];
        const gridWidth = this.scale.width / (this.snakeWidth * 0.1);
        const gridHeight = this.scale.height / (this.snakeHeight * 0.1);
        for (let y = 0; y < gridHeight; y+=1) {
            testGrid[y] = [];
            for ( let x = 0; x < gridWidth; x+=1) {
                if (y == 0 || x == 0 || x == gridWidth-1 || y  == gridHeight -1) {
                    testGrid[y][x] = false;
                } else {
                    testGrid[y][x] = true;
                }
            }
        }

        this.snake.updateGrid(testGrid);

        let validLocation = [];
        for (let y = 0; y < gridHeight; y+= 1) {
            for ( let x = 0; x < gridWidth; x+=1) {
                if (testGrid[y][x] == true) {
                    validLocation.push({x: x, y: y});
                }
            }
        }

        if (validLocation.length > 0) {
            let pos = Phaser.Math.RND.pick(validLocation);
            this.food.setPosition(pos.x * (this.widthGrape * 2), pos.y *(this.widthGrape * 2));
            return true;
        } else {
            return false;
        }
    }

}