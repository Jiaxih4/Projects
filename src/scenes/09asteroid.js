import {
  D12_ASSET_KEYS,
  D12_AUDIO_ASSET_KEYS,
  FULL_CARROT_ASSET_KEYS,
  GRAPE_ASSET_KEYS,
  GRAPE_CATCHER_ASSET_KEYS,
} from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { EndScene } from "./end-scene.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Asteroid extends Phaser.Scene {
  constructor() {
    super({
      key: SCENE_KEYS.ASTEROID_SCENE,
    });
    this.rotationspeed = 1;
    this.angle = 0;
    this.grapeSpeed = 400;
    
    this.dzhongSpeed =40;

    this.cooldown = 50;
    this.timer = 60;

    this.grapesGenerated = 0;
    this.totalGrapes = 4;
  }

  create() {
    //Set level
    const level = this.registry.get("level");
    this.add
      .text(0, 0, `GRAPE EATER:${level}`, { fontSize: "32px" })
      .setColor("#000")
      .setOrigin(0).setDepth(2);

    //Set Origin
    this.originx = this.scale.width / 2;
    this.originy = this.scale.height / 2;

    //Add Carrot
    this.carrot = this.physics.add.image(
      this.scale.width / 2,
      this.scale.height / 2,
      FULL_CARROT_ASSET_KEYS.FULL_CARROT,
    );
    this.carrot.body.allowGravity = false;
    this.carrot.setScale(0.25);
    this.carrot.setOrigin(0.5, 0.5);

    //Add Grapes
    this.grapes = this.physics.add.group();

    //Add Dzhongs
    this.dzhongs = this.physics.add.group();
    this.startDzhongSpawner(this.totalGrapes-1, 2000);

    //Physics and Controls
    this.physics.add.collider(this.grapes, this.dzhongs, this.eatGrape, null, this)
    this.physics.add.overlap(this.carrot, this.dzhongs, () => {this.scene.start(SCENE_KEYS.END_SCENE)});
    this.cursors = this.input.keyboard.createCursorKeys();
  }

  update() {
    this.rotate();
    this.spawnGrape(2);
  }

  //Rotate Carrot
  rotate() {
    if (this.cursors.left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
      this.angle -= (this.rotationspeed * Math.PI) / 180;
    } else if (this.cursors.right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
      this.angle += (this.rotationspeed * Math.PI) / 180;
    }
    this.angle = Phaser.Math.Angle.Wrap(this.angle);
    this.carrot.setRotation(this.angle);
  }

  //Spawn Grapes
  spawnGrape(grapeScale) {
    if (this.cooldown > this.timer) {
      this.timer += 1;
      return;
    }
    let xmul = 0;
    let ymul = 0;
    if (this.angle > 0) {
      ymul = 1;
    } else {
      ymul = -1;
    }
    if (this.angle < Math.PI / 2 && this.angle > -Math.PI / 2) {
      xmul = 1;
    } else {
      xmul = -1;
    }
    if (this.cursors.space.isDown) {
      const grape = this.grapes.create(
        this.originx + Math.cos(this.angle) * 30,
        this.originy + Math.sin(this.angle) * 30,
        GRAPE_ASSET_KEYS.GRAPE,
      );
      grape
        .setScale(grapeScale)
        .setOrigin(0.5, 0.5)
        .setVelocityX(
          Math.cos(this.angle) * Math.cos(this.angle) * this.grapeSpeed * xmul,
        )
        .setVelocityY(
          Math.sin(this.angle) * Math.sin(this.angle) * this.grapeSpeed * ymul,
        );
      grape.body.allowGravity = false;

      grape.body.setSize(
        grape.displayWidth / grapeScale,
        grape.displayHeight / grapeScale,
      );
      this.timer = 0;
    }
  }

  //SpawnDzhongs
  spawnDzhong(dScale, angle, radius) {
    //const angle = Phaser.Math.FloatBetween(0, Math.PI * 2);
    let angleR = angle * Math.PI / 180;

    const spawnX = this.originx + Math.cos(angleR) * radius;
    const spawnY = this.originy + Math.sin(angleR) * radius;

    const dzhong = this.dzhongs.create(
      spawnX,
      spawnY,
      D12_ASSET_KEYS.GRAPER,
    );

    dzhong.setScale(dScale * 4);
    dzhong.setOrigin(0.5, 0.5);
    dzhong.body.allowGravity = false;
    dzhong.setData('size', dScale);
    dzhong.setData('angle', angle);

    const dx = this.originx - spawnX;
    const dy = this.originy - spawnY;

    const length = Math.sqrt(dx * dx + dy * dy);

    const velocityX = (dx / length) * this.dzhongSpeed;
    const velocityY = (dy / length) * this.dzhongSpeed;

    dzhong.setVelocity(velocityX, velocityY);

    dzhong.body.setSize(
      dzhong.displayWidth / (dScale*4),
      dzhong.displayHeight / (dScale*4),
    );
  }

  //Destroy Dzhongs and split
  eatGrape(grape, dzhong) {
    let dScale = dzhong.getData('size');
    let angle = dzhong.getData('angle');
    let dzx = dzhong.x - this.originx;
    let dzy = dzhong.y - this.originy;
    let radius = Math.sqrt(dzx * dzx + dzy * dzy);
    dzhong.destroy();
    grape.destroy();
    if (dScale > 0.25) {
        this.spawnDzhong(dScale/2, angle+5, radius);
        this.spawnDzhong(dScale/2, angle-5, radius);

    }
    console.log(this.dzhongs.countActive());
    if (this.dzhongs.countActive() == 0) {
      //Next Scene
      this.sound.play(D12_AUDIO_ASSET_KEYS.LAUGH);
      this.registry.set('level', this.registry.get('level') + 1);
      this.scene.start(SCENE_KEYS.STACKER);
    }

  }

  startDzhongSpawner(times, delay) {
    this.time.addEvent({
          delay: delay,
          callback: () => {
            let randAngle = Phaser.Math.Between(0, 360);
            this.spawnDzhong(1, randAngle,(this.originx + 100));
            this.grapesGenerated++;
          },
          callbackScope: this,
          repeat: times,
        });
  }
}
