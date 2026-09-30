"use strict";

const loadGameButton = document.getElementById("load-game");
const gameFrame = document.getElementById("game-frame");
const playerStart = document.getElementById("player-start");
const playerStatus = document.getElementById("player-status");

loadGameButton.hidden = false;

loadGameButton.addEventListener("click", () => {
    playerStart.hidden = true;
    playerStatus.hidden = false;
    playerStatus.textContent = "Loading the browser edition…";
    gameFrame.hidden = false;

    gameFrame.addEventListener("load", () => {
        const hasGameCanvas = gameFrame.contentDocument?.getElementById("canvas");
        if (!hasGameCanvas) {
            gameFrame.hidden = true;
            playerStatus.textContent = "The browser game could not load. Try opening it in a new tab.";
            playerStart.hidden = false;
            loadGameButton.textContent = "Try again";
            return;
        }
        playerStatus.textContent = "Choose your game folder in the player below, then select Start game.";
        gameFrame.focus();
    }, { once: true });

    gameFrame.src = gameFrame.dataset.src;
});
