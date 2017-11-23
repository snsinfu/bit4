import sys
from .patcher import Patcher
sys.modules[__name__] = Patcher(sys.modules[__name__],
                                blacklist=['sys', 'patcher', 'Patcher'])
