#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define UART_DEVICE_NODE DT_NODELABEL(uart0)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

#define RX_BUF_SIZE 128
static char rx_buf[RX_BUF_SIZE];
static int rx_pos = 0;

static void uart_isr(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);

    while (uart_irq_update(dev) && uart_irq_rx_ready(dev)) {
        unsigned char c;
        int ret = uart_fifo_read(dev, &c, 1);
        if (ret > 0) {
            if (c == '\r' || c == '\n') {
                rx_buf[rx_pos] = '\0';
                printk("Recebido: %s\n", rx_buf);
                rx_pos = 0;
            } else if (rx_pos < RX_BUF_SIZE - 1) {
                rx_buf[rx_pos++] = c;
            }
        }
    }
}

void main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART não está pronta!\n");
        return;
    }

    printk("UART por interrupção pronta. Envie texto.\n");

    uart_irq_callback_user_data_set(uart_dev, uart_isr, NULL);
    uart_irq_rx_enable(uart_dev);

    while (1) {
        k_sleep(K_FOREVER);
    }
}
