#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

QueueHandle_t xQueueButId_R;
QueueHandle_t xQueueButId_G;

void led_1_task(void *p) {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    int delayr = 0;
    while (true) {
        if (xQueueReceive(xQueueButId_R, &delayr, 0)) {
            printf("%d\n", delayr);
        }

        if (delayr > 0) {
            gpio_put(LED_PIN_R, 1);
            vTaskDelay(pdMS_TO_TICKS(delayr));
            gpio_put(LED_PIN_R, 0);
            vTaskDelay(pdMS_TO_TICKS(delayr));
        }
    }
}

void btn_1_task(void *p) {
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    int delayr = 0;
    while (true) {
        if (!gpio_get(BTN_PIN_R)) {

            while (!gpio_get(BTN_PIN_R)) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            if (delayr < 1000) {
                delayr += 100;
            } else {
                delayr = 100;
            }
            printf("delay btn %d \n", delayr);
            xQueueSend(xQueueButId_R, &delayr, 0);
        }
    }
}

void led_2_task(void *p) {
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    int delayg = 0;
    while (true) {
        if (xQueueReceive(xQueueButId_G, &delayg, 0)) {
            printf("%d\n", delayg);
        }

        if (delayg > 0) {
            gpio_put(LED_PIN_G, 1);
            vTaskDelay(pdMS_TO_TICKS(delayg));
            gpio_put(LED_PIN_G, 0);
            vTaskDelay(pdMS_TO_TICKS(delayg));
        }
    }
}

void btn_2_task(void *p) {
    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    int delayg = 0;
    while (true) {
        if (!gpio_get(BTN_PIN_G)) {

            while (!gpio_get(BTN_PIN_G)) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            if (delayg < 1000) {
                delayg += 100;
            } else {
                delayg = 100;
            }
            printf("delay btn %d \n", delayg);
            xQueueSend(xQueueButId_G, &delayg, 0);
        }
    }
}


int main() {
    stdio_init_all();
    printf("Start RTOS \n");

    xQueueButId_R = xQueueCreate(32, sizeof(int));
    xQueueButId_G = xQueueCreate(32, sizeof(int));

    xTaskCreate(led_1_task, "LED_Task 1", 256, NULL, 1, NULL);
    xTaskCreate(btn_1_task, "BTN_Task 1", 256, NULL, 1, NULL);
    xTaskCreate(led_2_task, "LED_Task 2", 256, NULL, 1, NULL);
    xTaskCreate(btn_2_task, "BTN_Task 2", 256, NULL, 1, NULL);

    

    vTaskStartScheduler();

    while (true)
        ;
}
