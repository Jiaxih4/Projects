import { BATTLE_BACKGROUND_ASSET_KEYS, CARROT_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class DinosaurGame extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.DINOSAURGAME,
        })
        this.speed = 350;
        this.totalCarrots = 4;
        this.carrotsCleared = 0;
        this.scales = [0.2, 0.5, 0.4, 0.3]
    }

    create() {
        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        //Add Grape
        this.grape = this.physics.add.image(this.scale.width,this.scale.height, GRAPE_ASSET_KEYS.GRAPE).setOrigin(0, 1);
        this.grape.setScale(70)
        this.grape.setImmovable(true);
        this.grape.body.allowGravity = false;

        //Add carrot
        const scale = this.scales[Phaser.Math.Between(0, 3)]
        this.carrot = this.physics.add.image(this.scale.width,this.scale.height, CARROT_ASSET_KEYS.CARROT).setOrigin(1);
        this.carrot.setScale(scale)
        this.carrot.setImmovable(true);
        this.carrot.body.allowGravity = false;
        this.carrot.setVelocityX(-300);

        //Add Grape Catcher
        this.catcher = this.physics.add.image(150, this.scale.height - 50, D12_ASSET_KEYS.GRAPEPERSON).setOrigin(0.5);
        this.catcher.setScale(2)
        this.catcher.setImmovable(true);
        this.catcher.body.allowGravity = true;
        this.catcher.setCollideWorldBounds(true);
        this.catcher.setSize(this.catcher.width- 40, this.catcher.height- 10)

        //Add keyboard cursor keys
        this.cursors = this.input.keyboard.createCursorKeys();
        this.physics.add.overlap(this.grape, this.catcher, this.eatGrapes, null, this);
        this.physics.add.overlap(this.carrot, this.catcher, this.HitCarrot, null, this);
    }

    update() {
        this.newCarrot();
        this.movement();
    }

    HitCarrot(){
        
        this.scene.start(SCENE_KEYS.END_SCENE);
    }

    movement() {
        if (this.catcher.y >= this.scale.height - 70 && (this.cursors.up.isDown || this.cursors.space.isDown)) {
            this.catcher.setVelocityY(-this.speed);
        }
    }

    newCarrot() {
        if (this.carrot.x < 0) {
            this.carrot.setX(this.scale.width);
            const scale = this.scales[Phaser.Math.Between(0, 3)];
            this.carrot.setScale(scale);
            this.carrotsCleared++;
            if (this.carrotsCleared == this.totalCarrots) {
                this.grape.setVelocityX(-400);
                this.carrot.setScale(0);
                this.carrot.setVelocityX(0)
            }
        }
    }

    eatGrapes(){
        this.sound.play(D12_AUDIO_ASSET_KEYS.LALALA);
        this.registry.set('level', this.registry.get('level') + 1);
        this.scene.start(SCENE_KEYS.FLAPPY_SCENE);
    }




}