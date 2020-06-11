import QtQuick 2.15
import QtQuick.Window 2.15
import com.sysmon 1.0

Window {
    id: root

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    readonly property int elementWidth: root.width
    readonly property int elementHeight: 20

    SystemInformation{
        id: sysinfo
    }

    Column{

        OSDisplay{
            width: root.elementWidth
            height: root.elementHeight
            osName: sysinfo.operatingSystem
        }
        KernelDisplay{
            width: root.elementWidth
            height: root.elementHeight

            kernelName: "5.3.0-51-generic"
        }
        CPUUtilizationDisplay{
            width: root.elementWidth
            height: root.elementHeight

            progressBarColor: "#3399FF" // blue
            minMaxTextColor: "blue"

            value: 50.0
        }
        CoreUtilizationDisplay{
            width: root.elementWidth
            height: root.elementHeight
            core: 1

            progressBarColor: "#3399FF" // blue
            minMaxTextColor: "blue"
        }
        CoreUtilizationDisplay{
            width: root.elementWidth
            height: root.elementHeight
            core: 2

            progressBarColor: "#3399FF" // blue
            minMaxTextColor: "blue"
        }
        CoreUtilizationDisplay{
            width: root.elementWidth
            height: root.elementHeight
            core: 3

            progressBarColor: "#3399FF" // blue
            minMaxTextColor: "blue"
        }
        CoreUtilizationDisplay{
            width: root.elementWidth
            height: root.elementHeight
            core: 4

            progressBarColor: "#3399FF" // blue
            minMaxTextColor: "blue"
        }
        MemoryDisplay{
            width: root.elementWidth
            height: root.elementHeight

            nonCachedNonBufferValue: 60
            bufferValue: 15
            cachedValue: 10

            nonCachedNonBufferColor: "#3399FF"
            bufferValueColor: "yellow"
            cachedColor: "green"
            emptyColor: "lightgray"
            minMaxTextColor: "blue"

        }
        SwapDisplay{
            width: root.elementWidth
            height: root.elementHeight

            progressBarColor: "#FF9933" // orange
            minMaxTextColor: "#FF8000" // orange

            value: 70
        }
        TotalProcessesDisplay{
            id: totalProcessesDisplay

            width: root.elementWidth
            height: root.elementHeight

            function set(){
                countOfProcesses = sysinfo.totalProcesses
            }
        }
        RunningProcessesDisplay{
            id: runningProcessesDisplay

            width: root.elementWidth
            height: root.elementHeight

            function set() {
                countOfRunningProcesses = sysinfo.runningProcesses
            }
        }
        UpTimeDisplay{
            id: upTimeDisplay

            width: root.elementWidth
            height: root.elementHeight

            function set() {
                upTimeInSeconds = sysinfo.UpTime
            }
        }
    }

    function update()
    {
        totalProcessesDisplay.set();
        runningProcessesDisplay.set();
        upTimeDisplay.set();
    }

    Timer {
        id: updateTimer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: update()
    }
}
