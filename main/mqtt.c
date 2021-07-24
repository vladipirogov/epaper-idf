#include "mqtt.h"

/** */
esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event.context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
        	 //xEventGroupSetBits(mqtt_event_group, BIT_1);
            ESP_LOGI(TAG1, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "esp-home/cmnd/#", 0);
            ESP_LOGI(TAG1, "sent subscribe successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG1, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG1, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "esp-home/status/activ", "1", 0, 0, 1);
            ESP_LOGI(TAG1, "sent publish successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG1, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG1, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG1, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG1, "MQTT_EVENT_ERROR");
            break;

        default:
            break;
    }
    return ESP_OK;
}

/** */
esp_mqtt_client_handle_t mqtt_app_start()
{
	mqtt_event_group = xEventGroupCreate();
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://192.168.1.107:1883",    //mqtt://mqtt.eclipse.org:1883
        .event_handle =  mqtt_event_handler,
		.keepalive = 10,
        .lwt_topic = "esp-home/status/activ",
		.lwt_msg = "0",
        .lwt_retain = 1,
    };

    ESP_LOGI(TAG1, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
    return client;
}