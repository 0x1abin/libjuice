/**
 * Copyright (c) 2020 Paul-Louis Ageneau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef JUICE_LOG_H
#define JUICE_LOG_H

#include "juice.h"

#include <stdarg.h>

#ifndef ESP_PLATFORM__

bool juice_log_is_enabled(juice_log_level_t level);
void juice_log_write(juice_log_level_t level, const char *file, int line, const char *fmt, ...);

#define JLOG_VERBOSE(...) juice_log_write(JUICE_LOG_LEVEL_VERBOSE, __FILE__, __LINE__, __VA_ARGS__)
#define JLOG_DEBUG(...) juice_log_write(JUICE_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define JLOG_INFO(...) juice_log_write(JUICE_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define JLOG_WARN(...) juice_log_write(JUICE_LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define JLOG_ERROR(...) juice_log_write(JUICE_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define JLOG_FATAL(...) juice_log_write(JUICE_LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#define JLOG_VERBOSE_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_VERBOSE)
#define JLOG_DEBUG_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_DEBUG)
#define JLOG_INFO_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_INFO)
#define JLOG_WARN_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_WARN)
#define JLOG_ERROR_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_ERROR)
#define JLOG_FATAL_ENABLED juice_log_is_enabled(JUICE_LOG_LEVEL_FATAL)

#else

#include <stdarg.h>
#include "esp_log.h"

#define JUICE_LOG_TAG "JUICE"

#define JLOG_VERBOSE(...) ESP_LOGV(JUICE_LOG_TAG, __VA_ARGS__)
#define JLOG_DEBUG(...) ESP_LOGD(JUICE_LOG_TAG, __VA_ARGS__)
#define JLOG_INFO(...) ESP_LOGI(JUICE_LOG_TAG, __VA_ARGS__)
#define JLOG_WARN(...) ESP_LOGW(JUICE_LOG_TAG, __VA_ARGS__)
#define JLOG_ERROR(...) ESP_LOGE(JUICE_LOG_TAG, __VA_ARGS__)
#define JLOG_FATAL(...) ESP_LOGE(JUICE_LOG_TAG, __VA_ARGS__)

#define JLOG_VERBOSE_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_VERBOSE)
#define JLOG_DEBUG_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_DEBUG)
#define JLOG_INFO_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_INFO)
#define JLOG_WARN_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_WARN)
#define JLOG_ERROR_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_ERROR)
#define JLOG_FATAL_ENABLED (esp_log_level_get(JUICE_LOG_TAG) <= ESP_LOG_ERROR)

#endif

#endif // JUICE_LOG_H
