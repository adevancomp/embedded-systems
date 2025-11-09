#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define UART_DEVICE_NODE DT_NODELABEL(uart1)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

#define RX_BUF_SIZE 128
static char rx_buf[RX_BUF_SIZE];

void main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready!\n");
        return;
    }

    printk("UART Polling ready. Send string into terminal.\n");

    int pos = 0;
    for (;;) {
        unsigned char c;
        int ret = uart_poll_in(uart_dev, &c);
        if (ret == 0) {
            if (c == '\r' || c == '\n') {
                rx_buf[pos] = '\0';
                printk("Received: %s\n", rx_buf);

                const char *reply = "OK\n";
                for (int i = 0; reply[i]; i++)
                    uart_poll_out(uart_dev, reply[i]);

                pos = 0;
            } else if (pos < RX_BUF_SIZE - 1) {
                rx_buf[pos++] = c;
            }
        } else {
            k_sleep(K_MSEC(10));
        }
    }
}
