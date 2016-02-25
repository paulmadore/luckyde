
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

Item {
    id: stacking
    property variant page: parent

    PropertyAnimation {
        id: fadeIn
        target: stacking.page
        property: 'opacity'
        to: 1
        duration: 500
        easing.type: Easing.OutCubic

        onStopped: {
            pgst.loadPageInProgress = false;
        }
    }

    PropertyAnimation {
        id: fadeOut
        target: stacking.page
        property: 'opacity'
        to: 0
        duration: 500
        easing.type: Easing.OutCubic
    }

    function startFadeOut() {
        fadeOut.start();
        pgst.topOfStackChanged(-1);
        page.destroy(500);
    }

    function fadeInAgain() {
        fadeIn.start();
    }

    function stopAllAnimations() {
        fadeIn.stop();
    }

    Component.onCompleted: {
        if (pgst.loadPageInProgress) {
            page.x = 0;
            page.opacity = 0;
            fadeIn.start();
        }
    }
}

