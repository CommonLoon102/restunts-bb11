package org.restunts.android;

import android.content.Intent;
import android.content.Context;
import android.os.Bundle;
import java.io.File;
import org.libsdl.app.SDLActivity;

/** SDL owns lifecycle, graphics, audio, multitouch and the software keyboard. */
public final class GameActivity extends SDLActivity {
    private static Context applicationContext;

    @Override
    public void onCreate(Bundle state) {
        applicationContext = getApplicationContext();
        super.onCreate(state);
    }

    /** Called on the native game thread after a complete game-file write. */
    public static void persistSavedFile(String path) {
        if (applicationContext != null) {
            GameDataStorage.publish(applicationContext, path);
        }
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {"SDL3", "nuked-opl2", "main"};
    }

    @Override
    protected String[] getArguments() {
        Intent intent = getIntent();
        LaunchOptions options = new LaunchOptions(
            intent.getBooleanExtra(LaunchOptions.NEW_MIDI_KEY, LaunchOptions.DEFAULT_NEW_MIDI),
            intent.getBooleanExtra(LaunchOptions.SHOW_FPS_KEY, LaunchOptions.DEFAULT_SHOW_FPS),
            LaunchOptions.HyperVision.fromStored(intent.getStringExtra(LaunchOptions.HYPERVISION_KEY)));
        return options.arguments(new File(getFilesDir(), "game").getAbsolutePath());
    }
}
