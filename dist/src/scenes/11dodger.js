import {
  D12_ASSET_KEYS,
  D12_AUDIO_ASSET_KEYS,
  FULL_CARROT_ASSET_KEYS,
  FULL_CARROT_VERT_ASSET_KEYS,
  GRAPE_ASSET_KEYS,
  GRAPE_CATCHER_ASSET_KEYS,
  GRAPE_EATER_ASSET_KEYS,
  WALL_ASSET_KEYS,
} from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { EndScene } from "./end-scene.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Dodger extends Phaser.Scene {
  constructor() {
    super({
      key: SCENE_KEYS.DODGER,
    });
    this.grapeScale = 5;
    this.pressed = false;
    this.total = 4;
    this.passed = 0;
  }
   
  create() {
    //Set level
    const level = this.registry.get("level");
    this.add
      .text(0, 0, `GRAPE EATER:${level}`, { fontSize: "32px" })
      .setColor("#000")
      .setOrigin(0).setDepth(2);
    //Load Grape
    const grapeTexture = this.textures.get(GRAPE_ASSET_KEYS.GRAPE);
    const grapeSourceImage = grapeTexture.source[0].image;
    this.grapeWidth = grapeSourceImage.width * this.grapeScale;
    this.grapeHeight = grapeSourceImage.height * this.grapeScale;

    //Load Carrot
    const carrotTexture = this.textures.get(FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT);
    const carrotSourceImage = carrotTexture.source[0].image;
    this.carrotWidth = carrotSourceImage.width;
    this.carrotHeight = carrotSourceImage.height;
    this.carrotScale = this.grapeWidth / this.carrotWidth;


    //Set Bound
    this.bound = 2.5 * this.grapeWidth;

    //Add Right Bound
    this.rightBound = this.add.image(this.scale.width/2 + this.bound + this.grapeWidth, 0, WALL_ASSET_KEYS.WALL);
    this.rightBound.setScale(0.1, 20);
    this.rightBound.setOrigin(0);

    //Add Left Bound
    this.leftBound = this.add.image(this.scale.width/2 - this.bound - this.grapeWidth, 0, WALL_ASSET_KEYS.WALL);
    this.leftBound.setScale(0.1, 20);
    this.leftBound.setOrigin(1,0);

    //Add Grape
    this.grape = this.physics.add.image(this.scale.width/2, this.scale.height - this.grapeHeight, GRAPE_ASSET_KEYS.GRAPE);
    this.grape.setCollideWorldBounds(true).setBounce(0);
    this.grape.setScale(this.grapeScale).setOrigin(0.5,0);
    this.grape.body.allowGravity = false;

    //Add Carrot

    //Add Target
    this.target = this.physics.add.image((this.scale.width/2 - this.bound - (this.carrotWidth * this.carrotScale /2)) + 7 * this.carrotWidth * this.carrotScale,
        this.scale.height,
        FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,);
    this.target.setScale(this.carrotScale, 0.2).setOrigin(0.5,0);
    this.target.body.allowGravity = false;
    this.target.setVisible(false)

    //Add Eater
    this.eater = this.physics.add.image(this.scale.width/2, 0, D12_ASSET_KEYS.GRAPEOPEN);
    this.eater.setOrigin(0.5, 0)
    this.eater.body.allowGravity = false;
    this.eater.setScale(4)

    this.carrots = this.physics.add.group();
    this.markers = this.physics.add.group();
    this.makeObstacle();
    this.startObstacle(2000);

    this.cursors = this.input.keyboard.createCursorKeys();
    this.physics.add.overlap(this.grape, this.carrots, () =>{this.scene.start(SCENE_KEYS.END_SCENE)}, null, this)
    this.physics.add.overlap(this.grape, this.eater, () =>{this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPEX2);this.registry.set('level', this.registry.get('level') + 1);this.scene.start(SCENE_KEYS.PARRY)}, null, this)//Next Scene
    this.physics.add.overlap(this.target, this.markers, this.destroyMarker, null, this)

    
  }

  update() {
    this.move();
    if (this.passed/6 == this.total) {
        this.eater.setVelocityY(200);
    }
  }

  move() {
    if ((this.cursors.right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) && this.pressed == false && this.grape.x < this.scale.width/2 + this.bound) {
        this.grape.x += this.grapeWidth;
        this.pressed = true;
    } else if ((this.cursors.left.isDown||this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) && this.pressed == false && this.grape.x > this.scale.width/2 - this.bound){
        this.grape.x -= this.grapeWidth;
        this.pressed = true;
    } else if (this.cursors.left.isUp && this.cursors.right.isUp && this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isUp&& this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isUp) {
        this.pressed = false;
    }
  }

  makeObstacle() {
    let gap = Phaser.Math.Between(0, 6);
    for (let row = 0; row < 7; row++) {
      if (row == gap) {
        continue;
      }
      const carrot = this.carrots.create(
        (this.scale.width/2 - this.bound - (this.carrotWidth * this.carrotScale /2)) + row * this.carrotWidth * this.carrotScale,
        0 + 0 * this.carrotHeight * 0.2,
        FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,
      );
      const marker = this.markers.create((this.scale.width/2 - this.bound - (this.carrotWidth * this.carrotScale /2)) + 7 * this.carrotWidth * this.carrotScale,
        0 + 0 * this.carrotHeight * 0.2,
        FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,);

      carrot.setScale(this.carrotScale, 0.2).setOrigin(0.5,0);
      carrot.body.allowGravity = false;
      carrot.setVelocityY(200);

      marker.setScale(this.carrotScale, 0.2).setOrigin(0.5,0);
      marker.body.allowGravity = false;
      marker.setVelocityY(200);
      marker.setVisible(false)

      //carrot.body.setSize(carrot.displayWidth, carrot.displayHeight);
    }
  }

  destroyMarker(marker, target){
    target.destroy();
    this.passed +=1;
    console.log(this.passed);
  }

  startObstacle(delay) {
    this.timer = this.time.addEvent({
              delay: delay,
              callback: () => {
                this.makeObstacle();
              },
              callbackScope: this,
              repeat: this.total - 2,
            });
  }
}