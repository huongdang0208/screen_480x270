#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <gui_guider.h>
#include <pthread.h>

#include "../custom/sensors/sht30.c"
#include "../custom/led_change.c"


#define DISP_BUF_SIZE (1024 * 1024)

lv_style_t  style;
lv_ui guider_ui;

int main(int argc, char *argv[])
{
    int hor_res = 800;
    int ver_res = 600;

    if (argc >= 3)
    {
        hor_res = atoi(argv[1]);
        ver_res = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr,"Warring: base Usage: %s [w,h]\r\n"
               "base eg: %s 1024 600 ",
               argv[0], argv[0]);
    }

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    // Touch pointer device init
	evdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = hor_res;
    disp_drv.ver_res = ver_res;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    evdev_init();
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = (void (*)(struct _lv_indev_drv_t *, lv_indev_data_t *))evdev_read; // defined in lv_drivers/indev/evdev.h
    lv_indev_t *lv_indev = lv_indev_drv_register(&indev_drv);
    if (!lv_indev)
    {
        printf("lv_indev rregister error %d \r\n", __LINE__);
        return 0;
    }

    //app

    ui_init_style(&style);
    init_scr_del_flag(&guider_ui);
    setup_ui(&guider_ui);

    // Create a new thread for sensor reading
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, sensor_thread, (void*)&guider_ui) != 0) {
        fprintf(stderr, "Failed to create sensor thread.\n");
        return 1;
    }

    pthread_t thread_id_led;
    if (pthread_create(&thread_id_led, NULL, led_change_st, (void*)&guider_ui) != 0) {
        fprintf(stderr, "Failed to create sensor thread.\n");
        return 1;
    }

    /*Handle LitlevGL tasks (tickless mode)*/
    while (1)
    {
        lv_task_handler();
        usleep(5000);
    }

    // Join the thread before exiting (if needed)
    // pthread_join(thread_id, NULL);


    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}