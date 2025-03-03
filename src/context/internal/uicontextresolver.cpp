/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "uicontextresolver.h"

#include "log.h"

using namespace mu::context;
using namespace mu::ui;

static const mu::Uri HOME_PAGE_URI("musescore://home");
static const mu::Uri NOTATION_PAGE_URI("musescore://notation");

static const QString NOTATION_NAVIGATION_SECTION("NotationView");

void UiContextResolver::init()
{
    interactive()->currentUri().ch.onReceive(this, [this](const Uri&) {
        notifyAboutContextChanged();
    });

    playbackController()->isPlayingChanged().onNotify(this, [this]() {
        notifyAboutContextChanged();
    });

    globalContext()->currentNotationChanged().onNotify(this, [this]() {
        auto notation = globalContext()->currentNotation();
        if (notation) {
            notation->interaction()->selectionChanged().onNotify(this, [this]() {
                notifyAboutContextChanged();
            });

            notation->interaction()->textEditingStarted().onNotify(this, [this]() {
                notifyAboutContextChanged();
            });
        }
        notifyAboutContextChanged();
    });

    navigationController()->navigationChanged().onNotify(this, [this]() {
        notifyAboutContextChanged();
    });
}

void UiContextResolver::notifyAboutContextChanged()
{
    TRACEFUNC;
    m_currentUiContextChanged.notify();
}

UiContext UiContextResolver::currentUiContext() const
{
    TRACEFUNC;
    ValCh<Uri> currentUri = interactive()->currentUri();
    if (currentUri.val == HOME_PAGE_URI) {
        if (playbackController()->isPlaying()) {
            return context::UiCtxPlaying;
        }

        return context::UiCtxHomeOpened;
    }

    if (currentUri.val == NOTATION_PAGE_URI) {
        auto notation = globalContext()->currentNotation();
        if (!notation) {
            //! NOTE The notation page is open, but the notation itself is not loaded - we consider that the notation is not open.
            //! We need to think, maybe we need a separate value for this case.
            return context::UiCtxUnknown;
        }

        if (notation->interaction()->isTextEditingStarted()) {
            return context::UiCtxNotationTextEditing;
        }

        if (playbackController()->isPlaying()) {
            return context::UiCtxPlaying;
        }

        ui::INavigationSection* activeSection = navigationController()->activeSection();
        if (activeSection && activeSection->name() != NOTATION_NAVIGATION_SECTION) {
            //! TODO Needs add other ui contexts
            return context::UiCtxUnknown;
        }

        if (!notation->interaction()->selection()->isNone()) {
            return context::UiCtxNotationHasSelection;
        }

        return context::UiCtxNotationOpened;
    }

    return context::UiCtxUnknown;
}

bool UiContextResolver::match(const ui::UiContext& currentCtx, const ui::UiContext& actCtx) const
{
    //! NOTE If now editing the notation text, then we allow actions only with the context `UiCtxNotationTextEditing`,
    //! all others, even `UiCtxAny`, are forbidden
    if (currentCtx == context::UiCtxNotationTextEditing) {
        return actCtx == context::UiCtxNotationTextEditing;
    }

    if (actCtx == context::UiCtxAny) {
        return true;
    }

    //! NOTE If the current context is `UiCtxNotationHasSelection`, then we allow `UiCtxNotationOpened` too
    if (currentCtx == context::UiCtxNotationHasSelection) {
        if (actCtx == context::UiCtxNotationOpened) {
            return true;
        }
        return actCtx == context::UiCtxNotationHasSelection;
    }

    return currentCtx == actCtx;
}

bool UiContextResolver::matchWithCurrent(const UiContext& ctx) const
{
    if (ctx == ui::UiCtxAny) {
        return true;
    }

    UiContext currentCtx = currentUiContext();
    return currentCtx == ctx;
}

mu::async::Notification UiContextResolver::currentUiContextChanged() const
{
    return m_currentUiContextChanged;
}
