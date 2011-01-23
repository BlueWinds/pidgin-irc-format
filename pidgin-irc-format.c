
#define PURPLE_PLUGINS

#include <glib.h>
#include <plugin.h>
#include <account.h>
#include <accountopt.h>

#ifndef _
#	define _(a) a
#endif

/* TODO use this to find the closest matching irc colour */
static guchar
ircformat_rgb_get_hue(guchar red, guchar green, guchar blue)
{
	guchar min, max;
	
	min = MIN(red, MIN(green, blue));
	max = MAX(red, MAX(green, blue));
	
	if (red == max)
		return 43 * (green - blue) / (max - min);
	if (green == max)
		return 85 + 43 * (blue - red) / (max - min);
	return 171 + 43 * (red - green) / (max - min);
}

static void
ircformat_format_message(PurpleAccount *account, gchar **message)
{
	gchar *tmp;
#define irc_format(a, b) \
							tmp = purple_strreplace(*message, a, b); \
							g_free(*message); \
							*message = tmp;
	
	irc_format("<b>", "\x02"); irc_format("</b>", "\x02");
	irc_format("<u>", "\x1F"); irc_format("</u>", "\x1F");
	irc_format("<i>", "\x16"); irc_format("</i>", "\x16");
	
	irc_format("<font color=\"#FFFFFF\">", "\x03" "0"); /* white */
	irc_format("<font color=\"#000000\">", "\x03" "1"); /* black */
	irc_format("<font color=\"#070292\">", "\x03" "2"); /* blue */
	irc_format("<font color=\"#007700\">", "\x03" "3"); /* dark green */
	irc_format("<font color=\"#FF2F00\">", "\x03" "4"); /* red */
	irc_format("<font color=\"#803046\">", "\x03" "5"); /* brown */
	irc_format("<font color=\"#7A3180\">", "\x03" "6"); /* purple */
	irc_format("<font color=\"#FF9057\">", "\x03" "7"); /* orange */
	irc_format("<font color=\"#FFFF00\">", "\x03" "8"); /* yellow */
	irc_format("<font color=\"#78FF00\">", "\x03" "9"); /* green */
	irc_format("<font color=\"#178A6E\">", "\x03" "10"); /* teal */
	irc_format("<font color=\"#3CE9FF\">", "\x03" "11"); /* cyan */
	irc_format("<font color=\"#2320E2\">", "\x03" "12"); /* light blue */
	irc_format("<font color=\"#E659EE\">", "\x03" "13"); /* pink */
	irc_format("<font color=\"#777777\">", "\x03" "14"); /* grey */
	irc_format("<font color=\"#BBBBBB\">", "\x03" "15"); /* light grey */
	
	irc_format("</font>", "\x03,");
	
}

static gboolean
ircformat_sending_im_cb(PurpleAccount *account, const gchar *receiver, gchar **message)
{
	ircformat_format_message(account, message);
	return FALSE;
}

static gboolean
ircformat_sending_chat_cb(PurpleAccount *account, gchar **message, gint id)
{
	ircformat_format_message(account, message);
	return FALSE;
}

static void
ircformat_enable_formatting_on_conversation(PurpleConversation *conv)
{
	if (!conv || !conv->account)
		return;
	if (!g_str_equal(conv->account->protocol_id, "prpl-irc"))
		return;
	
	purple_conversation_set_features(conv, purple_conversation_get_features(conv)
			| PURPLE_CONNECTION_HTML | PURPLE_CONNECTION_NO_URLDESC
			| PURPLE_CONNECTION_NO_FONTSIZE | PURPLE_CONNECTION_NO_IMAGES);
}

static void
ircformat_disable_formatting_on_conversation(PurpleConversation *conv)
{
	if (!conv || !conv->account)
		return;
	if (!g_str_equal(conv->account->protocol_id, "prpl-irc"))
		return;
	
	purple_conversation_set_features(conv, purple_conversation_get_features(conv) & ~PURPLE_CONNECTION_HTML);
}

static void
ircformat_enable_formatting_on_connection(PurpleConnection *pc)
{
	pc->flags |= PURPLE_CONNECTION_HTML;
	pc->flags |= PURPLE_CONNECTION_NO_BGCOLOR;
	pc->flags |= PURPLE_CONNECTION_NO_URLDESC;
	pc->flags |= PURPLE_CONNECTION_NO_FONTSIZE;
	pc->flags |= PURPLE_CONNECTION_NO_IMAGES;
	
	purple_conversation_foreach(ircformat_enable_formatting_on_conversation);
}

static void
ircformat_disable_formatting_on_connection(PurpleConnection *pc)
{
	pc->flags &= ~PURPLE_CONNECTION_HTML;
	
	purple_conversation_foreach(ircformat_disable_formatting_on_conversation);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	void *handle;
	GList *conns;
	
	handle = purple_conversations_get_handle();
	
	purple_signal_connect(handle, "sending-im-msg", plugin, PURPLE_CALLBACK(ircformat_sending_im_cb), NULL);
	purple_signal_connect(handle, "sending-chat-msg", plugin, PURPLE_CALLBACK(ircformat_sending_chat_cb), NULL);
	
	conns = purple_connections_get_all();
	for(; conns; conns = conns->next)
	{
		ircformat_enable_formatting_on_connection(conns->data);
	}
	
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	GList *conns;
	
	conns = purple_connections_get_all();
	for(; conns; conns = conns->next)
	{
		ircformat_disable_formatting_on_connection(conns->data);
	}
	
	return TRUE;
}

static void
plugin_init(PurplePlugin *plugin)
{
}

static PurplePluginInfo info = 
{
	PURPLE_PLUGIN_MAGIC,
	2,
	2,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_LOWEST,

	"eionrobb-irc-format",
	"IRC Message Format",
	"0.1",
	"Format outgoing IRC messages",
	"Allows you to use the Pidgin formatting toolbar to make messages bold and stuff",
	"Eion Robb <eionrobb@gmail.com>",
	"http://pidgin-irc-format.googlecode.com/", //URL
	
	plugin_load,
	plugin_unload,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(icon_override, plugin_init, info);
