////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#include <hal/hal_i2c.h>
#include <hal/hal_pctl.h>
#include <hal/hal_nvic.h>
#include <hal/hal_gpio.h>
#include <hal/hal_ustimer.h>
#include <debug.h>

#if 0
static void i2c_dump_reg(void)
{
    printf("------------------------------\n");
    printf("  I2C_SLAVE_ADDR       = %04X \n", (int)I2C_SLAVE_REG);
    printf("  I2C_INT_MASK         = %04X \n", (int)I2C_INT_MASK_REG);
    printf("  I2C_INT_STAT         = %04X \n", (int)I2C_INT_STAT_REG);
    printf("  I2C_CONTROL          = %04X \n", (int)I2C_CONTROL_REG);
    printf("  I2C_TRANSFER_LEN     = %04X \n", (int)I2C_TRANSFER_LEN_REG);
    printf("  I2C_TRANSAC_LEN      = %04X \n", (int)I2C_TRANSAC_LEN_REG);
    printf("  I2C_DELAY_LEN        = %04X \n", (int)I2C_DELAY_LEN_REG);
    printf("  I2C_TIMING           = %04X \n", (int)I2C_TIMING_REG);
    printf("  I2C_FIFO_STAT        = %04X \n", (int)I2C_FIFO_STAT_REG);
    printf("  I2C_IO_CONFIG        = %04X \n", (int)I2C_IO_CONFIG_REG);
    printf("  I2C_HS               = %04X \n", (int)I2C_HS_REG);
    printf("  I2C_TRANSFER_LEN_AUX = %04X \n", (int)I2C_TRANSFER_LEN_AUX_REG);
    printf("  I2C_TIMEOUT          = %04X \n", (int)I2C_TIMEOUT_REG);
}
#endif

static int i2c_current_owner = 0;
static int i2c_in_use(i2c_context_t *ctx)
{
    if (NULL == ctx)
        return -1;
    if (ctx->id == i2c_current_owner)
        return 0;
    return -1;
}

i2c_context_t *I2C_Create(void)
{
    i2c_context_t *ctx = (i2c_context_t *)calloc(sizeof(spi_context), 1);
    static int owner = 1;
    ctx->id = owner++;
    ctx->config.SlaveAddress = 0x3C; // 7 bit
    ctx->config.LoSpeed = 100;
    ctx->config.HiSpeed = 100;
    ctx->config.Delay = 4;
    ctx->config.IO = 0; // 4;
    return ctx;
}

int I2C_Close(i2c_context_t *ctx)
{
    if (i2c_in_use(ctx))
        return -1;

    //begin-protect
    i2c_current_owner = 0;
    //end-protect

    return 0;
}

int I2C_Open(i2c_context_t *ctx)
{
    if (i2c_current_owner || ctx == NULL)
        return -1; // is open

    //begin-protect
    i2c_current_owner = ctx->id;
    //end-protect

    I2C_SOFTRESET_REG = 1;
    GPIO_Setup(GPIO43, GPMODE(GPIO43_MODE_SCL) | GPPULLEN | GPPUP);
    GPIO_Setup(GPIO44, GPMODE(GPIO44_MODE_SDA) | GPPULLEN | GPPUP);
    return 0;
}

void I2C_RecalculateSpeed(i2c_context_t *ctx)
{
    if (NULL == ctx)
        return;
    uint32_t step_cnt_div, sample_cnt_div, temp;

    //Fast Mode Speed
    for (sample_cnt_div = 1; sample_cnt_div <= 8; sample_cnt_div++)
    {
        if (0 != ctx->config.LoSpeed)
            temp = (ctx->config.LoSpeed * 2 * sample_cnt_div);
        else
            temp = 2 * sample_cnt_div;
        step_cnt_div = (I2C_CLOCK + temp - 1) / temp;
        if (step_cnt_div <= 64)
            break;
    }
    if (step_cnt_div > 64 && sample_cnt_div > 8)
    {
        step_cnt_div = 64;
        sample_cnt_div = 8;
    }
    ctx->fs_sample_cnt_div = sample_cnt_div - 1;
    ctx->fs_step_cnt_div = step_cnt_div - 1;

    //HS Mode Speed
    for (sample_cnt_div = 1; sample_cnt_div <= 8; sample_cnt_div++)
    {
        if (0 != ctx->config.HiSpeed)
            temp = (ctx->config.HiSpeed * 2 * sample_cnt_div);
        else
            temp = (1 * 2 * sample_cnt_div);
        step_cnt_div = (I2C_CLOCK + temp - 1) / temp;
        if (step_cnt_div <= 8)
            break;
    }
    ctx->hs_sample_cnt_div = sample_cnt_div - 1;
    ctx->hs_step_cnt_div = step_cnt_div - 1;
}

int I2C_UpdateSpeed(i2c_context_t *ctx, bool recalculate)
{
    if (i2c_in_use(ctx))
        return -1;
    if (recalculate)
        I2C_RecalculateSpeed(ctx);
    if (ctx->config.Mode)
    {
        I2C_HS.EN = 1;
        //I2C_HS.NACKERR_DET_EN = 1;
        I2C_HS.SAMPLE_CNT_DIV = ctx->hs_sample_cnt_div;
        I2C_HS.STEP_CNT_DIV = ctx->hs_step_cnt_div;
        I2C_CONTROL.RS_STOP = 1; // In HS mode, this bit must be set to 1.
        if (ctx->config.Delay)
            I2C_DELAY_LEN_REG == ctx->config.Delay - 1;
    }
    else
    {
        I2C_HS.EN = 0;
        I2C_TIMING.SAMPLE_CNT_DIV = ctx->fs_sample_cnt_div;
        I2C_TIMING.STEP_CNT_DIV = ctx->fs_step_cnt_div;
    }
    return 0;
}

int I2C_BeginTransaction(i2c_context_t *ctx, I2C_TRANSACTION_TYPE command,
                         uint8_t *write_buffer, uint32_t write_len,
                         uint8_t *read_buffer, uint32_t read_len)
{
    if (i2c_in_use(ctx))
        return -1;

    PCTL_PowerUp(PD_I2C);
    I2C_HS.NACKERR_DET_EN = 1;
    I2C_CONTROL_REG = I2C_CONTROL_ACKERR_DET_EN || I2C_CONTROL_CLKEXT_EN;
    I2C_IO_CONFIG_REG = ctx->config.IO;

    I2C_INT_STAT_REG = -1; // clear all
    I2C_FIFO_ADDR_CLR_REG = 1;

    I2C_TRANSAC_LEN_REG = 1;
    I2C_SLAVE_REG = ctx->config.SlaveAddress << 1;

    I2C_UpdateSpeed(ctx, false);

    switch (command)
    {
    case I2C_TRANSACTION_WRITE_AND_READ:
        I2C_CONTROL.RS_STOP = 1;
        I2C_CONTROL.DIR_CHANGE = 1;
        I2C_TRANSFER_LEN_AUX_REG = read_len;
        /* no break */
    case I2C_TRANSACTION_CONT_WRITE:
        if (ctx->config.Delay == 0)
            I2C_CONTROL.RS_STOP = 1;
        /* no break */
    case I2C_TRANSACTION_WRITE:
        I2C_TRANSFER_LEN_REG = write_len;
        for (int i = 0; i < write_len; i++)
            I2C_DATA_PORT_REG = *write_buffer++;
        break;
    case I2C_TRANSACTION_CONT_READ:
        if (ctx->config.Delay == 0)
            I2C_CONTROL.RS_STOP = 1;
        /* no break */
    case I2C_TRANSACTION_READ:
        I2C_ADDRESS.RW = 1;
        I2C_TRANSFER_LEN_REG = read_len;
        break;
    }

    I2C_START_REG = 1;
    int sta;
    do
        sta = I2C_INT_STAT_REG;
    while (0 == sta);

    int res = 0;
    if (sta >> 1) // test errors
    {
        res = -(sta >> 1);
        goto end;
    }

    if (read_buffer && read_len)
        for (int i = 0; i < read_len; ++i)
            *read_buffer++ = I2C_DATA_PORT_REG;

end:
    I2C_INT_STAT_REG = -1;
    PCTL_PowerDown(PD_I2C);
    return res;
}
