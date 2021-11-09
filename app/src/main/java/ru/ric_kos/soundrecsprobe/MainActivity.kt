package ru.ric_kos.soundrecsprobe

import android.Manifest
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.widget.Button
import android.widget.TextView
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.io.File
import java.text.SimpleDateFormat
import java.util.*
import androidx.activity.compose.setContent
import ru.ric_kos.soundrecsprobe.ui.RecordPlayScreen

class MainActivity : AppCompatActivity() {
    var fPath = ""
    var fPath1 = ""
    lateinit var btnStart: Button
    lateinit var btnStop: Button
    lateinit var btnInfo: Button
    val RECORD_REQUEST = 11111
    val ALL_PERMISSIONS = arrayOf(
        Manifest.permission.RECORD_AUDIO,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.WRITE_EXTERNAL_STORAGE
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
   // setContent {
   //    RecordPlayScreen()
   // }

        // Example of a call to a native method
        findViewById<TextView>(R.id.sample_text).text = stringFromJNI()

         btnStart = findViewById<Button>(R.id.btnStart)
         btnStop = findViewById<Button>(R.id.btnStop)
        btnStart.isEnabled = true
        btnStop.isEnabled = false
        btnInfo = findViewById<Button>(R.id.btnInfo)
        checkALLPermissions()
        val folder = File(getExternalFilesDir(null),"Recs")
        if (folder.exists()) {
            if (!folder.isDirectory) {
                folder.mkdir()
            }
        } else {
            folder.mkdir()
        }
        val timeStamp: String = SimpleDateFormat("yyyyMMdd_HHmmss", Locale("ru")).format(Date())


        fPath = getExternalFilesDir(null).toString() + "/Recs/${timeStamp}_record.wav"
        fPath1 = getExternalFilesDir(null).toString() + "/Recs/"
        btnStart.setOnClickListener{
                threadStartRecord()
        }

        btnStop.setOnClickListener{
            threadStopRecord()

        }
        btnInfo.setOnClickListener{

            val f = (folder);
            val files: Array<File> = f.listFiles()!!
            for (file in f.listFiles()!!) {
                Log.e("RAN-W", file.toString())
            }
        }
    }

    private fun threadStopRecord() {
        Thread(Runnable { stopRecording() }).start()
        btnStart.isEnabled = true
        btnStop.isEnabled = false
    }

    private fun threadStartRecord() {
        Thread(Runnable {
            startRecording(fPath)
        }).start()
        btnStart.isEnabled = false
        btnStop.isEnabled = true
    }


    fun checkALLPermissions(){
        ActivityCompat.requestPermissions(this, ALL_PERMISSIONS, RECORD_REQUEST)

    }
    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (RECORD_REQUEST != requestCode) {
            return
        }
    /*    when (requestCode) {
            1 -> {
                if (grantResults.isEmpty() || grantResults[0] != PackageManager.PERMISSION_GRANTED) {

                } else {


                }
            }
            2 -> {
                if (grantResults.isEmpty() || grantResults[0] != PackageManager.PERMISSION_GRANTED) {

                } else {

                }
            }
            3 -> {
                if (grantResults.isEmpty() || grantResults[0] != PackageManager.PERMISSION_GRANTED) {

                } else {

                }
            }
        }*/
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun stopRecording(): Boolean
    external fun startRecording(fPathToFile: String): Boolean
    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}