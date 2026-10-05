"use strict";

(() => {
    const MILLISECONDS_PER_SECOND = 1000;
    const PHYSICS_HZ = 20;
    const HYPERVISION_FPS = 60;
    const PHYSICS_INTERVAL_MS = MILLISECONDS_PER_SECOND / PHYSICS_HZ;
    const VIDEO_INTERVAL_MS = MILLISECONDS_PER_SECOND / HYPERVISION_FPS;
    const SAMPLES_PER_STATE = HYPERVISION_FPS / PHYSICS_HZ;
    const INTERPOLATION_DELAY_MS = PHYSICS_INTERVAL_MS - VIDEO_INTERVAL_MS;
    const TIMELINE_INTERVALS = 3;
    const TIMELINE_END_MS = PHYSICS_INTERVAL_MS * TIMELINE_INTERVALS;
    const INPUT_STEP_MS = 1;
    const SCRUB_STEP_MS = 0.1;
    const DEFAULT_INPUT_MS = 20;
    const DEFAULT_DISPLAY_MS = 0;
    const DEFAULT_CURSOR_MS = 50;
    const PLAYBACK_SLOWDOWN = 40;
    const TIME_EPSILON_MS = 0.000001;
    const DECIMAL_PLACES = 1;
    const SVG_NAMESPACE = "http://www.w3.org/2000/svg";
    const CHART = Object.freeze({
        left: 245, right: 1045, top: 55, bottom: 367,
        labelX: 22, axisY: 34, subtitleOffset: 22,
        inputY: 100, physicsY: 180, classicY: 260, hvY: 340,
        markerRadius: 14, backgroundRadius: 5, textBaseline: 5,
        eventLabelOffset: 31, waitLabelOffset: 22,
    });

    const element = (id) => document.getElementById(id);
    const inputTime = element("input-time");
    const displayDelay = element("display-delay");
    const scrubTime = element("scrub-time");
    const playButton = element("play-timeline");
    const eventLayer = element("timeline-events");
    const cursor = element("time-cursor");
    const status = element("event-status");
    const reducedMotion = window.matchMedia("(prefers-reduced-motion: reduce)");
    let animationId = null;
    let previousTimestamp = null;
    let currentTime = DEFAULT_CURSOR_MS;
    let scenario;

    const formatTime = (time) => `${Number(time.toFixed(DECIMAL_PLACES))} ms`;
    const atOrAfter = (time, eventTime) => time + TIME_EPSILON_MS >= eventTime;
    const xAt = (time) => CHART.left + time / TIMELINE_END_MS * (CHART.right - CHART.left);
    const setText = (id, value) => { element(id).textContent = value; };

    function configureRange(control, maximum, step, value) {
        control.min = 0;
        control.max = maximum;
        control.step = step;
        control.value = value;
    }

    // Ideal steady-state schedule from race_presentation_fraction in race.c.
    // Input on a tick is assumed to arrive just before that tick samples it.
    function calculateScenario(press, display) {
        const physics = Math.ceil(press / PHYSICS_INTERVAL_MS) * PHYSICS_INTERVAL_MS;
        const first = physics + display;
        const full = first + INTERPOLATION_DELAY_MS;
        return { press, display, physics, first, full, wait: physics - press,
            events: [...new Set([press, physics, first, first + VIDEO_INTERVAL_MS, full])]
                .sort((earlier, later) => earlier - later) };
    }

    function svgElement(tag, attributes, text) {
        const node = document.createElementNS(SVG_NAMESPACE, tag);
        Object.entries(attributes).forEach(([name, value]) => node.setAttribute(name, value));
        if (text !== undefined) {
            node.textContent = text;
        }
        eventLayer.append(node);
        return node;
    }

    function line(x1, y1, x2, y2, className) {
        svgElement("line", { x1, y1, x2, y2, class: className });
    }

    function label(x, y, text, className = "", anchor = "middle") {
        svgElement("text", { x, y, class: className, "text-anchor": anchor }, text);
    }

    function marker(time, y, className, text, title) {
        const selected = text !== undefined;
        const circle = svgElement("circle", { cx: xAt(time), cy: y,
            r: selected ? CHART.markerRadius : CHART.backgroundRadius,
            class: selected ? `${className} selected-mark` : "background-mark" });
        const description = document.createElementNS(SVG_NAMESPACE, "title");
        description.textContent = title;
        circle.append(description);
        if (selected) {
            label(xAt(time), y + CHART.textBaseline, text, "frame-label");
            label(xAt(time), y + CHART.eventLabelOffset, formatTime(time), "event-label");
        }
    }

    function drawTimeline() {
        eventLayer.replaceChildren();
        const axisStep = PHYSICS_INTERVAL_MS / 2;
        for (let time = 0; time <= TIMELINE_END_MS; time += axisStep) {
            line(xAt(time), CHART.top, xAt(time), CHART.bottom, "grid-line");
            label(xAt(time), CHART.axisY, formatTime(time), "tick-label");
        }
        const lanes = [
            [CHART.inputY, "Your input", "Held key press"],
            [CHART.physicsY, "Sample + physics", "Both · 20 Hz / every 50 ms"],
            [CHART.classicY, "Original video", "20 FPS / every 50 ms"],
            [CHART.hvY, "HyperVision video", "60 FPS / every 16.7 ms"],
        ];
        lanes.forEach(([y, title, subtitle]) => {
            label(CHART.labelX, y, title, "lane-title", "start");
            label(CHART.labelX, y + CHART.subtitleOffset, subtitle, "tick-label", "start");
            line(CHART.left, y, CHART.right, y, "lane-line");
        });
        line(xAt(scenario.press), CHART.inputY, xAt(scenario.press), CHART.bottom, "input-line");
        line(xAt(scenario.press), CHART.inputY, xAt(scenario.physics), CHART.inputY, "wait-line");
        label(xAt((scenario.press + scenario.physics) / 2), CHART.inputY - CHART.waitLabelOffset,
            `${formatTime(scenario.wait)} to sampling`, "event-label");
        marker(scenario.press, CHART.inputY, "physics-mark", "↓", "Key pressed and held");
        line(xAt(scenario.physics), CHART.hvY, xAt(scenario.full), CHART.hvY, "output-line");

        for (let time = 0; time <= TIMELINE_END_MS; time += PHYSICS_INTERVAL_MS) {
            const selected = time === scenario.physics;
            marker(time, CHART.physicsY, "physics-mark", selected ? "S" : undefined,
                `Input sampled; physics state calculated at ${formatTime(time)}`);
            const outputTime = time + scenario.display;
            if (outputTime <= TIMELINE_END_MS) {
                marker(outputTime, CHART.classicY, "classic-mark", selected ? "S" : undefined,
                    `Full physics state visible at ${formatTime(outputTime)}`);
            }
        }
        const fractions = ["⅓", "⅔", "S"];
        const visualSlots = TIMELINE_END_MS / VIDEO_INTERVAL_MS;
        for (let slot = 0; slot <= visualSlots; slot++) {
            const time = slot * VIDEO_INTERVAL_MS + scenario.display;
            if (time > TIMELINE_END_MS + TIME_EPSILON_MS) {
                break;
            }
            const physicsTime = Math.floor(slot / SAMPLES_PER_STATE) * PHYSICS_INTERVAL_MS;
            const fraction = slot % SAMPLES_PER_STATE;
            const selected = physicsTime === scenario.physics;
            marker(time, CHART.hvY, "hv-mark", selected ? fractions[fraction] : undefined,
                `Blend ${fraction + 1}/${SAMPLES_PER_STATE} of state completed at ` +
                `${formatTime(physicsTime)}, visible at ${formatTime(time)}`);
        }
        setText("timeline-description", `Key press at ${formatTime(scenario.press)}. ` +
            `Input sampled and physics state S calculated at ${formatTime(scenario.physics)}. ` +
            `Original displays S at ${formatTime(scenario.first)}. HyperVision displays one third ` +
            `at ${formatTime(scenario.first)}, two thirds at ` +
            `${formatTime(scenario.first + VIDEO_INTERVAL_MS)}, and all of S at ${formatTime(scenario.full)}.`);
    }

    function updateReadouts() {
        setText("input-time-value", formatTime(scenario.press));
        setText("display-delay-value", formatTime(scenario.display));
        inputTime.setAttribute("aria-valuetext", `${formatTime(scenario.press)} after the zero tick`);
        displayDelay.setAttribute("aria-valuetext", `${formatTime(scenario.display)} added to both outputs`);
        setText("input-wait", formatTime(scenario.wait));
        setText("physics-summary", `Pressed at ${formatTime(scenario.press)}; sampled and state S ` +
            `calculated at ${formatTime(scenario.physics)}.`);
        setText("classic-output", formatTime(scenario.display));
        setText("classic-summary", `The full state appears at ${formatTime(scenario.first)}, ` +
            `${formatTime(scenario.first - scenario.press)} after the press.`);
        setText("hv-output", formatTime(INTERPOLATION_DELAY_MS + scenario.display));
        setText("hv-summary", `The first ⅓ appears at ${formatTime(scenario.first)}. ` +
            `The full state appears at ${formatTime(scenario.full)}.`);
        ["classic-physics-time", "hv-physics-time"].forEach((id) => setText(id, formatTime(scenario.physics)));
        setText("classic-first-time", `${formatTime(scenario.first)} · full state`);
        setText("hv-first-time", `${formatTime(scenario.first)} · ⅓ blend`);
        setText("classic-full-time", formatTime(scenario.first));
        setText("hv-full-time", formatTime(scenario.full));
        setText("classic-total", formatTime(scenario.first - scenario.press));
        setText("hv-total", formatTime(scenario.full - scenario.press));
    }

    function updateCursor(time) {
        currentTime = Math.max(0, Math.min(TIMELINE_END_MS, time));
        scrubTime.value = currentTime;
        setText("scrub-time-value", formatTime(currentTime));
        scrubTime.setAttribute("aria-valuetext", formatTime(currentTime));
        cursor.setAttribute("x1", xAt(currentTime));
        cursor.setAttribute("x2", xAt(currentTime));
        let message;
        if (!atOrAfter(currentTime, scenario.press)) {
            message = "Before the key press. Both renderers show motion from earlier physics states.";
        } else if (!atOrAfter(currentTime, scenario.physics)) {
            message = "Key held; waiting for the next input sample. Neither physics nor video has responded yet.";
        } else if (!atOrAfter(currentTime, scenario.first)) {
            message = "Physics has used the input and completed state S. Both video outputs are waiting through the added display delay.";
        } else if (!atOrAfter(currentTime, scenario.full)) {
            const blend = atOrAfter(currentTime, scenario.first + VIDEO_INTERVAL_MS) ? "⅔" : "⅓";
            message = `The original has shown all of S. HyperVision is displaying a ${blend} blend toward S from the previous state.`;
        } else {
            message = "Both renderers have now shown the full state S. Later frames continue on their own schedules.";
        }
        // Announce only changes of stage, rather than every animation frame.
        if (status.textContent !== message) {
            status.textContent = message;
        }
        element("next-event").disabled = !scenario.events.some((event) => !atOrAfter(currentTime, event));
    }

    function pause() {
        if (animationId !== null) {
            window.cancelAnimationFrame(animationId);
        }
        animationId = null;
        previousTimestamp = null;
        playButton.textContent = "Play slowly";
    }

    function animate(timestamp) {
        if (previousTimestamp !== null) {
            updateCursor(currentTime + (timestamp - previousTimestamp) / PLAYBACK_SLOWDOWN);
        }
        previousTimestamp = timestamp;
        if (atOrAfter(currentTime, TIMELINE_END_MS)) {
            pause();
            return;
        }
        animationId = window.requestAnimationFrame(animate);
    }

    function updateScenario() {
        pause();
        scenario = calculateScenario(inputTime.valueAsNumber, displayDelay.valueAsNumber);
        drawTimeline();
        updateReadouts();
        updateCursor(currentTime);
    }

    configureRange(inputTime, PHYSICS_INTERVAL_MS, INPUT_STEP_MS, DEFAULT_INPUT_MS);
    configureRange(displayDelay, PHYSICS_INTERVAL_MS, INPUT_STEP_MS, DEFAULT_DISPLAY_MS);
    configureRange(scrubTime, TIMELINE_END_MS, SCRUB_STEP_MS, DEFAULT_CURSOR_MS);
    inputTime.addEventListener("input", updateScenario);
    displayDelay.addEventListener("input", updateScenario);
    scrubTime.addEventListener("input", () => { pause(); updateCursor(scrubTime.valueAsNumber); });
    document.querySelectorAll("[data-input-preset]").forEach((button) => {
        button.addEventListener("click", () => {
            inputTime.value = button.dataset.inputPreset === "early" ? INPUT_STEP_MS :
                PHYSICS_INTERVAL_MS - INPUT_STEP_MS;
            updateScenario();
        });
    });
    playButton.addEventListener("click", () => {
        if (animationId !== null) {
            pause();
            return;
        }
        if (atOrAfter(currentTime, TIMELINE_END_MS)) {
            updateCursor(0);
        }
        playButton.textContent = "Pause";
        animationId = window.requestAnimationFrame(animate);
    });
    element("next-event").addEventListener("click", () => {
        pause();
        const next = scenario.events.find((event) => !atOrAfter(currentTime, event));
        if (next !== undefined) {
            updateCursor(next);
        }
    });
    element("reset-timeline").addEventListener("click", () => { pause(); updateCursor(0); });
    document.addEventListener("visibilitychange", () => { if (document.hidden) { pause(); } });
    reducedMotion.addEventListener("change", pause);
    // No autoplay, including for visitors who prefer reduced motion.
    updateScenario();
    document.querySelector(".timing-lab").hidden = false;
})();
