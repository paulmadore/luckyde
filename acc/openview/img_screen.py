# imgv screen functions by Ryan Kulla, rkulla@gmail.com
import gl
from pygame.image import load
from pygame.display import update, set_icon
from pygame.draw import line
from status_bar import img_info
from pygame.locals import Rect


def paint_screen(screen, color, *rect):
    if not rect:
        screen.fill(color)
        update()
    else:
        screen.fill(color, rect)
        update(rect)


def junk_rect():
    return (Rect(-1, -1, -1, -1))


def clean_screen():
    from pygame.display import quit
    quit()
    from pygame.font import quit
    quit()


def init_screen():
    import pygame
    pygame.init() # seems needed for Mac OSX
    pygame.display.init()
    pygame.font.init()
    pygame.display.set_icon(load(gl.DATA_DIR + "imgv-icon.png"))


def my_update_screen(new_img, screen, rect, file, num_imgs, *ns):
    screen.fill(gl.IMGV_COLOR)
    screen.blit(new_img, rect)
    update()
#    rect = get_center(screen, new_img)
    if not ns:
        "ns wasn't passed, store last ns value in ns"
        ns = gl.NS_GLOBAL
    else:
        "ns was passed update gl.NS_GLOBAL"
        gl.NS_GLOBAL = ns
    try:
        if gl.IMG_BORDER:
            img_border(screen, new_img.get_width(), new_img.get_height(), rect[0], rect[1])
        img_info(screen, gl.files[file], file, num_imgs, new_img, ns[0])
    except:
        pass

        
def get_center(screen, new_img):
    "find out where the center of the screen is"
    screen_center = screen.get_rect().center
    rect = new_img.get_rect()
    rect.center = screen_center
    return rect


def img_border(screen, img_width, img_height, wpos, downpos):
    "draw a border around the image"
    ll = line(screen, gl.IMG_BORDER_COLOR, (wpos, downpos), (wpos, img_height + downpos + 2)) # left side of border
    rl = line(screen, gl.IMG_BORDER_COLOR, (wpos + img_width, downpos), (wpos + img_width, img_height + downpos + 2)) # right side
    tl = line(screen, gl.IMG_BORDER_COLOR, (wpos, downpos), ((wpos + img_width), downpos)) # top of border
    bl = line(screen, gl.IMG_BORDER_COLOR, (wpos, img_height + downpos + 2), ((wpos + img_width), img_height + downpos + 2)) # bottom
    update(ll)
    update(tl)
    update(rl)
    update(bl)
    # save the current border positions to be able to draw over them later:
    gl.LRECT = ll
    gl.RRECT = rl
    gl.TRECT = tl
    gl.BRECT = bl