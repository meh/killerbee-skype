Skype plugin for BitlBee (bleeding edge version)
===

This is just a plugin for BitlBee to let me use skype without having to use its shitty GUI.

Thanks to "Miklos Vajna":http://vmiklos.hu/ for his "bitlbee-skype":http://vmiklos.hu/project/bitlbee-skype/,
but for what I need it easier rewrite everything.

Requirements
---

The requirements are nearly the same as bitlbee-skype, because I use its python daemon.

- Skype &gt;= 1.4.0.99. The latest version I've tested is 2.1.0.81.
- BitlBee == The bleeding edge version, possibly the killerbee branch but it should work
  on other versions too.
- Skype4Py &gt;= 0.9.28.7. Previous versions won't work due to API changes.
  The latest version I've tested is 1.0.32.0.
- Python &gt;= 2.5. Skype4Py does not work with 2.4.
- PyGObject &gt;= 2.8.0. Older versions are part of PyGTK. (And you don't want to
  install GTK for nothing, right?)
- pyopenssl or python-gnutls.

How it works
---

Skype has to be ran anyway, if you don't want to see it you can hide every notice and hide
the windows with your window manager features (you can find a FluxBox example for that).

skyped is a daemon that lets you talk with the Skype public API and you need to start it before
you can connect to Skype through BitlBee.

Installation
---

Make sure you have every dependency installed then just download this, run rake and move the
skype.so to the BitlBee's plugin directory (for me it's /usr/lib/bitlbee).

