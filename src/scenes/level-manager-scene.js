import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class LevelManagerScene extends Phaser.Scene {
    constructor() {
        super({
            key: SCENE_KEYS.LEVEL_MANAGER
        });
    }

    create() {
        this.registry.set('level', 0);
        console.log("Start Level Manager");
    }
}