import { D12_AUDIO_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class EndScene extends Phaser.Scene{
    constructor(){
        super({
            key: SCENE_KEYS.END_SCENE,
        });
    
        console.log(SCENE_KEYS.END_SCENE)
    }

    create(){
        const level = this.registry.get('level');
        this.add.text(this.scale.width/2,this.scale.height/2 - 40, `GAME OVER`, {fontSize:'100px'}).setColor('#000').setOrigin(0.5);
        this.add.text(this.scale.width/2,this.scale.height/2+20, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0.5);
        const clickButton = this.add.text(this.scale.width/2, this.scale.height/2+60, 'Reset', {fontSize:'32px'}).setColor('#0f0').setOrigin(0.5).setInteractive().on('pointerdown', () => this.reset(level) );
        this.sound.play(D12_AUDIO_ASSET_KEYS.BED);
    }

    reset(level) {
        if (level == 0) {
            this.scene.start(SCENE_KEYS.DRAG_GRAPES);
        } else if (level == 1) {
            this.scene.start(SCENE_KEYS.GRAPE_CATCHER_SCENE);
        } else if (level == 2) {
            this.scene.start(SCENE_KEYS.DINOSAURGAME);
        } else if (level == 3) {
            this.scene.start(SCENE_KEYS.FLAPPY_SCENE);
        } else if (level == 4) {
            this.scene.start(SCENE_KEYS.BOUNCE_SCENE);
        } else if (level == 5) {
            this.scene.start(SCENE_KEYS.CARROT_BREAKER_SCENE);
        } else if (level == 6) {
            this.scene.start(SCENE_KEYS.SNAKE_SCENE);
        } else if (level == 7) {
            this.scene.start(SCENE_KEYS.FROGGER_SCENE);
        } else if (level == 8) {
            this.scene.start(SCENE_KEYS.ASTEROID_SCENE);
        } else if (level == 9) {
            this.scene.start(SCENE_KEYS.STACKER);
        }else if (level == 10) {
            this.scene.start(SCENE_KEYS.DODGER);
        } else if (level == 11) {
            this.scene.start(SCENE_KEYS.PARRY);
        }
    }
}