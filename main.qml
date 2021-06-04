import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.qmlmodels 1.0

import MyTableModel 0.1

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Game of Life"

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Generate"

                // Slots are named onSIGNALNAME
                onClicked: {
                    console.log("generate")
                }
            }
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "Generate"
                onClicked: tableModel.generate()
            }
            ToolButton {
                text: "Start"
                onClicked: tableModel.start()
            }
            ToolButton {
                text: "Stop"
                onClicked: tableModel.stop()
            }

            // Fill up rest of the space
            Item {
                Layout.fillWidth: true
            }
        }
    }

    // use "footer" for status bar etc.

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Label {
                text: "Width"
            }
            SpinBox {
                editable: true
                from: 3
                to: 1000

                // Values are binded. Which means whenever "tableModel.gridWidth" changes, QML will also set "value"
                value: tableModel.gridWidth

                // We will update tableModel whenever the value was edited on QML side
                onValueChanged: tableModel.setGridWidth(value)
            }
            Label {
                text: "Height"
            }
            SpinBox {
                editable: true
                from: 3
                to: 1000
                value: tableModel.gridHeight
                onValueChanged: tableModel.setGridHeight(value)
            }
            Button {
                text: "Generate"

                // We can call slots on C++ side directly
                onClicked: tableModel.generate()
            }
            Label {
                text: "Interval (ms)"
            }
            SpinBox {
                editable: true
                from: 10
                to: 1000
                value: tableModel.interval
                onValueChanged: tableModel.setInterval(value)
            }
            Button {
                text: "Single Iteration"
                onClicked: tableModel.singleIteration()
            }
        }

        TableView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            // columnWidthProvider is called for each column to get the width
            columnWidthProvider: function (column) { return cellSize() }

            // rowHeightProvider is called for each row to get the height
            // We want square cells, so return same value on both
            rowHeightProvider: function (row) { return cellSize() }

            // We can define functions inside items
            function cellSize() {
                if(tableModel.columnCount() === 0)
                    return 1;
                if(tableModel.rowCount() === 0)
                    return 1;
                var cellWidth = view.width/tableModel.columnCount();
                var cellHeight = view.height/tableModel.rowCount();
                if(cellWidth<cellHeight)
                    return cellWidth;
                return cellHeight;
            }

            // MyTableModel is the C++ class TableModel
            // It is instantiated here
            model: MyTableModel {
                id: tableModel
            }

            // Delegate is the template for each cell
            delegate: cellDelegate
        }

        // Component items are not instantiated. They are the template. TableView will create an Item from this template for each cell.
        Component {
            id: cellDelegate

            Rectangle {
                // In delegates, we access model data as model.ROLE
                // Here we are using default role : display
                color: model.display ? "lightgray" : "black"

                MouseArea {
                    anchors.fill: parent
                    
                    // Model data is writable
                    onClicked: {model.display = !model.display;}
                }
            }
        }
    }
}
