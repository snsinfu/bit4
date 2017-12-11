import decimal
import json

class DecimalAwareEncoder(json.JSONEncoder):
    def encode(self, o, **kwargs):
        if isinstance(o, decimal.Decimal):
            return str(o)
        return super(DecimalAwareEncoder, self).encode(o, **kwargs)

    def iterencode(self, o, **kwargs):
        if isinstance(o, decimal.Decimal):
            return iter((str(o), ))
        return super(DecimalAwareEncoder, self).iterencode(o, **kwargs)

data = {
    'value': decimal.Decimal('1.23')
}
js = json.dumps(data, cls=DecimalAwareEncoder)
print(js)
