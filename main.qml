import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtMultimedia 5.8
import com.amin.classes 1.0

ApplicationWindow
{
    visible: true
    width: 800
    height: 600
    title: qsTr("Template Matching")

    CvDetectFilter
    {
        id: testFilter
        onObjectDetected:
        {
//            if((w == 0) || (h == 0))
//            {
//                smile.visible = false;
//            }
//            else
//            {
//                var r = video.mapNormalizedRectToItem(Qt.rect(x, y, w, h));
//                smile.x = r.x;
//                smile.y = r.y;
//                smile.width = r.width;
//                smile.height = r.height;
//                smile.visible = true;
//            }
        }
    }

    MediaPlayer
    {
        id: mediaPlayer
        source: "file:///home/vm/imagia/field.mp4"
        autoPlay: true
        onStopped: console.time("mediaPlayer stop!");
    }

    ColumnLayout
    {
        anchors.fill: parent
        VideoOutput
        {
            id: video
            Layout.fillHeight: true
            Layout.fillWidth: true
            source: mediaPlayer
            autoOrientation: false

            filters: [testFilter]

            Image
            {
                id: smile
                source: "qrc:/smile.png"
                visible: false
            }
        }

        RowLayout
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            Button
            {
                text: "Ok"
                Layout.fillWidth: true
                onClicked:
                {
                    mediaPlayer.play();
                }
            }

        }
    }
}
