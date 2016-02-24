# image randomizer code by Ryan Kulla, rkulla@gmail.com
import gl
from cursor import wait_cursor, normal_cursor
from load_timers import start_timer, check_timer
from img_screen import get_center, my_update_screen
from load_img import load_img
from random import shuffle


def command_shuffle(new_img, img, screen, rect, file, num_imgs):
    "randomize the images"
    wait_cursor()
    start = start_timer()
    shuffle(gl.files)
    new_img = load_img(gl.files[file], screen)
    rect = get_center(screen, new_img)
    ns = check_timer(start)
    my_update_screen(new_img, screen, rect, file, num_imgs, ns)
    normal_cursor()
    return (new_img, new_img, new_img, rect)


def command_unshuffle(new_img, img, screen, rect, file, num_imgs):
    "un-randomize the images"
    was_on = gl.files[file]
    wait_cursor()
    start = start_timer()
    gl.files.sort(lambda x, y: cmp(x.lower(), y.lower()))
    file = gl.files.index(was_on)
    new_img = load_img(gl.files[file], screen)
    rect = get_center(screen, new_img)
    ns = check_timer(start)
    my_update_screen(new_img, screen, rect, file, num_imgs, ns)
    normal_cursor()
    return (new_img, new_img, new_img, rect, file)