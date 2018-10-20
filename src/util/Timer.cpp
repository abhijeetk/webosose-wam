// Copyright (c) 2016-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "Timer.h"
#include <glib.h>
#include <cstdio>
#include <unistd.h>

int timeout_cb(void* data)
{
        fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    Timer* timer = (Timer*) data;
    timer->handleCallback();
    return timer->isRepeating();
}

int timeout_cb_destroy(void* data)
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    Timer* timer = (Timer*) data;
    timer->handleCallback();
    delete timer;
    return 0;
}

void Timer::start(int delayInMilliSeconds, bool willDestroy)
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_isRunning = true;
    if (!willDestroy)
        m_sourceId = g_timeout_add(delayInMilliSeconds, timeout_cb, this);
    else
        m_sourceId = g_timeout_add(delayInMilliSeconds, timeout_cb_destroy, this);
}

void Timer::stop()
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_isRunning = false;
    if (m_sourceId) {
        g_source_remove(m_sourceId);
        m_sourceId = 0;
    }
}

ElapsedTimer::ElapsedTimer()
    : m_isRunning(false),
      m_timer(g_timer_new())
{
}

ElapsedTimer::~ElapsedTimer()
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    g_timer_destroy(m_timer);
}

bool ElapsedTimer::isRunning() const
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    return m_isRunning;
}

void ElapsedTimer::start()
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    g_timer_start(m_timer);
    m_isRunning = true;
}

void ElapsedTimer::stop()
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    g_timer_stop(m_timer);
    m_isRunning = false;
}

int ElapsedTimer::elapsed_ms() const
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    return static_cast<int>(g_timer_elapsed(m_timer, NULL) * 1000);
}

int ElapsedTimer::elapsed_us() const
{    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    return static_cast<int>(g_timer_elapsed(m_timer, NULL) * 1000000);
}
