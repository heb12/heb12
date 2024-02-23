package libui;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Typeface;
import android.os.Handler;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.PopupWindow;
import android.widget.ScrollView;
import android.widget.TabHost;
import android.widget.TabWidget;
import android.widget.TextView;
import android.widget.Toast;
import android.app.ActionBar;

import java.util.ArrayList;

public class LibUI {
    public static Context ctx = null;
    public static ActionBar actionBar = null;

    // uiWindow (popup) background drawable style resource
    private static int popupDrawableResource = 0;

    // Background drawable resource for buttons
    public static int buttonBackgroundResource = 0;

    public static Boolean useActionBar = true;

    public static void start(Activity act) {
        ctx = (Context)act;
        waitUntilActivityLoaded(act);
    }

    // Common way of telling when activity is done loading
    private static void waitUntilActivityLoaded(Activity activity) {
        ViewTreeObserver viewTreeObserver = activity.getWindow().getDecorView().getViewTreeObserver();
        viewTreeObserver.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                activity.getWindow().getDecorView().getViewTreeObserver().removeOnGlobalLayoutListener(this);
                init();
            }
        });
    }

    private static void init() {
        if (useActionBar) {
            actionBar = ((Activity)ctx).getActionBar();
            actionBar.setDisplayHomeAsUpEnabled(true);
        }
    }

    private static class MySelectListener implements AdapterView.OnItemSelectedListener {
        byte[] struct;
        public MySelectListener(byte[] struct) {
            this.struct = struct;
        }

        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
            LibUI.callFunction2(this.struct);
        }

        @Override
        public void onNothingSelected(AdapterView<?> parent) {}
    }

    private static class MyOnClickListener2 implements View.OnClickListener {
        byte[] struct;
        public MyOnClickListener2(byte[] struct) {
            this.struct = struct;
        }

        @Override
        public void onClick(View v) {
            LibUI.callFunction2(struct);
        }
    }

    private static class MyOnClickListener implements View.OnClickListener {
        long ptr;
        long arg1;
        long arg2;
        public MyOnClickListener(long ptr, long arg1, long arg2) {
            this.ptr = ptr;
            this.arg1 = arg1;
            this.arg2 = arg2;
        }

        @Override
        public void onClick(View v) {
            LibUI.callFunction(ptr, arg1, arg2);
        }
    }

    public static View form(String name) {
        LinearLayout layout = new LinearLayout(ctx);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        TextView title = new TextView(ctx);
        title.setPadding(5, 5, 5, 5);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        title.setText(name);
        title.setLayoutParams(new LinearLayout.LayoutParams(
                LayoutParams.MATCH_PARENT,
                LayoutParams.WRAP_CONTENT
        ));

        layout.addView(title);

        return layout;
    }

    public static void formAppend(View form, String name, View child) {
        LinearLayout entry = new LinearLayout(ctx);
        entry.setOrientation(LinearLayout.HORIZONTAL);
        entry.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        TextView entryName = new TextView(ctx);
        entryName.setPadding(20, 10, 20, 10);
        entryName.setText(name);
        entryName.setLayoutParams(new LinearLayout.LayoutParams(
                LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT
        ));
        entry.addView(entryName);

        entry.addView(child);

        ((LinearLayout)form).addView(entry);
    }

    @SuppressWarnings("deprecation")
    public static View tabLayout() {
        TabHost tabHost = new TabHost(ctx, null);
        tabHost.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        LinearLayout linearLayout = new LinearLayout(ctx);
        linearLayout.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        linearLayout.setOrientation(LinearLayout.VERTICAL);

        TabWidget tabWidget = new TabWidget(ctx);
        tabWidget.setId(android.R.id.tabs);

        FrameLayout frameLayout = new FrameLayout(ctx);
        frameLayout.setId(android.R.id.tabcontent);

        linearLayout.addView(tabWidget);
        linearLayout.addView(frameLayout);
        tabHost.addView(linearLayout);

        tabHost.setup();

        return tabHost;
    }

    @SuppressWarnings("deprecation")
    public static void addTab(View parent, String name, View child) {
        TabHost tabHost = (TabHost)parent;
        TabHost.TabSpec tab1Spec = tabHost.newTabSpec(name);
        tab1Spec.setIndicator(name);
        tab1Spec.setContent(new TabHost.TabContentFactory() {
            public View createTabContent(String tag) {
                return child;
            }
        });
        tabHost.addTab(tab1Spec);
    }

    public static class Screen {
        int displayOptions;
        int id;
        String title;
        View content;
    };

    static ArrayList<Screen> screens = new ArrayList<Screen>();
    static Screen origActivity = new Screen();

    public static void switchScreen(View view, String title) {
        Boolean delay = true;

        if (delay) {
            userSleep();
        }

        ScrollView layout = new ScrollView(ctx);
        layout.addView(view);

        Screen screen = new Screen();
        screen.id = screens.size();
        screen.title = title;
        screen.content = layout;

        if (screens.size() == 0) {
            origActivity.content = ((ViewGroup)((Activity)ctx).findViewById(android.R.id.content)).getChildAt(0);
            origActivity.title = (String)actionBar.getTitle();
            origActivity.displayOptions = actionBar.getDisplayOptions();
        }

        screens.add(screen);

        ((Activity)ctx).setContentView(layout);
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setTitle(title);
    }

    public static void screenGoBack() {
        Screen screen = screens.remove(screens.size() - 1);

        if (screens.size() == 0) {
            ((Activity)ctx).setContentView(origActivity.content);

            actionBar.setTitle(origActivity.title);
            if ((origActivity.displayOptions & ActionBar.DISPLAY_SHOW_HOME) == 1) {
                actionBar.setDisplayHomeAsUpEnabled(true);
            } else {
                actionBar.setDisplayHomeAsUpEnabled(false);
            }
        } else {
            ((Activity)ctx).setContentView(screen.content);

            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setTitle(screen.title);
        }
    }

    public static boolean handleBack(boolean allowBack) {
        if (screens.size() == 0) {
            if (allowBack) {
                ((Activity) ctx).finish();
            }
        } else {
            screenGoBack();
        }
        return true;
    }

    public static boolean handleOptions(MenuItem item, boolean allowBack) {
        switch (item.getItemId()) {
            case android.R.id.home:
                handleBack(allowBack);
                return true;
        }

        return ((Activity)ctx).onOptionsItemSelected(item);
    }

    // Being too fast doesn't feel right, brain need delay
    public static void userSleep() {
        try {
            Thread.sleep(100);
        } catch (Exception e) {}
    }

    public static class Popup {
        PopupWindow popupWindow;
        public void dismiss() {
            this.popupWindow.dismiss();
        }

        String title;

        public void setChild(View v) {
            LinearLayout rel = new LinearLayout(ctx);

            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setTitle(title);

            LinearLayout bar = new LinearLayout(ctx);
            rel.setPadding(10, 10, 10, 10);
            rel.setOrientation(LinearLayout.HORIZONTAL);
            rel.setLayoutParams(new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT));

            Button back = new Button(ctx);
            back.setText("Close");
            if (buttonBackgroundResource != 0) {
                back.setBackground(ctx.getResources().getDrawable(buttonBackgroundResource));
            }

            back.setTextSize(14f);

            back.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    userSleep();
                    dismiss();
                }
            });

            bar.addView(back);
            TextView tv = new TextView(ctx);
            tv.setText(title);
            tv.setLayoutParams(new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.MATCH_PARENT));
            tv.setPadding(20, 0, 0, 0);
            tv.setTextSize(20f);
            tv.setGravity(Gravity.CENTER);
            bar.addView(tv);

            rel.setOrientation(LinearLayout.VERTICAL);
            if (popupDrawableResource != 0) {
                rel.setBackground(ctx.getResources().getDrawable(popupDrawableResource));
            }
            rel.setLayoutParams(new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.MATCH_PARENT));

            TypedValue typedValue = new TypedValue();
            if (ctx.getTheme().resolveAttribute(android.R.attr.windowBackground, typedValue, true)) {
                rel.setBackgroundColor(typedValue.data);
            }

            rel.addView(bar);

            LinearLayout layout = new LinearLayout(ctx);
            layout.setPadding(20, 20, 20, 20);
            layout.setOrientation(LinearLayout.VERTICAL);
            layout.setLayoutParams(new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT));

            layout.addView(v);
            rel.addView(layout);

            this.popupWindow.setContentView(rel);
            this.popupWindow.showAtLocation(((Activity)ctx).getWindow().getDecorView().getRootView(), Gravity.CENTER, 0, 0);
        }

        Popup(String title, int options) {
            userSleep();
            DisplayMetrics displayMetrics = new DisplayMetrics();
            ((Activity)ctx).getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
            int height = displayMetrics.heightPixels;
            int width = displayMetrics.widthPixels;
            this.title = title;

            this.popupWindow = new PopupWindow(
                    (int)(width / 1.2),
                    (int)(height / 1.2)
            );

            this.popupWindow.setOutsideTouchable(false);
        }
    }

    public static LibUI.Popup openWindow(String title, int options) {
        LibUI.Popup popup = new LibUI.Popup(title, options);
        return popup;
    }

    public static void setClickListener(View v, long ptr, long arg1, long arg2) {
        v.setOnClickListener(new MyOnClickListener(ptr, arg1, arg2));
    }

    public static ViewGroup linearLayout(int orientation) {
        LinearLayout layout = new LinearLayout(ctx);
        layout.setOrientation(orientation);
        layout.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        return (ViewGroup)layout;
    }

    public static void setPadding(View v, int l, int t, int r, int b) {
        v.setPadding(l, t, r, b);
    }

    public static String getString(String name) {
        Resources res = ctx.getResources();
        return res.getString(res.getIdentifier(name, "string", ctx.getPackageName()));
    }

    public static View getView(String name) {
        Resources res = ctx.getResources();
        int id = res.getIdentifier(name, "id", ctx.getPackageName());
        return ((Activity)ctx).findViewById(id);
    }

    public static void toast(String text) {
        Toast.makeText(ctx, text, Toast.LENGTH_SHORT).show();
    }

    public static void runRunnable(long ptr, long arg1, long arg2) {
        Handler handler = new Handler(Looper.getMainLooper());
        handler.post(new Runnable() {
            @Override
            public void run() {
                callFunction(ptr, arg1, arg2);
            }
        });
    }

    public static native void callFunction(long ptr, long arg1, long arg2);
    public static native void callFunction2(byte[] struct);

    public int dpToPx(int dp) {
        Resources r = ctx.getResources();
        return Math.round(TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, r.getDisplayMetrics()));
    }
}
