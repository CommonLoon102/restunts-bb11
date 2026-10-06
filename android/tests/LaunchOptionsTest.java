package org.restunts.android;

import java.util.Arrays;

public final class LaunchOptionsTest {
    private static final String DATA_DIRECTORY = "/local/imported game";

    private static void require(boolean value, String message) {
        if (!value) {
            throw new AssertionError(message);
        }
    }

    private static void requireArguments(LaunchOptions options, String... expected) {
        String[] actual = options.arguments(DATA_DIRECTORY);
        require(Arrays.equals(actual, expected), "Unexpected arguments: " + Arrays.toString(actual));
    }

    public static void main(String[] args) {
        require(!LaunchOptions.DEFAULT_NEW_MIDI, "New MIDI must default to original AdLib.");
        require(!LaunchOptions.DEFAULT_SHOW_FPS, "FPS must default to hidden.");
        requireArguments(new LaunchOptions(LaunchOptions.DEFAULT_NEW_MIDI,
            LaunchOptions.DEFAULT_SHOW_FPS, LaunchOptions.HyperVision.fromStored(null)),
            "--data-dir", DATA_DIRECTORY, "--ogg:off", "--fps:off");
        requireArguments(new LaunchOptions(true, false, LaunchOptions.HyperVision.OFF),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:off");
        requireArguments(new LaunchOptions(false, true, LaunchOptions.HyperVision.OFF),
            "--data-dir", DATA_DIRECTORY, "--ogg:off", "--fps:on");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.OFF),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.AUTO),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on", "--hv:auto");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.FULL),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on", "--hv:full");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.HIGH),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on", "--hv:high");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.MEDIUM),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on", "--hv:medium");
        requireArguments(new LaunchOptions(true, true, LaunchOptions.HyperVision.LOW),
            "--data-dir", DATA_DIRECTORY, "--ogg:on", "--fps:on", "--hv:low");
        String[] labels = {"Off", "Auto", "Full", "High", "Medium", "Low"};
        LaunchOptions.HyperVision[] presets = LaunchOptions.HyperVision.values();
        require(presets.length == labels.length, "Unexpected HyperVision choices.");
        for (int index = 0; index < presets.length; index++) {
            require(presets[index].toString().equals(labels[index]), "Unexpected preset label or order.");
            require(LaunchOptions.HyperVision.fromStored(presets[index].name()) == presets[index],
                "Stored preset must survive a launcher restart.");
        }
        for (String invalid : new String[] {null, "", "auto", "unknown", "7"}) {
            require(LaunchOptions.HyperVision.fromStored(invalid) == LaunchOptions.HyperVision.OFF,
                "Invalid stored preset must fall back to Off.");
        }
        requireArguments(new LaunchOptions(false, false, null),
            "--data-dir", DATA_DIRECTORY, "--ogg:off", "--fps:off");
        System.out.println("Android launch defaults, independent flags and HyperVision presets passed.");
    }
}
