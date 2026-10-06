package org.restunts.android;

/** Launcher choices shared by preference storage, Intent extras and native arguments. */
public final class LaunchOptions {
    public static final String NEW_MIDI_KEY = "new_midi";
    public static final String SHOW_FPS_KEY = "show_fps";
    public static final String HYPERVISION_KEY = "hypervision";
    public static final boolean DEFAULT_NEW_MIDI = false;
    public static final boolean DEFAULT_SHOW_FPS = false;

    public enum HyperVision {
        OFF("Off", null),
        AUTO("Auto", "auto"),
        FULL("Full", "full"),
        HIGH("High", "high"),
        MEDIUM("Medium", "medium"),
        LOW("Low", "low");

        private final String label;
        private final String argument;

        HyperVision(String label, String argument) {
            this.label = label;
            this.argument = argument;
        }

        public static HyperVision fromStored(String value) {
            if (value != null) {
                for (HyperVision option : values()) {
                    if (option.name().equals(value)) {
                        return option;
                    }
                }
            }
            return OFF;
        }

        @Override
        public String toString() {
            return label;
        }
    }

    public final boolean newMidi;
    public final boolean showFps;
    public final HyperVision hyperVision;

    public LaunchOptions(boolean newMidi, boolean showFps, HyperVision hyperVision) {
        this.newMidi = newMidi;
        this.showFps = showFps;
        this.hyperVision = hyperVision == null ? HyperVision.OFF : hyperVision;
    }

    public String[] arguments(String dataDirectory) {
        String midi = newMidi ? "--ogg:on" : "--ogg:off";
        String fps = showFps ? "--fps:on" : "--fps:off";
        if (hyperVision == HyperVision.OFF) {
            return new String[] {"--data-dir", dataDirectory, midi, fps};
        }
        return new String[] {"--data-dir", dataDirectory, midi, fps, "--hv:" + hyperVision.argument};
    }
}
