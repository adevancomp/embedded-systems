#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define UART_DEVICE_NODE DT_NODELABEL(uart1)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

#define RX_BUF_SIZE 128
static char rx_buf[RX_BUF_SIZE];
static size_t rx_pos = 0;

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(user_data);

    switch (evt->type) {
    case UART_RX_RDY:
        for (size_t i = 0; i < evt->data.rx.len; i++) {
            char c = evt->data.rx.buf[evt->data.rx.offset + i];
            if (c == '\r' || c == '\n') {
                rx_buf[rx_pos] = '\0';
                printk("Received: %s\n", rx_buf);
                rx_pos = 0;
            } else if (rx_pos < RX_BUF_SIZE - 1) {
                rx_buf[rx_pos++] = c;
            }
        }
        break;

    case UART_RX_DISABLED:
        uart_rx_enable(uart_dev, evt->data.rx.buf, RX_BUF_SIZE, 50);
        break;

    default:
        break;
    }
}

void main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready!\n");
        return;
    }

    printk("UART ready. Send string into terminal.\n");

    static uint8_t rx_temp_buf[RX_BUF_SIZE];
    uart_callback_set(uart_dev, uart_cb, NULL);
    uart_rx_enable(uart_dev, rx_temp_buf, sizeof(rx_temp_buf), 50);

    while (1) {
        k_sleep(K_FOREVER);
    }
}
