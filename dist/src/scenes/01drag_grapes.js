import { BATTLE_BACKGROUND_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_EATER_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class DragGrapesScene extends Phaser.Scene{
    constructor() {
        super({
            key: SCENE_KEYS.DRAG_GRAPES
        })
        this.dzhong
        this.grapesSpawn = 4
        this.grapesEaten = 0
    }

    create() {
        //add main background
        //this.add.image(0,0, BATTLE_BACKGROUND_ASSET_KEYS.FOREST).setOrigin(0);

        const level = this.registry.get('level');
        this.add.text(0,0, `GRAPE EATER:${level}`, {fontSize:'32px'}).setColor('#000').setOrigin(0);

        //add dzhong
        this.dzhong = this.physics.add.image(this.scale.width/2, this.scale.height/4, D12_ASSET_KEYS.GRAPEOPEN);
        this.dzhong.setImmovable(true);
        this.dzhong.body.allowGravity = false;
        this.dzhong.setScale(5);
        this.dzhong.setSize(this.dzhong.width/3, this.dzhong.height/3).setOffset(this.dzhong.width/3, this.dzhong.height - this.dzhong.height/2 )

        //Grape Collectables
        this.collectibles = this.physics.add.group();
        this.spawnOrder = [];
        this.nextSpawnId = 0;
        this.currentCollectIndex = 0;
        this.spawnCollectibles(this.grapesSpawn);

        //Add collision function

        this.physics.add.overlap(this.dzhong, this.collectibles, this.eatGrapes, null, this);


    }

    //Spawn Grapes at random position
    spawnCollectibles(amount) {
        let spawned = 0;

        this.time.addEvent({
            delay: 500,
            repeat: amount-1,
            callback: () => {
            const x = Phaser.Math.Between(0, this.scale.width - 20);
            const y = Phaser.Math.Between(this.scale.height/2, this.scale.height - 20);

            const item = this.collectibles.create(x, y, GRAPE_ASSET_KEYS.GRAPE);
            item.setScale(4);
            item.setInteractive({ draggable: true });
            item.body.allowGravity = false;
            item.on('drag', (pointer, dragX, dragY) => item.setPosition(dragX, dragY));

            item.spawnId = this.nextSpawnId;

            this.spawnOrder.push(this.nextSpawnId);

        
            this.nextSpawnId++;
            spawned++;

            },
            callbackScope: this
        });
    }

    //Detect Collision
    eatGrapes(player, item){
        this.dzhong.setTexture(D12_ASSET_KEYS.GRAPECLOSE);
        const expectedId = this.spawnOrder[this.currentCollectIndex];
        console.log("Collide")
        if (item.spawnId === expectedId) {
            this.grapesEaten++;
            item.destroy();
            this.currentCollectIndex++;
            if (this.grapesEaten == this.grapesSpawn){
                this.registry.set('level', this.registry.get('level') + 1);
                this.scene.start(SCENE_KEYS.GRAPE_CATCHER_SCENE);
            }

            console.log("Eaten")
        } else {
            this.scene.start(SCENE_KEYS.END_SCENE);
            console.log("Wrong order!")
        }
        this.sound.play(D12_AUDIO_ASSET_KEYS.GRAPE);

        this.time.delayedCall(200, () => {
        this.dzhong.setTexture(D12_ASSET_KEYS.GRAPEOPEN);
        });

    }


    update(){}





}