import { ATTACK_LEFT_ASSET_KEYS, ATTACK_RIGHT_ASSET_KEYS, BATTLE_ASSET_KEYS, BATTLE_BACKGROUND_ASSET_KEYS, BIG_BOI_ASSET_KEYS, CARROT_ASSET_KEYS, D12_ASSET_KEYS, D12_AUDIO_ASSET_KEYS, FULL_CARROT_ASSET_KEYS, FULL_CARROT_VERT_ASSET_KEYS, GRAPE_ASSET_KEYS, GRAPE_CATCHER_ASSET_KEYS, GRAPE_EATER_ASSET_KEYS, GRAPE_WARRIOR_ASSET_KEYS, HAND_ASSET_KEYS, HEALTH_BAR_ASSET_KEYS, IDLE_ASSET_KEYS, IDLE_LEFT_ASSET_KEYS, JUMP_LEFT_ASSET_KEYS, JUMP_RIGHT_ASSET_KEYS, LOG_ASSET_KEYS, MONSTER_ASSET_KEYS, ROAD_ASSET_KEYS, RUN_ASSET_KEYS, RUN_LEFT_ASSET_KEYS, SKY_ASSET_KEYS, SONG_ASSET_KEYS, TILE_ANI_ASSET_KEYS, TILE_ASSET_KEYS, WALL_ASSET_KEYS, WATER_ASSET_KEYS } from "../assets/asset-keys.js";
import Phaser from "../lib/phaser.js";
import { SCENE_KEYS } from "./scene-keys.js";

export class PreloadScene extends Phaser.Scene{
    constructor(){
        super({
            key: SCENE_KEYS.PRELOAD_SCENE,
        });
        console.log(SCENE_KEYS.PRELOAD_SCENE)
    }

    //init() {
    //    console.log('init');
    //}

    preload() {
        const grapeEaterPath = 'assets/images/GrapeEater'
        const grapeAudioPath = 'assets/audio'

        //Grape Eater 12
        this.load.image(
            GRAPE_EATER_ASSET_KEYS.CLOSED_MOUTH,
            `${grapeEaterPath}/dzhongshut-removebg-preview.png`
        )
        this.load.image(
            GRAPE_EATER_ASSET_KEYS.OPEN_MOUTH,
            `${grapeEaterPath}/dzhongOpen-removebg-preview.png`
        )

        //GRAPE
        this.load.image(
            GRAPE_ASSET_KEYS.GRAPE,
            `${grapeEaterPath}/not_so_grape.png`
        )

        //GRAPE CATCHER
        this.load.image(
            GRAPE_CATCHER_ASSET_KEYS.GRAPE_CATCHER,
            `${grapeEaterPath}/GrapeCatcher.png`
        )

        //CARROT
        this.load.image(
            CARROT_ASSET_KEYS.CARROT,
            `${grapeEaterPath}/carrot.png`
        )

        //HAND
        this.load.image(
            HAND_ASSET_KEYS.HAND,
            `${grapeEaterPath}/hand.png`
        )

        //FULL CARROT
        this.load.image(
            FULL_CARROT_ASSET_KEYS.FULL_CARROT,
            `${grapeEaterPath}/fullcarrot.png`
        )

        //WALL
        this.load.image(
            WALL_ASSET_KEYS.WALL,
            `${grapeEaterPath}/wall.png`
        )

        //ROAD
        this.load.image(
            ROAD_ASSET_KEYS.ROAD,
            `${grapeEaterPath}/road.png`
        )

        //WATER
        this.load.image(
            WATER_ASSET_KEYS.WATER,
            `${grapeEaterPath}/water.png`
        )

        //FULL CARROT VERTICAL
        this.load.image(
            FULL_CARROT_VERT_ASSET_KEYS.FULL_CARROT_VERT,
            `${grapeEaterPath}/fullcarrotVert.png`
        )
        //LOG
        this.load.image(
            LOG_ASSET_KEYS.LOG,
            `${grapeEaterPath}/log.png`
        )

        //Add Idle animation
        this.load.spritesheet(IDLE_ASSET_KEYS.IDLE, `${grapeEaterPath}/GrapeIdle.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )

        //Add Idle Left animation
        this.load.spritesheet(IDLE_LEFT_ASSET_KEYS.IDLE_LEFT, `${grapeEaterPath}/GrapeIdleLeft.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )

        //Add Run animation
        this.load.spritesheet(RUN_ASSET_KEYS.RUN, `${grapeEaterPath}/GrapeRun.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )

        //Add Run Left animation
        this.load.spritesheet(RUN_LEFT_ASSET_KEYS.RUN_LEFT, `${grapeEaterPath}/GrapeRunLeft.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )
        //GRAPEWARRIOR
        this.load.image(
            GRAPE_WARRIOR_ASSET_KEYS.GRAPE_WARRIOR,
            `${grapeEaterPath}/GrapeWarrior.png`
        )
        this.load.image(
            TILE_ASSET_KEYS.TILE,
            `${grapeEaterPath}/tile.png`
        )

        this.load.image(
            BIG_BOI_ASSET_KEYS.BIG_BOI,
            `${grapeEaterPath}/bigboi.png`
        )

        this.load.image(
            D12_ASSET_KEYS.GRAPECATCHER2,
            `${grapeEaterPath}/GrapeCatcher2.png`
        )

        this.load.image(
            D12_ASSET_KEYS.GRAPECLOSE,
            `${grapeEaterPath}/GraperClosed.png`
        )
        this.load.image(
            D12_ASSET_KEYS.GRAPEFLAPPER,
            `${grapeEaterPath}/grapeFlapper.png`
        )

        this.load.image(
            D12_ASSET_KEYS.GRAPEOPEN,
            `${grapeEaterPath}/GraperOpen.png`
        )

        this.load.image(
            D12_ASSET_KEYS.GRAPEPERSON,
            `${grapeEaterPath}/grapePerson.png`
        )

        this.load.image(
            D12_ASSET_KEYS.GRAPER,
            `${grapeEaterPath}/graper.png`
        )
        //Add ATTACK Left animation
        this.load.spritesheet(ATTACK_LEFT_ASSET_KEYS.ATTACK_LEFT, `${grapeEaterPath}/AttackLeft.png`,
            {
                frameWidth: 60,
                frameHeight:40,
            }
        )
        //Add ATTACK Right animation
        this.load.spritesheet(ATTACK_RIGHT_ASSET_KEYS.ATTACK_RIGHT, `${grapeEaterPath}/AttackRight.png`,
            {
                frameWidth: 60,
                frameHeight:40,
            }
        )
        //Jump LEFT
        this.load.spritesheet(JUMP_LEFT_ASSET_KEYS.JUMP_LEFT, `${grapeEaterPath}/JumpLeft.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )
        //Jump RIGHT
        this.load.spritesheet(JUMP_RIGHT_ASSET_KEYS.JUMP_RIGHT, `${grapeEaterPath}/JumpRight.png`,
            {
                frameWidth: 40,
                frameHeight:40,
            }
        )
        this.load.spritesheet(TILE_ANI_ASSET_KEYS.TILE_ANI, `${grapeEaterPath}/tileAni.png`,
            {
                frameWidth: 32,
                frameHeight:32,
            }
        )
        this.load.spritesheet(SKY_ASSET_KEYS.SKY, `${grapeEaterPath}/sky.png`,
            {
                frameWidth: 1024,
                frameHeight:576,
            }
        )

        this.load.audio(SONG_ASSET_KEYS.SONG,`${grapeAudioPath}/GrapeEater.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.LAUGH,`${grapeAudioPath}/grapes/laugh.ogg`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.GRAPEX2,`${grapeAudioPath}/grapes/grapex2.ogg`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.GRAPE,`${grapeAudioPath}/grapes/grape.ogg`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.ZECSONG,`${grapeAudioPath}/grapes/d12-zecsong.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.WTH,`${grapeAudioPath}/grapes/d12-WTH.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.SQUEEZE,`${grapeAudioPath}/grapes/d12-squeeze.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.LALALA,`${grapeAudioPath}/grapes/d12-lalala.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.INDIAN,`${grapeAudioPath}/grapes/d12-indian.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.GRAPED,`${grapeAudioPath}/grapes/d12-graped.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.COCK,`${grapeAudioPath}/grapes/d12-cock.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.CHICKENJOCKEY,`${grapeAudioPath}/grapes/d12-chickenJockey.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.BED,`${grapeAudioPath}/grapes/d12-bed.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.SIXSEVEN,`${grapeAudioPath}/grapes/d12-67.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.OHH,`${grapeAudioPath}/grapes/d12 OHH.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.BLACK,`${grapeAudioPath}/grapes/d12black.mp3`);
        this.load.audio(D12_AUDIO_ASSET_KEYS.AHAHA,`${grapeAudioPath}/grapes/d12 ahaha.mp3`);
    }

    create() {
        this.scene.launch(SCENE_KEYS.LEVEL_MANAGER);
        this.scene.start(SCENE_KEYS.PARRY);
    }

    //update() {
    //    console.log('update');
    //}
}