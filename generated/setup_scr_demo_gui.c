/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



int demo_gui_digital_clock_1_min_value = 25;
int demo_gui_digital_clock_1_hour_value = 11;
int demo_gui_digital_clock_1_sec_value = 50;
char demo_gui_digital_clock_1_meridiem[] = "AM";
int demo_gui_digital_clock_2_min_value = 25;
int demo_gui_digital_clock_2_hour_value = 11;
int demo_gui_digital_clock_2_sec_value = 50;
char demo_gui_digital_clock_2_meridiem[] = "AM";
void setup_scr_demo_gui(lv_ui *ui)
{
	//Write codes demo_gui
	ui->demo_gui = lv_obj_create(NULL);
	lv_obj_set_size(ui->demo_gui, 480, 272);
	lv_obj_set_scrollbar_mode(ui->demo_gui, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_tileview_1
	ui->demo_gui_tileview_1 = lv_tileview_create(ui->demo_gui);
	ui->demo_gui_tileview_1_main = lv_tileview_add_tile(ui->demo_gui_tileview_1, 0, 0, LV_DIR_BOTTOM);
	ui->demo_gui_tileview_1_settings = lv_tileview_add_tile(ui->demo_gui_tileview_1, 0, 1, LV_DIR_TOP);
	lv_obj_set_pos(ui->demo_gui_tileview_1, 0, 0);
	lv_obj_set_size(ui->demo_gui_tileview_1, 480, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_tileview_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_tileview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_tileview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_tileview_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_tileview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_tileview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_tileview_1, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_tileview_1, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_tileview_1, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_tileview_1, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_tileview_1, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes demo_gui_tileview_2
	ui->demo_gui_tileview_2 = lv_tileview_create(ui->demo_gui_tileview_1_main);
	ui->demo_gui_tileview_2_home_page = lv_tileview_add_tile(ui->demo_gui_tileview_2, 0, 0, LV_DIR_RIGHT);
	ui->demo_gui_tileview_2_side_page = lv_tileview_add_tile(ui->demo_gui_tileview_2, 1, 0, LV_DIR_LEFT);
	lv_obj_set_pos(ui->demo_gui_tileview_2, 0, 0);
	lv_obj_set_size(ui->demo_gui_tileview_2, 480, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_tileview_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_tileview_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_tileview_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_tileview_2, lv_color_hex(0xf6f6f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_tileview_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_tileview_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_tileview_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_tileview_2, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_tileview_2, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_tileview_2, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_tileview_2, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_tileview_2, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes demo_gui_img_1
	ui->demo_gui_img_1 = lv_img_create(ui->demo_gui_tileview_2_home_page);
	lv_obj_add_flag(ui->demo_gui_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->demo_gui_img_1, &_background_alpha_480x280);
	lv_img_set_pivot(ui->demo_gui_img_1, 50,50);
	lv_img_set_angle(ui->demo_gui_img_1, 0);
	lv_obj_set_pos(ui->demo_gui_img_1, 0, 0);
	lv_obj_set_size(ui->demo_gui_img_1, 480, 280);

	//Write style for demo_gui_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->demo_gui_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_1
	ui->demo_gui_cont_1 = lv_obj_create(ui->demo_gui_tileview_2_home_page);
	lv_obj_set_pos(ui->demo_gui_cont_1, 254, 5);
	lv_obj_set_size(ui->demo_gui_cont_1, 224, 44);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_2
	ui->demo_gui_imgbtn_2 = lv_imgbtn_create(ui->demo_gui_cont_1);
	lv_obj_add_flag(ui->demo_gui_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, &_itemperature_alpha_27x21, NULL);
	ui->demo_gui_imgbtn_2_label = lv_label_create(ui->demo_gui_imgbtn_2);
	lv_label_set_text(ui->demo_gui_imgbtn_2_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_2, 59, 10);
	lv_obj_set_size(ui->demo_gui_imgbtn_2, 27, 21);

	//Write style for demo_gui_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_2, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_2, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_2
	ui->demo_gui_label_2 = lv_label_create(ui->demo_gui_cont_1);
	lv_label_set_text(ui->demo_gui_label_2, "25°C");
	lv_label_set_long_mode(ui->demo_gui_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_2, 86, 11);
	lv_obj_set_size(ui->demo_gui_label_2, 46, 21);

	//Write style for demo_gui_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_2, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_1
	ui->demo_gui_imgbtn_1 = lv_imgbtn_create(ui->demo_gui_cont_1);
	lv_obj_add_flag(ui->demo_gui_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, &_ihumidity_alpha_16x15, NULL);
	ui->demo_gui_imgbtn_1_label = lv_label_create(ui->demo_gui_imgbtn_1);
	lv_label_set_text(ui->demo_gui_imgbtn_1_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_1, 154, 12);
	lv_obj_set_size(ui->demo_gui_imgbtn_1, 16, 15);

	//Write style for demo_gui_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_1, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_1, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_1
	ui->demo_gui_label_1 = lv_label_create(ui->demo_gui_cont_1);
	lv_label_set_text(ui->demo_gui_label_1, "70%");
	lv_label_set_long_mode(ui->demo_gui_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_1, 178, 12);
	lv_obj_set_size(ui->demo_gui_label_1, 38, 21);

	//Write style for demo_gui_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_1, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_2
	ui->demo_gui_cont_2 = lv_obj_create(ui->demo_gui_tileview_2_home_page);
	lv_obj_set_pos(ui->demo_gui_cont_2, 315, 207);
	lv_obj_set_size(ui->demo_gui_cont_2, 156, 64);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_4
	ui->demo_gui_label_4 = lv_label_create(ui->demo_gui_cont_2);
	lv_label_set_text(ui->demo_gui_label_4, "11 May, 2024");
	lv_label_set_long_mode(ui->demo_gui_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_4, -23, -8);
	lv_obj_set_size(ui->demo_gui_label_4, 179, 23);

	//Write style for demo_gui_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_4, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_3
	ui->demo_gui_label_3 = lv_label_create(ui->demo_gui_cont_2);
	lv_label_set_text(ui->demo_gui_label_3, "9:00 AM");
	lv_label_set_long_mode(ui->demo_gui_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_3, 19, 25);
	lv_obj_set_size(ui->demo_gui_label_3, 114, 32);

	//Write style for demo_gui_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_3, &lv_font_Antonio_Regular_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_3, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes demo_gui_cont_28
	ui->demo_gui_cont_28 = lv_obj_create(ui->demo_gui_tileview_2_side_page);
	lv_obj_set_pos(ui->demo_gui_cont_28, 0, 0);
	lv_obj_set_size(ui->demo_gui_cont_28, 480, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_28, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_28, 244, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_28, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_28, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_41
	ui->demo_gui_cont_41 = lv_obj_create(ui->demo_gui_cont_28);
	lv_obj_set_pos(ui->demo_gui_cont_41, 0, 0);
	lv_obj_set_size(ui->demo_gui_cont_41, 142, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_41, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_41, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_41, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_41, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_41, lv_color_hex(0xa5aaae), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_41, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_29
	ui->demo_gui_label_29 = lv_label_create(ui->demo_gui_cont_41);
	lv_label_set_text(ui->demo_gui_label_29, "Home ");
	lv_label_set_long_mode(ui->demo_gui_label_29, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_29, 8, 25);
	lv_obj_set_size(ui->demo_gui_label_29, 125, 45);

	//Write style for demo_gui_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_29, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_label_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_label_29, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_label_29, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_29, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_29, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_29, &lv_font_Acme_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_29, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_label_29, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_label_29, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_29, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_29, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_42
	ui->demo_gui_cont_42 = lv_obj_create(ui->demo_gui_cont_41);
	lv_obj_set_pos(ui->demo_gui_cont_42, 6, 80);
	lv_obj_set_size(ui->demo_gui_cont_42, 130, 49);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_42, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_42, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_42, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_42, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_42, lv_color_hex(0x4e9ee1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_42, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_10
	ui->demo_gui_imgbtn_10 = lv_imgbtn_create(ui->demo_gui_cont_42);
	lv_obj_add_flag(ui->demo_gui_imgbtn_10, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_10, LV_IMGBTN_STATE_RELEASED, NULL, &_led_alpha_25x24, NULL);
	ui->demo_gui_imgbtn_10_label = lv_label_create(ui->demo_gui_imgbtn_10);
	lv_label_set_text(ui->demo_gui_imgbtn_10_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_10_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_10_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_10, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_10, 8, 10);
	lv_obj_set_size(ui->demo_gui_imgbtn_10, 25, 24);

	//Write style for demo_gui_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_10, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_10, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_10, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_10, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_btn_5
	ui->demo_gui_btn_5 = lv_btn_create(ui->demo_gui_cont_42);
	ui->demo_gui_btn_5_label = lv_label_create(ui->demo_gui_btn_5);
	lv_label_set_text(ui->demo_gui_btn_5_label, "Lights");
	lv_label_set_long_mode(ui->demo_gui_btn_5_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_btn_5_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_btn_5, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->demo_gui_btn_5_label, LV_PCT(100));
	lv_obj_set_pos(ui->demo_gui_btn_5, 35, 8);
	lv_obj_set_size(ui->demo_gui_btn_5, 51, 29);

	//Write style for demo_gui_btn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_btn_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_btn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_btn_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_btn_5, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_btn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_btn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_43
	ui->demo_gui_cont_43 = lv_obj_create(ui->demo_gui_cont_41);
	lv_obj_set_pos(ui->demo_gui_cont_43, 6, 140);
	lv_obj_set_size(ui->demo_gui_cont_43, 130, 49);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_43, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_43, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_43, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_43, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_43, lv_color_hex(0x4e9ee1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_43, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_11
	ui->demo_gui_imgbtn_11 = lv_imgbtn_create(ui->demo_gui_cont_43);
	lv_obj_add_flag(ui->demo_gui_imgbtn_11, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_11, LV_IMGBTN_STATE_RELEASED, NULL, &_switch_alpha_25x24, NULL);
	ui->demo_gui_imgbtn_11_label = lv_label_create(ui->demo_gui_imgbtn_11);
	lv_label_set_text(ui->demo_gui_imgbtn_11_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_11_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_11_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_11, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_11, 6, 11);
	lv_obj_set_size(ui->demo_gui_imgbtn_11, 25, 24);

	//Write style for demo_gui_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_11, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_11, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_11, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_11, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_btn_6
	ui->demo_gui_btn_6 = lv_btn_create(ui->demo_gui_cont_43);
	ui->demo_gui_btn_6_label = lv_label_create(ui->demo_gui_btn_6);
	lv_label_set_text(ui->demo_gui_btn_6_label, "Switchs");
	lv_label_set_long_mode(ui->demo_gui_btn_6_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_btn_6_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_btn_6, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->demo_gui_btn_6_label, LV_PCT(100));
	lv_obj_set_pos(ui->demo_gui_btn_6, 35, 8);
	lv_obj_set_size(ui->demo_gui_btn_6, 59, 29);

	//Write style for demo_gui_btn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_btn_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_btn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_btn_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_btn_6, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_btn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_btn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_44
	ui->demo_gui_cont_44 = lv_obj_create(ui->demo_gui_cont_41);
	lv_obj_set_pos(ui->demo_gui_cont_44, 6, 203);
	lv_obj_set_size(ui->demo_gui_cont_44, 130, 49);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_44, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_44, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_44, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_44, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_44, lv_color_hex(0x4e9ee1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_44, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_12
	ui->demo_gui_imgbtn_12 = lv_imgbtn_create(ui->demo_gui_cont_44);
	lv_obj_add_flag(ui->demo_gui_imgbtn_12, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_12, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_25x24, NULL);
	ui->demo_gui_imgbtn_12_label = lv_label_create(ui->demo_gui_imgbtn_12);
	lv_label_set_text(ui->demo_gui_imgbtn_12_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_12_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_12_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_12, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_12, 3, 14);
	lv_obj_set_size(ui->demo_gui_imgbtn_12, 25, 24);

	//Write style for demo_gui_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_12, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_12, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_12, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_12, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_12, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_btn_7
	ui->demo_gui_btn_7 = lv_btn_create(ui->demo_gui_cont_44);
	ui->demo_gui_btn_7_label = lv_label_create(ui->demo_gui_btn_7);
	lv_label_set_text(ui->demo_gui_btn_7_label, "Set timer");
	lv_label_set_long_mode(ui->demo_gui_btn_7_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_btn_7_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_btn_7, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->demo_gui_btn_7_label, LV_PCT(100));
	lv_obj_set_pos(ui->demo_gui_btn_7, 31, 10);
	lv_obj_set_size(ui->demo_gui_btn_7, 74, 29);

	//Write style for demo_gui_btn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_btn_7, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_btn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_btn_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_btn_7, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_btn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_btn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_33
	ui->demo_gui_cont_33 = lv_obj_create(ui->demo_gui_cont_28);
	lv_obj_set_pos(ui->demo_gui_cont_33, 159, 19);
	lv_obj_set_size(ui->demo_gui_cont_33, 300, 250);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_33, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_33, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_45
	ui->demo_gui_cont_45 = lv_obj_create(ui->demo_gui_cont_33);
	lv_obj_set_pos(ui->demo_gui_cont_45, 8, 12);
	lv_obj_set_size(ui->demo_gui_cont_45, 285, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_45, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_45, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_45, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_45, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_45, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_45, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_13
	ui->demo_gui_imgbtn_13 = lv_imgbtn_create(ui->demo_gui_cont_45);
	lv_obj_add_flag(ui->demo_gui_imgbtn_13, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_13, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_39x37, NULL);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_13, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_onlight_alpha_39x37, NULL);
	ui->demo_gui_imgbtn_13_label = lv_label_create(ui->demo_gui_imgbtn_13);
	lv_label_set_text(ui->demo_gui_imgbtn_13_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_13_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_13_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_13, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_13, 6, 2);
	lv_obj_set_size(ui->demo_gui_imgbtn_13, 39, 37);

	//Write style for demo_gui_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_13, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_13, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_13, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_13, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_13, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_30
	ui->demo_gui_label_30 = lv_label_create(ui->demo_gui_cont_45);
	lv_label_set_text(ui->demo_gui_label_30, "Light 1");
	lv_label_set_long_mode(ui->demo_gui_label_30, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_30, 58, 11);
	lv_obj_set_size(ui->demo_gui_label_30, 62, 21);

	//Write style for demo_gui_label_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_30, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_30, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_30, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_30, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_30, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_32
	ui->demo_gui_label_32 = lv_label_create(ui->demo_gui_cont_45);
	lv_label_set_text(ui->demo_gui_label_32, "On");
	lv_label_set_long_mode(ui->demo_gui_label_32, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_32, 227, 12);
	lv_obj_set_size(ui->demo_gui_label_32, 33, 21);
	lv_obj_add_flag(ui->demo_gui_label_32, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_32, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_32, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_32, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_32, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_32, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_46
	ui->demo_gui_cont_46 = lv_obj_create(ui->demo_gui_cont_33);
	lv_obj_set_pos(ui->demo_gui_cont_46, 8, 68);
	lv_obj_set_size(ui->demo_gui_cont_46, 285, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_46, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_46, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_46, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_46, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_46, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_46, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_14
	ui->demo_gui_imgbtn_14 = lv_imgbtn_create(ui->demo_gui_cont_46);
	lv_obj_add_flag(ui->demo_gui_imgbtn_14, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_14, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_39x37, NULL);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_14, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_onlight_alpha_39x37, NULL);
	ui->demo_gui_imgbtn_14_label = lv_label_create(ui->demo_gui_imgbtn_14);
	lv_label_set_text(ui->demo_gui_imgbtn_14_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_14_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_14_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_14, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_14, 6, 2);
	lv_obj_set_size(ui->demo_gui_imgbtn_14, 39, 37);

	//Write style for demo_gui_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_14, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_14, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_14, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_14, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_14, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_35
	ui->demo_gui_label_35 = lv_label_create(ui->demo_gui_cont_46);
	lv_label_set_text(ui->demo_gui_label_35, "Light 2");
	lv_label_set_long_mode(ui->demo_gui_label_35, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_35, 58, 11);
	lv_obj_set_size(ui->demo_gui_label_35, 60, 21);

	//Write style for demo_gui_label_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_35, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_35, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_35, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_35, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_35, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_35, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_35, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_33
	ui->demo_gui_label_33 = lv_label_create(ui->demo_gui_cont_46);
	lv_label_set_text(ui->demo_gui_label_33, "On");
	lv_label_set_long_mode(ui->demo_gui_label_33, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_33, 227, 12);
	lv_obj_set_size(ui->demo_gui_label_33, 33, 21);
	lv_obj_add_flag(ui->demo_gui_label_33, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_33, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_33, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_33, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_33, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_33, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_47
	ui->demo_gui_cont_47 = lv_obj_create(ui->demo_gui_cont_33);
	lv_obj_set_pos(ui->demo_gui_cont_47, 8, 127);
	lv_obj_set_size(ui->demo_gui_cont_47, 285, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_47, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_47, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_47, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_47, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_47, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_47, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_15
	ui->demo_gui_imgbtn_15 = lv_imgbtn_create(ui->demo_gui_cont_47);
	lv_obj_add_flag(ui->demo_gui_imgbtn_15, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_15, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_39x37, NULL);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_15, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_onlight_alpha_39x37, NULL);
	ui->demo_gui_imgbtn_15_label = lv_label_create(ui->demo_gui_imgbtn_15);
	lv_label_set_text(ui->demo_gui_imgbtn_15_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_15_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_15_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_15, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_15, 6, 2);
	lv_obj_set_size(ui->demo_gui_imgbtn_15, 39, 37);

	//Write style for demo_gui_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_15, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_15, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_15, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_15, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_15, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_38
	ui->demo_gui_label_38 = lv_label_create(ui->demo_gui_cont_47);
	lv_label_set_text(ui->demo_gui_label_38, "Light 3");
	lv_label_set_long_mode(ui->demo_gui_label_38, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_38, 58, 11);
	lv_obj_set_size(ui->demo_gui_label_38, 66, 21);

	//Write style for demo_gui_label_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_38, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_38, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_38, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_38, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_38, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_38, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_36
	ui->demo_gui_label_36 = lv_label_create(ui->demo_gui_cont_47);
	lv_label_set_text(ui->demo_gui_label_36, "On");
	lv_label_set_long_mode(ui->demo_gui_label_36, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_36, 227, 12);
	lv_obj_set_size(ui->demo_gui_label_36, 33, 21);
	lv_obj_add_flag(ui->demo_gui_label_36, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_label_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_36, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_36, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_36, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_36, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_36, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_36, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_48
	ui->demo_gui_cont_48 = lv_obj_create(ui->demo_gui_cont_28);
	lv_obj_set_pos(ui->demo_gui_cont_48, 159, 18);
	lv_obj_set_size(ui->demo_gui_cont_48, 300, 250);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_48, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_48, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_48, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_51
	ui->demo_gui_cont_51 = lv_obj_create(ui->demo_gui_cont_48);
	lv_obj_set_pos(ui->demo_gui_cont_51, 9, 10);
	lv_obj_set_size(ui->demo_gui_cont_51, 285, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_51, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_51, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_51, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_51, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_51, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_51, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_47
	ui->demo_gui_label_47 = lv_label_create(ui->demo_gui_cont_51);
	lv_label_set_text(ui->demo_gui_label_47, "Switch 1");
	lv_label_set_long_mode(ui->demo_gui_label_47, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_47, 70, 10);
	lv_obj_set_size(ui->demo_gui_label_47, 74, 21);

	//Write style for demo_gui_label_47, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_47, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_47, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_47, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_47, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_47, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_47, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_sw_3
	ui->demo_gui_sw_3 = lv_switch_create(ui->demo_gui_cont_51);
	lv_obj_set_pos(ui->demo_gui_sw_3, 15, 11);
	lv_obj_set_size(ui->demo_gui_sw_3, 40, 20);

	//Write style for demo_gui_sw_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_3, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_3, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_sw_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_sw_3, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_3, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_3, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_3, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_sw_3, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for demo_gui_sw_3, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_3, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_3, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_3, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_3, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_3, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_52
	ui->demo_gui_cont_52 = lv_obj_create(ui->demo_gui_cont_48);
	lv_obj_set_pos(ui->demo_gui_cont_52, 9, 62);
	lv_obj_set_size(ui->demo_gui_cont_52, 287, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_52, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_52, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_52, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_52, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_52, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_52, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_48
	ui->demo_gui_label_48 = lv_label_create(ui->demo_gui_cont_52);
	lv_label_set_text(ui->demo_gui_label_48, "Switch 2");
	lv_label_set_long_mode(ui->demo_gui_label_48, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_48, 70, 10);
	lv_obj_set_size(ui->demo_gui_label_48, 74, 21);

	//Write style for demo_gui_label_48, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_48, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_48, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_48, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_48, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_48, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_48, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_sw_4
	ui->demo_gui_sw_4 = lv_switch_create(ui->demo_gui_cont_52);
	lv_obj_set_pos(ui->demo_gui_sw_4, 15, 11);
	lv_obj_set_size(ui->demo_gui_sw_4, 40, 20);

	//Write style for demo_gui_sw_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_4, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_4, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_sw_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_sw_4, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_4, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_4, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_4, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_sw_4, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for demo_gui_sw_4, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_4, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_4, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_4, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_4, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_4, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_53
	ui->demo_gui_cont_53 = lv_obj_create(ui->demo_gui_cont_48);
	lv_obj_set_pos(ui->demo_gui_cont_53, 9, 114);
	lv_obj_set_size(ui->demo_gui_cont_53, 287, 43);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_53, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_53, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_53, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_53, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_53, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_53, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_49
	ui->demo_gui_label_49 = lv_label_create(ui->demo_gui_cont_53);
	lv_label_set_text(ui->demo_gui_label_49, "Switch 3");
	lv_label_set_long_mode(ui->demo_gui_label_49, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_49, 70, 10);
	lv_obj_set_size(ui->demo_gui_label_49, 74, 21);

	//Write style for demo_gui_label_49, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_49, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_49, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_49, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_49, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_49, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_49, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_sw_5
	ui->demo_gui_sw_5 = lv_switch_create(ui->demo_gui_cont_53);
	lv_obj_set_pos(ui->demo_gui_sw_5, 15, 11);
	lv_obj_set_size(ui->demo_gui_sw_5, 40, 20);

	//Write style for demo_gui_sw_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_5, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_5, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_sw_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_sw_5, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_5, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_5, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_5, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_sw_5, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for demo_gui_sw_5, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_5, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_5, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_5, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_5, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_5, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_54
	ui->demo_gui_cont_54 = lv_obj_create(ui->demo_gui_cont_28);
	lv_obj_set_pos(ui->demo_gui_cont_54, 156, 15);
	lv_obj_set_size(ui->demo_gui_cont_54, 314, 250);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_54, LV_SCROLLBAR_MODE_ON);
	lv_obj_add_flag(ui->demo_gui_cont_54, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_54, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_57
	ui->demo_gui_cont_57 = lv_obj_create(ui->demo_gui_cont_54);
	lv_obj_set_pos(ui->demo_gui_cont_57, 2, 1);
	lv_obj_set_size(ui->demo_gui_cont_57, 310, 130);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_57, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_57, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_57, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_57, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_57, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_57, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_52
	ui->demo_gui_label_52 = lv_label_create(ui->demo_gui_cont_57);
	lv_label_set_text(ui->demo_gui_label_52, "Time/Date:");
	lv_label_set_long_mode(ui->demo_gui_label_52, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_52, 6, 11);
	lv_obj_set_size(ui->demo_gui_label_52, 109, 22);

	//Write style for demo_gui_label_52, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_52, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_52, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_52, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_52, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_52, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_52, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_52, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_digital_clock_1
	static bool demo_gui_digital_clock_1_timer_enabled = false;
	ui->demo_gui_digital_clock_1 = lv_dclock_create(ui->demo_gui_cont_57, "11:25:50 AM");
	if (!demo_gui_digital_clock_1_timer_enabled) {
		lv_timer_create(demo_gui_digital_clock_1_timer, 1000, NULL);
		demo_gui_digital_clock_1_timer_enabled = true;
	}
	lv_obj_set_pos(ui->demo_gui_digital_clock_1, 107, 8);
	lv_obj_set_size(ui->demo_gui_digital_clock_1, 97, 30);

	//Write style for demo_gui_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_digital_clock_1, &lv_font_Antonio_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_digital_clock_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_datetext_1
	ui->demo_gui_datetext_1 = lv_label_create(ui->demo_gui_cont_57);
	lv_label_set_text(ui->demo_gui_datetext_1, "2023/07/31");
	lv_obj_set_style_text_align(ui->demo_gui_datetext_1, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_flag(ui->demo_gui_datetext_1, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->demo_gui_datetext_1, demo_gui_datetext_1_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_set_pos(ui->demo_gui_datetext_1, 201, 9);
	lv_obj_set_size(ui->demo_gui_datetext_1, 101, 25);

	//Write style for demo_gui_datetext_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_datetext_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_datetext_1, &lv_font_Antonio_Regular_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_datetext_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_datetext_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_datetext_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_datetext_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_58
	ui->demo_gui_cont_58 = lv_obj_create(ui->demo_gui_cont_57);
	lv_obj_set_pos(ui->demo_gui_cont_58, 15, 42);
	lv_obj_set_size(ui->demo_gui_cont_58, 127, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_58, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_58, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_58, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_58, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_58, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_58, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_58, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_53
	ui->demo_gui_label_53 = lv_label_create(ui->demo_gui_cont_58);
	lv_label_set_text(ui->demo_gui_label_53, "Light 1");
	lv_label_set_long_mode(ui->demo_gui_label_53, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_53, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_53, 62, 18);

	//Write style for demo_gui_label_53, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_53, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_53, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_53, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_53, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_53, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_18
	ui->demo_gui_imgbtn_18 = lv_imgbtn_create(ui->demo_gui_cont_58);
	lv_obj_add_flag(ui->demo_gui_imgbtn_18, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_18, LV_IMGBTN_STATE_RELEASED, NULL, &_onlight_alpha_26x29, NULL);
	ui->demo_gui_imgbtn_18_label = lv_label_create(ui->demo_gui_imgbtn_18);
	lv_label_set_text(ui->demo_gui_imgbtn_18_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_18_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_18_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_18, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_18, 4, 0);
	lv_obj_set_size(ui->demo_gui_imgbtn_18, 26, 29);

	//Write style for demo_gui_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_18, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_18, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_18, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_18, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_18, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_59
	ui->demo_gui_cont_59 = lv_obj_create(ui->demo_gui_cont_57);
	lv_obj_set_pos(ui->demo_gui_cont_59, 164, 43);
	lv_obj_set_size(ui->demo_gui_cont_59, 127, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_59, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_59, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_59, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_59, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_59, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_59, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_59, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_56
	ui->demo_gui_label_56 = lv_label_create(ui->demo_gui_cont_59);
	lv_label_set_text(ui->demo_gui_label_56, "Light 2");
	lv_label_set_long_mode(ui->demo_gui_label_56, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_56, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_56, 62, 18);

	//Write style for demo_gui_label_56, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_56, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_56, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_56, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_56, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_56, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_19
	ui->demo_gui_imgbtn_19 = lv_imgbtn_create(ui->demo_gui_cont_59);
	lv_obj_add_flag(ui->demo_gui_imgbtn_19, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_19, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_28x29, NULL);
	ui->demo_gui_imgbtn_19_label = lv_label_create(ui->demo_gui_imgbtn_19);
	lv_label_set_text(ui->demo_gui_imgbtn_19_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_19_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_19_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_19, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_19, 4, 0);
	lv_obj_set_size(ui->demo_gui_imgbtn_19, 28, 29);

	//Write style for demo_gui_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_19, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_19, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_19, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_19, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_19, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_60
	ui->demo_gui_cont_60 = lv_obj_create(ui->demo_gui_cont_57);
	lv_obj_set_pos(ui->demo_gui_cont_60, 16, 91);
	lv_obj_set_size(ui->demo_gui_cont_60, 128, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_60, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_60, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_60, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_60, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_60, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_60, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_60, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_58
	ui->demo_gui_label_58 = lv_label_create(ui->demo_gui_cont_60);
	lv_label_set_text(ui->demo_gui_label_58, "Switch 1");
	lv_label_set_long_mode(ui->demo_gui_label_58, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_58, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_58, 62, 18);

	//Write style for demo_gui_label_58, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_58, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_58, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_58, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_58, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_58, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_20
	ui->demo_gui_imgbtn_20 = lv_imgbtn_create(ui->demo_gui_cont_60);
	lv_obj_add_flag(ui->demo_gui_imgbtn_20, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_20, LV_IMGBTN_STATE_RELEASED, NULL, &_iswitch_alpha_26x22, NULL);
	ui->demo_gui_imgbtn_20_label = lv_label_create(ui->demo_gui_imgbtn_20);
	lv_label_set_text(ui->demo_gui_imgbtn_20_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_20_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_20_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_20, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_20, 4, 2);
	lv_obj_set_size(ui->demo_gui_imgbtn_20, 26, 22);

	//Write style for demo_gui_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_20, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_20, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_20, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_20, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_20, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_83
	ui->demo_gui_cont_83 = lv_obj_create(ui->demo_gui_cont_54);
	lv_obj_set_pos(ui->demo_gui_cont_83, 2, 138);
	lv_obj_set_size(ui->demo_gui_cont_83, 310, 125);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_83, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_83, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_83, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_83, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_83, lv_color_hex(0x7f7c7c), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_83, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_83, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_80
	ui->demo_gui_label_80 = lv_label_create(ui->demo_gui_cont_83);
	lv_label_set_text(ui->demo_gui_label_80, "Time/Date:");
	lv_label_set_long_mode(ui->demo_gui_label_80, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_80, 6, 11);
	lv_obj_set_size(ui->demo_gui_label_80, 109, 22);

	//Write style for demo_gui_label_80, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_80, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_80, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_80, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_80, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_80, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_80, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_digital_clock_2
	static bool demo_gui_digital_clock_2_timer_enabled = false;
	ui->demo_gui_digital_clock_2 = lv_dclock_create(ui->demo_gui_cont_83, "11:25:50 AM");
	if (!demo_gui_digital_clock_2_timer_enabled) {
		lv_timer_create(demo_gui_digital_clock_2_timer, 1000, NULL);
		demo_gui_digital_clock_2_timer_enabled = true;
	}
	lv_obj_set_pos(ui->demo_gui_digital_clock_2, 107, 8);
	lv_obj_set_size(ui->demo_gui_digital_clock_2, 97, 30);

	//Write style for demo_gui_digital_clock_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_digital_clock_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_digital_clock_2, &lv_font_Antonio_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_digital_clock_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_digital_clock_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_digital_clock_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_digital_clock_2, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_digital_clock_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_datetext_2
	ui->demo_gui_datetext_2 = lv_label_create(ui->demo_gui_cont_83);
	lv_label_set_text(ui->demo_gui_datetext_2, "2023/07/31");
	lv_obj_set_style_text_align(ui->demo_gui_datetext_2, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_flag(ui->demo_gui_datetext_2, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(ui->demo_gui_datetext_2, demo_gui_datetext_2_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_set_pos(ui->demo_gui_datetext_2, 201, 8);
	lv_obj_set_size(ui->demo_gui_datetext_2, 101, 25);

	//Write style for demo_gui_datetext_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_datetext_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_datetext_2, &lv_font_Antonio_Regular_15, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_datetext_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_datetext_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_datetext_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_datetext_2, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_datetext_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_86
	ui->demo_gui_cont_86 = lv_obj_create(ui->demo_gui_cont_83);
	lv_obj_set_pos(ui->demo_gui_cont_86, 15, 42);
	lv_obj_set_size(ui->demo_gui_cont_86, 127, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_86, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_86, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_86, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_86, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_86, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_86, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_86, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_86, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_79
	ui->demo_gui_label_79 = lv_label_create(ui->demo_gui_cont_86);
	lv_label_set_text(ui->demo_gui_label_79, "Light 1");
	lv_label_set_long_mode(ui->demo_gui_label_79, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_79, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_79, 62, 18);

	//Write style for demo_gui_label_79, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_79, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_79, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_79, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_79, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_79, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_30
	ui->demo_gui_imgbtn_30 = lv_imgbtn_create(ui->demo_gui_cont_86);
	lv_obj_add_flag(ui->demo_gui_imgbtn_30, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_30, LV_IMGBTN_STATE_RELEASED, NULL, &_onlight_alpha_26x29, NULL);
	ui->demo_gui_imgbtn_30_label = lv_label_create(ui->demo_gui_imgbtn_30);
	lv_label_set_text(ui->demo_gui_imgbtn_30_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_30_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_30_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_30, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_30, 4, 0);
	lv_obj_set_size(ui->demo_gui_imgbtn_30, 26, 29);

	//Write style for demo_gui_imgbtn_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_30, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_30, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_30, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_30, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_30, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_30, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_30, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_30, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_30, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_30, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_30, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_30, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_85
	ui->demo_gui_cont_85 = lv_obj_create(ui->demo_gui_cont_83);
	lv_obj_set_pos(ui->demo_gui_cont_85, 164, 43);
	lv_obj_set_size(ui->demo_gui_cont_85, 127, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_85, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_85, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_85, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_85, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_85, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_85, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_85, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_85, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_78
	ui->demo_gui_label_78 = lv_label_create(ui->demo_gui_cont_85);
	lv_label_set_text(ui->demo_gui_label_78, "Light 2");
	lv_label_set_long_mode(ui->demo_gui_label_78, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_78, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_78, 62, 18);

	//Write style for demo_gui_label_78, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_78, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_78, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_78, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_78, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_78, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_29
	ui->demo_gui_imgbtn_29 = lv_imgbtn_create(ui->demo_gui_cont_85);
	lv_obj_add_flag(ui->demo_gui_imgbtn_29, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_29, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_28x29, NULL);
	ui->demo_gui_imgbtn_29_label = lv_label_create(ui->demo_gui_imgbtn_29);
	lv_label_set_text(ui->demo_gui_imgbtn_29_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_29_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_29_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_29, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_29, 4, 0);
	lv_obj_set_size(ui->demo_gui_imgbtn_29, 28, 29);

	//Write style for demo_gui_imgbtn_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_29, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_29, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_29, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_29, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_29, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_29, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_29, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_29, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_29, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_29, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_29, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_29, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_84
	ui->demo_gui_cont_84 = lv_obj_create(ui->demo_gui_cont_83);
	lv_obj_set_pos(ui->demo_gui_cont_84, 16, 91);
	lv_obj_set_size(ui->demo_gui_cont_84, 128, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_84, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_84, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_84, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_84, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_84, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_84, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_84, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_84, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_77
	ui->demo_gui_label_77 = lv_label_create(ui->demo_gui_cont_84);
	lv_label_set_text(ui->demo_gui_label_77, "Switch 1");
	lv_label_set_long_mode(ui->demo_gui_label_77, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_77, 32, 7);
	lv_obj_set_size(ui->demo_gui_label_77, 62, 18);

	//Write style for demo_gui_label_77, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_77, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_77, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_77, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_77, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_77, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_28
	ui->demo_gui_imgbtn_28 = lv_imgbtn_create(ui->demo_gui_cont_84);
	lv_obj_add_flag(ui->demo_gui_imgbtn_28, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_28, LV_IMGBTN_STATE_RELEASED, NULL, &_iswitch_alpha_26x22, NULL);
	ui->demo_gui_imgbtn_28_label = lv_label_create(ui->demo_gui_imgbtn_28);
	lv_label_set_text(ui->demo_gui_imgbtn_28_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_28_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_28_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_28, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_28, 4, 2);
	lv_obj_set_size(ui->demo_gui_imgbtn_28, 26, 22);

	//Write style for demo_gui_imgbtn_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_28, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_28, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_28, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_28, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_28, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_28, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_28, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_28, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_28, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_28, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_28, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_28, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);



	//Write codes demo_gui_cont_65
	ui->demo_gui_cont_65 = lv_obj_create(ui->demo_gui_tileview_1_settings);
	lv_obj_set_pos(ui->demo_gui_cont_65, 0, 0);
	lv_obj_set_size(ui->demo_gui_cont_65, 480, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_65, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_65, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_65, 244, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_65, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_65, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_65, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_78
	ui->demo_gui_cont_78 = lv_obj_create(ui->demo_gui_cont_65);
	lv_obj_set_pos(ui->demo_gui_cont_78, 0, 0);
	lv_obj_set_size(ui->demo_gui_cont_78, 160, 280);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_78, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_78, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_78, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_cont_78, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_cont_78, lv_color_hex(0xa5aaae), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_cont_78, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_78, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_82
	ui->demo_gui_cont_82 = lv_obj_create(ui->demo_gui_cont_78);
	lv_obj_set_pos(ui->demo_gui_cont_82, 7, 75);
	lv_obj_set_size(ui->demo_gui_cont_82, 142, 35);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_82, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_82, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_82, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_82, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_82, lv_color_hex(0x292323), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_82, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_82, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_76
	ui->demo_gui_label_76 = lv_label_create(ui->demo_gui_cont_82);
	lv_label_set_text(ui->demo_gui_label_76, "Wifi\n");
	lv_label_set_long_mode(ui->demo_gui_label_76, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_76, 8, 8);
	lv_obj_set_size(ui->demo_gui_label_76, 49, 21);

	//Write style for demo_gui_label_76, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_76, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_76, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_76, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_76, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_76, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_76, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_27
	ui->demo_gui_imgbtn_27 = lv_imgbtn_create(ui->demo_gui_cont_82);
	lv_obj_add_flag(ui->demo_gui_imgbtn_27, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_27, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_22x23, NULL);
	ui->demo_gui_imgbtn_27_label = lv_label_create(ui->demo_gui_imgbtn_27);
	lv_label_set_text(ui->demo_gui_imgbtn_27_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_27_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_27_label, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_27, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_27, 110, 8);
	lv_obj_set_size(ui->demo_gui_imgbtn_27, 22, 23);

	//Write style for demo_gui_imgbtn_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_27, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_27, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_27, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_27, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_27, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_27, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_27, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_27, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_27, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_27, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_27, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_27, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_27, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_27, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_label_75
	ui->demo_gui_label_75 = lv_label_create(ui->demo_gui_cont_78);
	lv_label_set_text(ui->demo_gui_label_75, "Settings");
	lv_label_set_long_mode(ui->demo_gui_label_75, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_75, 9, 12);
	lv_obj_set_size(ui->demo_gui_label_75, 138, 45);

	//Write style for demo_gui_label_75, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_75, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->demo_gui_label_75, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->demo_gui_label_75, lv_color_hex(0xa1a3a5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->demo_gui_label_75, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_75, lv_color_hex(0x1b77bb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_75, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_75, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_75, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_75, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_75, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_81
	ui->demo_gui_cont_81 = lv_obj_create(ui->demo_gui_cont_78);
	lv_obj_set_pos(ui->demo_gui_cont_81, 6, 123);
	lv_obj_set_size(ui->demo_gui_cont_81, 142, 35);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_81, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_81, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_81, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_81, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_81, lv_color_hex(0x292323), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_81, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_81, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_74
	ui->demo_gui_label_74 = lv_label_create(ui->demo_gui_cont_81);
	lv_label_set_text(ui->demo_gui_label_74, "Bluetooth\n");
	lv_label_set_long_mode(ui->demo_gui_label_74, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_74, 11, 8);
	lv_obj_set_size(ui->demo_gui_label_74, 87, 21);

	//Write style for demo_gui_label_74, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_74, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_74, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_74, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_74, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_74, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_74, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_26
	ui->demo_gui_imgbtn_26 = lv_imgbtn_create(ui->demo_gui_cont_81);
	lv_obj_add_flag(ui->demo_gui_imgbtn_26, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_26, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_24x22, NULL);
	ui->demo_gui_imgbtn_26_label = lv_label_create(ui->demo_gui_imgbtn_26);
	lv_label_set_text(ui->demo_gui_imgbtn_26_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_26_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_26_label, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_26, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_26, 109, 6);
	lv_obj_set_size(ui->demo_gui_imgbtn_26, 24, 22);

	//Write style for demo_gui_imgbtn_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_26, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_26, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_26, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_26, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_26, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_26, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_26, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_26, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_26, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_26, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_26, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_26, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_26, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_80
	ui->demo_gui_cont_80 = lv_obj_create(ui->demo_gui_cont_78);
	lv_obj_set_pos(ui->demo_gui_cont_80, 7, 218);
	lv_obj_set_size(ui->demo_gui_cont_80, 142, 35);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_80, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_80, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_80, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_80, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_80, lv_color_hex(0x292323), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_80, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_80, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_73
	ui->demo_gui_label_73 = lv_label_create(ui->demo_gui_cont_80);
	lv_label_set_text(ui->demo_gui_label_73, "Notification\n\n");
	lv_label_set_long_mode(ui->demo_gui_label_73, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_73, 7, 8);
	lv_obj_set_size(ui->demo_gui_label_73, 102, 21);

	//Write style for demo_gui_label_73, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_73, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_73, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_73, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_73, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_73, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_73, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_25
	ui->demo_gui_imgbtn_25 = lv_imgbtn_create(ui->demo_gui_cont_80);
	lv_obj_add_flag(ui->demo_gui_imgbtn_25, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_25, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_27x23, NULL);
	ui->demo_gui_imgbtn_25_label = lv_label_create(ui->demo_gui_imgbtn_25);
	lv_label_set_text(ui->demo_gui_imgbtn_25_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_25_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_25_label, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_25, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_25, 108, 6);
	lv_obj_set_size(ui->demo_gui_imgbtn_25, 27, 23);

	//Write style for demo_gui_imgbtn_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_25, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_25, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_25, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_25, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_25, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_25, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_25, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_25, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_25, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_25, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_25, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_25, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_25, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_79
	ui->demo_gui_cont_79 = lv_obj_create(ui->demo_gui_cont_78);
	lv_obj_set_pos(ui->demo_gui_cont_79, 7, 170);
	lv_obj_set_size(ui->demo_gui_cont_79, 142, 35);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_79, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_79, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_79, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_79, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_cont_79, lv_color_hex(0x292323), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_cont_79, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_79, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_72
	ui->demo_gui_label_72 = lv_label_create(ui->demo_gui_cont_79);
	lv_label_set_text(ui->demo_gui_label_72, "Language\n");
	lv_label_set_long_mode(ui->demo_gui_label_72, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_72, 7, 8);
	lv_obj_set_size(ui->demo_gui_label_72, 92, 21);

	//Write style for demo_gui_label_72, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_72, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_72, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_72, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_72, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_72, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_72, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_imgbtn_24
	ui->demo_gui_imgbtn_24 = lv_imgbtn_create(ui->demo_gui_cont_79);
	lv_obj_add_flag(ui->demo_gui_imgbtn_24, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->demo_gui_imgbtn_24, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_25x21, NULL);
	ui->demo_gui_imgbtn_24_label = lv_label_create(ui->demo_gui_imgbtn_24);
	lv_label_set_text(ui->demo_gui_imgbtn_24_label, "");
	lv_label_set_long_mode(ui->demo_gui_imgbtn_24_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_imgbtn_24_label, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_imgbtn_24, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->demo_gui_imgbtn_24, 108, 7);
	lv_obj_set_size(ui->demo_gui_imgbtn_24, 25, 21);

	//Write style for demo_gui_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_24, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_imgbtn_24, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_24, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for demo_gui_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->demo_gui_imgbtn_24, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_imgbtn_24, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->demo_gui_imgbtn_24, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes demo_gui_cont_74
	ui->demo_gui_cont_74 = lv_obj_create(ui->demo_gui_cont_65);
	lv_obj_set_pos(ui->demo_gui_cont_74, 176, 18);
	lv_obj_set_size(ui->demo_gui_cont_74, 274, 222);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_74, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_74, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_74, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_77
	ui->demo_gui_cont_77 = lv_obj_create(ui->demo_gui_cont_74);
	lv_obj_set_pos(ui->demo_gui_cont_77, 7, 3);
	lv_obj_set_size(ui->demo_gui_cont_77, 245, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_77, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_77, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_77, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_71
	ui->demo_gui_label_71 = lv_label_create(ui->demo_gui_cont_77);
	lv_label_set_text(ui->demo_gui_label_71, "Wifi");
	lv_label_set_long_mode(ui->demo_gui_label_71, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_71, 7, 4);
	lv_obj_set_size(ui->demo_gui_label_71, 56, 22);

	//Write style for demo_gui_label_71, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_71, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_71, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_71, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_71, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_71, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_71, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_sw_7
	ui->demo_gui_sw_7 = lv_switch_create(ui->demo_gui_cont_77);
	lv_obj_set_pos(ui->demo_gui_sw_7, 80, 6);
	lv_obj_set_size(ui->demo_gui_sw_7, 37, 18);

	//Write style for demo_gui_sw_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_7, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_7, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_sw_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_sw_7, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_7, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_7, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_7, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_sw_7, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for demo_gui_sw_7, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_7, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_7, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_7, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_7, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_7, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_75
	ui->demo_gui_cont_75 = lv_obj_create(ui->demo_gui_cont_74);
	lv_obj_set_pos(ui->demo_gui_cont_75, -1, 61);
	lv_obj_set_size(ui->demo_gui_cont_75, 530, 122);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_75, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_75, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_75, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_75, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_76
	ui->demo_gui_cont_76 = lv_obj_create(ui->demo_gui_cont_75);
	lv_obj_set_pos(ui->demo_gui_cont_76, 0, -2);
	lv_obj_set_size(ui->demo_gui_cont_76, 521, 48);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_76, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_76, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_76, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_btn_9
	ui->demo_gui_btn_9 = lv_btn_create(ui->demo_gui_cont_76);
	ui->demo_gui_btn_9_label = lv_label_create(ui->demo_gui_btn_9);
	lv_label_set_text(ui->demo_gui_btn_9_label, "Connect");
	lv_label_set_long_mode(ui->demo_gui_btn_9_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_btn_9_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_btn_9, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->demo_gui_btn_9_label, LV_PCT(100));
	lv_obj_set_pos(ui->demo_gui_btn_9, 441, 17);
	lv_obj_set_size(ui->demo_gui_btn_9, 72, 28);

	//Write style for demo_gui_btn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_btn_9, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_btn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_btn_9, lv_color_hex(0x1b77bb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_btn_9, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_btn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_btn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_70
	ui->demo_gui_label_70 = lv_label_create(ui->demo_gui_cont_76);
	lv_label_set_text(ui->demo_gui_label_70, "Wifi AB");
	lv_label_set_long_mode(ui->demo_gui_label_70, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_70, 13, 16);
	lv_obj_set_size(ui->demo_gui_label_70, 69, 17);

	//Write style for demo_gui_label_70, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_70, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_70, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_70, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_70, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_70, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_70
	ui->demo_gui_cont_70 = lv_obj_create(ui->demo_gui_cont_65);
	lv_obj_set_pos(ui->demo_gui_cont_70, 183, 16);
	lv_obj_set_size(ui->demo_gui_cont_70, 276, 219);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_70, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_70, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_70, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_70, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_73
	ui->demo_gui_cont_73 = lv_obj_create(ui->demo_gui_cont_70);
	lv_obj_set_pos(ui->demo_gui_cont_73, 1, 10);
	lv_obj_set_size(ui->demo_gui_cont_73, 232, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_73, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_73, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_73, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_69
	ui->demo_gui_label_69 = lv_label_create(ui->demo_gui_cont_73);
	lv_label_set_text(ui->demo_gui_label_69, "Bluetooth");
	lv_label_set_long_mode(ui->demo_gui_label_69, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_69, 7, 4);
	lv_obj_set_size(ui->demo_gui_label_69, 91, 22);

	//Write style for demo_gui_label_69, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_69, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_69, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_69, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_69, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_69, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_69, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_sw_6
	ui->demo_gui_sw_6 = lv_switch_create(ui->demo_gui_cont_73);
	lv_obj_set_pos(ui->demo_gui_sw_6, 125, 8);
	lv_obj_set_size(ui->demo_gui_sw_6, 37, 18);

	//Write style for demo_gui_sw_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_6, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_6, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_sw_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_sw_6, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_6, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_6, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_6, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_sw_6, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for demo_gui_sw_6, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_sw_6, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_sw_6, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_sw_6, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_sw_6, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_sw_6, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_71
	ui->demo_gui_cont_71 = lv_obj_create(ui->demo_gui_cont_70);
	lv_obj_set_pos(ui->demo_gui_cont_71, -1, 62);
	lv_obj_set_size(ui->demo_gui_cont_71, 516, 143);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_71, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_71, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_71, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_71, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_72
	ui->demo_gui_cont_72 = lv_obj_create(ui->demo_gui_cont_71);
	lv_obj_set_pos(ui->demo_gui_cont_72, 0, -2);
	lv_obj_set_size(ui->demo_gui_cont_72, 527, 48);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_72, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_72, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_72, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_btn_8
	ui->demo_gui_btn_8 = lv_btn_create(ui->demo_gui_cont_72);
	ui->demo_gui_btn_8_label = lv_label_create(ui->demo_gui_btn_8);
	lv_label_set_text(ui->demo_gui_btn_8_label, "Connect");
	lv_label_set_long_mode(ui->demo_gui_btn_8_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->demo_gui_btn_8_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->demo_gui_btn_8, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->demo_gui_btn_8_label, LV_PCT(100));
	lv_obj_set_pos(ui->demo_gui_btn_8, 432, 15);
	lv_obj_set_size(ui->demo_gui_btn_8, 72, 28);

	//Write style for demo_gui_btn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->demo_gui_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_btn_8, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_btn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_btn_8, lv_color_hex(0x1b77bb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_btn_8, &lv_font_Amiko_Regular_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_btn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_btn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_68
	ui->demo_gui_label_68 = lv_label_create(ui->demo_gui_cont_72);
	lv_label_set_text(ui->demo_gui_label_68, "Bluetooth 1");
	lv_label_set_long_mode(ui->demo_gui_label_68, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_68, 1, 14);
	lv_obj_set_size(ui->demo_gui_label_68, 105, 17);

	//Write style for demo_gui_label_68, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_68, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_68, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_68, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_68, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_68, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_68
	ui->demo_gui_cont_68 = lv_obj_create(ui->demo_gui_cont_65);
	lv_obj_set_pos(ui->demo_gui_cont_68, 175, 22);
	lv_obj_set_size(ui->demo_gui_cont_68, 288, 223);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_68, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_68, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_68, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_68, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_69
	ui->demo_gui_cont_69 = lv_obj_create(ui->demo_gui_cont_68);
	lv_obj_set_pos(ui->demo_gui_cont_69, 7, 10);
	lv_obj_set_size(ui->demo_gui_cont_69, 273, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_69, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_69, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_69, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_69, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_67
	ui->demo_gui_label_67 = lv_label_create(ui->demo_gui_cont_69);
	lv_label_set_text(ui->demo_gui_label_67, "Language");
	lv_label_set_long_mode(ui->demo_gui_label_67, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_67, 8, -9);
	lv_obj_set_size(ui->demo_gui_label_67, 91, 22);

	//Write style for demo_gui_label_67, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_67, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_67, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_67, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_67, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_67, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_67, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_ddlist_2
	ui->demo_gui_ddlist_2 = lv_dropdown_create(ui->demo_gui_cont_68);
	lv_dropdown_set_options(ui->demo_gui_ddlist_2, "Vietnamese\nEnglish");
	lv_obj_set_pos(ui->demo_gui_ddlist_2, 6, 50);
	lv_obj_set_size(ui->demo_gui_ddlist_2, 259, 30);

	//Write style for demo_gui_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->demo_gui_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_ddlist_2, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->demo_gui_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_ddlist_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_ddlist_2, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_ddlist_2, 126, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->demo_gui_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for demo_gui_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_text_color(ui->demo_gui_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->demo_gui_ddlist_2, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->demo_gui_ddlist_2, 1, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_color(ui->demo_gui_ddlist_2, lv_color_hex(0x654f4f), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_side(ui->demo_gui_ddlist_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_top(ui->demo_gui_ddlist_2, 8, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_left(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_right(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_radius(ui->demo_gui_ddlist_2, 3, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->demo_gui_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->demo_gui_ddlist_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for demo_gui_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
	lv_obj_set_style_text_color(ui->demo_gui_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_text_font(ui->demo_gui_ddlist_2, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_text_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_width(ui->demo_gui_ddlist_2, 1, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_color(ui->demo_gui_ddlist_2, lv_color_hex(0x654f4f), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(ui->demo_gui_ddlist_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_top(ui->demo_gui_ddlist_2, 8, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_left(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_right(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_radius(ui->demo_gui_ddlist_2, 3, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_color(ui->demo_gui_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_shadow_width(ui->demo_gui_ddlist_2, 0, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style for demo_gui_ddlist_2, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
	lv_obj_set_style_text_color(ui->demo_gui_ddlist_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_text_font(ui->demo_gui_ddlist_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_text_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_width(ui->demo_gui_ddlist_2, 1, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_color(ui->demo_gui_ddlist_2, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_side(ui->demo_gui_ddlist_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_top(ui->demo_gui_ddlist_2, 8, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_left(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_right(ui->demo_gui_ddlist_2, 6, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_radius(ui->demo_gui_ddlist_2, 3, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_opa(ui->demo_gui_ddlist_2, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_color(ui->demo_gui_ddlist_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_grad_dir(ui->demo_gui_ddlist_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_shadow_width(ui->demo_gui_ddlist_2, 0, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for &style_demo_gui_ddlist_2_extra_list_selected_checked
	static lv_style_t style_demo_gui_ddlist_2_extra_list_selected_checked;
	ui_init_style(&style_demo_gui_ddlist_2_extra_list_selected_checked);
	
	lv_style_set_border_width(&style_demo_gui_ddlist_2_extra_list_selected_checked, 2);
	lv_style_set_border_opa(&style_demo_gui_ddlist_2_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_demo_gui_ddlist_2_extra_list_selected_checked, lv_color_hex(0x654f4f));
	lv_style_set_border_side(&style_demo_gui_ddlist_2_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_demo_gui_ddlist_2_extra_list_selected_checked, 3);
	lv_style_set_bg_opa(&style_demo_gui_ddlist_2_extra_list_selected_checked, 110);
	lv_style_set_bg_color(&style_demo_gui_ddlist_2_extra_list_selected_checked, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_demo_gui_ddlist_2_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->demo_gui_ddlist_2), &style_demo_gui_ddlist_2_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_ddlist_2_extra_list_main_default
	static lv_style_t style_demo_gui_ddlist_2_extra_list_main_default;
	ui_init_style(&style_demo_gui_ddlist_2_extra_list_main_default);
	
	lv_style_set_max_height(&style_demo_gui_ddlist_2_extra_list_main_default, 90);
	lv_style_set_text_color(&style_demo_gui_ddlist_2_extra_list_main_default, lv_color_hex(0xffffff));
	lv_style_set_text_font(&style_demo_gui_ddlist_2_extra_list_main_default, &lv_font_montserratMedium_14);
	lv_style_set_text_opa(&style_demo_gui_ddlist_2_extra_list_main_default, 255);
	lv_style_set_border_width(&style_demo_gui_ddlist_2_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_demo_gui_ddlist_2_extra_list_main_default, 255);
	lv_style_set_border_color(&style_demo_gui_ddlist_2_extra_list_main_default, lv_color_hex(0x654f4f));
	lv_style_set_border_side(&style_demo_gui_ddlist_2_extra_list_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_demo_gui_ddlist_2_extra_list_main_default, 3);
	lv_style_set_bg_opa(&style_demo_gui_ddlist_2_extra_list_main_default, 138);
	lv_style_set_bg_color(&style_demo_gui_ddlist_2_extra_list_main_default, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_demo_gui_ddlist_2_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->demo_gui_ddlist_2), &style_demo_gui_ddlist_2_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_ddlist_2_extra_list_scrollbar_default
	static lv_style_t style_demo_gui_ddlist_2_extra_list_scrollbar_default;
	ui_init_style(&style_demo_gui_ddlist_2_extra_list_scrollbar_default);
	
	lv_style_set_radius(&style_demo_gui_ddlist_2_extra_list_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_demo_gui_ddlist_2_extra_list_scrollbar_default, 255);
	lv_style_set_bg_color(&style_demo_gui_ddlist_2_extra_list_scrollbar_default, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_demo_gui_ddlist_2_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->demo_gui_ddlist_2), &style_demo_gui_ddlist_2_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_66
	ui->demo_gui_cont_66 = lv_obj_create(ui->demo_gui_cont_65);
	lv_obj_set_pos(ui->demo_gui_cont_66, 179, 20);
	lv_obj_set_size(ui->demo_gui_cont_66, 286, 241);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_66, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->demo_gui_cont_66, LV_OBJ_FLAG_HIDDEN);

	//Write style for demo_gui_cont_66, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_cont_67
	ui->demo_gui_cont_67 = lv_obj_create(ui->demo_gui_cont_66);
	lv_obj_set_pos(ui->demo_gui_cont_67, -27, -14);
	lv_obj_set_size(ui->demo_gui_cont_67, 254, 31);
	lv_obj_set_scrollbar_mode(ui->demo_gui_cont_67, LV_SCROLLBAR_MODE_OFF);

	//Write style for demo_gui_cont_67, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_cont_67, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_label_66
	ui->demo_gui_label_66 = lv_label_create(ui->demo_gui_cont_67);
	lv_label_set_text(ui->demo_gui_label_66, "Notification");
	lv_label_set_long_mode(ui->demo_gui_label_66, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->demo_gui_label_66, 31, 30);
	lv_obj_set_size(ui->demo_gui_label_66, 117, 22);

	//Write style for demo_gui_label_66, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->demo_gui_label_66, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->demo_gui_label_66, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->demo_gui_label_66, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->demo_gui_label_66, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->demo_gui_label_66, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->demo_gui_label_66, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->demo_gui_label_66, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes demo_gui_list_2
	ui->demo_gui_list_2 = lv_list_create(ui->demo_gui_cont_66);
	ui->demo_gui_list_2_item0 = lv_list_add_btn(ui->demo_gui_list_2, LV_SYMBOL_BATTERY_1, "Battery is running low");
	ui->demo_gui_list_2_item1 = lv_list_add_btn(ui->demo_gui_list_2, LV_SYMBOL_REFRESH, "New software is available");
	ui->demo_gui_list_2_item2 = lv_list_add_btn(ui->demo_gui_list_2, LV_SYMBOL_WARNING, "Lights ");
	lv_obj_set_pos(ui->demo_gui_list_2, -4, 58);
	lv_obj_set_size(ui->demo_gui_list_2, 293, 153);
	lv_obj_set_scrollbar_mode(ui->demo_gui_list_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_list_2_main_main_default
	static lv_style_t style_demo_gui_list_2_main_main_default;
	ui_init_style(&style_demo_gui_list_2_main_main_default);
	
	lv_style_set_pad_top(&style_demo_gui_list_2_main_main_default, 1);
	lv_style_set_pad_left(&style_demo_gui_list_2_main_main_default, 0);
	lv_style_set_pad_right(&style_demo_gui_list_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_demo_gui_list_2_main_main_default, 0);
	lv_style_set_bg_opa(&style_demo_gui_list_2_main_main_default, 0);
	lv_style_set_border_width(&style_demo_gui_list_2_main_main_default, 0);
	lv_style_set_radius(&style_demo_gui_list_2_main_main_default, 7);
	lv_style_set_shadow_width(&style_demo_gui_list_2_main_main_default, 0);
	lv_obj_add_style(ui->demo_gui_list_2, &style_demo_gui_list_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_list_2_main_scrollbar_default
	static lv_style_t style_demo_gui_list_2_main_scrollbar_default;
	ui_init_style(&style_demo_gui_list_2_main_scrollbar_default);
	
	lv_style_set_radius(&style_demo_gui_list_2_main_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_demo_gui_list_2_main_scrollbar_default, 255);
	lv_style_set_bg_color(&style_demo_gui_list_2_main_scrollbar_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_demo_gui_list_2_main_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(ui->demo_gui_list_2, &style_demo_gui_list_2_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_list_2_extra_btns_main_default
	static lv_style_t style_demo_gui_list_2_extra_btns_main_default;
	ui_init_style(&style_demo_gui_list_2_extra_btns_main_default);
	
	lv_style_set_pad_top(&style_demo_gui_list_2_extra_btns_main_default, 5);
	lv_style_set_pad_left(&style_demo_gui_list_2_extra_btns_main_default, 10);
	lv_style_set_pad_right(&style_demo_gui_list_2_extra_btns_main_default, 0);
	lv_style_set_pad_bottom(&style_demo_gui_list_2_extra_btns_main_default, 10);
	lv_style_set_border_width(&style_demo_gui_list_2_extra_btns_main_default, 1);
	lv_style_set_border_opa(&style_demo_gui_list_2_extra_btns_main_default, 255);
	lv_style_set_border_color(&style_demo_gui_list_2_extra_btns_main_default, lv_color_hex(0x6f6969));
	lv_style_set_border_side(&style_demo_gui_list_2_extra_btns_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_text_color(&style_demo_gui_list_2_extra_btns_main_default, lv_color_hex(0xfef6ea));
	lv_style_set_text_font(&style_demo_gui_list_2_extra_btns_main_default, &lv_font_FontAwesome5_18);
	lv_style_set_text_opa(&style_demo_gui_list_2_extra_btns_main_default, 255);
	lv_style_set_radius(&style_demo_gui_list_2_extra_btns_main_default, 0);
	lv_style_set_bg_opa(&style_demo_gui_list_2_extra_btns_main_default, 0);
	lv_obj_add_style(ui->demo_gui_list_2_item2, &style_demo_gui_list_2_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->demo_gui_list_2_item1, &style_demo_gui_list_2_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->demo_gui_list_2_item0, &style_demo_gui_list_2_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_demo_gui_list_2_extra_texts_main_default
	static lv_style_t style_demo_gui_list_2_extra_texts_main_default;
	ui_init_style(&style_demo_gui_list_2_extra_texts_main_default);
	
	lv_style_set_pad_top(&style_demo_gui_list_2_extra_texts_main_default, 6);
	lv_style_set_pad_left(&style_demo_gui_list_2_extra_texts_main_default, 5);
	lv_style_set_pad_right(&style_demo_gui_list_2_extra_texts_main_default, 0);
	lv_style_set_pad_bottom(&style_demo_gui_list_2_extra_texts_main_default, 0);
	lv_style_set_border_width(&style_demo_gui_list_2_extra_texts_main_default, 0);
	lv_style_set_text_color(&style_demo_gui_list_2_extra_texts_main_default, lv_color_hex(0x0D3055));
	lv_style_set_text_font(&style_demo_gui_list_2_extra_texts_main_default, &lv_font_montserratMedium_18);
	lv_style_set_text_opa(&style_demo_gui_list_2_extra_texts_main_default, 255);
	lv_style_set_radius(&style_demo_gui_list_2_extra_texts_main_default, 3);
	lv_style_set_bg_opa(&style_demo_gui_list_2_extra_texts_main_default, 0);

	//The custom code of demo_gui.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->demo_gui);

	//Init events for screen.
	events_init_demo_gui(ui);
}
