import Phaser from "../lib/phaser.js";
export class SnakeFoodCharacter extends Phaser.GameObjects.Image {
    constructor(scene, x, y, image, size, scale){
        super(scene, x * size * scale, y * size * scale, image);
        this.setScale(scale)
        this.setOrigin(0.5,0.5)

        this.scale = scale;
        this.size = size;

        scene.children.add(this);
        this.total = 0;
    }

    eat() {
        this.total++;
    }
}