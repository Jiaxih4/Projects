import Phaser from './lib/phaser.js';
import { Bouncer } from './scenes/05bounce.js';
import { CarrotBreaker } from './scenes/06carrotbreaker.js';
import { DinosaurGame } from './scenes/03dinosaur.js';
import { DragGrapesScene } from './scenes/01drag_grapes.js';
import { EndScene } from './scenes/end-scene.js';
import { FlappyGame } from './scenes/04flappy.js';
import { GrapeCatcher } from './scenes/02grape_catcher.js';
import { LevelManagerScene } from './scenes/level-manager-scene.js';
import { PreloadScene } from './scenes/preload-scene.js';
import { SCENE_KEYS } from './scenes/scene-keys.js';
import { Snake } from './scenes/07snake.js';
import { Frogger } from './scenes/08frogger.js';
import { Asteroid } from './scenes/09asteroid.js';
import { Stacker } from './scenes/10stacker.js';
import { Dodger } from './scenes/11dodger.js';
import { Parry } from './scenes/12Parry.js';
import { WinScene } from './scenes/win-scene.js';

const speedDown = 300;

const game = new Phaser.Game({
    type: Phaser.CANVAS,
    pixelArt: false,
    scale: {
        parent: 'game-container',
        width: 1024,
        height: 576,
        mode: Phaser.Scale.FIT,
        autoCenter: Phaser.Scale.CENTER_BOTH,
    },
    backgroundColor: '#004c5f',
    physics: {
        default: "arcade",
        arcade: {
            gravity:{y:speedDown},
            debug: false,
        }
    },
});

game.scene.add(SCENE_KEYS.PRELOAD_SCENE, PreloadScene);
game.scene.add(SCENE_KEYS.DRAG_GRAPES, DragGrapesScene);
game.scene.add(SCENE_KEYS.LEVEL_MANAGER, LevelManagerScene);
game.scene.add(SCENE_KEYS.END_SCENE, EndScene);
game.scene.add(SCENE_KEYS.GRAPE_CATCHER_SCENE, GrapeCatcher);
game.scene.add(SCENE_KEYS.DINOSAURGAME, DinosaurGame);
game.scene.add(SCENE_KEYS.FLAPPY_SCENE, FlappyGame);
game.scene.add(SCENE_KEYS.BOUNCE_SCENE, Bouncer);
game.scene.add(SCENE_KEYS.CARROT_BREAKER_SCENE, CarrotBreaker);
game.scene.add(SCENE_KEYS.SNAKE_SCENE, Snake);
game.scene.add(SCENE_KEYS.FROGGER_SCENE, Frogger);
game.scene.add(SCENE_KEYS.ASTEROID_SCENE, Asteroid);
game.scene.add(SCENE_KEYS.STACKER, Stacker);
game.scene.add(SCENE_KEYS.DODGER, Dodger);
game.scene.add(SCENE_KEYS.PARRY, Parry);
game.scene.add(SCENE_KEYS.WIN, WinScene);
game.scene.start(SCENE_KEYS.PRELOAD_SCENE);