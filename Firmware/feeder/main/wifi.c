#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "wifi.h"
#include "rtc.h"
#include "defines.h"


#define EXAMPLE_ESP_WIFI_SSID      "esp_cunha"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
#define EXAMPLE_ESP_WIFI_CHANNEL   6
#define EXAMPLE_MAX_STA_CONN       4

static const char *TAG = "wifi softAP";



// Função para lidar com a requisição POST /message
esp_err_t message_post_handler(httpd_req_t *req) {
    char buf[100];
    if (req->content_len > sizeof(buf) - 1) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Message too large");
        return ESP_FAIL;
    }

    int ret, remaining = req->content_len;
    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf) - 1))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }
        remaining -= ret;
        buf[ret] = '\0'; // Certifique-se de que a string seja terminada com null
        ESP_LOGI(TAG, "Received message: %s", buf);

        char *token;
        char *key;
        char *value_str;
        int value;

        char *rest = buf;
        while ((token = strtok_r(rest, ",", &rest))) {
            // Remover espaços extras ao redor de token
            while (isspace((unsigned char)*token)) token++;
            char *end = token + strlen(token) - 1;
            while (end > token && isspace((unsigned char)*end)) end--;
            end[1] = '\0';

            key = strtok(token, "=");
            value_str = strtok(NULL, "=");

            if (key != NULL && value_str != NULL) {
                while (isspace((unsigned char)*value_str)) value_str++;
                end = value_str + strlen(value_str) - 1;
                while (end > value_str && isspace((unsigned char)*end)) end--;
                end[1] = '\0';

                sscanf(value_str, "%d", &value);
                if (strcmp(key, "Timer") == 0) {
                    int timer_value = value;
                    const char *result = insert_alarm(timer_value, 2);
                    ESP_LOGI(TAG, "Timer value: %s", result);
                } else if (strcmp(key, "Remove_timer") == 0) {
                    int remove_timer_value = value;
                    const char *result = disable_alarm(remove_timer_value);
                    ESP_LOGI(TAG, "Remove_timer: %s", result);
                } else if (strcmp(key, "doses") == 0) {
                    int doses_value = value;
                    ESP_LOGI(TAG, "Doses value: %d", doses_value);
                } else if (strcmp(key, "SetHour") == 0) {
                    int set_hour_value = value;
                    long int unix_time = set_date(set_hour_value);
                    ESP_LOGI(TAG, "SetHour value: %ld", unix_time);
                } else {
                    ESP_LOGW(TAG, "Unknown command: %s", key);
                }
            } else {
                ESP_LOGE(TAG, "Invalid token: %s", token);
            }
        }

        print_date();
    }

    httpd_resp_send(req, "Message received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}



int incrementa = 0;
// Função para lidar com a requisição GET /message
esp_err_t message_get_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;
    char param[32];

    // Obtenha o tamanho do buffer para query string
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Query: %s", buf);
            // Obtenha o valor do parâmetro "message"
            if (httpd_query_key_value(buf, "message", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Received message: %s", param);
                httpd_resp_send(req, "Message received", HTTPD_RESP_USE_STRLEN);
                free(buf);
                return ESP_OK;
            }
                if (httpd_query_key_value(buf, "hora", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Hora em Unix: %s", param);
               // httpd_resp_send(req, get_alarm_format(0), HTTPD_RESP_USE_STRLEN);
                httpd_resp_send(req, "Unix Received", HTTPD_RESP_USE_STRLEN);
                set_date(atoi(param));
                print_date();
                free(buf);
                return ESP_OK;
            }
        }
        free(buf);
    }
    if(incrementa==count_alarms()){
        incrementa=0;
        httpd_resp_send(req, "", HTTPD_RESP_USE_STRLEN); //Responde string em branco para Aplicativo entender que não tem mais comando
        DEBUG_PRINT(("Incrementa:%d\n",incrementa));
    }else{
        httpd_resp_send(req, get_alarm_format(incrementa), HTTPD_RESP_USE_STRLEN);
        incrementa++;
        DEBUG_PRINT(("Incrementa++:%d\n",incrementa));
    }
    //httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "No message found");
    return ESP_OK;
}

// Função para lidar com a requisição GET /
esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    const char* resp_str = "<form method='POST' action='/message'><input name='message' type='text'><input type='submit' value='Enviar'></form>";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Função para lidar com a requisição GET /favicon.ico
esp_err_t favicon_get_handler(httpd_req_t *req) {
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Favicon not found");
    return ESP_OK;
}

httpd_uri_t message_post = { .uri = "/message", .method = HTTP_POST, .handler = message_post_handler, .user_ctx = NULL };
httpd_uri_t message_get = { .uri = "/message", .method = HTTP_GET, .handler = message_get_handler, .user_ctx = NULL };
httpd_uri_t root = { .uri = "/", .method = HTTP_GET, .handler = root_get_handler, .user_ctx = NULL };
httpd_uri_t favicon = { .uri = "/favicon.ico", .method = HTTP_GET, .handler = favicon_get_handler, .user_ctx = NULL };

// Inicia o servidor web
static httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16;
    config.max_resp_headers = 16;
    config.stack_size = 8192;

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &message_post);
        httpd_register_uri_handler(server, &message_get);
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &favicon);
    }

    return server;
}

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d, reason=%d", MAC2STR(event->mac), event->aid, event->reason);
    }
}

void wifi_init_softap(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = { .required = false },
        },
    };

    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}


void wifi_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    start_webserver();
}
