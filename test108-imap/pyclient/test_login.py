from imaplib import IMAP4

user = 'ubuntu'
password = 'ubuntu'

with IMAP4('localhost', port=10143) as imap:
    print(imap.capabilities)
    print(imap.noop())
    print(imap.login_cram_md5(user, password))
    print(imap.list())
    print(imap.select())
    print(imap.close())
    print(imap.logout())
