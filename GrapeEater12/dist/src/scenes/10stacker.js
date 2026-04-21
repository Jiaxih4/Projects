import {
  D12_ASSET_KEYS,
  D12_AUDIO_ASSET_KEYS,
  FULL_CARROT_ASSET_KEYS,
  GRAPE_ASSET_KEYS,
  GRAPE_CATCHER_ASSET_KEYS,
  GRAPE_EATER_ASSET_KEYS,
  WALL_ASSET_KEYS,
} from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { EndScene } from "./end-scene.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Stacker extends Phaser.Scene {
  constructor() {
    super({
      key: SCENE_KEYS.STACKER,
    });
    this.grapeScale = 5;
    this.direction = 0;
    this.right = 0;
    this.left = 1;
    this.progress = 2
    this.dropped = false;
    this.delay = 1000;

    this.grapeRemaining =3;
  }

  create() {
    //Set level
    const level = this.registry.get("level");
    this.add
      .text(0, 0, `GRAPE EATER:${level}`, { fontSize: "32px" })
      .setColor("#000")
      .setOrigin(0).setDepth(2);
    this.physics.world.setBoundsCollision(true, true, true, true);

    const grapeTexture = this.textures.get(GRAPE_ASSET_KEYS.GRAPE);
    const grapeSourceImage = grapeTexture.source[0].image;
    this.grapeWidth = grapeSourceImage.width * this.grapeScale;
    this.grapeHeight = grapeSourceImage.height * this.grapeScale;

    this.bound = 2.5 * this.grapeWidth;

    this.rightBound = this.add.image(this.scale.width/2 + this.bound + this.grapeWidth, 0, WALL_ASSET_KEYS.WALL);
    this.rightBound.setScale(0.1, 20);
    this.rightBound.setOrigin(0);

    this.leftBound = this.add.image(this.scale.width/2 - this.bound - this.grapeWidth, 0, WALL_ASSET_KEYS.WALL);
    this.leftBound.setScale(0.1, 20);
    this.leftBound.setOrigin(1,0);

    this.x1 = this.scale.width/2-this.grapeWidth;
    this.x2 = this.scale.width/2;
    this.x3 = this.scale.width/2+this.grapeWidth

    //Add Bottom Row
    this.grape1 = this.physics.add.image(this.x1, this.scale.height - this.grapeHeight, GRAPE_ASSET_KEYS.GRAPE);
    this.grape1.setCollideWorldBounds(true).setBounce(0);
    this.grape1.setScale(this.grapeScale).setOrigin(0.5,0);
    this.grape1.body.allowGravity = false;

    this.grape2 = this.physics.add.image(this.x2, this.scale.height - this.grapeHeight, GRAPE_ASSET_KEYS.GRAPE);
    this.grape2.setCollideWorldBounds(true).setBounce(0);
    this.grape2.setScale(this.grapeScale).setOrigin(0.5,0);
    this.grape2.body.allowGravity = false;

    this.grape3 = this.physics.add.image(this.x3, this.scale.height - this.grapeHeight, GRAPE_ASSET_KEYS.GRAPE);
    this.grape3.setCollideWorldBounds(true).setBounce(0);
    this.grape3.setScale(this.grapeScale).setOrigin(0.5,0);
    this.grape3.body.allowGravity = false;

    this.makeRow(this.grapeScale, this.progress);
    this.startmove(this.grapeScale);
    this.cursors = this.input.keyboard.createCursorKeys();

    this.eater = this.add.image(this.scale.width/2, 0, D12_ASSET_KEYS.GRAPEOPEN);
    this.eater.setOrigin(0.5, 0)
    this.eater.setScale(4);
  }

  update() {
    this.drop();
    if (this.grapeRemaining == 0) {
      this.scene.start(SCENE_KEYS.END_SCENE);
    } else if (this.progress == 11) {
      this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPE);
      this.registry.set('level', this.registry.get('level') + 1);
      this.scene.start(SCENE_KEYS.DODGER);
    }
  }

  makeRow(scale, level) {
    this.direction = this.right;

    if (this.grapeRemaining > 0) {
      this.grape1 = this.physics.add.image(this.scale.width/2-this.grapeWidth - 2 * this.grapeWidth, this.scale.height - this.grapeHeight * level, GRAPE_ASSET_KEYS.GRAPE);
      this.grape1.setCollideWorldBounds(true).setBounce(0);
      this.grape1.setScale(scale).setOrigin(0.5,0);
      this.grape1.body.allowGravity = false;
    } else {
      this.grape1 = null;
    }

    if (this.grapeRemaining > 1) {
      this.grape2 = this.physics.add.image(this.scale.width/2 - 2 * this.grapeWidth, this.scale.height - this.grapeHeight * level, GRAPE_ASSET_KEYS.GRAPE);
      this.grape2.setCollideWorldBounds(true).setBounce(0);
      this.grape2.setScale(scale).setOrigin(0.5,0);
      this.grape2.body.allowGravity = false;
    } else {
      this.grape2 = null;
    }

    if (this.grapeRemaining > 2) {
      this.grape3 = this.physics.add.image(this.scale.width/2+this.grapeWidth - 2 * this.grapeWidth, this.scale.height - this.grapeHeight * level, GRAPE_ASSET_KEYS.GRAPE);
      this.grape3.setCollideWorldBounds(true).setBounce(0);
      this.grape3.setScale(scale).setOrigin(0.5,0);
      this.grape3.body.allowGravity = false;
    } else {
      this.grape3 = null;
    }
  }

  move(scale) {
    if (this.direction == this.right) {
      if (this.grape3) {
        this.grape3.x += this.grape3.width * scale;
        if (this.grape3.x >= this.scale.width/2 + this.bound) {
          this.direction = this.left;
        }
      }
      if (this.grape2) {
        this.grape2.x += this.grape2.width * scale;
        if (this.grape2.x >= this.scale.width/2 +this.bound) {
          this.direction = this.left;
        }
      }
      if (this.grape1) {
        this.grape1.x += this.grape1.width * scale;
        if (this.grape1.x >= this.scale.width/2 +this.bound) {
          this.direction = this.left;
        }
      }
    } else {
      if (this.grape3) {
        this.grape3.x -= this.grape3.width * scale;
        if (this.grape3.x <= this.scale.width/2 -this.bound) {
          this.direction = this.right;
        }
      }
      if (this.grape2) {
        this.grape2.x -= this.grape2.width * scale;
        if (this.grape2.x <= this.scale.width/2 -this.bound) {
          this.direction = this.right;
        }
      }
      if (this.grape1) {
        this.grape1.x -= this.grape1.width * scale;
        if (this.grape1.x <= this.scale.width/2 -this.bound) {
          this.direction = this.right;
        }
      }
    }
    
  }

  startmove(scale) {
    this.timer = this.time.addEvent({
              delay: this.delay,
              callback: () => {
                this.move(scale)
                this.startmove(scale);
              },
              callbackScope: this,
            });
  }

  drop() {
    if ((this.cursors.space.isDown || this.cursors.down.isDown) && this.dropped == false) {
      let grape1x = -2;
      if (this.grape1) {
        grape1x = this.grape1.x;
      }
      let grape2x = -2;
      if (this.grape2) {
        grape2x = this.grape2.x;
      }
      let grape3x = -2;
      if (this.grape3) {
        grape3x = this.grape3.x;
      }
      if ((this.grape1)&&(grape1x != this.x1 && grape1x != this.x2 && grape1x != this.x3)) {
        this.grapeRemaining-=1;
        this.grape1.visible = false;
        this.grape1 = null;
      }
      if ((this.grape2)&&(grape2x != this.x1 && grape2x != this.x2 && grape2x != this.x3)) {
        this.grapeRemaining-=1;
        this.grape2.visible = false;
        this.grape2 = null;
      }
      if ((this.grape3)&&(grape3x != this.x1 && grape3x != this.x2 && grape3x != this.x3)) {
        this.grapeRemaining-=1;
        this.grape3.visible = false;
        this.grape3 = null;
      }
      if (this.x1 != grape1x && this.x1 != grape2x && this.x1 != grape3x) {
        this.x1 = -1;
      }
      if (this.x2 != grape1x && this.x2 != grape2x && this.x2 != grape3x) {
        this.x2 = -1;
      }
      if (this.x3 != grape1x && this.x3 != grape2x && this.x3 != grape3x) {
        this.x3 = -1;
      }

      this.makeRow(this.grapeScale, this.progress+=1);
      this.dropped = true;
      this.delay = 1000 / (3/4 * this.progress);
    } else if ((this.cursors.space.isUp && this.cursors.down.isUp)) {
      this.dropped = false;
    }
  }


}
