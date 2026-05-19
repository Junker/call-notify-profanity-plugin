// call_notify.c - Profanity plugin for XEP-0353 Jingle Message call notifications
// Sends desktop notification when receiving an incoming audio/video call via XEP-0353

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include <profapi.h>
#include <strophe.h>

// XEP-0353 constants
#define JINGLE_PROPOSE_STANZA           "propose"
#define JINGLE_MESSAGE_NS               "urn:xmpp:jingle-message:0"
#define JINGLE_RTP_NS                   "urn:xmpp:jingle:apps:rtp:1"

// Notification settings
#define NOTIFY_TIMEOUT_MS               10000
#define NOTIFY_SUMMARY                  "Incoming Call"
#define NOTIFY_ICON_PREFIX              "Ring ring"

// Plugin state
static xmpp_ctx_t *g_xmpp_ctx = NULL;

static void notify_incoming_call(const char *caller_jid)
{
    char *message = g_strdup_printf("%s: %s is trying to call you",
                                    NOTIFY_ICON_PREFIX, caller_jid);

    prof_notify(message, NOTIFY_TIMEOUT_MS, NOTIFY_SUMMARY);
    prof_chat_show_themed(caller_jid, NULL, NULL, NULL, "*", message);

    g_free(message);
}

static gboolean is_jingle_call_propose(xmpp_stanza_t *stanza)
{
    xmpp_stanza_t *propose = xmpp_stanza_get_child_by_name_and_ns(
        stanza, JINGLE_PROPOSE_STANZA, JINGLE_MESSAGE_NS);
    if (propose == NULL) {
        return FALSE;
    }

    xmpp_stanza_t *description = xmpp_stanza_get_child_by_ns(
        propose, JINGLE_RTP_NS);

    return (description != NULL);
}

int prof_on_message_stanza_receive(const char *const stanza_str)
{
    if (stanza_str == NULL || g_xmpp_ctx == NULL) {
        return 1;
    }

    xmpp_stanza_t *message = xmpp_stanza_new_from_string(g_xmpp_ctx, stanza_str);
    if (message == NULL) {
        return 1;
    }

    const char *from_jid = xmpp_stanza_get_attribute(message, "from");
    if (from_jid == NULL || !is_jingle_call_propose(message)) {
        xmpp_stanza_release(message);
        return 1;
    }

    char *bare_jid = xmpp_jid_bare(g_xmpp_ctx, from_jid);
    if (bare_jid != NULL) {
        notify_incoming_call(bare_jid);
        xmpp_free(g_xmpp_ctx, bare_jid);
    }

    xmpp_stanza_release(message);
    return 1;
}

void prof_init(const char *const version, const char *const status,
               const char *const account_name, const char *const fulljid)
{
    g_xmpp_ctx = xmpp_ctx_new(NULL, NULL);
    prof_log_info("call_notify plugin loaded - monitoring for XEP-0353 incoming calls");
}

void prof_close(void)
{
    if (g_xmpp_ctx != NULL) {
        xmpp_ctx_free(g_xmpp_ctx);
        g_xmpp_ctx = NULL;
    }
    prof_log_info("call_notify plugin unloaded");
}
