import { BATTLE_BACKGROUND_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class GrapeCatcher extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.GRAPE_CATCHER_SCENE,
        })
        this.speed = 500;
        this.totalGrapes = 4;
        this.grapesEaten = 0;
    }

    create() {
        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        this.keys = this.input.keyboard.addKeys('W,S,A,D');

        //Add falling grape
        this.grape = this.physics.add.image(Phaser.Math.Between(0, this.scale.width - 20),0, GRAPE_ASSET_KEYS.GRAPE).setOrigin(0);
        this.grape.setScale(4)

        //Add Grape Catcher
        this.catcher = this.physics.add.image(this.scale.width/2, this.scale.height - 50, D12_ASSET_KEYS.GRAPECATCHER2).setOrigin(0.5);
        this.catcher.setScale(2)
        this.catcher.setImmovable(true);
        this.catcher.body.allowGravity = false;
        this.catcher.setCollideWorldBounds(true);
        this.catcher.setSize(this.catcher.width- 20, this.catcher.height- 20)

        //Add keyboard cursor keys
        this.cursors = this.input.keyboard.createCursorKeys();
        this.physics.add.overlap(this.grape, this.catcher, this.eatGrapes, null, this);
    }

    update() {
        this.SadGrape();
        this.movement();
    }

    SadGrape(){
        if (this.grape.y >= this.scale.height) {
            this.scene.start(SCENE_KEYS.END_SCENE);
        }
    }

    movement() {
        const {left, right} = this.cursors;

        if (left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
            this.catcher.setVelocityX(-this.speed);
        } else if (right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
            this.catcher.setVelocityX(this.speed);
        } else {
            this.catcher.setVelocityX(0)
        }
    }

    eatGrapes() {
        this.grape.setY(0);
        this.grape.setX(Phaser.Math.Between(0, this.scale.width - 20));
        this.grape.setVelocityY(0);
        this.grapesEaten++;
        this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPEX2);
        if (this.grapesEaten == this.totalGrapes) {
            this.registry.set('level', this.registry.get('level') + 1);
            this.scene.start(SCENE_KEYS.DINOSAURGAME);
        }
    }


}