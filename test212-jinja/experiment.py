import jinja2
import urllib.parse

template = jinja2.Template("""\
q={{ '
  CREATE USER user WITH PASSWORD \\'user\\';
  CREATE DATABASE userdb;
  GRANT WRITE ON userdb TO user;
' | urlencode }}
""")

result = template.render()

print(result)
print('---')
print(urllib.parse.parse_qs(result))
