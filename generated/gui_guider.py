# Copyright 2024 NXP
# NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct
import fs_driver

lv.init()
SDL.init(w=480,h=272)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(480*10)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 480
disp_drv.ver_res = 272
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_zoom(v)

def anim_img_rotate_cb(obj, v):
    obj.set_angle(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.font_load(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.img_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = e.get_current_target()
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RES.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    target = e.get_target()
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = target.get_screen()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = target.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, target), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    target = e.get_current_target()
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if target.get_pressed_date(date) == lv.RES.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.clear_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            target.delete()

# Create demo_gui
demo_gui = lv.obj()
demo_gui.set_size(480, 272)
demo_gui.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_tileview_1
demo_gui_tileview_1 = lv.tileview(demo_gui)
demo_gui_tileview_1_main = demo_gui_tileview_1.add_tile(0, 0, lv.DIR.BOTTOM)
demo_gui_tileview_1_settings = demo_gui_tileview_1.add_tile(0, 1, lv.DIR.TOP)
demo_gui_tileview_1.set_pos(0, 0)
demo_gui_tileview_1.set_size(800, 600)
demo_gui_tileview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_tileview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_tileview_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_tileview_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
demo_gui_tileview_1.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_1.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create demo_gui_tileview_2
demo_gui_tileview_2 = lv.tileview(demo_gui_tileview_1_main)
demo_gui_tileview_2_home_page = demo_gui_tileview_2.add_tile(0, 0, lv.DIR.RIGHT)
demo_gui_tileview_2_side_page = demo_gui_tileview_2.add_tile(1, 0, lv.DIR.LEFT)
demo_gui_tileview_2.set_pos(0, 0)
demo_gui_tileview_2.set_size(800, 600)
demo_gui_tileview_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_tileview_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_tileview_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_bg_color(lv.color_hex(0xf6f6f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_tileview_2, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
demo_gui_tileview_2.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_tileview_2.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create demo_gui_img_1
demo_gui_img_1 = lv.img(demo_gui_tileview_2_home_page)
demo_gui_img_1.set_src("B:MicroPython/_background_alpha_800x600.bin")
demo_gui_img_1.add_flag(lv.obj.FLAG.CLICKABLE)
demo_gui_img_1.set_pivot(50,50)
demo_gui_img_1.set_angle(0)
demo_gui_img_1.set_pos(0, 0)
demo_gui_img_1.set_size(800, 600)
# Set style for demo_gui_img_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_img_1.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_1
demo_gui_cont_1 = lv.obj(demo_gui_tileview_2_home_page)
demo_gui_cont_1.set_pos(572, 3)
demo_gui_cont_1.set_size(224, 44)
demo_gui_cont_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_2
demo_gui_imgbtn_2 = lv.imgbtn(demo_gui_cont_1)
demo_gui_imgbtn_2.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_2.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_itemperature_alpha_27x21.bin", None)
demo_gui_imgbtn_2.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_2_label = lv.label(demo_gui_imgbtn_2)
demo_gui_imgbtn_2_label.set_text("")
demo_gui_imgbtn_2_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_2_label.set_width(lv.pct(100))
demo_gui_imgbtn_2_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_2.set_pos(59, 10)
demo_gui_imgbtn_2.set_size(27, 21)
# Set style for demo_gui_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_2.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_2.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_2, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_2
demo_gui_label_2 = lv.label(demo_gui_cont_1)
demo_gui_label_2.set_text("25°C")
demo_gui_label_2.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_2.set_width(lv.pct(100))
demo_gui_label_2.set_pos(86, 11)
demo_gui_label_2.set_size(46, 21)
# Set style for demo_gui_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_1
demo_gui_imgbtn_1 = lv.imgbtn(demo_gui_cont_1)
demo_gui_imgbtn_1.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_1.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_ihumidity_alpha_16x15.bin", None)
demo_gui_imgbtn_1.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_1_label = lv.label(demo_gui_imgbtn_1)
demo_gui_imgbtn_1_label.set_text("")
demo_gui_imgbtn_1_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_1_label.set_width(lv.pct(100))
demo_gui_imgbtn_1_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_1.set_pos(154, 12)
demo_gui_imgbtn_1.set_size(16, 15)
# Set style for demo_gui_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_1.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_1.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_1, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_1
demo_gui_label_1 = lv.label(demo_gui_cont_1)
demo_gui_label_1.set_text("70%")
demo_gui_label_1.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_1.set_width(lv.pct(100))
demo_gui_label_1.set_pos(178, 12)
demo_gui_label_1.set_size(38, 21)
# Set style for demo_gui_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_2
demo_gui_cont_2 = lv.obj(demo_gui_tileview_2_home_page)
demo_gui_cont_2.set_pos(619, 496)
demo_gui_cont_2.set_size(178, 97)
demo_gui_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_4
demo_gui_label_4 = lv.label(demo_gui_cont_2)
demo_gui_label_4.set_text("11 May, 2024")
demo_gui_label_4.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_4.set_width(lv.pct(100))
demo_gui_label_4.set_pos(-30, -4)
demo_gui_label_4.set_size(204, 23)
# Set style for demo_gui_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_3
demo_gui_label_3 = lv.label(demo_gui_cont_2)
demo_gui_label_3.set_text("9:00 AM")
demo_gui_label_3.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_3.set_width(lv.pct(100))
demo_gui_label_3.set_pos(33, 35)
demo_gui_label_3.set_size(114, 32)
# Set style for demo_gui_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_font(test_font("Antonio_Regular", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_28
demo_gui_cont_28 = lv.obj(demo_gui_tileview_2_side_page)
demo_gui_cont_28.set_pos(0, 0)
demo_gui_cont_28.set_size(800, 600)
demo_gui_cont_28.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_28, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_28.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_bg_opa(244, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_28.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_41
demo_gui_cont_41 = lv.obj(demo_gui_cont_28)
demo_gui_cont_41.set_pos(0, 0)
demo_gui_cont_41.set_size(142, 600)
demo_gui_cont_41.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_41, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_41.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_border_color(lv.color_hex(0xa5aaae), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_41.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_29
demo_gui_label_29 = lv.label(demo_gui_cont_41)
demo_gui_label_29.set_text("Home ")
demo_gui_label_29.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_29.set_width(lv.pct(100))
demo_gui_label_29.set_pos(8, 25)
demo_gui_label_29.set_size(125, 45)
# Set style for demo_gui_label_29, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_29.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_border_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_font(test_font("Acme_Regular", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_pad_top(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_pad_left(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_29.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_42
demo_gui_cont_42 = lv.obj(demo_gui_cont_41)
demo_gui_cont_42.set_pos(6, 80)
demo_gui_cont_42.set_size(130, 49)
demo_gui_cont_42.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_42, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_42.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_border_color(lv.color_hex(0x4e9ee1), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_42.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_10
demo_gui_imgbtn_10 = lv.imgbtn(demo_gui_cont_42)
demo_gui_imgbtn_10.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_10.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_led_alpha_25x24.bin", None)
demo_gui_imgbtn_10.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_10_label = lv.label(demo_gui_imgbtn_10)
demo_gui_imgbtn_10_label.set_text("")
demo_gui_imgbtn_10_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_10_label.set_width(lv.pct(100))
demo_gui_imgbtn_10_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_10.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_10.set_pos(8, 10)
demo_gui_imgbtn_10.set_size(25, 24)
# Set style for demo_gui_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_10.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_10.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_10.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_10.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_10, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_btn_5
demo_gui_btn_5 = lv.btn(demo_gui_cont_42)
demo_gui_btn_5_label = lv.label(demo_gui_btn_5)
demo_gui_btn_5_label.set_text("Lights")
demo_gui_btn_5_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_btn_5_label.set_width(lv.pct(100))
demo_gui_btn_5_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_btn_5.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_btn_5.set_pos(35, 8)
demo_gui_btn_5.set_size(51, 29)
# Set style for demo_gui_btn_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_btn_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_43
demo_gui_cont_43 = lv.obj(demo_gui_cont_41)
demo_gui_cont_43.set_pos(6, 140)
demo_gui_cont_43.set_size(130, 49)
demo_gui_cont_43.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_43, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_43.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_border_color(lv.color_hex(0x4e9ee1), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_43.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_11
demo_gui_imgbtn_11 = lv.imgbtn(demo_gui_cont_43)
demo_gui_imgbtn_11.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_11.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_switch_alpha_25x24.bin", None)
demo_gui_imgbtn_11.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_11_label = lv.label(demo_gui_imgbtn_11)
demo_gui_imgbtn_11_label.set_text("")
demo_gui_imgbtn_11_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_11_label.set_width(lv.pct(100))
demo_gui_imgbtn_11_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_11.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_11.set_pos(6, 11)
demo_gui_imgbtn_11.set_size(25, 24)
# Set style for demo_gui_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_11.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_11.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_11.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_11.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_11, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_btn_6
demo_gui_btn_6 = lv.btn(demo_gui_cont_43)
demo_gui_btn_6_label = lv.label(demo_gui_btn_6)
demo_gui_btn_6_label.set_text("Switchs")
demo_gui_btn_6_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_btn_6_label.set_width(lv.pct(100))
demo_gui_btn_6_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_btn_6.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_btn_6.set_pos(35, 8)
demo_gui_btn_6.set_size(59, 29)
# Set style for demo_gui_btn_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_btn_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_44
demo_gui_cont_44 = lv.obj(demo_gui_cont_41)
demo_gui_cont_44.set_pos(6, 203)
demo_gui_cont_44.set_size(130, 49)
demo_gui_cont_44.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_44, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_44.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_border_color(lv.color_hex(0x4e9ee1), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_44.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_12
demo_gui_imgbtn_12 = lv.imgbtn(demo_gui_cont_44)
demo_gui_imgbtn_12.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_12.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_25x24.bin", None)
demo_gui_imgbtn_12.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_12_label = lv.label(demo_gui_imgbtn_12)
demo_gui_imgbtn_12_label.set_text("")
demo_gui_imgbtn_12_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_12_label.set_width(lv.pct(100))
demo_gui_imgbtn_12_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_12.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_12.set_pos(3, 14)
demo_gui_imgbtn_12.set_size(25, 24)
# Set style for demo_gui_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_12.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_12.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_12.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_12.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_12, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_btn_7
demo_gui_btn_7 = lv.btn(demo_gui_cont_44)
demo_gui_btn_7_label = lv.label(demo_gui_btn_7)
demo_gui_btn_7_label.set_text("Set timer")
demo_gui_btn_7_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_btn_7_label.set_width(lv.pct(100))
demo_gui_btn_7_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_btn_7.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_btn_7.set_pos(31, 10)
demo_gui_btn_7.set_size(74, 29)
# Set style for demo_gui_btn_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_btn_7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_33
demo_gui_cont_33 = lv.obj(demo_gui_cont_28)
demo_gui_cont_33.set_pos(208, 38)
demo_gui_cont_33.set_size(525, 515)
demo_gui_cont_33.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_33.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_33, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_33.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_33.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_45
demo_gui_cont_45 = lv.obj(demo_gui_cont_33)
demo_gui_cont_45.set_pos(118, 23)
demo_gui_cont_45.set_size(287, 43)
demo_gui_cont_45.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_45, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_45.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_45.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_13
demo_gui_imgbtn_13 = lv.imgbtn(demo_gui_cont_45)
demo_gui_imgbtn_13.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_13.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_39x37.bin", None)
demo_gui_imgbtn_13.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_onlight_alpha_39x37.bin", None)
demo_gui_imgbtn_13.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_13_label = lv.label(demo_gui_imgbtn_13)
demo_gui_imgbtn_13_label.set_text("")
demo_gui_imgbtn_13_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_13_label.set_width(lv.pct(100))
demo_gui_imgbtn_13_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_13.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_13.set_pos(6, 2)
demo_gui_imgbtn_13.set_size(39, 37)
# Set style for demo_gui_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_13.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_13.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_13.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_13.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_13, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_30
demo_gui_label_30 = lv.label(demo_gui_cont_45)
demo_gui_label_30.set_text("Light 1")
demo_gui_label_30.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_30.set_width(lv.pct(100))
demo_gui_label_30.set_pos(58, 11)
demo_gui_label_30.set_size(62, 21)
# Set style for demo_gui_label_30, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_30.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_30.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_31
demo_gui_label_31 = lv.label(demo_gui_cont_45)
demo_gui_label_31.set_text("Off")
demo_gui_label_31.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_31.set_width(lv.pct(100))
demo_gui_label_31.set_pos(227, 13)
demo_gui_label_31.set_size(33, 21)
# Set style for demo_gui_label_31, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_31.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_31.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_32
demo_gui_label_32 = lv.label(demo_gui_cont_45)
demo_gui_label_32.set_text("On")
demo_gui_label_32.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_32.set_width(lv.pct(100))
demo_gui_label_32.set_pos(227, 12)
demo_gui_label_32.set_size(33, 21)
demo_gui_label_32.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for demo_gui_label_32, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_32.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_32.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_46
demo_gui_cont_46 = lv.obj(demo_gui_cont_33)
demo_gui_cont_46.set_pos(117, 86)
demo_gui_cont_46.set_size(287, 43)
demo_gui_cont_46.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_46, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_46.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_46.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_14
demo_gui_imgbtn_14 = lv.imgbtn(demo_gui_cont_46)
demo_gui_imgbtn_14.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_14.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_39x37.bin", None)
demo_gui_imgbtn_14.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_onlight_alpha_39x37.bin", None)
demo_gui_imgbtn_14.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_14_label = lv.label(demo_gui_imgbtn_14)
demo_gui_imgbtn_14_label.set_text("")
demo_gui_imgbtn_14_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_14_label.set_width(lv.pct(100))
demo_gui_imgbtn_14_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_14.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_14.set_pos(6, 2)
demo_gui_imgbtn_14.set_size(39, 37)
# Set style for demo_gui_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_14.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_14.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_14.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_14, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_35
demo_gui_label_35 = lv.label(demo_gui_cont_46)
demo_gui_label_35.set_text("Light 2")
demo_gui_label_35.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_35.set_width(lv.pct(100))
demo_gui_label_35.set_pos(58, 11)
demo_gui_label_35.set_size(60, 21)
# Set style for demo_gui_label_35, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_35.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_35.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_34
demo_gui_label_34 = lv.label(demo_gui_cont_46)
demo_gui_label_34.set_text("Off")
demo_gui_label_34.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_34.set_width(lv.pct(100))
demo_gui_label_34.set_pos(227, 13)
demo_gui_label_34.set_size(33, 21)
# Set style for demo_gui_label_34, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_34.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_34.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_33
demo_gui_label_33 = lv.label(demo_gui_cont_46)
demo_gui_label_33.set_text("On")
demo_gui_label_33.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_33.set_width(lv.pct(100))
demo_gui_label_33.set_pos(227, 12)
demo_gui_label_33.set_size(33, 21)
demo_gui_label_33.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for demo_gui_label_33, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_33.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_33.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_47
demo_gui_cont_47 = lv.obj(demo_gui_cont_33)
demo_gui_cont_47.set_pos(118, 148)
demo_gui_cont_47.set_size(287, 43)
demo_gui_cont_47.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_47, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_47.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_47.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_imgbtn_15
demo_gui_imgbtn_15 = lv.imgbtn(demo_gui_cont_47)
demo_gui_imgbtn_15.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_15.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_39x37.bin", None)
demo_gui_imgbtn_15.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_onlight_alpha_39x37.bin", None)
demo_gui_imgbtn_15.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_15_label = lv.label(demo_gui_imgbtn_15)
demo_gui_imgbtn_15_label.set_text("")
demo_gui_imgbtn_15_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_15_label.set_width(lv.pct(100))
demo_gui_imgbtn_15_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_15.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_15.set_pos(6, 2)
demo_gui_imgbtn_15.set_size(39, 37)
# Set style for demo_gui_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_15.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_15.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_15.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_15.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_15, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_38
demo_gui_label_38 = lv.label(demo_gui_cont_47)
demo_gui_label_38.set_text("Light 3")
demo_gui_label_38.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_38.set_width(lv.pct(100))
demo_gui_label_38.set_pos(58, 11)
demo_gui_label_38.set_size(66, 21)
# Set style for demo_gui_label_38, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_38.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_38.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_37
demo_gui_label_37 = lv.label(demo_gui_cont_47)
demo_gui_label_37.set_text("Off")
demo_gui_label_37.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_37.set_width(lv.pct(100))
demo_gui_label_37.set_pos(227, 13)
demo_gui_label_37.set_size(33, 21)
# Set style for demo_gui_label_37, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_37.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_37.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_36
demo_gui_label_36 = lv.label(demo_gui_cont_47)
demo_gui_label_36.set_text("On")
demo_gui_label_36.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_36.set_width(lv.pct(100))
demo_gui_label_36.set_pos(227, 12)
demo_gui_label_36.set_size(33, 21)
demo_gui_label_36.add_flag(lv.obj.FLAG.HIDDEN)
# Set style for demo_gui_label_36, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_36.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_36.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_48
demo_gui_cont_48 = lv.obj(demo_gui_cont_28)
demo_gui_cont_48.set_pos(208, 39)
demo_gui_cont_48.set_size(525, 515)
demo_gui_cont_48.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_48.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_48, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_48.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_48.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_51
demo_gui_cont_51 = lv.obj(demo_gui_cont_48)
demo_gui_cont_51.set_pos(114, 23)
demo_gui_cont_51.set_size(285, 43)
demo_gui_cont_51.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_51, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_51.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_51.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_47
demo_gui_label_47 = lv.label(demo_gui_cont_51)
demo_gui_label_47.set_text("Switch 1")
demo_gui_label_47.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_47.set_width(lv.pct(100))
demo_gui_label_47.set_pos(70, 10)
demo_gui_label_47.set_size(74, 21)
# Set style for demo_gui_label_47, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_47.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_47.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_sw_3
demo_gui_sw_3 = lv.switch(demo_gui_cont_51)
demo_gui_sw_3.set_pos(15, 11)
demo_gui_sw_3.set_size(40, 20)
# Set style for demo_gui_sw_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_sw_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_sw_3, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
demo_gui_sw_3.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_3.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_3.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for demo_gui_sw_3, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
demo_gui_sw_3.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_3.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create demo_gui_cont_52
demo_gui_cont_52 = lv.obj(demo_gui_cont_48)
demo_gui_cont_52.set_pos(116, 77)
demo_gui_cont_52.set_size(287, 43)
demo_gui_cont_52.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_52, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_52.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_52.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_48
demo_gui_label_48 = lv.label(demo_gui_cont_52)
demo_gui_label_48.set_text("Switch 2")
demo_gui_label_48.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_48.set_width(lv.pct(100))
demo_gui_label_48.set_pos(70, 10)
demo_gui_label_48.set_size(74, 21)
# Set style for demo_gui_label_48, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_48.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_48.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_sw_4
demo_gui_sw_4 = lv.switch(demo_gui_cont_52)
demo_gui_sw_4.set_pos(15, 11)
demo_gui_sw_4.set_size(40, 20)
# Set style for demo_gui_sw_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_sw_4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_sw_4, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
demo_gui_sw_4.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_4.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_4.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for demo_gui_sw_4, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
demo_gui_sw_4.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_4.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create demo_gui_cont_53
demo_gui_cont_53 = lv.obj(demo_gui_cont_48)
demo_gui_cont_53.set_pos(117, 134)
demo_gui_cont_53.set_size(287, 43)
demo_gui_cont_53.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_53, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_53.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_53.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_49
demo_gui_label_49 = lv.label(demo_gui_cont_53)
demo_gui_label_49.set_text("Switch 3")
demo_gui_label_49.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_49.set_width(lv.pct(100))
demo_gui_label_49.set_pos(70, 10)
demo_gui_label_49.set_size(74, 21)
# Set style for demo_gui_label_49, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_49.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_49.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_sw_5
demo_gui_sw_5 = lv.switch(demo_gui_cont_53)
demo_gui_sw_5.set_pos(15, 11)
demo_gui_sw_5.set_size(40, 20)
# Set style for demo_gui_sw_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_sw_5.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_sw_5, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
demo_gui_sw_5.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_5.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_5.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for demo_gui_sw_5, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
demo_gui_sw_5.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_5.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create demo_gui_cont_54
demo_gui_cont_54 = lv.obj(demo_gui_cont_28)
demo_gui_cont_54.set_pos(208, 38)
demo_gui_cont_54.set_size(525, 515)
demo_gui_cont_54.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_54.set_scrollbar_mode(lv.SCROLLBAR_MODE.ON)
# Set style for demo_gui_cont_54, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_54.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_54.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_57
demo_gui_cont_57 = lv.obj(demo_gui_cont_54)
demo_gui_cont_57.set_pos(13, 9)
demo_gui_cont_57.set_size(497, 137)
demo_gui_cont_57.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_57, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_57.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_57.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_52
demo_gui_label_52 = lv.label(demo_gui_cont_57)
demo_gui_label_52.set_text("Time/Date:")
demo_gui_label_52.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_52.set_width(lv.pct(100))
demo_gui_label_52.set_pos(31, 7)
demo_gui_label_52.set_size(109, 22)
# Set style for demo_gui_label_52, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_52.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_52.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_digital_clock_1
demo_gui_digital_clock_1_time = [int(11), int(25), int(50), "AM"]
demo_gui_digital_clock_1 = lv.dclock(demo_gui_cont_57, "11:25:50 AM")
demo_gui_digital_clock_1_timer = lv.timer_create_basic()
demo_gui_digital_clock_1_timer.set_period(1000)
demo_gui_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(demo_gui_digital_clock_1_timer, demo_gui_digital_clock_1, demo_gui_digital_clock_1_time, True, True ))
demo_gui_digital_clock_1.set_pos(184, 6)
demo_gui_digital_clock_1.set_size(97, 30)
# Set style for demo_gui_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_text_font(test_font("Antonio_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_datetext_1
demo_gui_datetext_1 = lv.label(demo_gui_cont_57)
demo_gui_datetext_1.set_text("2023/07/31")
demo_gui_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
demo_gui_datetext_1.add_flag(lv.obj.FLAG.CLICKABLE)
demo_gui_datetext_1_calendar = None
demo_gui_datetext_1.add_event_cb(lambda e: datetext_event_handler(e, demo_gui_datetext_1_calendar), lv.EVENT.ALL, None)
demo_gui_datetext_1.set_pos(309, 7)
demo_gui_datetext_1.set_size(101, 25)
# Set style for demo_gui_datetext_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_datetext_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_text_font(test_font("Antonio_Regular", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_58
demo_gui_cont_58 = lv.obj(demo_gui_cont_57)
demo_gui_cont_58.set_pos(28, 42)
demo_gui_cont_58.set_size(136, 31)
demo_gui_cont_58.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_58, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_58.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_58.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_53
demo_gui_label_53 = lv.label(demo_gui_cont_58)
demo_gui_label_53.set_text("Light 1")
demo_gui_label_53.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_53.set_width(lv.pct(100))
demo_gui_label_53.set_pos(32, 7)
demo_gui_label_53.set_size(62, 18)
# Set style for demo_gui_label_53, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_53.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_53.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_54
demo_gui_label_54 = lv.label(demo_gui_cont_58)
demo_gui_label_54.set_text("On")
demo_gui_label_54.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_54.set_width(lv.pct(100))
demo_gui_label_54.set_pos(101, 7)
demo_gui_label_54.set_size(29, 18)
# Set style for demo_gui_label_54, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_54.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_54.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_18
demo_gui_imgbtn_18 = lv.imgbtn(demo_gui_cont_58)
demo_gui_imgbtn_18.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_18.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_onlight_alpha_26x29.bin", None)
demo_gui_imgbtn_18.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_18_label = lv.label(demo_gui_imgbtn_18)
demo_gui_imgbtn_18_label.set_text("")
demo_gui_imgbtn_18_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_18_label.set_width(lv.pct(100))
demo_gui_imgbtn_18_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_18.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_18.set_pos(4, 0)
demo_gui_imgbtn_18.set_size(26, 29)
# Set style for demo_gui_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_18.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_18.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_18.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_18.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_18, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_59
demo_gui_cont_59 = lv.obj(demo_gui_cont_57)
demo_gui_cont_59.set_pos(314, 45)
demo_gui_cont_59.set_size(136, 31)
demo_gui_cont_59.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_59, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_59.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_59.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_56
demo_gui_label_56 = lv.label(demo_gui_cont_59)
demo_gui_label_56.set_text("Light 2")
demo_gui_label_56.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_56.set_width(lv.pct(100))
demo_gui_label_56.set_pos(32, 7)
demo_gui_label_56.set_size(62, 18)
# Set style for demo_gui_label_56, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_56.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_56.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_55
demo_gui_label_55 = lv.label(demo_gui_cont_59)
demo_gui_label_55.set_text("Off")
demo_gui_label_55.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_55.set_width(lv.pct(100))
demo_gui_label_55.set_pos(101, 7)
demo_gui_label_55.set_size(29, 18)
# Set style for demo_gui_label_55, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_55.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_55.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_19
demo_gui_imgbtn_19 = lv.imgbtn(demo_gui_cont_59)
demo_gui_imgbtn_19.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_19.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_28x29.bin", None)
demo_gui_imgbtn_19.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_19_label = lv.label(demo_gui_imgbtn_19)
demo_gui_imgbtn_19_label.set_text("")
demo_gui_imgbtn_19_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_19_label.set_width(lv.pct(100))
demo_gui_imgbtn_19_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_19.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_19.set_pos(4, 0)
demo_gui_imgbtn_19.set_size(28, 29)
# Set style for demo_gui_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_19.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_19.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_19.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_19.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_19, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_60
demo_gui_cont_60 = lv.obj(demo_gui_cont_57)
demo_gui_cont_60.set_pos(30, 90)
demo_gui_cont_60.set_size(136, 31)
demo_gui_cont_60.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_60, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_60.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_60.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_58
demo_gui_label_58 = lv.label(demo_gui_cont_60)
demo_gui_label_58.set_text("Switch 1")
demo_gui_label_58.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_58.set_width(lv.pct(100))
demo_gui_label_58.set_pos(32, 7)
demo_gui_label_58.set_size(62, 18)
# Set style for demo_gui_label_58, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_58.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_58.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_57
demo_gui_label_57 = lv.label(demo_gui_cont_60)
demo_gui_label_57.set_text("Off")
demo_gui_label_57.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_57.set_width(lv.pct(100))
demo_gui_label_57.set_pos(101, 7)
demo_gui_label_57.set_size(29, 18)
# Set style for demo_gui_label_57, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_57.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_57.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_20
demo_gui_imgbtn_20 = lv.imgbtn(demo_gui_cont_60)
demo_gui_imgbtn_20.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_20.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_iswitch_alpha_26x22.bin", None)
demo_gui_imgbtn_20.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_20_label = lv.label(demo_gui_imgbtn_20)
demo_gui_imgbtn_20_label.set_text("")
demo_gui_imgbtn_20_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_20_label.set_width(lv.pct(100))
demo_gui_imgbtn_20_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_20.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_20.set_pos(4, 2)
demo_gui_imgbtn_20.set_size(26, 22)
# Set style for demo_gui_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_20.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_20.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_20.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_20.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_20, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_61
demo_gui_cont_61 = lv.obj(demo_gui_cont_54)
demo_gui_cont_61.set_pos(13, 158)
demo_gui_cont_61.set_size(494, 137)
demo_gui_cont_61.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_61, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_61.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_radius(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_bg_opa(123, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_bg_color(lv.color_hex(0x7f7c7c), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_61.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_65
demo_gui_label_65 = lv.label(demo_gui_cont_61)
demo_gui_label_65.set_text("Time/Date:")
demo_gui_label_65.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_65.set_width(lv.pct(100))
demo_gui_label_65.set_pos(36, 10)
demo_gui_label_65.set_size(103, 22)
# Set style for demo_gui_label_65, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_65.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_65.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_digital_clock_2
demo_gui_digital_clock_2_time = [int(11), int(25), int(50), "AM"]
demo_gui_digital_clock_2 = lv.dclock(demo_gui_cont_61, "11:25:50 AM")
demo_gui_digital_clock_2_timer = lv.timer_create_basic()
demo_gui_digital_clock_2_timer.set_period(1000)
demo_gui_digital_clock_2_timer.set_cb(lambda src: digital_clock_cb(demo_gui_digital_clock_2_timer, demo_gui_digital_clock_2, demo_gui_digital_clock_2_time, True, True ))
demo_gui_digital_clock_2.set_pos(185, 7)
demo_gui_digital_clock_2.set_size(97, 30)
# Set style for demo_gui_digital_clock_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_digital_clock_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_text_font(test_font("Antonio_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_digital_clock_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_datetext_2
demo_gui_datetext_2 = lv.label(demo_gui_cont_61)
demo_gui_datetext_2.set_text("2023/07/31")
demo_gui_datetext_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, 0)
demo_gui_datetext_2.add_flag(lv.obj.FLAG.CLICKABLE)
demo_gui_datetext_2_calendar = None
demo_gui_datetext_2.add_event_cb(lambda e: datetext_event_handler(e, demo_gui_datetext_2_calendar), lv.EVENT.ALL, None)
demo_gui_datetext_2.set_pos(315, 6)
demo_gui_datetext_2.set_size(101, 25)
# Set style for demo_gui_datetext_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_datetext_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_text_font(test_font("Antonio_Regular", 15), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_datetext_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_64
demo_gui_cont_64 = lv.obj(demo_gui_cont_61)
demo_gui_cont_64.set_pos(34, 45)
demo_gui_cont_64.set_size(136, 31)
demo_gui_cont_64.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_64, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_64.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_64.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_64
demo_gui_label_64 = lv.label(demo_gui_cont_64)
demo_gui_label_64.set_text("Light 1")
demo_gui_label_64.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_64.set_width(lv.pct(100))
demo_gui_label_64.set_pos(32, 7)
demo_gui_label_64.set_size(62, 18)
# Set style for demo_gui_label_64, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_64.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_64.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_63
demo_gui_label_63 = lv.label(demo_gui_cont_64)
demo_gui_label_63.set_text("On")
demo_gui_label_63.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_63.set_width(lv.pct(100))
demo_gui_label_63.set_pos(101, 7)
demo_gui_label_63.set_size(29, 18)
# Set style for demo_gui_label_63, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_63.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_63.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_23
demo_gui_imgbtn_23 = lv.imgbtn(demo_gui_cont_64)
demo_gui_imgbtn_23.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_23.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_onlight_alpha_26x29.bin", None)
demo_gui_imgbtn_23.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_23_label = lv.label(demo_gui_imgbtn_23)
demo_gui_imgbtn_23_label.set_text("")
demo_gui_imgbtn_23_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_23_label.set_width(lv.pct(100))
demo_gui_imgbtn_23_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_23.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_23.set_pos(4, 0)
demo_gui_imgbtn_23.set_size(26, 29)
# Set style for demo_gui_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_23.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_23.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_23.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_23.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_23, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_63
demo_gui_cont_63 = lv.obj(demo_gui_cont_61)
demo_gui_cont_63.set_pos(313, 45)
demo_gui_cont_63.set_size(136, 31)
demo_gui_cont_63.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_63, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_63.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_63.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_62
demo_gui_label_62 = lv.label(demo_gui_cont_63)
demo_gui_label_62.set_text("Light 2")
demo_gui_label_62.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_62.set_width(lv.pct(100))
demo_gui_label_62.set_pos(32, 7)
demo_gui_label_62.set_size(62, 18)
# Set style for demo_gui_label_62, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_62.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_62.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_61
demo_gui_label_61 = lv.label(demo_gui_cont_63)
demo_gui_label_61.set_text("Off")
demo_gui_label_61.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_61.set_width(lv.pct(100))
demo_gui_label_61.set_pos(101, 7)
demo_gui_label_61.set_size(29, 18)
# Set style for demo_gui_label_61, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_61.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_61.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_22
demo_gui_imgbtn_22 = lv.imgbtn(demo_gui_cont_63)
demo_gui_imgbtn_22.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_22.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_28x29.bin", None)
demo_gui_imgbtn_22.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_22_label = lv.label(demo_gui_imgbtn_22)
demo_gui_imgbtn_22_label.set_text("")
demo_gui_imgbtn_22_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_22_label.set_width(lv.pct(100))
demo_gui_imgbtn_22_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_22.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_22.set_pos(4, 0)
demo_gui_imgbtn_22.set_size(28, 29)
# Set style for demo_gui_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_22.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_22.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_22.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_22.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_22, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_62
demo_gui_cont_62 = lv.obj(demo_gui_cont_61)
demo_gui_cont_62.set_pos(35, 90)
demo_gui_cont_62.set_size(136, 31)
demo_gui_cont_62.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_62, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_62.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_62.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_60
demo_gui_label_60 = lv.label(demo_gui_cont_62)
demo_gui_label_60.set_text("Switch 1")
demo_gui_label_60.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_60.set_width(lv.pct(100))
demo_gui_label_60.set_pos(32, 7)
demo_gui_label_60.set_size(62, 18)
# Set style for demo_gui_label_60, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_60.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_60.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_59
demo_gui_label_59 = lv.label(demo_gui_cont_62)
demo_gui_label_59.set_text("Off")
demo_gui_label_59.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_59.set_width(lv.pct(100))
demo_gui_label_59.set_pos(101, 7)
demo_gui_label_59.set_size(29, 18)
# Set style for demo_gui_label_59, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_59.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_59.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_21
demo_gui_imgbtn_21 = lv.imgbtn(demo_gui_cont_62)
demo_gui_imgbtn_21.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_21.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_iswitch_alpha_26x22.bin", None)
demo_gui_imgbtn_21.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_21_label = lv.label(demo_gui_imgbtn_21)
demo_gui_imgbtn_21_label.set_text("")
demo_gui_imgbtn_21_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_21_label.set_width(lv.pct(100))
demo_gui_imgbtn_21_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_imgbtn_21.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_21.set_pos(4, 2)
demo_gui_imgbtn_21.set_size(26, 22)
# Set style for demo_gui_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_21.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_21.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_21.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_21.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_21, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_65
demo_gui_cont_65 = lv.obj(demo_gui_tileview_1_settings)
demo_gui_cont_65.set_pos(0, 0)
demo_gui_cont_65.set_size(800, 600)
demo_gui_cont_65.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_65, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_65.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_bg_opa(244, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_65.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_78
demo_gui_cont_78 = lv.obj(demo_gui_cont_65)
demo_gui_cont_78.set_pos(0, 0)
demo_gui_cont_78.set_size(160, 600)
demo_gui_cont_78.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_78, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_78.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_border_color(lv.color_hex(0xa5aaae), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_78.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_82
demo_gui_cont_82 = lv.obj(demo_gui_cont_78)
demo_gui_cont_82.set_pos(7, 75)
demo_gui_cont_82.set_size(142, 35)
demo_gui_cont_82.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_82, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_82.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_bg_color(lv.color_hex(0x292323), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_82.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_76
demo_gui_label_76 = lv.label(demo_gui_cont_82)
demo_gui_label_76.set_text("Wifi\n")
demo_gui_label_76.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_76.set_width(lv.pct(100))
demo_gui_label_76.set_pos(8, 8)
demo_gui_label_76.set_size(49, 21)
# Set style for demo_gui_label_76, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_76.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_76.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_27
demo_gui_imgbtn_27 = lv.imgbtn(demo_gui_cont_82)
demo_gui_imgbtn_27.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_27.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_22x23.bin", None)
demo_gui_imgbtn_27.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_27_label = lv.label(demo_gui_imgbtn_27)
demo_gui_imgbtn_27_label.set_text("")
demo_gui_imgbtn_27_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_27_label.set_width(lv.pct(100))
demo_gui_imgbtn_27_label.align(lv.ALIGN.LEFT_MID, 0, 0)
demo_gui_imgbtn_27.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_27.set_pos(110, 8)
demo_gui_imgbtn_27.set_size(22, 23)
# Set style for demo_gui_imgbtn_27, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_27.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_27.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_27.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_27.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_27.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_27, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_27.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_27.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_27.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_27.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_27.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_27, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_27.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_27.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_27.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_27.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_27.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_27, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_27.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_label_75
demo_gui_label_75 = lv.label(demo_gui_cont_78)
demo_gui_label_75.set_text("Settings")
demo_gui_label_75.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_75.set_width(lv.pct(100))
demo_gui_label_75.set_pos(9, 12)
demo_gui_label_75.set_size(138, 45)
# Set style for demo_gui_label_75, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_75.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_border_color(lv.color_hex(0xa1a3a5), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_color(lv.color_hex(0x1b77bb), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_75.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_81
demo_gui_cont_81 = lv.obj(demo_gui_cont_78)
demo_gui_cont_81.set_pos(6, 123)
demo_gui_cont_81.set_size(142, 35)
demo_gui_cont_81.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_81, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_81.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_bg_color(lv.color_hex(0x292323), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_81.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_74
demo_gui_label_74 = lv.label(demo_gui_cont_81)
demo_gui_label_74.set_text("Bluetooth\n")
demo_gui_label_74.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_74.set_width(lv.pct(100))
demo_gui_label_74.set_pos(11, 8)
demo_gui_label_74.set_size(87, 21)
# Set style for demo_gui_label_74, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_74.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_74.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_26
demo_gui_imgbtn_26 = lv.imgbtn(demo_gui_cont_81)
demo_gui_imgbtn_26.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_26.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_24x22.bin", None)
demo_gui_imgbtn_26.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_26_label = lv.label(demo_gui_imgbtn_26)
demo_gui_imgbtn_26_label.set_text("")
demo_gui_imgbtn_26_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_26_label.set_width(lv.pct(100))
demo_gui_imgbtn_26_label.align(lv.ALIGN.LEFT_MID, 0, 0)
demo_gui_imgbtn_26.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_26.set_pos(109, 6)
demo_gui_imgbtn_26.set_size(24, 22)
# Set style for demo_gui_imgbtn_26, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_26.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_26.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_26.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_26.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_26.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_26, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_26.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_26.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_26.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_26.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_26.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_26, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_26.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_26.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_26.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_26.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_26.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_26, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_26.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_80
demo_gui_cont_80 = lv.obj(demo_gui_cont_78)
demo_gui_cont_80.set_pos(7, 218)
demo_gui_cont_80.set_size(142, 35)
demo_gui_cont_80.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_80, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_80.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_bg_color(lv.color_hex(0x292323), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_80.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_73
demo_gui_label_73 = lv.label(demo_gui_cont_80)
demo_gui_label_73.set_text("Notification\n\n")
demo_gui_label_73.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_73.set_width(lv.pct(100))
demo_gui_label_73.set_pos(7, 8)
demo_gui_label_73.set_size(102, 21)
# Set style for demo_gui_label_73, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_73.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_73.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_25
demo_gui_imgbtn_25 = lv.imgbtn(demo_gui_cont_80)
demo_gui_imgbtn_25.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_25.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_27x23.bin", None)
demo_gui_imgbtn_25.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_25_label = lv.label(demo_gui_imgbtn_25)
demo_gui_imgbtn_25_label.set_text("")
demo_gui_imgbtn_25_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_25_label.set_width(lv.pct(100))
demo_gui_imgbtn_25_label.align(lv.ALIGN.LEFT_MID, 0, 0)
demo_gui_imgbtn_25.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_25.set_pos(108, 6)
demo_gui_imgbtn_25.set_size(27, 23)
# Set style for demo_gui_imgbtn_25, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_25.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_25.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_25.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_25.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_25.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_25, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_25.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_25.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_25.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_25.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_25.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_25, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_25.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_25.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_25.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_25.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_25.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_25, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_25.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_79
demo_gui_cont_79 = lv.obj(demo_gui_cont_78)
demo_gui_cont_79.set_pos(7, 170)
demo_gui_cont_79.set_size(142, 35)
demo_gui_cont_79.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_79, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_79.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_bg_color(lv.color_hex(0x292323), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_79.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_72
demo_gui_label_72 = lv.label(demo_gui_cont_79)
demo_gui_label_72.set_text("Language\n")
demo_gui_label_72.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_72.set_width(lv.pct(100))
demo_gui_label_72.set_pos(7, 8)
demo_gui_label_72.set_size(92, 21)
# Set style for demo_gui_label_72, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_72.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_72.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_imgbtn_24
demo_gui_imgbtn_24 = lv.imgbtn(demo_gui_cont_79)
demo_gui_imgbtn_24.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_24.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_25x21.bin", None)
demo_gui_imgbtn_24.add_flag(lv.obj.FLAG.CHECKABLE)
demo_gui_imgbtn_24_label = lv.label(demo_gui_imgbtn_24)
demo_gui_imgbtn_24_label.set_text("")
demo_gui_imgbtn_24_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_imgbtn_24_label.set_width(lv.pct(100))
demo_gui_imgbtn_24_label.align(lv.ALIGN.LEFT_MID, 0, 0)
demo_gui_imgbtn_24.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_imgbtn_24.set_pos(108, 7)
demo_gui_imgbtn_24.set_size(25, 21)
# Set style for demo_gui_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_imgbtn_24.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_24.set_style_text_align(lv.TEXT_ALIGN.LEFT, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
demo_gui_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_24.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
demo_gui_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for demo_gui_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_24.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_imgbtn_24, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
demo_gui_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create demo_gui_cont_74
demo_gui_cont_74 = lv.obj(demo_gui_cont_65)
demo_gui_cont_74.set_pos(208, 38)
demo_gui_cont_74.set_size(525, 515)
demo_gui_cont_74.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_74, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_74.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_74.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_77
demo_gui_cont_77 = lv.obj(demo_gui_cont_74)
demo_gui_cont_77.set_pos(-3, 11)
demo_gui_cont_77.set_size(300, 31)
demo_gui_cont_77.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_77, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_77.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_77.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_71
demo_gui_label_71 = lv.label(demo_gui_cont_77)
demo_gui_label_71.set_text("Wifi")
demo_gui_label_71.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_71.set_width(lv.pct(100))
demo_gui_label_71.set_pos(7, 4)
demo_gui_label_71.set_size(56, 22)
# Set style for demo_gui_label_71, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_71.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_71.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_sw_7
demo_gui_sw_7 = lv.switch(demo_gui_cont_77)
demo_gui_sw_7.set_pos(80, 6)
demo_gui_sw_7.set_size(37, 18)
# Set style for demo_gui_sw_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_sw_7.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_sw_7, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
demo_gui_sw_7.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_7.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_7.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for demo_gui_sw_7, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
demo_gui_sw_7.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_7.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create demo_gui_cont_75
demo_gui_cont_75 = lv.obj(demo_gui_cont_74)
demo_gui_cont_75.set_pos(-1, 61)
demo_gui_cont_75.set_size(530, 122)
demo_gui_cont_75.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_75.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_75, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_75.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_75.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_76
demo_gui_cont_76 = lv.obj(demo_gui_cont_75)
demo_gui_cont_76.set_pos(0, -2)
demo_gui_cont_76.set_size(521, 48)
demo_gui_cont_76.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_76, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_76.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_76.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_btn_9
demo_gui_btn_9 = lv.btn(demo_gui_cont_76)
demo_gui_btn_9_label = lv.label(demo_gui_btn_9)
demo_gui_btn_9_label.set_text("Connect")
demo_gui_btn_9_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_btn_9_label.set_width(lv.pct(100))
demo_gui_btn_9_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_btn_9.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_btn_9.set_pos(441, 17)
demo_gui_btn_9.set_size(72, 28)
# Set style for demo_gui_btn_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_btn_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_text_color(lv.color_hex(0x1b77bb), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_text_font(test_font("montserratMedium", 13), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_9.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_70
demo_gui_label_70 = lv.label(demo_gui_cont_76)
demo_gui_label_70.set_text("Wifi AB")
demo_gui_label_70.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_70.set_width(lv.pct(100))
demo_gui_label_70.set_pos(13, 16)
demo_gui_label_70.set_size(69, 17)
# Set style for demo_gui_label_70, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_70.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_font(test_font("montserratMedium", 13), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_70.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_70
demo_gui_cont_70 = lv.obj(demo_gui_cont_65)
demo_gui_cont_70.set_pos(208, 38)
demo_gui_cont_70.set_size(525, 515)
demo_gui_cont_70.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_70.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_70, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_70.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_70.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_73
demo_gui_cont_73 = lv.obj(demo_gui_cont_70)
demo_gui_cont_73.set_pos(-3, 9)
demo_gui_cont_73.set_size(300, 31)
demo_gui_cont_73.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_73, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_73.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_73.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_69
demo_gui_label_69 = lv.label(demo_gui_cont_73)
demo_gui_label_69.set_text("Bluetooth")
demo_gui_label_69.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_69.set_width(lv.pct(100))
demo_gui_label_69.set_pos(7, 4)
demo_gui_label_69.set_size(91, 22)
# Set style for demo_gui_label_69, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_69.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_69.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_sw_6
demo_gui_sw_6 = lv.switch(demo_gui_cont_73)
demo_gui_sw_6.set_pos(125, 8)
demo_gui_sw_6.set_size(37, 18)
# Set style for demo_gui_sw_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_sw_6.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_sw_6, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
demo_gui_sw_6.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_6.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
demo_gui_sw_6.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for demo_gui_sw_6, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
demo_gui_sw_6.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
demo_gui_sw_6.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create demo_gui_cont_71
demo_gui_cont_71 = lv.obj(demo_gui_cont_70)
demo_gui_cont_71.set_pos(-1, 62)
demo_gui_cont_71.set_size(516, 143)
demo_gui_cont_71.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_71.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_71, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_71.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_71.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_72
demo_gui_cont_72 = lv.obj(demo_gui_cont_71)
demo_gui_cont_72.set_pos(0, -2)
demo_gui_cont_72.set_size(527, 48)
demo_gui_cont_72.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_72, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_72.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_72.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_btn_8
demo_gui_btn_8 = lv.btn(demo_gui_cont_72)
demo_gui_btn_8_label = lv.label(demo_gui_btn_8)
demo_gui_btn_8_label.set_text("Connect")
demo_gui_btn_8_label.set_long_mode(lv.label.LONG.WRAP)
demo_gui_btn_8_label.set_width(lv.pct(100))
demo_gui_btn_8_label.align(lv.ALIGN.CENTER, 0, 0)
demo_gui_btn_8.set_style_pad_all(0, lv.STATE.DEFAULT)
demo_gui_btn_8.set_pos(432, 15)
demo_gui_btn_8.set_size(72, 28)
# Set style for demo_gui_btn_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_btn_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_text_color(lv.color_hex(0x1b77bb), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_text_font(test_font("Amiko_Regular", 13), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_btn_8.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_label_68
demo_gui_label_68 = lv.label(demo_gui_cont_72)
demo_gui_label_68.set_text("Bluetooth 1")
demo_gui_label_68.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_68.set_width(lv.pct(100))
demo_gui_label_68.set_pos(1, 14)
demo_gui_label_68.set_size(105, 17)
# Set style for demo_gui_label_68, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_68.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_font(test_font("montserratMedium", 13), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_68.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_cont_68
demo_gui_cont_68 = lv.obj(demo_gui_cont_65)
demo_gui_cont_68.set_pos(208, 38)
demo_gui_cont_68.set_size(525, 515)
demo_gui_cont_68.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_68.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_68, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_68.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_68.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_69
demo_gui_cont_69 = lv.obj(demo_gui_cont_68)
demo_gui_cont_69.set_pos(-3, 9)
demo_gui_cont_69.set_size(300, 31)
demo_gui_cont_69.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_69, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_69.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_69.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_67
demo_gui_label_67 = lv.label(demo_gui_cont_69)
demo_gui_label_67.set_text("Language")
demo_gui_label_67.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_67.set_width(lv.pct(100))
demo_gui_label_67.set_pos(7, 4)
demo_gui_label_67.set_size(91, 22)
# Set style for demo_gui_label_67, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_67.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_67.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_ddlist_2
demo_gui_ddlist_2 = lv.dropdown(demo_gui_cont_68)
demo_gui_ddlist_2.set_options("Vietnamese\nEnglish")
demo_gui_ddlist_2.set_pos(4, 62)
demo_gui_ddlist_2.set_size(500, 30)
# Set style for demo_gui_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_ddlist_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_bg_opa(126, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_ddlist_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
demo_gui_ddlist_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_border_width(1, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_border_color(lv.color_hex(0x654f4f), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_radius(3, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.CHECKED)
demo_gui_ddlist_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for demo_gui_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.FOCUSED.
demo_gui_ddlist_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_border_width(1, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_border_color(lv.color_hex(0x654f4f), lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_radius(3, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.FOCUSED)
demo_gui_ddlist_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.FOCUSED)
# Set style for demo_gui_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.DISABLED.
demo_gui_ddlist_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_border_color(lv.color_hex(0xe1e6ee), lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_radius(3, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DISABLED)
demo_gui_ddlist_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DISABLED)
# Set style for demo_gui_ddlist_2, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_demo_gui_ddlist_2_extra_list_selected_checked = lv.style_t()
style_demo_gui_ddlist_2_extra_list_selected_checked.init()
style_demo_gui_ddlist_2_extra_list_selected_checked.set_border_width(2)
style_demo_gui_ddlist_2_extra_list_selected_checked.set_border_opa(255)
style_demo_gui_ddlist_2_extra_list_selected_checked.set_border_color(lv.color_hex(0x654f4f))
style_demo_gui_ddlist_2_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_demo_gui_ddlist_2_extra_list_selected_checked.set_radius(3)
style_demo_gui_ddlist_2_extra_list_selected_checked.set_bg_opa(110)
style_demo_gui_ddlist_2_extra_list_selected_checked.set_bg_color(lv.color_hex(0x000000))
style_demo_gui_ddlist_2_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
demo_gui_ddlist_2.get_list().add_style(style_demo_gui_ddlist_2_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for demo_gui_ddlist_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_demo_gui_ddlist_2_extra_list_main_default = lv.style_t()
style_demo_gui_ddlist_2_extra_list_main_default.init()
style_demo_gui_ddlist_2_extra_list_main_default.set_max_height(90)
style_demo_gui_ddlist_2_extra_list_main_default.set_text_color(lv.color_hex(0xffffff))
style_demo_gui_ddlist_2_extra_list_main_default.set_text_font(test_font("montserratMedium", 14))
style_demo_gui_ddlist_2_extra_list_main_default.set_text_opa(255)
style_demo_gui_ddlist_2_extra_list_main_default.set_border_width(1)
style_demo_gui_ddlist_2_extra_list_main_default.set_border_opa(255)
style_demo_gui_ddlist_2_extra_list_main_default.set_border_color(lv.color_hex(0x654f4f))
style_demo_gui_ddlist_2_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_demo_gui_ddlist_2_extra_list_main_default.set_radius(3)
style_demo_gui_ddlist_2_extra_list_main_default.set_bg_opa(138)
style_demo_gui_ddlist_2_extra_list_main_default.set_bg_color(lv.color_hex(0x000000))
style_demo_gui_ddlist_2_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
demo_gui_ddlist_2.get_list().add_style(style_demo_gui_ddlist_2_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for demo_gui_ddlist_2, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_demo_gui_ddlist_2_extra_list_scrollbar_default = lv.style_t()
style_demo_gui_ddlist_2_extra_list_scrollbar_default.init()
style_demo_gui_ddlist_2_extra_list_scrollbar_default.set_radius(3)
style_demo_gui_ddlist_2_extra_list_scrollbar_default.set_bg_opa(255)
style_demo_gui_ddlist_2_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x000000))
style_demo_gui_ddlist_2_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
demo_gui_ddlist_2.get_list().add_style(style_demo_gui_ddlist_2_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create demo_gui_cont_66
demo_gui_cont_66 = lv.obj(demo_gui_cont_65)
demo_gui_cont_66.set_pos(208, 38)
demo_gui_cont_66.set_size(525, 515)
demo_gui_cont_66.add_flag(lv.obj.FLAG.HIDDEN)
demo_gui_cont_66.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_66, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_66.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_66.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_cont_67
demo_gui_cont_67 = lv.obj(demo_gui_cont_66)
demo_gui_cont_67.set_pos(-3, 9)
demo_gui_cont_67.set_size(300, 31)
demo_gui_cont_67.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_cont_67, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_cont_67.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_cont_67.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create demo_gui_label_66
demo_gui_label_66 = lv.label(demo_gui_cont_67)
demo_gui_label_66.set_text("Notification")
demo_gui_label_66.set_long_mode(lv.label.LONG.WRAP)
demo_gui_label_66.set_width(lv.pct(100))
demo_gui_label_66.set_pos(7, 4)
demo_gui_label_66.set_size(117, 22)
# Set style for demo_gui_label_66, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_label_66.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_pad_top(4, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_label_66.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create demo_gui_list_2
demo_gui_list_2 = lv.list(demo_gui_cont_66)
demo_gui_list_2_item0 = demo_gui_list_2.add_btn(lv.SYMBOL.BATTERY_1, "Battery is running low")
demo_gui_list_2_item1 = demo_gui_list_2.add_btn(lv.SYMBOL.REFRESH, "New software is available")
demo_gui_list_2_item2 = demo_gui_list_2.add_btn(lv.SYMBOL.WARNING, "Lights ")
demo_gui_list_2.set_pos(0, 57)
demo_gui_list_2.set_size(520, 153)
demo_gui_list_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for demo_gui_list_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
demo_gui_list_2.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_radius(7, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_list_2, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
demo_gui_list_2.set_style_radius(3, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
demo_gui_list_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
# Set style for demo_gui_list_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_demo_gui_list_2_extra_btns_main_default = lv.style_t()
style_demo_gui_list_2_extra_btns_main_default.init()
style_demo_gui_list_2_extra_btns_main_default.set_pad_top(5)
style_demo_gui_list_2_extra_btns_main_default.set_pad_left(10)
style_demo_gui_list_2_extra_btns_main_default.set_pad_right(0)
style_demo_gui_list_2_extra_btns_main_default.set_pad_bottom(10)
style_demo_gui_list_2_extra_btns_main_default.set_border_width(1)
style_demo_gui_list_2_extra_btns_main_default.set_border_opa(255)
style_demo_gui_list_2_extra_btns_main_default.set_border_color(lv.color_hex(0x6f6969))
style_demo_gui_list_2_extra_btns_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_demo_gui_list_2_extra_btns_main_default.set_text_color(lv.color_hex(0xfef6ea))
style_demo_gui_list_2_extra_btns_main_default.set_text_font(test_font("FontAwesome5", 18))
style_demo_gui_list_2_extra_btns_main_default.set_text_opa(255)
style_demo_gui_list_2_extra_btns_main_default.set_radius(0)
style_demo_gui_list_2_extra_btns_main_default.set_bg_opa(0)
demo_gui_list_2_item2.add_style(style_demo_gui_list_2_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2_item1.add_style(style_demo_gui_list_2_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
demo_gui_list_2_item0.add_style(style_demo_gui_list_2_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for demo_gui_list_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_demo_gui_list_2_extra_texts_main_default = lv.style_t()
style_demo_gui_list_2_extra_texts_main_default.init()
style_demo_gui_list_2_extra_texts_main_default.set_pad_top(6)
style_demo_gui_list_2_extra_texts_main_default.set_pad_left(5)
style_demo_gui_list_2_extra_texts_main_default.set_pad_right(0)
style_demo_gui_list_2_extra_texts_main_default.set_pad_bottom(0)
style_demo_gui_list_2_extra_texts_main_default.set_border_width(0)
style_demo_gui_list_2_extra_texts_main_default.set_text_color(lv.color_hex(0x0D3055))
style_demo_gui_list_2_extra_texts_main_default.set_text_font(test_font("montserratMedium", 18))
style_demo_gui_list_2_extra_texts_main_default.set_text_opa(255)
style_demo_gui_list_2_extra_texts_main_default.set_radius(3)
style_demo_gui_list_2_extra_texts_main_default.set_bg_opa(0)

demo_gui.update_layout()

def demo_gui_imgbtn_10_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_10.add_event_cb(lambda e: demo_gui_imgbtn_10_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_btn_5_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_btn_5.add_event_cb(lambda e: demo_gui_btn_5_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_11_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_11.add_event_cb(lambda e: demo_gui_imgbtn_11_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_btn_6_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_btn_6.add_event_cb(lambda e: demo_gui_btn_6_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_12_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.clear_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_12.add_event_cb(lambda e: demo_gui_imgbtn_12_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_btn_7_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_33.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_48.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_54.clear_flag(lv.obj.FLAG.HIDDEN)

demo_gui_btn_7.add_event_cb(lambda e: demo_gui_btn_7_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_27_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_66.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_68.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_70.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_74.clear_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_27.add_event_cb(lambda e: demo_gui_imgbtn_27_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_26_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_66.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_68.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_70.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_74.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_26.add_event_cb(lambda e: demo_gui_imgbtn_26_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_25_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_66.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_68.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_70.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_74.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_25.add_event_cb(lambda e: demo_gui_imgbtn_25_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_imgbtn_24_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        demo_gui_cont_68.clear_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_66.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_70.add_flag(lv.obj.FLAG.HIDDEN);demo_gui_cont_74.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_imgbtn_24.add_event_cb(lambda e: demo_gui_imgbtn_24_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_sw_7_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED and demo_gui_sw_7.has_state(lv.STATE.CHECKED)):
        demo_gui_cont_75.clear_flag(lv.obj.FLAG.HIDDEN)
    if (code == lv.EVENT.VALUE_CHANGED and not demo_gui_sw_7.has_state(lv.STATE.CHECKED)):
        demo_gui_cont_75.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_sw_7.add_event_cb(lambda e: demo_gui_sw_7_event_handler(e), lv.EVENT.ALL, None)

def demo_gui_sw_6_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED and demo_gui_sw_6.has_state(lv.STATE.CHECKED)):
        demo_gui_cont_71.clear_flag(lv.obj.FLAG.HIDDEN)
    if (code == lv.EVENT.VALUE_CHANGED and not demo_gui_sw_6.has_state(lv.STATE.CHECKED)):
        demo_gui_cont_71.add_flag(lv.obj.FLAG.HIDDEN)

demo_gui_sw_6.add_event_cb(lambda e: demo_gui_sw_6_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.scr_load(demo_gui)

while SDL.check():
    time.sleep_ms(5)

