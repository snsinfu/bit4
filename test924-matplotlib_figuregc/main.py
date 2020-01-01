from matplotlib.backends.backend_agg import FigureCanvas
from matplotlib.figure import Figure


RED = "\x1b[1;31m"
GREEN = "\x1b[1;32m"
RESET = "\x1b[m"


class MyFigure(Figure):
    def __del__(self):
        print(f"{RED}del{RESET} {id(self):x}")


def make_figure(*args, **kwargs):
    fig = MyFigure(*args, **kwargs)
    FigureCanvas(fig)
    return fig


print("=== Start ===")

for i in range(30):
    fig = make_figure()
    ax = fig.add_subplot(111)
    print(f"{GREEN}new{RESET} {id(fig):x}")

print("=== End ===")
