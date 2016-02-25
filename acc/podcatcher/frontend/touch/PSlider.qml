
/**
 *
 * gPodder QML UI Reference Implementation
 * Copyright (c) 2013, Thomas Perl <m@thp.io>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

import QtQuick 2.0

import 'common/constants.js' as Constants

Item {
    id: slider

    property real value
    property real min: 0.0
    property real max: 1.0
    property real step: 0.0
    property color color: Constants.colors.highlight

    property real displayedValue: mouseArea.pressed ? temporaryValue : value
    property real temporaryValue

    signal valueChangeRequested(real newValue)

    clip: true

    height: 50 * pgst.scalef

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        function updateValue(x) {
            if (x > width) {
                x = width;
            } else if (x < 0) {
                x = 0;
            }
            var v = (max - min) * (x / width);
            if (slider.step > 0.0) {
                v = slider.step * parseInt(0.5 + v / slider.step);
            }
            slider.temporaryValue = min + v;
            return slider.temporaryValue;
        }
        onClicked: slider.valueChangeRequested(updateValue(mouse.x));
        onPressed: updateValue(mouse.x);
        onPositionChanged: updateValue(mouse.x);
        preventStealing: true
    }

    Rectangle {
        anchors.fill: parent
        color: slider.color
        opacity: .3
    }

    Rectangle {
        id: fillBackground
        color: slider.color
        height: parent.height
        width: parent.width * (parent.displayedValue - parent.min) / (parent.max - parent.min)

        anchors {
            verticalCenter: parent.verticalCenter
        }
    }

    Repeater {
        model: slider.step ? ((slider.max - slider.min) / slider.step - 1) : 0
        delegate: Rectangle {
            width: Math.max(1, 1 * pgst.scalef)
            height: parent.height
            color: Constants.colors.area
            x: parent.width * ((slider.step * (1 + index)) / (slider.max - slider.min));
        }
    }
}
