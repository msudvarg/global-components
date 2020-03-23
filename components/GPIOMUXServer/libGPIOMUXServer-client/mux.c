/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <errno.h>

#include <utils/util.h>
#include <gpiomuxserver.h>

typedef struct gpiomuxserver_mux_interface_data {
    int (*gpiomuxserver_feature_enable)(mux_feature_t mux_feature, mux_gpio_dir_t mux_gpio_dir);
    int (*gpiomuxserver_feature_disable)(mux_feature_t mux_feature);
} gpiomuxserver_mux_interface_data_t;

static gpiomuxserver_mux_interface_data_t gpiomuxserver_mux_private_data;

static int gpiomuxserver_feature_enable_wrapper(mux_sys_t *mux, mux_feature_t feature, enum mux_gpio_dir dir)
{
    gpiomuxserver_mux_interface_data_t *gpiomuxserver_data = mux->priv;

    return gpiomuxserver_data->gpiomuxserver_feature_enable(feature, dir);
}

static int gpiomuxserver_feature_disable_wrapper(mux_sys_t *mux, mux_feature_t feature)
{
    gpiomuxserver_mux_interface_data_t *gpiomuxserver_data = mux->priv;

    return gpiomuxserver_data->gpiomuxserver_feature_disable(feature);
}

int gpiomuxserver_mux_interface_init(int (*gpiomuxserver_feature_enable)(mux_feature_t mux_feature,
                                                                         mux_gpio_dir_t mux_gpio_dir),
                                     int (*gpiomuxserver_feature_disable)(mux_feature_t mux_feature),
                                     mux_sys_t *mux_sys)
{
    if (!gpiomuxserver_feature_enable) {
        ZF_LOGE("gpiomuxserver_feature_enable is NULL");
        return -EINVAL;
    }

    if (!gpiomuxserver_feature_disable) {
        ZF_LOGE("gpiomuxserver_feature_disable is NULL");
        return -EINVAL;
    }

    if (!mux_sys) {
        ZF_LOGE("mux_sys is NULL");
        return -EINVAL;
    }

    gpiomuxserver_mux_private_data.gpiomuxserver_feature_enable = gpiomuxserver_feature_enable;
    gpiomuxserver_mux_private_data.gpiomuxserver_feature_disable = gpiomuxserver_feature_disable;

    mux_sys->priv = &gpiomuxserver_mux_private_data;
    mux_sys->feature_enable = gpiomuxserver_feature_enable_wrapper;
    mux_sys->feature_disable = gpiomuxserver_feature_disable_wrapper;

    return 0;
}
