#!/usr/bin/env node
'use strict';

// Uses Playwright as described in docs/wasm.md and an existing WebAssembly build:
//   NODE_PATH=out/wasm-browser-test/node_modules node tools/scripts/test-wasm-audio.js \
//       --build-dir out/sdl3-wasm --sdl-include out/sdl3-wasm/_deps/sdl3-src/include
// Activate the Emscripten SDK first, or supply --emcc /path/to/emcc.
// The synthetic instruments and included Vorbis fixture need no original game data.
const assert = require('node:assert/strict');
const { spawnSync } = require('node:child_process');
const fs = require('node:fs/promises');
const path = require('node:path');
const { pathToFileURL } = require('node:url');
const { chromium, devices } = require('playwright');

const ROOT = path.resolve(__dirname, '../..');
const TIMEOUT_MS = 60000;
const ASYNCIFY_STACK_BYTES = 65536;
const STACK_BYTES = 262144;
const SILENCE_TAIL_FRAMES = 64;
const MIN_PLAYBACK_BUFFERS = 16;
const MIN_PAUSED_BUFFERS = 2;
const DEVICE_SAMPLE_RATES = [44100, 48000];
const MOBILE_QUEUE_MS = 500;
const DESKTOP_QUEUE_MS = 100;
const ADLIB_DEVICE_HINT_FRAMES = [1024, 2048];
const SDL_DEVICE_PERIOD_MULTIPLIER = 2;
const TIMER_TICK_MILLISECONDS = 10;
const MILLISECONDS_PER_SECOND = 1000;

function options() {
    const result = {
        emcc: 'emcc', 'build-dir': path.join(ROOT, 'out/sdl3-wasm'),
        output: path.join(ROOT, 'out/wasm-audio-test'), 'cpu-slowdown': 1,
    };
    const args = process.argv.slice(2);
    for (let index = 0; index < args.length; index++) {
        const name = args[index].replace(/^--/, '');
        if (name === 'help') {
            console.log('Usage: node tools/scripts/test-wasm-audio.js ' +
                '[--emcc COMMAND] [--build-dir DIR] [--sdl-include DIR] ' +
                '[--output DIR] [--browser-executable FILE] [--cpu-slowdown RATE] ' +
                '[--build-only] [--skip-build]');
            process.exit(0);
        }
        if (name === 'build-only' || name === 'skip-build') { result[name] = true; continue; }
        assert.ok(['emcc', 'build-dir', 'sdl-include', 'output', 'browser-executable',
            'cpu-slowdown'].includes(name),
            'Unknown option: ' + args[index]);
        assert.ok(args[index + 1], 'Missing value for ' + args[index]);
        result[name] = args[++index];
    }
    result['cpu-slowdown'] = Number(result['cpu-slowdown']);
    assert.ok(Number.isFinite(result['cpu-slowdown']) && result['cpu-slowdown'] >= 1,
        'CPU slowdown must be a number greater than or equal to one');
    result['build-dir'] = path.resolve(result['build-dir']);
    result.output = path.resolve(result.output);
    result['sdl-include'] ||= path.join(result['build-dir'], '_deps/sdl3-src/include');
    return result;
}

async function exercise(browser, fixture, mobile, deviceHintFrames, sampleRate, cpuSlowdown) {
    const context = await browser.newContext(mobile ? devices['Pixel 7'] : {});
    const page = await context.newPage();
    if (cpuSlowdown > 1) {
        const client = await context.newCDPSession(page);
        await client.send('Emulation.setCPUThrottlingRate', { rate: cpuSlowdown });
    }
    const errors = [];
    page.on('pageerror', error => errors.push(String(error)));
    await page.addInitScript(({ silenceTail, sampleRate, mobile, deviceHintFrames }) => {
        const observation = globalThis.audioFixture = {
            phase: 'startup', finished: false, mobile, deviceHintFrames, buffers: [], nodes: [],
        };
        const NativeAudioContext = window.AudioContext;
        window.AudioContext = class extends NativeAudioContext {
            constructor(settings) {
                super({ ...settings, sampleRate });
            }
            createScriptProcessor(...args) {
                const node = super.createScriptProcessor(...args);
                const state = { disconnected: false };
                observation.nodes.push(state);
                const disconnect = node.disconnect;
                node.disconnect = (...targets) => {
                    state.disconnected = true;
                    return disconnect.apply(node, targets);
                };
                const setter = Object.getOwnPropertyDescriptor(
                    ScriptProcessorNode.prototype, 'onaudioprocess').set;
                Object.defineProperty(node, 'onaudioprocess', {
                    set(callback) {
                        setter.call(node, event => {
                            callback(event);
                            if (observation.finished) return;
                            const buffer = event.outputBuffer;
                            let trailing = buffer.length;
                            let peak = 0;
                            for (let channel = 0; channel < buffer.numberOfChannels; channel++) {
                                const samples = buffer.getChannelData(channel);
                                let tail = samples.length;
                                while (tail > 0 && samples[tail - 1] === 0) tail--;
                                trailing = Math.min(trailing, samples.length - tail);
                                for (const sample of samples) peak = Math.max(peak, Math.abs(sample));
                            }
                            observation.buffers.push({ phase: observation.phase,
                                frames: buffer.length, trailing, peak,
                                starved: trailing >= silenceTail });
                        });
                    },
                });
                return node;
            }
        };
    }, { silenceTail: SILENCE_TAIL_FRAMES, sampleRate, mobile, deviceHintFrames });
    try {
        await page.goto(pathToFileURL(fixture).href, { timeout: TIMEOUT_MS });
        await page.waitForFunction(() => globalThis.audioFixture?.finished,
            null, { timeout: TIMEOUT_MS });
        const report = await page.evaluate(() => ({
            configuration: audioFixture.configuration,
            buffers: audioFixture.buffers,
            disconnected: audioFixture.nodes.map(node => node.disconnected),
        }));
        assert.deepEqual(errors, [], 'Browser audio fixture raised errors');
        const config = report.configuration;
        assert.ok(config, 'The fixture must report audio configuration');
        config.reserveMilliseconds = config.reserveFrames * MILLISECONDS_PER_SECOND /
            config.sourceSampleRate;
        config.devicePeriodMilliseconds = config.deviceFrames * MILLISECONDS_PER_SECOND /
            config.deviceSampleRate;
        const label = (mobile ? 'mobile' : 'desktop') + '-' +
            configLabel(deviceHintFrames, sampleRate, cpuSlowdown);
        await fs.writeFile(path.join(path.dirname(fixture), label + '-buffers.json'),
            JSON.stringify(report, null, 2) + '\n');
        assert.equal(config.deviceSampleRate, sampleRate);
        assert.equal(config.deviceFrames, deviceHintFrames * SDL_DEVICE_PERIOD_MULTIPLIER,
            'The fixture must exercise the requested physical browser device period');
        assert.equal(config.supplyCallback, mobile,
            'Only mobile Chrome may install a main-thread audio supply callback');
        assert.equal(config.musicQueueMilliseconds, mobile ? MOBILE_QUEUE_MS : DESKTOP_QUEUE_MS,
            'Desktop music must retain its existing 100 ms queue');
        if (mobile) {
            const periodFrames = Math.ceil(config.deviceFrames * config.sourceSampleRate /
                config.deviceSampleRate);
            const tickFrames = Math.floor(config.sourceSampleRate * TIMER_TICK_MILLISECONDS /
                MILLISECONDS_PER_SECOND);
            assert.equal(config.reserveFrames, periodFrames + tickFrames,
                'Mobile effects must reserve one device period plus one timer tick');
            assert.equal(config.consumerPhases, TIMER_TICK_MILLISECONDS,
                'Every millisecond of the synthesis timer phase must deliver complete periods');
            assert.ok(config.minimumPrimeFrames >= config.deviceFrames,
                'The converted reserve must contain at least one complete device period');
            assert.ok(config.demandTicks > 0,
                'An SDL consumption callback must advance overdue timers before reading PCM');
        } else {
            assert.equal(config.reserveFrames, 0, 'Desktop effects must retain no added reserve');
        }
        const phases = {};
        for (const phase of mobile ? ['adlib', 'render', 'demand', 'ogg', 'resumed', 'paused'] : []) {
            const buffers = report.buffers.filter(buffer => buffer.phase === phase);
            const paused = phase === 'paused';
            assert.ok(buffers.length >= (paused ? MIN_PAUSED_BUFFERS : MIN_PLAYBACK_BUFFERS),
                phase + ': too few browser audio buffers to test delivery');
            phases[phase] = { buffers: buffers.length,
                silent: buffers.filter(buffer => buffer.peak === 0).length,
                starved: buffers.filter(buffer => buffer.starved).length,
                frames: buffers[0].frames };
            if (paused) {
                assert.ok(buffers.every(buffer => buffer.peak === 0),
                    'Paused music must remain silent while timer/audio servicing continues');
            } else {
                assert.ok(buffers.every(buffer => !buffer.starved),
                    phase + ': browser PCM contains an underrun: ' + JSON.stringify(phases[phase]));
            }
        }
        if (mobile) {
            assert.equal(phases.adlib.frames, config.deviceFrames);
            assert.ok(phases.ogg.frames > phases.adlib.frames,
                'The Vorbis case must exercise a wider actual browser device period');
        }
        assert.ok(report.disconnected.length > 0 && report.disconnected.every(Boolean),
            'Audio shutdown must disconnect every browser playback node');
        return { configuration: config, cpuSlowdown, phases };
    } finally {
        await context.close();
    }
}

function configLabel(deviceHintFrames, sampleRate, cpuSlowdown) {
    return deviceHintFrames * SDL_DEVICE_PERIOD_MULTIPLIER + '-frames-' +
        sampleRate + '-hz-' + cpuSlowdown + 'x';
}

async function main() {
    const settings = options();
    await fs.mkdir(settings.output, { recursive: true });
    const fixture = path.join(settings.output, 'test-wasm-audio.html');
    if (!settings['skip-build']) {
        const build = spawnSync(settings.emcc, [
            '-O2', '-Wno-pointer-sign', '-DRESTUNTS_SDL3', '-I' + settings['sdl-include'],
            '-I' + path.join(ROOT, 'third_party/stb'),
            '-I' + path.join(ROOT, 'third_party/nuked-opl2-lite'),
            '-sASYNCIFY', '-sASYNCIFY_STACK_SIZE=' + ASYNCIFY_STACK_BYTES,
            '-sSTACK_SIZE=' + STACK_BYTES, '-sALLOW_MEMORY_GROWTH=1',
            '-sENVIRONMENT=web', '-sSINGLE_FILE=1', '-sMINIFY_HTML=0', '-sASSERTIONS=1',
            path.join(ROOT, 'src/restunts/tests/test-wasm-audio.c'),
            path.join(ROOT, 'third_party/stb/stb_vorbis.c'),
            path.join(settings['build-dir'], '_deps/sdl3-build/libSDL3.a'),
            path.join(settings['build-dir'], 'third_party/nuked-opl2-lite/libnuked-opl2.a'),
            '--embed-file', path.join(ROOT, 'src/restunts/tests/fixtures/music/tone-stereo.ogg') +
                '@/assets/music/slct.ogg', '-o', fixture,
        ], { cwd: ROOT, encoding: 'utf8' });
        assert.ifError(build.error);
        assert.equal(build.status, 0, build.stdout + build.stderr);
    }
    if (settings['build-only']) { console.log('Built ' + fixture); return; }
    const browser = await chromium.launch({ headless: true,
        executablePath: settings['browser-executable'],
        args: ['--autoplay-policy=no-user-gesture-required'],
    });
    try {
        const report = { desktop: await exercise(browser, fixture, false,
            ADLIB_DEVICE_HINT_FRAMES[0], DEVICE_SAMPLE_RATES[1], settings['cpu-slowdown']), mobile: {} };
        for (const sampleRate of DEVICE_SAMPLE_RATES) {
            for (const hintFrames of ADLIB_DEVICE_HINT_FRAMES) {
                report.mobile[configLabel(hintFrames, sampleRate, settings['cpu-slowdown'])] =
                    await exercise(browser, fixture, true, hintFrames, sampleRate,
                        settings['cpu-slowdown']);
            }
        }
        await fs.writeFile(path.join(settings.output, 'results-' + settings['cpu-slowdown'] + 'x.json'),
            JSON.stringify(report, null, 2) + '\n');
        console.log('WebAssembly audio: mobile AdLib, render, consumer demand, Vorbis, pause/resume and desktop baseline passed');
        console.log(JSON.stringify(report));
    } finally {
        await browser.close();
    }
}

main().catch(error => {
    console.error(error);
    process.exitCode = 1;
});
