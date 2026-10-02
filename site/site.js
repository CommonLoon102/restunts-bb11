"use strict";

document.querySelectorAll("[data-comparison]").forEach((comparison) => {
    const slider = comparison.querySelector(".comparison-slider");
    const image = comparison.querySelector(".comparison-image");
    const minPercent = Number(slider.min);
    const maxPercent = Number(slider.max);
    const primaryPointerButton = 0;
    let activePointerId = null;

    const updateComparison = () => {
        const beforePercent = slider.valueAsNumber;
        comparison.style.setProperty("--comparison-position", `${beforePercent}%`);
        slider.setAttribute("aria-valuetext",
            `${beforePercent}% original graphics, ${maxPercent - beforePercent}% enhanced graphics`);
    };

    const moveDivider = (event) => {
        const bounds = image.getBoundingClientRect();
        const position = (event.clientX - bounds.left) / bounds.width;
        const beforePercent = minPercent + position * (maxPercent - minPercent);
        slider.valueAsNumber = Math.round(Math.max(minPercent, Math.min(maxPercent, beforePercent)));
        updateComparison();
    };

    slider.addEventListener("input", updateComparison);
    // Keep pointer dragging on the image, outside the native range widget.
    image.addEventListener("pointerdown", (event) => {
        if (!event.isPrimary || event.button !== primaryPointerButton) {
            return;
        }
        event.preventDefault();
        activePointerId = event.pointerId;
        slider.focus({ preventScroll: true });
        image.setPointerCapture(activePointerId);
        moveDivider(event);
    });
    image.addEventListener("pointermove", (event) => {
        if (event.pointerId === activePointerId) {
            moveDivider(event);
        }
    });
    image.addEventListener("pointerup", (event) => {
        if (event.pointerId === activePointerId) {
            moveDivider(event);
            image.releasePointerCapture(activePointerId);
            activePointerId = null;
        }
    });
    image.addEventListener("lostpointercapture", () => {
        activePointerId = null;
    });

    updateComparison();
    image.classList.add("comparison-interactive");
    comparison.querySelectorAll("[hidden]").forEach((element) => {
        element.hidden = false;
    });
});

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
