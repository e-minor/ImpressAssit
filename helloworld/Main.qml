import QtQuick 2.0
import Ubuntu.Components 1.1
import Helloworld 1.0
/*!
    \brief MainView with a Label and Button elements.
*/

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "helloworld.username"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    //automaticOrientation: true

    // Removes the old toolbar and enables new features of the new header.
    useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(75)

    Page {
        title: i18n.tr("helloworld")

        Impress {
            id: impress

            Component.onCompleted: {
                impress.helloWorld = i18n.tr("Hello world..")
            }
        }

        Column {
            spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }

            Label {
                id: label
                objectName: "label"

                text: impress.helloWorld
            }

            Button {
                objectName: "connect"
                width: parent.width
                text: i18n.tr("connect")

                onClicked: {
                    impress.connect_server();
                }
            }

            Button {
                objectName: "button"
                width: parent.width

                text: i18n.tr("Tap me!")

                onClicked: {
                    impress.helloWorld = i18n.tr("..from Cpp Backend")
                    impress.next_page();
                }
            }
        }
    }
}

