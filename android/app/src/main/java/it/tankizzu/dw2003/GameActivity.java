package it.tankizzu.dw2003;

import android.os.Build;
import android.os.Bundle;
import android.view.WindowManager;

import org.libsdl.app.SDLActivity;

/* SDL's activity: it loads libSDL3.so and libmain.so (android/loader.c), which loads the game low in memory */
public class GameActivity extends SDLActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            /* the picture also under a camera's cut-out: the game keeps its buttons in the safe area (touch.c) */
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }
    }
}
