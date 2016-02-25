
/**
 *
 * gPodder QML UI Reference Implementation
 * Copyright (c) 2014, Thomas Perl <m@thp.io>
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
import 'common'

import 'common/constants.js' as Constants
import 'icons/icons.js' as Icons

Rectangle {
    id: toolbar
    property bool showing: true

    color: platform.invertedToolbar ? Constants.colors.inverted.toolbar : Constants.colors.toolbar

    height: 80 * pgst.scalef

    MouseArea {
        // Capture all touch events
        anchors.fill: parent
    }

    anchors {
        left: parent.left
        right: parent.right
        topMargin: toolbar.showing ? 0 : -toolbar.height
        bottomMargin: toolbar.showing ? 0 : -toolbar.height
    }

    Behavior on anchors.bottomMargin { PropertyAnimation { duration: 100 } }
    Behavior on anchors.topMargin { PropertyAnimation { duration: 100 } }
}
