package org.restunts.android;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.UriPermission;
import android.content.res.Configuration;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.DocumentsContract;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/** Import once into writable app storage, then launch the native game. */
public final class LauncherActivity extends Activity {
    private static final int REQUEST_FOLDER = 1;
    private static final int REQUEST_ZIP = 2;
    private static final int PADDING_DP = 24;
    private static final int TV_PADDING_DP = 48;
    private static final int TEXT_SIZE_SP = 18;
    private static final int TV_TEXT_SIZE_SP = 22;
    private static final int TV_BUTTON_MINIMUM_WIDTH_DP = 360;
    private static final int OPTION_TEXT_SIZE_SP = 16;
    private static final int OPTION_SPACING_DP = 12;
    private static final String OPTIONS_PREFERENCES = "launch_options";
    private static final String DIRECTORY_MIME = DocumentsContract.Document.MIME_TYPE_DIR;
    private static final String[] DOCUMENT_COLUMNS = {
        DocumentsContract.Document.COLUMN_DOCUMENT_ID,
        DocumentsContract.Document.COLUMN_DISPLAY_NAME,
        DocumentsContract.Document.COLUMN_MIME_TYPE
    };
    // Jobs and their state outlive an Activity; only the main thread changes this state.
    private static final ExecutorService WORKER = Executors.newSingleThreadExecutor();
    private static final Handler MAIN_THREAD = new Handler(Looper.getMainLooper());
    private static WeakReference<LauncherActivity> active = new WeakReference<>(null);
    private static boolean importing;
    private static boolean preparingAssets;
    private static boolean assetsReady;
    private static boolean permissionsCleaned;
    private static String currentStatus = "Import your Brøderbund Stunts 1.1 game files.\n"
        + "Game files are stored locally; the APK does not include them.";
    private TextView status;
    private Button start;
    private Button folder;
    private Button zip;
    private CheckBox newMidi;
    private CheckBox showFps;
    private Spinner hyperVision;
    private SharedPreferences preferences;
    private boolean television;

    @Override
    public void onCreate(Bundle state) {
        super.onCreate(state);
        television = (getResources().getConfiguration().uiMode & Configuration.UI_MODE_TYPE_MASK)
            == Configuration.UI_MODE_TYPE_TELEVISION;
        preferences = getSharedPreferences(OPTIONS_PREFERENCES, MODE_PRIVATE);
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setGravity(Gravity.CENTER);
        int padding = Math.round((television ? TV_PADDING_DP : PADDING_DP)
            * getResources().getDisplayMetrics().density);
        layout.setPadding(padding, padding, padding, padding);
        status = new TextView(this);
        status.setTextSize(television ? TV_TEXT_SIZE_SP : TEXT_SIZE_SP);
        status.setGravity(Gravity.CENTER);
        layout.addView(status);
        if (television) {
            TextView controls = new TextView(this);
            controls.setText("Use a keyboard or game controller for racing.");
            controls.setTextSize(TEXT_SIZE_SP);
            controls.setGravity(Gravity.CENTER);
            layout.addView(controls);
        }
        folder = button(layout, "Import Stunts folder", view -> chooseFolder());
        zip = button(layout, "Import Stunts ZIP", view -> chooseZip());
        addOptions(layout);
        start = button(layout, "Start game", view -> {
            LaunchOptions options = selectedOptions();
            Intent intent = new Intent(this, GameActivity.class);
            intent.putExtra(LaunchOptions.NEW_MIDI_KEY, options.newMidi);
            intent.putExtra(LaunchOptions.SHOW_FPS_KEY, options.showFps);
            intent.putExtra(LaunchOptions.HYPERVISION_KEY, options.hyperVision.name());
            startActivity(intent);
        });
        ScrollView scroll = new ScrollView(this);
        scroll.setFillViewport(true);
        scroll.addView(layout, new ScrollView.LayoutParams(
            ScrollView.LayoutParams.MATCH_PARENT, ScrollView.LayoutParams.WRAP_CONTENT));
        setContentView(scroll);
        refresh();
        Context context = getApplicationContext();
        if (!permissionsCleaned) {
            permissionsCleaned = true;
            // A killed process cannot run the job's finally block. Its imports are not resumed.
            for (UriPermission previous : context.getContentResolver().getPersistedUriPermissions()) {
                if (previous.isReadPermission()) {
                    releaseReadPermission(context, previous.getUri());
                }
            }
        }
        if (!preparingAssets && !assetsReady) {
            preparingAssets = true;
            WORKER.execute(() -> {
                try {
                    GameDataImport.recover(new File(context.getFilesDir(), "game"));
                    copyAssets(context, "", context.getFilesDir());
                    MAIN_THREAD.post(() -> {
                        preparingAssets = false;
                        assetsReady = true;
                        if (!importing) {
                            currentStatus = "Import your Brøderbund Stunts 1.1 game files, or start the imported game.";
                        }
                        refreshActive();
                    });
                } catch (IOException error) {
                    MAIN_THREAD.post(() -> {
                        preparingAssets = false;
                        currentStatus = "Cannot prepare port assets: " + error.getMessage();
                        refreshActive();
                    });
                }
            });
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        active = new WeakReference<>(this);
        refresh();
    }

    @Override
    protected void onPause() {
        // Spinner callbacks can be deferred; persist the current widgets before leaving.
        saveOptions();
        super.onPause();
    }

    @Override
    protected void onStop() {
        if (active.get() == this) {
            active.clear();
        }
        super.onStop();
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int keyCode;
        switch (event.getKeyCode()) {
            case KeyEvent.KEYCODE_BUTTON_A:
            case KeyEvent.KEYCODE_BUTTON_SELECT:
                keyCode = KeyEvent.KEYCODE_DPAD_CENTER;
                break;
            case KeyEvent.KEYCODE_BUTTON_B:
                keyCode = KeyEvent.KEYCODE_BACK;
                break;
            default:
                return super.dispatchKeyEvent(event);
        }
        return super.dispatchKeyEvent(new KeyEvent(event.getDownTime(), event.getEventTime(),
            event.getAction(), keyCode, event.getRepeatCount(), event.getMetaState(),
            event.getDeviceId(), event.getScanCode(), event.getFlags(), event.getSource()));
    }

    private Button button(LinearLayout layout, String label, View.OnClickListener action) {
        Button button = new Button(this);
        button.setText(label);
        button.setFocusable(true);
        if (television) {
            button.setMinWidth(Math.round(TV_BUTTON_MINIMUM_WIDTH_DP
                * getResources().getDisplayMetrics().density));
            button.setTextSize(TEXT_SIZE_SP);
        }
        button.setOnClickListener(action);
        layout.addView(button);
        return button;
    }

    private void addOptions(LinearLayout layout) {
        LinearLayout row = new LinearLayout(this);
        row.setOrientation(LinearLayout.HORIZONTAL);
        row.setGravity(Gravity.CENTER);
        int textSize = television ? TEXT_SIZE_SP : OPTION_TEXT_SIZE_SP;
        int spacing = Math.round(OPTION_SPACING_DP * getResources().getDisplayMetrics().density);
        newMidi = new CheckBox(this);
        newMidi.setText("New MIDI");
        newMidi.setTextSize(textSize);
        newMidi.setSingleLine(true);
        newMidi.setChecked(preferences.getBoolean(LaunchOptions.NEW_MIDI_KEY,
            LaunchOptions.DEFAULT_NEW_MIDI));
        row.addView(newMidi);
        showFps = new CheckBox(this);
        showFps.setText("Show FPS");
        showFps.setTextSize(textSize);
        showFps.setSingleLine(true);
        showFps.setChecked(preferences.getBoolean(LaunchOptions.SHOW_FPS_KEY,
            LaunchOptions.DEFAULT_SHOW_FPS));
        LinearLayout.LayoutParams fpsLayout = new LinearLayout.LayoutParams(
            LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        fpsLayout.setMarginStart(spacing);
        row.addView(showFps, fpsLayout);
        TextView label = new TextView(this);
        label.setText("HyperVision");
        label.setTextSize(textSize);
        label.setSingleLine(true);
        LinearLayout.LayoutParams labelLayout = new LinearLayout.LayoutParams(
            LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        labelLayout.setMarginStart(spacing);
        row.addView(label, labelLayout);
        hyperVision = new Spinner(this);
        hyperVision.setId(View.generateViewId());
        label.setLabelFor(hyperVision.getId());
        ArrayAdapter<LaunchOptions.HyperVision> adapter = new ArrayAdapter<>(this,
            android.R.layout.simple_spinner_item, LaunchOptions.HyperVision.values());
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        hyperVision.setAdapter(adapter);
        hyperVision.setSelection(LaunchOptions.HyperVision.fromStored(
            preferences.getString(LaunchOptions.HYPERVISION_KEY, null)).ordinal());
        row.addView(hyperVision);
        HorizontalScrollView optionsScroll = new HorizontalScrollView(this);
        optionsScroll.setFillViewport(true);
        optionsScroll.addView(row, new HorizontalScrollView.LayoutParams(
            HorizontalScrollView.LayoutParams.WRAP_CONTENT, HorizontalScrollView.LayoutParams.WRAP_CONTENT));
        layout.addView(optionsScroll);
        newMidi.setOnCheckedChangeListener((button, checked) -> saveOptions());
        showFps.setOnCheckedChangeListener((button, checked) -> saveOptions());
        hyperVision.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                saveOptions();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                // A populated Spinner keeps its current choice.
            }
        });
    }

    private LaunchOptions selectedOptions() {
        return new LaunchOptions(newMidi.isChecked(), showFps.isChecked(),
            (LaunchOptions.HyperVision) hyperVision.getSelectedItem());
    }

    private void saveOptions() {
        LaunchOptions options = selectedOptions();
        preferences.edit().putBoolean(LaunchOptions.NEW_MIDI_KEY, options.newMidi)
            .putBoolean(LaunchOptions.SHOW_FPS_KEY, options.showFps)
            .putString(LaunchOptions.HYPERVISION_KEY, options.hyperVision.name()).apply();
    }

    private void refresh() {
        status.setText(currentStatus);
        folder.setEnabled(!importing);
        zip.setEnabled(!importing);
        start.setEnabled(!importing && assetsReady
            && GameDataImport.isGameDirectory(new File(getFilesDir(), "game")));
        View focused = getCurrentFocus();
        if (television && (focused == null || !focused.isEnabled())) {
            (start.isEnabled() ? start : folder).requestFocus();
        }
    }

    private static void refreshActive() {
        LauncherActivity activity = active.get();
        if (activity != null) {
            activity.refresh();
        }
    }

    private void chooseFolder() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        openPicker(intent, REQUEST_FOLDER,
            "No folder picker is available. Try ZIP import or install a document picker.");
    }

    private void chooseZip() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        intent.putExtra(Intent.EXTRA_MIME_TYPES, new String[] {"application/zip", "application/x-zip-compressed"});
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        openPicker(intent, REQUEST_ZIP,
            "No file picker is available. Install a document picker to import your game files.");
    }

    private void openPicker(Intent intent, int request, String unavailableMessage) {
        try {
            startActivityForResult(intent, request);
        } catch (ActivityNotFoundException error) {
            currentStatus = unavailableMessage;
            refresh();
        } catch (SecurityException error) {
            currentStatus = "Cannot open the document picker: " + error.getMessage();
            refresh();
        }
    }

    /** Return true only for a grant acquired by this job, which it must release. */
    private static boolean retainReadPermission(Context context, Uri uri, int flags) {
        int permission = flags & Intent.FLAG_GRANT_READ_URI_PERMISSION;
        if (permission == 0 || (flags & Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION) == 0) {
            return false;
        }
        for (UriPermission previous : context.getContentResolver().getPersistedUriPermissions()) {
            if (uri.equals(previous.getUri()) && previous.isReadPermission()) {
                return false;
            }
        }
        try {
            context.getContentResolver().takePersistableUriPermission(uri, permission);
            return true;
        } catch (SecurityException error) {
            // Some document providers offer only the temporary Activity grant.
            return false;
        }
    }

    private static void releaseReadPermission(Context context, Uri uri) {
        try {
            context.getContentResolver().releasePersistableUriPermission(uri,
                Intent.FLAG_GRANT_READ_URI_PERMISSION);
        } catch (SecurityException error) {
            // A provider may revoke its grant independently while the job runs.
        }
    }

    @Override
    protected void onActivityResult(int request, int result, Intent data) {
        super.onActivityResult(request, result, data);
        if (result != RESULT_OK || data == null || data.getData() == null || importing
            || (request != REQUEST_FOLDER && request != REQUEST_ZIP)) {
            return;
        }
        Uri uri = data.getData();
        Context context = getApplicationContext();
        boolean retainedGrant = retainReadPermission(context, uri, data.getFlags());
        importing = true;
        currentStatus = "Importing game files…";
        refreshActive();
        WORKER.execute(() -> {
            File staging = new File(context.getFilesDir(), "importing");
            String resultMessage;
            try {
                GameDataImport.prepare(staging);
                GameDataImport importer = new GameDataImport(staging);
                if (request == REQUEST_ZIP) {
                    try (InputStream input = context.getContentResolver().openInputStream(uri)) {
                        importer.readZip(input);
                    }
                } else {
                    readFolder(context, importer, uri, DocumentsContract.getTreeDocumentId(uri), "", 0);
                }
                importer.install(new File(context.getFilesDir(), "game"));
                resultMessage = "Game files imported. Ready to play.";
            } catch (IOException | RuntimeException error) {
                resultMessage = "Import failed: " + error.getMessage();
            } finally {
                if (retainedGrant) {
                    releaseReadPermission(context, uri);
                }
            }
            String message = resultMessage;
            MAIN_THREAD.post(() -> {
                importing = false;
                currentStatus = message;
                refreshActive();
            });
        });
    }

    private static void readFolder(Context context, GameDataImport importer, Uri tree,
            String documentId, String parent, int depth) throws IOException {
        if (depth > GameDataImport.MAX_DEPTH) {
            throw new IOException("Game folder nesting is too deep.");
        }
        Uri children = DocumentsContract.buildChildDocumentsUriUsingTree(tree, documentId);
        try (Cursor cursor = context.getContentResolver().query(children, DOCUMENT_COLUMNS, null, null, null)) {
            if (cursor == null) {
                throw new IOException("Cannot read the selected folder.");
            }
            while (cursor.moveToNext()) {
                String id = cursor.getString(0);
                String name = cursor.getString(1);
                String mime = cursor.getString(2);
                String path = parent + name;
                if (DIRECTORY_MIME.equals(mime)) {
                    importer.directory(path);
                    readFolder(context, importer, tree, id, path + "/", depth + 1);
                } else {
                    Uri file = DocumentsContract.buildDocumentUriUsingTree(tree, id);
                    try (InputStream input = context.getContentResolver().openInputStream(file)) {
                        importer.file(path, input);
                    }
                }
            }
        }
    }

    private static void copyAssets(Context context, String path, File destination) throws IOException {
        String[] children = context.getAssets().list(path);
        if (children != null && children.length != 0) {
            GameDataImport.makeDirectory(destination);
            for (String child : children) {
                copyAssets(context, path.isEmpty() ? child : path + "/" + child, new File(destination, child));
            }
        } else {
            GameDataImport.makeDirectory(destination.getParentFile());
            try (InputStream input = context.getAssets().open(path);
                 FileOutputStream output = new FileOutputStream(destination)) {
                byte[] buffer = new byte[GameDataImport.BUFFER_SIZE];
                int count;
                while ((count = input.read(buffer)) != -1) {
                    output.write(buffer, 0, count);
                }
            }
        }
    }
}
