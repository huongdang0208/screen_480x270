/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *demo_gui;
	bool demo_gui_del;
	lv_obj_t *demo_gui_tileview_1;
	lv_obj_t *demo_gui_tileview_1_main;
	lv_obj_t *demo_gui_tileview_1_settings;
	lv_obj_t *demo_gui_tileview_2;
	lv_obj_t *demo_gui_tileview_2_home_page;
	lv_obj_t *demo_gui_tileview_2_side_page;
	lv_obj_t *demo_gui_img_1;
	lv_obj_t *demo_gui_cont_1;
	lv_obj_t *demo_gui_imgbtn_2;
	lv_obj_t *demo_gui_imgbtn_2_label;
	lv_obj_t *demo_gui_label_2;
	lv_obj_t *demo_gui_imgbtn_1;
	lv_obj_t *demo_gui_imgbtn_1_label;
	lv_obj_t *demo_gui_label_1;
	lv_obj_t *demo_gui_cont_2;
	lv_obj_t *demo_gui_label_4;
	lv_obj_t *demo_gui_label_3;
	lv_obj_t *demo_gui_cont_28;
	lv_obj_t *demo_gui_cont_41;
	lv_obj_t *demo_gui_label_29;
	lv_obj_t *demo_gui_cont_42;
	lv_obj_t *demo_gui_imgbtn_10;
	lv_obj_t *demo_gui_imgbtn_10_label;
	lv_obj_t *demo_gui_btn_5;
	lv_obj_t *demo_gui_btn_5_label;
	lv_obj_t *demo_gui_cont_43;
	lv_obj_t *demo_gui_imgbtn_11;
	lv_obj_t *demo_gui_imgbtn_11_label;
	lv_obj_t *demo_gui_btn_6;
	lv_obj_t *demo_gui_btn_6_label;
	lv_obj_t *demo_gui_cont_44;
	lv_obj_t *demo_gui_imgbtn_12;
	lv_obj_t *demo_gui_imgbtn_12_label;
	lv_obj_t *demo_gui_btn_7;
	lv_obj_t *demo_gui_btn_7_label;
	lv_obj_t *demo_gui_cont_33;
	lv_obj_t *demo_gui_cont_45;
	lv_obj_t *demo_gui_imgbtn_13;
	lv_obj_t *demo_gui_imgbtn_13_label;
	lv_obj_t *demo_gui_label_30;
	lv_obj_t *demo_gui_label_32;
	lv_obj_t *demo_gui_cont_46;
	lv_obj_t *demo_gui_imgbtn_14;
	lv_obj_t *demo_gui_imgbtn_14_label;
	lv_obj_t *demo_gui_label_35;
	lv_obj_t *demo_gui_label_33;
	lv_obj_t *demo_gui_cont_47;
	lv_obj_t *demo_gui_imgbtn_15;
	lv_obj_t *demo_gui_imgbtn_15_label;
	lv_obj_t *demo_gui_label_38;
	lv_obj_t *demo_gui_label_36;
	lv_obj_t *demo_gui_cont_48;
	lv_obj_t *demo_gui_cont_51;
	lv_obj_t *demo_gui_label_47;
	lv_obj_t *demo_gui_sw_3;
	lv_obj_t *demo_gui_cont_52;
	lv_obj_t *demo_gui_label_48;
	lv_obj_t *demo_gui_sw_4;
	lv_obj_t *demo_gui_cont_53;
	lv_obj_t *demo_gui_label_49;
	lv_obj_t *demo_gui_sw_5;
	lv_obj_t *demo_gui_cont_54;
	lv_obj_t *demo_gui_cont_57;
	lv_obj_t *demo_gui_label_52;
	lv_obj_t *demo_gui_digital_clock_1;
	lv_obj_t *demo_gui_datetext_1;
	lv_obj_t *demo_gui_cont_58;
	lv_obj_t *demo_gui_label_53;
	lv_obj_t *demo_gui_imgbtn_18;
	lv_obj_t *demo_gui_imgbtn_18_label;
	lv_obj_t *demo_gui_cont_59;
	lv_obj_t *demo_gui_label_56;
	lv_obj_t *demo_gui_imgbtn_19;
	lv_obj_t *demo_gui_imgbtn_19_label;
	lv_obj_t *demo_gui_cont_60;
	lv_obj_t *demo_gui_label_58;
	lv_obj_t *demo_gui_imgbtn_20;
	lv_obj_t *demo_gui_imgbtn_20_label;
	lv_obj_t *demo_gui_cont_83;
	lv_obj_t *demo_gui_label_80;
	lv_obj_t *demo_gui_digital_clock_2;
	lv_obj_t *demo_gui_datetext_2;
	lv_obj_t *demo_gui_cont_86;
	lv_obj_t *demo_gui_label_79;
	lv_obj_t *demo_gui_imgbtn_30;
	lv_obj_t *demo_gui_imgbtn_30_label;
	lv_obj_t *demo_gui_cont_85;
	lv_obj_t *demo_gui_label_78;
	lv_obj_t *demo_gui_imgbtn_29;
	lv_obj_t *demo_gui_imgbtn_29_label;
	lv_obj_t *demo_gui_cont_84;
	lv_obj_t *demo_gui_label_77;
	lv_obj_t *demo_gui_imgbtn_28;
	lv_obj_t *demo_gui_imgbtn_28_label;
	lv_obj_t *demo_gui_cont_65;
	lv_obj_t *demo_gui_cont_78;
	lv_obj_t *demo_gui_cont_82;
	lv_obj_t *demo_gui_label_76;
	lv_obj_t *demo_gui_imgbtn_27;
	lv_obj_t *demo_gui_imgbtn_27_label;
	lv_obj_t *demo_gui_label_75;
	lv_obj_t *demo_gui_cont_81;
	lv_obj_t *demo_gui_label_74;
	lv_obj_t *demo_gui_imgbtn_26;
	lv_obj_t *demo_gui_imgbtn_26_label;
	lv_obj_t *demo_gui_cont_80;
	lv_obj_t *demo_gui_label_73;
	lv_obj_t *demo_gui_imgbtn_25;
	lv_obj_t *demo_gui_imgbtn_25_label;
	lv_obj_t *demo_gui_cont_79;
	lv_obj_t *demo_gui_label_72;
	lv_obj_t *demo_gui_imgbtn_24;
	lv_obj_t *demo_gui_imgbtn_24_label;
	lv_obj_t *demo_gui_cont_74;
	lv_obj_t *demo_gui_cont_77;
	lv_obj_t *demo_gui_label_71;
	lv_obj_t *demo_gui_sw_7;
	lv_obj_t *demo_gui_cont_75;
	lv_obj_t *demo_gui_cont_76;
	lv_obj_t *demo_gui_btn_9;
	lv_obj_t *demo_gui_btn_9_label;
	lv_obj_t *demo_gui_label_70;
	lv_obj_t *demo_gui_cont_70;
	lv_obj_t *demo_gui_cont_73;
	lv_obj_t *demo_gui_label_69;
	lv_obj_t *demo_gui_sw_6;
	lv_obj_t *demo_gui_cont_71;
	lv_obj_t *demo_gui_cont_72;
	lv_obj_t *demo_gui_btn_8;
	lv_obj_t *demo_gui_btn_8_label;
	lv_obj_t *demo_gui_label_68;
	lv_obj_t *demo_gui_cont_68;
	lv_obj_t *demo_gui_cont_69;
	lv_obj_t *demo_gui_label_67;
	lv_obj_t *demo_gui_ddlist_2;
	lv_obj_t *demo_gui_cont_66;
	lv_obj_t *demo_gui_cont_67;
	lv_obj_t *demo_gui_label_66;
	lv_obj_t *demo_gui_list_2;
	lv_obj_t *demo_gui_list_2_item0;
	lv_obj_t *demo_gui_list_2_item1;
	lv_obj_t *demo_gui_list_2_item2;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_demo_gui(lv_ui *ui);
LV_IMG_DECLARE(_background_alpha_480x280);
LV_IMG_DECLARE(_itemperature_alpha_27x21);
LV_IMG_DECLARE(_ihumidity_alpha_16x15);
LV_IMG_DECLARE(_led_alpha_25x24);
LV_IMG_DECLARE(_switch_alpha_25x24);
LV_IMG_DECLARE(_timer_alpha_25x24);
LV_IMG_DECLARE(_offlight_alpha_39x37);
LV_IMG_DECLARE(_onlight_alpha_39x37);
LV_IMG_DECLARE(_offlight_alpha_39x37);
LV_IMG_DECLARE(_onlight_alpha_39x37);
LV_IMG_DECLARE(_offlight_alpha_39x37);
LV_IMG_DECLARE(_onlight_alpha_39x37);
LV_IMG_DECLARE(_onlight_alpha_26x29);
LV_IMG_DECLARE(_offlight_alpha_28x29);
LV_IMG_DECLARE(_iswitch_alpha_26x22);
LV_IMG_DECLARE(_onlight_alpha_26x29);
LV_IMG_DECLARE(_offlight_alpha_28x29);
LV_IMG_DECLARE(_iswitch_alpha_26x22);
LV_IMG_DECLARE(_arrow_alpha_22x23);
LV_IMG_DECLARE(_arrow_alpha_24x22);
LV_IMG_DECLARE(_arrow_alpha_27x23);
LV_IMG_DECLARE(_arrow_alpha_25x21);

LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_14)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_Antonio_Regular_30)
LV_FONT_DECLARE(lv_font_Acme_Regular_20)
LV_FONT_DECLARE(lv_font_montserratMedium_10)
LV_FONT_DECLARE(lv_font_Antonio_Regular_16)
LV_FONT_DECLARE(lv_font_Antonio_Regular_15)
LV_FONT_DECLARE(lv_font_montserratMedium_13)
LV_FONT_DECLARE(lv_font_Amiko_Regular_13)
LV_FONT_DECLARE(lv_font_FontAwesome5_18)


#ifdef __cplusplus
}
#endif
#endif
