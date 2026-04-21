import {
  CARROT_ASSET_KEYS,
  D12_ASSET_KEYS,
  D12_AUDIO_ASSET_KEYS,
  FULL_CARROT_ASSET_KEYS,
  FULL_CARROT_VERT_ASSET_KEYS,
  GRAPE_ASSET_KEYS,
  GRAPE_CATCHER_ASSET_KEYS,
  LOG_ASSET_KEYS,
  ROAD_ASSET_KEYS,
  WALL_ASSET_KEYS,
  WATER_ASSET_KEYS,
} from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { EndScene } from "./end-scene.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Frogger extends Phaser.Scene {
  constructor() {
    super({
      key: SCENE_KEYS.FROGGER_SCENE,
    });
    this.speed = 150;
    this.catcherScale = 1;
    this.moved = false;
    this.up = 1;
    this.down = -1;
    this.onLog = false;
  }

  create() {
    //Set Level
    const level = this.registry.get("level");
    this.add
      .text(0, 0, `GRAPE EATER:${level}`, { fontSize: "32px" })
      .setColor("#000")
      .setOrigin(0).setDepth(2);

    this.physics.world.setBoundsCollision(true, true, false, false);

    //Add Road
    this.road = this.add
      .image(64, 0, ROAD_ASSET_KEYS.ROAD)
      .setScale(3, 10)
      .setOrigin(0, 0);

    this.road = this.add
      .image(640, 0, ROAD_ASSET_KEYS.ROAD)
      .setScale(4, 10)
      .setOrigin(0, 0);

    //Add Water
    this.water = this.physics.add
      .image(320, 0, WATER_ASSET_KEYS.WATER)
      .setScale(4, 10)
      .setOrigin(0, 0).setImmovable(true);
    this.water.body.allowGravity =false

    //Load Wall
    const wallTexture = this.textures.get(WALL_ASSET_KEYS.WALL);
    const wallSourceImage = wallTexture.source[0].image;
    const wallWidth = wallSourceImage.width;
    const wallHeight = wallSourceImage.height;
    const wallScale = 1;
    const numRow = Phaser.Math.CeilTo(this.scale.height / wallHeight);
    const gap = Phaser.Math.RND.between(1, numRow - 2);

    //Add Wall
    this.walls = this.physics.add.staticGroup();

    for (let row = 0; row < numRow; row++) {
      if (row == gap) {
        continue;
      }
      const wall = this.walls.create(
        256 + 0 * wallWidth * wallScale,
        0 + row * wallHeight * wallScale,
        WALL_ASSET_KEYS.WALL,
      );

      wall.setScale(wallScale).setOrigin(0, 0);

      wall.body.setSize(wall.displayWidth, wall.displayHeight);
    }

    const first = Phaser.Math.Between(1, numRow - 2);
    let second = Phaser.Math.Between(1, numRow - 2);
    while (second === first) {
      second = Phaser.Math.Between(0, numRow - 1);
    }

    for (let row = 0; row < numRow; row++) {
      if (row == first || row == second) {
        continue;
      }
      const wall = this.walls.create(
        576 + 0 * wallWidth * wallScale,
        0 + row * wallHeight * wallScale,
        WALL_ASSET_KEYS.WALL,
      );

      wall.setScale(wallScale).setOrigin(0, 0);

      wall.body.setSize(wall.displayWidth, wall.displayHeight);
    }

    this.walls.refresh();

    //Load Carrot
    const carrotTexture = this.textures.get(
      FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,
    );
    const carrotSourceImage = wallTexture.source[0].image;
    this.carrotWidth = wallSourceImage.width;
    this.carrotHeight = wallSourceImage.height;

    //Add Carrot
    //startCarrotSpawner(direction, x, y, delay)
    this.carrots = this.physics.add.group();
    this.startCarrotSpawner(this.up, 64, 0 - this.carrotHeight * 2, 0);
    this.startCarrotSpawner(this.down, 160, this.scale.height, 0);

    this.makeCarrot(0.5, this.up, 64, 100);
    this.makeCarrot(0.5, this.up, 64, 300);

    this.makeCarrot(0.5, this.down, 160, 100);
    this.makeCarrot(0.5, this.down, 160, 300);

    this.startCarrotSpawner(this.up, 640, 0 - this.carrotHeight * 2, 0);
    this.startCarrotSpawner(this.down, 736, this.scale.height, 0);
    this.startCarrotSpawner(this.up, 812, 0 - this.carrotHeight * 2, 0);

    //Add Log
    //startLogSpawner(direction, x, y, delay, speed, scale)
    this.logs = this.physics.add.group();
    this.startLogSpawner(this.up, 320, 0, 0, 1, 3);
    this.startLogSpawner(this.down, 384, this.scale.height + 128, 0, 2, 2);
    this.startLogSpawner(this.up, 448, 0, 0, 3, 1);
    this.startLogSpawner(this.down, 512, this.scale.height + 128, 0, 4, 2); 


    //Add Grape
    this.grape = this.physics.add.image(this.scale.width, this.scale.height/2, GRAPE_ASSET_KEYS.GRAPE);
    this.grape.body.allowGravity = false;
    this.grape.setScale(8);
    this.grape.setOrigin(1, 0.5)

    //Add CATCHER
    this.catcher = this.physics.add
      .image(0, this.scale.height/2, D12_ASSET_KEYS.GRAPEPERSON)
      .setOrigin(0, 0.5);
    this.catcher.setScale(this.catcherScale);
    this.catcher.body.allowGravity = false;
    this.catcher.setCollideWorldBounds(true);
    this.catcher.setDepth(1);
    this.catcher.setSize(this.catcher.width- 25, this.catcher.height- 10);

    this.cursors = this.input.keyboard.createCursorKeys();
    this.physics.add.collider(this.catcher, this.walls);
    this.physics.add.overlap(this.catcher, this.carrots, this.end, () => {return !this.onLog}, this)
    this.physics.add.overlap(this.catcher, this.grape, this.pass, null, this);

  }

  update() {
    this.move();
    if (this.catcher.y < 0 || this.catcher.y > this.scale.height) {
      this.end();
    }



  }

  move() {
    let logVelocityY = 0;
    this.logs.children.each((log) => {
        if (this.physics.overlap(this.catcher, log)) {
            logVelocityY = log.body.velocity.y;
            this.onLog = true;
            return false;
        }
        return true;
    });
    if (this.physics.overlap(this.catcher, this.water) && !this.onLog) {
      this.end();
    }
    if (this.cursors.right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
      this.catcher.setVelocityX(this.speed);
    } else if (this.cursors.left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
      this.catcher.setVelocityX(-this.speed);
    } else {
      this.catcher.setVelocityX(0);
    }
    if (this.cursors.up.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W).isDown) {
      this.catcher.setVelocityY(-this.speed+ logVelocityY);
    } else if (this.cursors.down.isDown|| this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.S).isDown) {
      this.catcher.setVelocityY(this.speed+ logVelocityY);
    } else {
      this.catcher.setVelocityY(0+ logVelocityY);
    }
    this.onLog = false;
  }

  makeCarrot(carrotScale, direction, x, y) {
    const carrot = this.carrots.create(
      x + 32,
      y,
      FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,
    );
    carrot
      .setScale(carrotScale)
      .setOrigin(0, 0)
      .setVelocityY(50 * direction);
    carrot.body.allowGravity = false;
    if (direction == this.down) {
      carrot.setFlipY(true);
    }

    carrot.body.setSize(
      carrot.displayWidth / carrotScale * 0.8,
      carrot.displayHeight / carrotScale *0.9,
    );
  }

  startCarrotSpawner(direction, x, y, delay) {
    this.time.addEvent({
      delay: delay,
      callback: () => {
        this.makeCarrot(0.5, direction, x, y);

        // Schedule the next spawn
        this.startCarrotSpawner(
          direction,
          x,
          y,
          (delay = Phaser.Math.Between(4000, 6000)),
        );
      },
      callbackScope: this,
    });
  }

  makeLog(LogScale, direction, x, y, speed) {
    const log = this.logs.create(x, y, LOG_ASSET_KEYS.LOG);
    log
      .setScale(1, LogScale)
      .setOrigin(0, 1)
      .setVelocityY(50 * direction * speed);
    log.body.allowGravity = false;

    log.body.setSize(log.displayWidth, log.displayHeight / LogScale);
  }

  startLogSpawner(direction, x, y, delay, speed, scale) {
    this.time.addEvent({
      delay: delay,
      callback: () => {
        this.makeLog(scale, direction, x, y, speed);

        // Schedule the next spawn
        this.startLogSpawner(
          direction,
          x,
          y,
          (delay = Phaser.Math.Between(5000 / speed, 7000 / speed)),
          speed,
          scale,
        );
      },
      callbackScope: this,
    });
  }

  end() {
    this.scene.start(SCENE_KEYS.END_SCENE);
  }
  
  pass() {
    this.sound.play(D12_AUDIO_ASSET_KEYS.SQUEEZE);
    this.registry.set('level', this.registry.get('level') + 1);
    this.scene.start(SCENE_KEYS.ASTEROID_SCENE);
  }



}
