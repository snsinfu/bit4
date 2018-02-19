## MIME

- https://tools.ietf.org/html/rfc5322 - Internet Message Format
- https://tools.ietf.org/html/rfc2045 - Multipurpose Internet Mail Extensions (MIME) Part One: Format of Internet Message Bodies
- https://tools.ietf.org/html/rfc2046 - Multipurpose Internet Mail Extensions (MIME) Part Two: Media Types

## IMAP

- https://tools.ietf.org/html/rfc3501 - INTERNET MESSAGE ACCESS PROTOCOL - VERSION 4rev1
- https://tools.ietf.org/html/rfc6855 - IMAP Support for UTF-8
- https://tools.ietf.org/html/rfc4549 - Synchronization Operations for Disconnected IMAP4 Clients
- https://www.dovecot.org/imap-client-coding-howto.html - IMAP Client Coding HOWTO
- https://apple.stackexchange.com/questions/105145 - What are the default special folder names for IMAP accounts in Mail.app (like Drafts, Junk, Sent)?

## Python MIME and IMAP

- https://docs.python.org/3/library/email.html
  - https://docs.python.org/3/library/email.message.html
  - https://docs.python.org/3/library/email.policy.html
  - https://docs.python.org/3/library/email.contentmanager.html
- https://docs.python.org/3/library/imaplib.html

Use `email` package to compose a message. It seems `Message` class is going to be deprecated by newer `EmailMessage` class. Also `MIMEText` class is legacy and going to be replaced by `ContentManager` class.
