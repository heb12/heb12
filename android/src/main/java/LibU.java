// Temporary helper class for JNI
package libui;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Scanner;

public class LibU {
    public static String getDCIM() {
        String mainStorage = Environment.getExternalStorageDirectory().getAbsolutePath();
        return mainStorage + File.separator + "DCIM" + File.separator;
    }

    public static String getDocuments() {
        String mainStorage = Environment.getExternalStorageDirectory().getAbsolutePath();
        return mainStorage + File.separator + "Documents" + File.separator;
    }

    public static JSONObject getJSONSettings(Activity ctx, String key) throws Exception {
        SharedPreferences prefs = ctx.getSharedPreferences(ctx.getPackageName(), Context.MODE_PRIVATE);

        String value = prefs.getString(ctx.getPackageName() + "." + key, null);
        if (value == null) return null;

        return new JSONObject(value);
    }

    public static void storeJSONSettings(Activity ctx, String key, String value) throws Exception {
        SharedPreferences prefs = ctx.getSharedPreferences(ctx.getPackageName(), Context.MODE_PRIVATE);
        prefs.edit().putString(ctx.getPackageName() + "." + key, value).apply();
    }

    public static byte[] readFileFromAssets(Context ctx, String file) throws Exception {
        try {
            InputStream inputStream = ctx.getAssets().open(file);
            byte buffer[] = new byte[inputStream.available()];
            inputStream.read(buffer);
            inputStream.close();
            return buffer;
        } catch (IOException e) {
            e.printStackTrace();
            throw e;
        }
    }
}