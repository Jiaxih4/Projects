import { D12_AUDIO_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class WinScene extends Phaser.Scene{
    constructor(){
        super({
            key: SCENE_KEYS.WIN,
        });
    }

    create(){
        const level = this.registry.get('level');
        this.add.text(this.scale.width/2,this.scale.height/2 - 40, `THAT'S GRAPE`, {fontSize:'100px'}).setColor('#000').setOrigin(0.5);
        this.add.text(this.scale.width/2,this.scale.height/2+20, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0.5);
        this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPE);
    }
}