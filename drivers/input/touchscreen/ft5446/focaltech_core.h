/*
 *
 * FocalTech TouchScreen driver.
 *
 * Copyright (c) 2010-2017, Focaltech Ltd. All rights reserved.
 * Copyright (C) 2018 XiaoMi, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LINUX_FOCALTECH_CORE_H__
#define __LINUX_FOCALTECH_CORE_H__
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/firmware.h>
#include <linux/debugfs.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mount.h>
#include <linux/netdevice.h>
#include <linux/unistd.h>
#include <linux/ioctl.h>
#include <linux/vmalloc.h>
#include "focaltech_common.h"
#define FTS_MAX_POINTS_SUPPORT              10
#define FTS_MAX_KEYS                        4
#define FTS_KEY_WIDTH                       50
#define FTS_ONE_TCH_LEN                     6
#define FTS_MAX_ID                          0x0A
#define FTS_TOUCH_X_H_POS                   3
#define FTS_TOUCH_X_L_POS                   4
#define FTS_TOUCH_Y_H_POS                   5
#define FTS_TOUCH_Y_L_POS                   6
#define FTS_TOUCH_PRE_POS                   7
#define FTS_TOUCH_AREA_POS                  8
#define FTS_TOUCH_POINT_NUM                 2
#define FTS_TOUCH_EVENT_POS                 3
#define FTS_TOUCH_ID_POS                    5
#define FTS_COORDS_ARR_SIZE                 4
#define FTS_TOUCH_DOWN                      0
#define FTS_TOUCH_UP                        1
#define FTS_TOUCH_CONTACT                   2
#define EVENT_DOWN(flag)                    ((FTS_TOUCH_DOWN == flag) || (FTS_TOUCH_CONTACT == flag))
#define EVENT_UP(flag)                      (FTS_TOUCH_UP == flag)
#define EVENT_NO_DOWN(data)                 (!data->point_num)
#define KEY_EN(data)                        (data->pdata->have_key)
#define TOUCH_IS_KEY(y, key_y)              (y == key_y)
#define TOUCH_IN_RANGE(val, key_val, half)  ((val > (key_val - half)) && (val < (key_val + half)))
#define TOUCH_IN_KEY(x, key_x)              TOUCH_IN_RANGE(x, key_x, FTS_KEY_WIDTH)
struct fts_ts_platform_data {
	u32 irq_gpio;
	u32 irq_gpio_flags;
	u32 reset_gpio;
	u32 reset_gpio_flags;
	bool have_key;
	u32 key_number;
	u32 keys[FTS_MAX_KEYS];
	u32 key_y_coord;
	u32 key_x_coords[FTS_MAX_KEYS];
	u32 x_max;
	u32 y_max;
	u32 x_min;
	u32 y_min;
	u32 max_touch_number;
};
struct ts_event {
	int x;
	int y;
	int p;
	int flag;
	int id;
	int area;
};
struct fts_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct fts_ts_platform_data *pdata;
	struct ts_ic_info ic_info;
	struct workqueue_struct *ts_workqueue;
	struct regulator *vdd;
	struct regulator *vcc_i2c;
	spinlock_t irq_lock;
	struct mutex report_mutex;
	int irq;
	bool suspended;
	bool irq_disabled;
	struct ts_event *events;
	u8 *point_buf;
	int pnt_buf_size;
	int touchs;
	bool key_down;
	int touch_point;
	int point_num;
	struct pinctrl *pinctrl;
	struct pinctrl_state *pins_active;
	struct pinctrl_state *pins_suspend;
	struct pinctrl_state *pins_release;
#if defined(CONFIG_FB)
	struct notifier_block fb_notif;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
};
extern struct fts_ts_data *fts_data;
int fts_i2c_write_reg(struct i2c_client *client, u8 regaddr, u8 regvalue);
int fts_i2c_read_reg(struct i2c_client *client, u8 regaddr, u8 *regvalue);
int fts_i2c_read(struct i2c_client *client, char *writebuf, int writelen, char *readbuf, int readlen);
int fts_i2c_write(struct i2c_client *client, char *writebuf, int writelen);
void fts_i2c_hid2std(struct i2c_client *client);
int fts_i2c_init(void);
int fts_i2c_exit(void);
void fts_irq_disable(void);
void fts_irq_enable(void);
#endif