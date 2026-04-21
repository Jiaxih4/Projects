import { BATTLE_BACKGROUND_ASSET_KEYS, CARROT_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class FlappyGame extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.FLAPPY_SCENE,
        })
        this.jump = 150;
        this.totalCarrots = 4;
        this.carrotsCleared = 0;
        this.speed = 300;
    }

    create() {
        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        //Add Grape
        this.grape = this.physics.add.image(this.scale.width,this.scale.height, GRAPE_ASSET_KEYS.GRAPE).setOrigin(0, 1);
        this.grape.setScale(100)
        this.grape.setImmovable(true);
        this.grape.body.allowGravity = false;

        //Add carrot bottom
        const scale = Phaser.Math.Between(50, 100)/100
        this.carrot = this.physics.add.image(this.scale.width,this.scale.height, CARROT_ASSET_KEYS.CARROT).setOrigin(1);
        this.carrot.setScale(0.5, scale)
        this.carrot.setImmovable(true);
        this.carrot.body.allowGravity = false;
        this.carrotwidth = this.carrot.width * 0.8;
        this.carrot.body.setSize(this.carrotwidth, this.carrot.height)

        //Add carrot top
        this.carrot2 = this.physics.add.image(this.scale.width,0, CARROT_ASSET_KEYS.CARROT).setOrigin(1, 0).setFlipY(true);
        this.carrot2.setScale(0.5, 1.4-scale)
        this.carrot2.setImmovable(true);
        this.carrot2.body.allowGravity = false;
        this.carrot2.body.setSize(this.carrotwidth, this.carrot2.height)

        this.carrot.setVelocityX(-this.speed);
        this.carrot2.setVelocityX(-this.speed);
        

        //Add Grape Catcher
        this.catcher = this.physics.add.image(150, this.scale.height/2, D12_ASSET_KEYS.GRAPEFLAPPER).setOrigin(0.5);
        this.catcher.setScale(1)
        this.catcher.setImmovable(true);
        this.catcher.body.allowGravity = true;
        this.catcher.setSize(this.catcher.width- 40, this.catcher.height- 10);

        //Add keyboard cursor keys
        this.cursors = this.input.keyboard.createCursorKeys();

        //Add collision detection
        this.physics.add.overlap(this.grape, this.catcher, this.eatGrapes, null, this);
        this.physics.add.overlap(this.carrot, this.catcher, this.HitCarrot, null, this);
        this.physics.add.overlap(this.carrot2, this.catcher, this.HitCarrot, null, this);
    }

    update() {
        this.newCarrot();
        this.movement();
        if (this.catcher.y < 0 || this.catcher.y > this.scale.height) {
            this.scene.start(SCENE_KEYS.END_SCENE)
        }
    }

    HitCarrot(){
        this.scene.start(SCENE_KEYS.END_SCENE);
    }

    movement() {
        if ((this.cursors.up.isDown || this.cursors.space.isDown)) {
            this.catcher.setVelocityY(-this.jump);
        }
    }

    newCarrot() {
        if (this.carrot.x < 0) {
            this.carrot.setX(this.scale.width);
            this.carrot2.setX(this.scale.width);
            const scale = Phaser.Math.Between(50, 100)/100;
            this.carrot.setScale(0.5, scale);
            this.carrot2.setScale(0.5, 1.4-scale);
            this.carrotsCleared++;
            if (this.carrotsCleared == this.totalCarrots) {
                this.grape.setVelocityX(-400);
                this.carrot.setScale(0);
                this.carrot.setVelocityX(0)
                this.carrot2.setScale(0);
                this.carrot2.setVelocityX(0)
            }
        }
    }

    eatGrapes(){
        this.sound.play(D12_AUDIO_ASSET_KEYS.INDIAN);
        this.registry.set('level', this.registry.get('level') + 1);
        this.scene.start(SCENE_KEYS.BOUNCE_SCENE);
    }




}