import email.utils
import os
import uuid

from datetime import datetime, timezone
from email.message import EmailMessage
from imaplib import IMAP4


host = 'localhost'
port = int(os.getenv('PORT') or 1143)
user = os.getenv('IMAP_USER') or 'alice'
password = os.getenv('IMAP_PASS') or 'alice'

mailbox = 'Notes'
subject = 'Hello 📧 IMAP!'
keywords = 'test, hello'
sender = 'nobody@example.com'
body = 'The quick brown fox 🦊 jumps over the lazy dog 🐶 .'
date = datetime.now(timezone.utc)

message_id = str(uuid.uuid4()) + '@example.com'

message = EmailMessage()
message.add_header('MIME-Version', '1.0')
message.add_header('Date', email.utils.format_datetime(date))
message.add_header('From', sender)
message.add_header('Subject', subject)
message.add_header('Keywords', keywords)
message.add_header('Message-ID', message_id)
message.set_content(body)

with IMAP4(host, port) as imap:
    imap.debug = 4

    imap.noop()
    imap.login(user, password)
    imap.enable('UTF8=ACCEPT')
    imap.list()

    imap.create(mailbox)
    imap.select(mailbox)
    imap.append(mailbox, None, date, message.as_bytes())
    imap.close()

    imap.select(mailbox)
    imap.search(None, 'OR SUBJECT "dog" TEXT "dog"')
    imap.fetch('1:*', '(UID ENVELOPE)')
    imap.fetch('1:*', 'BODY[]')
    imap.close()

    imap.logout()
