```
poetry init -n
poetry add django
poetry export -f requirements.txt > requirements.txt
```

Create project and some app:

```
poetry run django-admin startproject examplesite .
poetry run django-admin startapp frontpage
```

Add some view.

Migrate:

```
poetry run ./manage.py migrate
```

Run:

```
poetry run ./manage.py runserver
```
