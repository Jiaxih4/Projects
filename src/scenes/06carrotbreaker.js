import { BATTLE_BACKGROUND_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, FULL_CARROT_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS, GRAPE_EATER_ASSET_KEYS, HAND_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class CarrotBreaker extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.CARROT_BREAKER_SCENE,
        })
        this.speed = 500;
    }

    create() {
        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        this.physics.world.setBoundsCollision(true, true, true, false);

        //Add grape
        this.grape = this.physics.add.image(this.scale.width/2,this.scale.height-100, GRAPE_ASSET_KEYS.GRAPE).setOrigin(0.5, 0);
        this.grape.setScale(4)
        this.grape.setCollideWorldBounds(true).setBounce(1).setMaxVelocity(300);
        this.grape.setData('onPaddle', true);
        this.grape.body.allowGravity = false;

        //Add Grape Catcher
        this.catcher = this.physics.add.image(this.scale.width/2, 0, D12_ASSET_KEYS.GRAPEOPEN).setOrigin(0.5,0);
        this.catcher.setScale(3)
        this.catcher.setImmovable(true);
        this.catcher.body.allowGravity = false;
        this.catcher.setSize(this.catcher.width- 20, this.catcher.height- 20);

        //Add hand
        this.hand = this.physics.add.image(this.scale.width/2, this.scale.height - 30, HAND_ASSET_KEYS.HAND).setOrigin(0.5);
        this.hand.setScale(0.5)
        this.hand.setImmovable(true);
        this.hand.body.allowGravity = false;
        this.hand.setCollideWorldBounds(true);

        //Load Full Carrot
        const texture = this.textures.get(FULL_CARROT_ASSET_KEYS.FULL_CARROT);
        const sourceImage = texture.source[0].image;
        const width = sourceImage.width;
        const height = sourceImage.height;

        //Add Full Carrot
        this.fullcarrot = this.physics.add.staticGroup();
        const scale = 0.4;
        const space = (this.scale.width - (11 * width * scale)) / 2

        for (let row = 0; row < 3; row++) {
            for (let col = 0; col < 12; col++) {

                const carrot = this.fullcarrot.create(
                    space + col * width * scale,
                    200 + row * height * scale,
                    FULL_CARROT_ASSET_KEYS.FULL_CARROT
                );

                carrot.setScale(scale);

                carrot.body.setSize(
                    carrot.displayWidth,
                    carrot.displayHeight
                );
            }
        }

        this.fullcarrot.refresh();
        
        //Add keyboard cursor keys
        this.cursors = this.input.keyboard.createCursorKeys();
        this.physics.add.overlap(this.grape, this.catcher, this.eatGrapes, null, this);
        this.physics.add.collider(this.grape, this.hand, this.hitHand, null, this);
        this.physics.add.collider(this.grape, this.fullcarrot, this.hitBrick, null, this);

    }

    update() {
        this.SadGrape();
        this.movement();
        this.startGrape();
    }

    SadGrape(){
        if (this.grape.y >= this.scale.height) {
            this.scene.start(SCENE_KEYS.END_SCENE);
        }
    }

    movement() {
        const {left, right} = this.cursors;

        if (left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
            this.hand.setVelocityX(-this.speed);
        } else if (right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
            this.hand.setVelocityX(this.speed);
        } else {
            this.hand.setVelocityX(0)
        }
    }

    startGrape() {
        if (this.cursors.space.isDown) {
            if (this.grape.getData('onPaddle'))
            {
                this.grape.setVelocity(-75, -300);
                this.grape.setData('onPaddle', false);
            }
        }
        if (this.grape.getData('onPaddle')) {
                this.grape.x = this.hand.x;
        }
    }

    eatGrapes() {
        this.sound.play(D12_AUDIO_ASSET_KEYS.SIXSEVEN);
        this.registry.set('level', this.registry.get('level') + 1);
        this.scene.start(SCENE_KEYS.SNAKE_SCENE);
    }

    hitHand() {
        const speed = Phaser.Math.Between(1, 5);
        const jump = Phaser.Math.Between(200, 500);
        //this.grape.setVelocityX(-speed);
        this.grape.setVelocityY(-jump);

        let diff = 0;

        if (this.grape.x < this.hand.x)
        {
            //  Ball is on the left-hand side of the paddle
            diff = this.hand.x - this.grape.x;
            this.grape.setVelocityX(-speed* diff);
        }
        else if (this.grape.x > this.hand.x)
        {
            //  Ball is on the right-hand side of the paddle
            diff = this.grape.x - this.hand.x;
            this.grape.setVelocityX(speed* diff);
        }
        else
        {
            //  Ball is perfectly in the middle
            //  Add a little random X to stop it bouncing straight up!
            this.grape.setVelocityX(2 + Math.random() * 8);
        }
    }

    hitBrick (ball, brick)
    {
        brick.disableBody(true, true);
    }

}