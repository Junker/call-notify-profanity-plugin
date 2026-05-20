# call_notify Profanity plugin

A [Profanity](https://profanity.im/) plugin that provides desktop notifications for incoming audio/video calls via [XEP-0353: Jingle Messages](https://xmpp.org/extensions/xep-0353.html).

When another client sends a call proposal to Profanity, this plugin triggers a desktop notification so you don't miss the call.

## Features

- Detects incoming XEP-0353 `propose` stanzas
- Sends a desktop notification with the caller's JID
- Logs the call attempt in the chat window with the caller
- Zero dependencies beyond Profanity's plugin API

## Building

```bash
make
```

This produces `build/call_notify.so`.

## Installation

Install the plugin to your local Profanity plugins directory:

```bash
make install
```

This copies the plugin to `~/.local/share/profanity/plugins/`.

## Usage

Load the plugin in Profanity:

```
/plugin load call_notify
```

When an incoming call proposal arrives, you'll see a desktop notification like:

> **Incoming Call**

> Ring ring: user@example.com is trying to call you

And a message will appear in the chat window with the caller.

## Uninstallation

```bash
rm ~/.local/share/profanity/plugins/call_notify.so
```

## License

MIT
