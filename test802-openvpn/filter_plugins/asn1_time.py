import datetime


def asn1_time(dt, **kwargs):
    dt = dt.astimezone(datetime.timezone.utc)
    return dt.strftime("%Y%m%d%H%M%SZ")


class FilterModule:
    def filters(self):
        return {"asn1_time": asn1_time}
