import datetime

from jinja2 import Environment, FileSystemLoader


def format_date(dt):
    weekday = "月火水木金土日"[dt.weekday()]
    return "{}/{} {}".format(dt.month, dt.day, weekday)


def format_time(dt):
    return "{}:{:02d}".format(dt.hour, dt.minute)


def is_midnight(dt):
    return dt.time() == datetime.time.min


environment = Environment(
    loader=FileSystemLoader("templates"), line_statement_prefix="#"
)
environment.filters["format_date"] = format_date
environment.filters["format_time"] = format_time
environment.tests["midnight"] = is_midnight

template = environment.get_template("main.html.j2")
document = template.render(
    {
        "timepoints": [
            datetime.datetime.fromisoformat("2018-08-15"),
            datetime.datetime.fromisoformat("2018-08-15 01:30"),
        ]
    }
)

print(document)
