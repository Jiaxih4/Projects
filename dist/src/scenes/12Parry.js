import {
  ATTACK_LEFT_ASSET_KEYS,
  ATTACK_RIGHT_ASSET_KEYS,
  BIG_BOI_ASSET_KEYS,
  D12_AUDIO_ASSET_KEYS,
  FULL_CARROT_ASSET_KEYS,
  FULL_CARROT_VERT_ASSET_KEYS,
  GRAPE_ASSET_KEYS,
  GRAPE_CATCHER_ASSET_KEYS,
  GRAPE_EATER_ASSET_KEYS,
  GRAPE_WARRIOR_ASSET_KEYS,
  IDLE_ASSET_KEYS,
  IDLE_LEFT_ASSET_KEYS,
  JUMP_LEFT_ASSET_KEYS,
  JUMP_RIGHT_ASSET_KEYS,
  RUN_ASSET_KEYS,
  RUN_LEFT_ASSET_KEYS,
  SKY_ASSET_KEYS,
  SONG_ASSET_KEYS,
  TILE_ANI_ASSET_KEYS,
  TILE_ASSET_KEYS,
  WALL_ASSET_KEYS,
} from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { EndScene } from "./end-scene.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class Parry extends Phaser.Scene {
  constructor() {
    super({
      key: SCENE_KEYS.PARRY,
    });
    this.grapeScale = 1;
    this.pressed = false;
    this.total = 8;
    this.passed = 0;
    this.grapeSpeed = 200;
    this.jumps = 1;
    this.jumpheight = 250;
    this.cooldown = false;
    this.direction = 1;
    this.isAttacking = false;
    this.isJumping = false;
  }

  create() {
    //Set level
    const level = this.registry.get("level");
    this.add
      .text(0, 0, `GRAPE EATER:${level}`, { fontSize: "32px" })
      .setColor("#000")
      .setOrigin(0).setDepth(2);
    this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPED, {loop: false, volume:0.3});
    this.sound.play(SONG_ASSET_KEYS.SONG, {loop: true, volume:0.3});

    this.anims.create({
      key: 'idle',
      frames: this.anims.generateFrameNumbers(IDLE_ASSET_KEYS.IDLE, { start: 0, end: 3 }),
      frameRate: 4,
      repeat: -1 // -1 means loop forever
    });
    this.anims.create({
      key: 'idleleft',
      frames: this.anims.generateFrameNumbers(IDLE_LEFT_ASSET_KEYS.IDLE_LEFT, { start: 0, end: 3 }),
      frameRate: 4,
      repeat: -1 // -1 means loop forever
    });
    this.anims.create({
      key: 'run',
      frames: this.anims.generateFrameNumbers(RUN_ASSET_KEYS.RUN, { start: 0, end: 3 }),
      frameRate: 8,
      repeat: -1 // -1 means loop forever
    });
    this.anims.create({
      key: 'runleft',
      frames: this.anims.generateFrameNumbers(RUN_LEFT_ASSET_KEYS.RUN_LEFT, { start: 0, end: 3 }),
      frameRate: 8,
      repeat: -1 // -1 means loop forever
    });

    this.anims.create({
      key: 'attackleft',
      frames: this.anims.generateFrameNumbers(ATTACK_LEFT_ASSET_KEYS.ATTACK_LEFT, { start: 0, end: 3 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'attackright',
      frames: this.anims.generateFrameNumbers(ATTACK_RIGHT_ASSET_KEYS.ATTACK_RIGHT, { start: 0, end: 3 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumpright',
      frames: this.anims.generateFrameNumbers(JUMP_RIGHT_ASSET_KEYS.JUMP_RIGHT, { start: 0, end: 1 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumpleft',
      frames: this.anims.generateFrameNumbers(JUMP_LEFT_ASSET_KEYS.JUMP_LEFT, { start: 0, end: 1 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumprightair',
      frames: this.anims.generateFrameNumbers(JUMP_RIGHT_ASSET_KEYS.JUMP_RIGHT, { start: 1, end: 1 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumpleftair',
      frames: this.anims.generateFrameNumbers(JUMP_LEFT_ASSET_KEYS.JUMP_LEFT, { start: 1, end: 1 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumprightfall',
      frames: this.anims.generateFrameNumbers(JUMP_RIGHT_ASSET_KEYS.JUMP_RIGHT, { start: 2, end: 2 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'jumpleftfall',
      frames: this.anims.generateFrameNumbers(JUMP_LEFT_ASSET_KEYS.JUMP_LEFT, { start: 2, end: 2 }),
      frameRate: 8,
      repeat: 0 // -1 means loop forever
    });

    this.anims.create({
      key: 'tile',
      frames: this.anims.generateFrameNumbers(TILE_ANI_ASSET_KEYS.TILE_ANI, { start: 0, end: 3 }),
      frameRate: 4,
      repeat: -1 // -1 means loop forever
    });

    this.anims.create({
      key: 'sky',
      frames: this.anims.generateFrameNumbers(SKY_ASSET_KEYS.SKY, { start: 0, end: 1 }),
      frameRate: 2,
      repeat: -1 // -1 means loop forever
    });

    this.sky = this.add.sprite(0, 0, SKY_ASSET_KEYS.SKY);
    this.sky.play('sky', true);
    this.sky.setOrigin(0, 0)

    this.platforms = this.physics.add.staticGroup();
    for (let i = 0; i < 40; i++) {

      let tile = this.platforms.create(
        32 * i,
        this.scale.height,
        TILE_ASSET_KEYS.TILE
      );

      tile.play('tile');

    }

    for (let i = 0; i < 10; i++) {

      let tile = this.platforms.create(
        50+32 * i,
        this.scale.height-150,
        TILE_ASSET_KEYS.TILE
      );

      tile.play('tile');

    }

    for (let i = 0; i < 10; i++) {

      let tile = this.platforms.create(
        this.scale.width - 50 + 32 * -i,
        this.scale.height-150,
        TILE_ASSET_KEYS.TILE
      );

      tile.play('tile');

    }

    for (let i = 0; i < 10; i++) {

      let tile = this.platforms.create(
        (this.scale.width/2 + 145)  + 32 * -i,
        this.scale.height-300,
        TILE_ASSET_KEYS.TILE
      );

      tile.play('tile');

    }

    this.physics.world.setBoundsCollision(true, true, true, true);
    const grapeTexture = this.textures.get(GRAPE_WARRIOR_ASSET_KEYS.GRAPE_WARRIOR);
    const grapeSourceImage = grapeTexture.source[0].image;
    this.grapeWidth = grapeSourceImage.width * this.grapeScale;
    this.grapeHeight = grapeSourceImage.height * this.grapeScale;
    console.log(this.grapeWidth);
    console.log(this.grapeHeight);
    this.grape = this.physics.add.sprite(
      50,
      this.scale.height - this.grapeHeight - 20,
      GRAPE_WARRIOR_ASSET_KEYS.GRAPE_WARRIOR,
    );
    this.grape.setScale(this.grapeScale);
    this.grape.setCollideWorldBounds(true);
    this.grape.body.setSize(30, 30);
    this.grape.body.setOffset(5, 5);
    this.grape.setOrigin(0, 1)

    this.swordHitbox = this.add.rectangle(0, 0, 32, this.grape.height, 0xffffff, 0); // Transparent hitbox
    this.physics.add.existing(this.swordHitbox);
    this.swordHitbox.body.setEnable(false); // Disable until attack
    this.swordHitbox.body.allowGravity = false;

    this.carrots =  this.physics.add.group();

    this.startCarrotSpawner(50, 0);

    this.cursors = this.input.keyboard.createCursorKeys();

    this.cursors = this.input.keyboard.createCursorKeys();

    this.eater = this.physics.add.image(this.scale.width - 70, this.scale.height/2, BIG_BOI_ASSET_KEYS.BIG_BOI);
    this.eater.body.allowGravity = false;
    this.eater.setScale(1, 1.2)
    this.eater.setOrigin(0, 0.5)

    this.physics.add.collider(this.grape, this.platforms);
    this.physics.add.overlap(
      this.swordHitbox,
      this.carrots,
      this.deflect,
      null,
      this,
    );

    this.physics.add.overlap(
      this.grape,
      this.carrots,
      () => {this.sound.stopAll();this.scene.start(SCENE_KEYS.END_SCENE)},
      null,
      this,
    );

    this.physics.add.overlap(this.eater, this.carrots, this.destroyMarker, null, this)
  }

  update() {
    this.move();
    this.attack();
    if (this.isAttacking) {
      if (this.direction == 0) {
        this.swordHitbox.setPosition(this.grape.x, this.grape.y -20);
      } else {
        this.swordHitbox.setPosition(this.grape.x + 30, this.grape.y -20);
      }
    }
    if (this.passed == this.total) {
      this.sound.get(SONG_ASSET_KEYS.SONG).stop();
      this.scene.start(SCENE_KEYS.WIN)
    }
  }

  move() {
    if (this.grape.body.touching.down) {
      this.jumps = 1
      this.isJumping = false;
    }
    if (!this.grape.body.touching.down && this.grape.body.velocity.y < 0 && this.isAttacking == false) {
      if (this.direction == 1) {
        this.grape.play('jumprightair', true)
      } else {
        this.grape.play('jumpleftair', true)
      }
    } else if (!this.grape.body.touching.down && this.grape.body.velocity.y > 0 && this.isAttacking == false  ) {
      if (this.direction == 1) {
        this.grape.play('jumprightfall', true)
      } else {
        this.grape.play('jumpleftfall', true)
      }
    }
    if (this.cursors.left.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A).isDown) {
      this.direction = 0;
      if (this.isAttacking == false && this.isJumping == false) {
        this.grape.play('runleft', true);
      }
      this.grape.setVelocityX(-160);
    } else if (this.cursors.right.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D).isDown) {
      this.direction = 1;
      if (this.isAttacking == false && this.isJumping == false) {
        this.grape.play('run', true);
      }
      this.grape.setVelocityX(160);
    } else {
      this.grape.setVelocityX(0);
      if (this.direction == 1) {
        if (this.isAttacking == false&& this.isJumping == false) {
          this.grape.play('idle', true)
        }
      } else {
        if (this.isAttacking == false&& this.isJumping == false) {
          this.grape.play('idleleft', true);
        }
      }
    }

    if ((this.cursors.up.isDown || this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W).isDown) && this.grape.body.touching.down) {
      this.grape.setVelocityY(-this.jumpheight);
      this.isJumping = true;
      if (this.direction == 0) {
        this.grape.play('jumpleft', true)
      } else {
        this.grape.play('jumpright', true)
      }
    }
    if (
      (Phaser.Input.Keyboard.JustDown(this.cursors.up)||  Phaser.Input.Keyboard.JustDown(this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.W))) &&
      this.jumps > 0 &&
      !this.grape.body.touching.down
    ) {
      this.grape.setVelocityY(-this.jumpheight);
      this.jumps--;
      this.isJumping = true;
      if (this.direction == 0) {
        this.grape.play('jumpleft', true)
      } else {
        this.grape.play('jumpright', true)
      }
    }
  }

  attack() {
    if (Phaser.Input.Keyboard.JustDown(this.cursors.space) && !this.cooldown) {
      if (this.direction == 0) {
        this.isAttacking = true;
        this.grape.play('attackleft', true);
        this.grape.body.setOffset(20, 5);
        this.grape.setOrigin(0.4, 1);

        this.time.delayedCall(250,() => {
          this.swordHitbox.body.setEnable(true);
          this.swordHitbox.setPosition(this.grape.x, this.grape.y -20);
          this.cooldown = true;
        });

        this.time.delayedCall(500, () => {
          this.swordHitbox.body.setEnable(false);
          this.cooldown = false;
          this.isAttacking = false;
          this.grape.body.setOffset(5, 5);
          this.grape.setOrigin(0, 1)
        });
      } else {
        this.isAttacking = true;
        this.grape.play('attackright', true);
        this.time.delayedCall(250,() => {
          this.swordHitbox.body.setEnable(true);
          this.swordHitbox.setPosition(this.grape.x + 30, this.grape.y -20);
          this.cooldown = true;
        });

        this.time.delayedCall(500, () => {
          this.swordHitbox.setPosition(this.grape.x + 30, this.grape.y -20);
          this.swordHitbox.body.setEnable(false);
          
          this.cooldown = false;
          this.isAttacking = false;
        });
      }
    }
  }

  deflect(sword, grape) {
    grape.setVelocityX(this.grapeSpeed);
  }

  spawnCarrot(spawnheight) {
    const carrot = this.carrots.create(
       this.eater.x - 30,
        spawnheight,
        FULL_CARROT_ASSET_KEYS.FULL_CARROT,
      );

    carrot.setScale(0.2);
    carrot.body.allowGravity = false;
    carrot.setVelocityX(-this.grapeSpeed);
  }
  startCarrotSpawner(y, delay) {
      this.time.addEvent({
        delay: delay,
        callback: () => {
          this.spawnCarrot(y);
  
          // Schedule the next spawn
          this.startCarrotSpawner(
            y = Phaser.Math.Between(100, this.scale.height - this.grape.height),
            1000
          );
        },
        callbackScope: this,
      });
    }

  destroyMarker(marker, target){
    target.destroy();
    this.passed +=1;
    if (this.passed == 2) {
      this.black = this.sound.play(D12_AUDIO_ASSET_KEYS.BLACK, {loop: false, volume: 1});
    } else if (this.passed == 4) {
      this.sound.play(D12_AUDIO_ASSET_KEYS.WTH, {loop: false, volume: 1});
    } else if (this.passed == 6) {
      this.sound.play(D12_AUDIO_ASSET_KEYS.COCK, {loop: false, volume: 1});
    } else if (this.passed == 8) {
      this.sound.play(D12_AUDIO_ASSET_KEYS.OHH, {loop: false, volume: 1});
    }
    console.log(this.passed);
  }
  
}
