// Copyright (c) 2014-2018 LG Electronics, Inc.
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

#include "BlinkWebView.h"
#include "WebPageBlinkDelegate.h"

#include "LogManager.h"
#include <QStringList>

BlinkWebView::BlinkWebView(bool doInitialize)
    : WebViewBase::WebViewBase()
    , m_delegate(NULL)
    , m_progress(0)
    , m_userScriptExecuted(false)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
}

void BlinkWebView::addUserScript(const std::string& script)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_userScripts.push_back(script);
}

void BlinkWebView::clearUserScripts()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_userScripts.clear();
}

void BlinkWebView::executeUserScripts()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (m_userScriptExecuted)
        return;

    for (std::vector<std::string>::const_iterator it = m_userScripts.begin(); it != m_userScripts.end(); ++it)
        RunJavaScript(*it);

    m_userScriptExecuted = true;
}

void BlinkWebView::setDelegate(WebPageBlinkDelegate* delegate)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_delegate = delegate;
}

void BlinkWebView::HandleBrowserControlCommand(const std::string& command, const std::vector<std::string>& arguments)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (m_delegate) {
        QString message = QString::fromStdString(command);
        QStringList params;
        Q_FOREACH (const std::string& arg, arguments)
             params << QString::fromStdString(arg);
        m_delegate->handleBrowserControlCommand(message, params);
    }
}

void BlinkWebView::HandleBrowserControlFunction(const std::string& command, const std::vector<std::string>& arguments, std::string* result)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (m_delegate) {
        QString message = QString::fromStdString(command);
        QStringList params;
        Q_FOREACH (const std::string& arg, arguments)
            params << QString::fromStdString(arg);
        QString ret;
        m_delegate->handleBrowserControlFunction(message, params, &ret);
        *result = ret.toStdString();
    }
}

void BlinkWebView::OnLoadProgressChanged(double progress)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_progress = (int)(progress * 100);
    LOG_INFO(MSGID_PAGE_LOADING, 1, PMLOGKS("", ""), "PROGRESS: %d%%", m_progress);
}

void BlinkWebView::Close()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->close();
}

bool BlinkWebView::DecidePolicyForResponse(bool isMainFrame, int statusCode,
                                           const std::string& url, const std::string& statusText)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return false;

    return m_delegate->decidePolicyForResponse(isMainFrame, statusCode, url, statusText);
}

bool BlinkWebView::AcceptsVideoCapture()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
  if (!m_delegate)
    return false;

  return m_delegate->acceptsVideoCapture();
}

bool BlinkWebView::AcceptsAudioCapture()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
  if (!m_delegate)
    return false;

  return m_delegate->acceptsAudioCapture();
}

void BlinkWebView::DidDropAllPeerConnections(webos::DropPeerConnectionReason reason)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return;

    m_delegate->didDropAllPeerConnections();
    LOG_DEBUG("Dropped all WebRTC peer connections with reason %d", reason);
}

void BlinkWebView::DidFirstFrameFocused()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->didFirstFrameFocused();
}

void BlinkWebView::DocumentLoadFinished()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    executeUserScripts();
}

void BlinkWebView::LoadVisuallyCommitted()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->loadVisuallyCommitted();
}

void BlinkWebView::LoadFinished(const std::string& url)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->loadFinished(url);

#if !defined(OS_WEBOS)
    LoadVisuallyCommitted();
#endif
}

void BlinkWebView::LoadFailed(const std::string& url, int errCode, const std::string& errDesc)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return;

    m_delegate->loadFailed(url, errCode, errDesc);
}

void BlinkWebView::LoadStopped(const std::string& url)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return;

    m_delegate->loadStopped(url);
}

void BlinkWebView::LoadStarted()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    m_userScriptExecuted = false;

    if (!m_delegate)
        return;

    m_delegate->loadStarted();
}

void BlinkWebView::RenderProcessCreated(int pid)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->renderProcessCreated(pid);
}

void BlinkWebView::RenderProcessGone()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return;

    m_delegate->renderProcessCrashed();
}

void BlinkWebView::TitleChanged(const std::string& title)
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
      return;

    m_delegate->titleChanged(title);
}

void BlinkWebView::NavigationHistoryChanged()
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return;

    m_delegate->navigationHistoryChanged();
}

bool BlinkWebView::AllowMouseOnOffEvent() const
{
    fprintf(stderr, "[%d] %s %s %d\r\n", (int)getpid(), __FILE__, __FUNCTION__, __LINE__);
    if (!m_delegate)
        return false;

    return m_delegate->allowMouseOnOffEvent();
}

