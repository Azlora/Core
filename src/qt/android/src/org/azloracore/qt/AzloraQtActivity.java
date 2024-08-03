package org.azloracore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class AzloraQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File azloraDir = new File(getFilesDir().getAbsolutePath() + "/.azlora");
        if (!azloraDir.exists()) {
            azloraDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
