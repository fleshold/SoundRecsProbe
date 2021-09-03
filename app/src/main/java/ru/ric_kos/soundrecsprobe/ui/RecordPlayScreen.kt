package ru.ric_kos.soundrecsprobe.ui

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.Button
import androidx.compose.material.ButtonDefaults
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.colorResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import ru.ric_kos.soundrecsprobe.R

@Composable
fun RecordPlayScreen(){
    var isRecording = remember { mutableStateOf(false) }
    Surface (
    color = Color.White,
    modifier = Modifier.fillMaxSize()
   ){
        Column(
            verticalArrangement = Arrangement.Top,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            RecordPlayScreenButton(stringResource(id = R.string.btn_start), true)
            RecordPlayScreenButton(stringResource(id = R.string.btn_stop), false)
        }
   }
}
@Composable
fun RecordPlayScreenButton(text: String, isRecording: Boolean){

    Button(

        modifier = Modifier
            .fillMaxWidth()
            .padding(start = 16.dp, end = 16.dp, top = 16.dp),
        shape = RoundedCornerShape(4.dp),
//        enabled = isRecording.value,
        colors = ButtonDefaults.buttonColors(backgroundColor = colorResource(id = R.color.teal_700)),
        onClick = { }
    ) {
        Text(
            text = text,
            color = Color.White
        )
    }
}
@Composable
fun AppDrawerPreview() {
    RecordPlayScreen()
}
